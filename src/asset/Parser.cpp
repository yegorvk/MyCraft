//
// Created by egorv on 4/23/2023.
//

#include "spdlog/spdlog.h"

#include "Parser.h"

using nlohmann::json;
using nlohmann::json_schema::json_validator;

namespace asset {
    detail::ParserState::ParserState(std::string_view rootDir) : directory(rootDir), parentName("@") {}

    detail::ParserState detail::ParserState::append(const std::string &name, std::string_view subDir) const {
        auto copy = *this;

        if (!subDir.empty())
            copy.directory.append(subDir);

        if (!name.empty()) {
            if (copy.parentName != "@") copy.parentName.push_back('/');
            copy.parentName += name;
        }

        return std::move(copy);
    }

    Parser::Parser(asset::Index &index,
                   FileReader &reader,
                   std::string_view schemaPath) : index(index),
                                                  reader(reader),
                                                  validator(json::parse(reader.getText(schemaPath.data()))) {}

    void Parser::parse(std::string_view rootDir, std::string_view mapFilename) {
        file(detail::ParserState(rootDir), mapFilename);
    }

    detail::AssetDescription Parser::file(const detail::ParserState &state, std::string_view mapFilename) {
        auto mapFilePath = state.directory;
        mapFilePath.append(mapFilename);

        auto root = json::parse(reader.getText(mapFilePath.generic_string().c_str()));
        validator.validate(root);

        return asset(state, root);
    }

    detail::AssetDescription Parser::asset(const detail::ParserState &state, const json &root) {
        if (root.contains("include"))
            return file(state.append("", root["include"].get<std::string>()));
        else {
            auto name = root["name"].get<std::string>();
            auto type = root["type"].get<std::string>();

            auto newState = state.append(name, "");
            auto fullName = newState.parentName;
            
            if (type == "group")
                group(std::move(newState), root);
            else if (type == "text")
                text(std::move(newState), root);
            else if (type == "image")
                image(std::move(newState), root);
            else if (type == "shader")
                shader(std::move(newState), root);
            else
                SPDLOG_ERROR("Unrecognized asset type: {}", type);

            return detail::AssetDescription{std::move(fullName)};
        }
    }

    void Parser::group(detail::ParserState &&state, const nlohmann::json &root) {
        auto assets = root["assets"].get<std::vector<json>>();

        for (const auto &child : assets)
            asset(state, child);
    }

    void Parser::text(detail::ParserState &&state, const json &root) {
        auto path = std::move(state.directory);
        path.append(root["path"].get<std::string>());

        index.insert(std::move(state.parentName), TextAsset(path.generic_string()));
    }

    void Parser::image(detail::ParserState &&state, const json &root) {
        auto path = std::move(state.directory);
        path.append(root["path"].get<std::string>());

        index.insert(std::move(state.parentName), ImageAsset(path.generic_string()));
    }

    void Parser::shader(detail::ParserState &&state, const json &root) {
        const auto &stages = root["stages"];
        ShaderAsset shaderAsset;

        if (stages.contains("vertex"))
            shaderAsset.set(ShaderType::Vertex, asset(state, stages["vertex"]).name);

        if (stages.contains("fragment"))
            shaderAsset.set(ShaderType::Fragment, asset(state, stages["fragment"]).name);

        index.insert(std::move(state.parentName), std::move(shaderAsset));
    }
} // asset



