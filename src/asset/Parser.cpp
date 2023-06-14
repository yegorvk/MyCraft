//
// Created by egorv on 4/23/2023.
//

#include "spdlog/spdlog.h"

#include "chunk/Constants.h"
#include "Parser.h"

using nlohmann::json;
using nlohmann::json_schema::json_validator;

namespace asset {
    detail::ParserState::ParserState(std::string_view rootDir) : directory(rootDir), parentName("@") {}

    detail::ParserState detail::ParserState::append(std::string_view nameSuffix, std::string_view subDir) const {
        auto copy = *this;

        if (!subDir.empty())
            copy.directory /= subDir;

        if (!nameSuffix.empty()) {
            if (copy.parentName != "@") copy.parentName.push_back('/');
            copy.parentName += nameSuffix;
        }

        return copy;
    }

    Parser::Parser(asset::Index &index,
                   FileReader &reader,
                   std::string_view schemaPath) : validator(json::parse(reader.getText(schemaPath.data()))),
                                                  index(index),
                                                  reader(reader) {}

    void Parser::parse(std::string_view rootDir, std::string_view rootFilename) {
        file(detail::ParserState(rootDir), rootFilename);
    }

    detail::AssetDescription Parser::file(const detail::ParserState &state, std::string_view filename) {
        auto filePath = state.directory;
        filePath.append(filename);

        auto root = json::parse(reader.getText(filePath.generic_string()));
        validator.validate(root);

        return asset(state, root);
    }

    detail::AssetDescription Parser::asset(const detail::ParserState &state, const json &root) {
        auto nameSuffix = root.contains("name") ? root["name"].get<std::string>() : "";
        auto type = root["type"].get<std::string>();

        auto newState = state.append(nameSuffix, "");
        auto fullName = newState.parentName;

        if (type == "group")
            group(std::move(newState), root);
        else if (type == "text")
            text(std::move(newState), root);
        else if (type == "image")
            image(std::move(newState), root);
        else if (type == "shader")
            shader(std::move(newState), root);
        else if (type == "block")
            block(std::move(newState), root);
        else
            SPDLOG_ERROR("Unrecognized asset type: {}", type);

        return detail::AssetDescription{std::move(fullName)};
    }

    void Parser::group(detail::ParserState &&state, const nlohmann::json &root) {
        auto assets = root["members"].get<std::vector<json>>();

        for (const auto &child : assets) {
            if (child.is_string()) {
                auto childStr = child.get<std::string>();

                auto tempPath = state.directory;
                tempPath /= childStr + ".json";

                auto tempPathStr = tempPath.generic_string();

                if (reader.exists(tempPathStr)) {
                    file(state.append(childStr, ""), tempPathStr);
                } else
                    file(state.append(childStr, childStr));
            } else
                asset(state, child);
        }
    }

    void Parser::text(detail::ParserState &&state, const json &root) {
        auto path = std::move(state.directory);
        path.append(root["path"].get<std::string>());

        index.insert(std::move(state.parentName), TextAsset(path.generic_string()));
    }

    void Parser::image(detail::ParserState &&state, const json &root) {
        ImageDescription description{};

        if (root.contains("width"))
            description.width = root["width"].get<int>();

        if (root.contains("height"))
            description.height = root["height"].get<int>();

        if (root.contains("channelCount"))
            description.channelCount = root["channelCount"].get<int>();

        if (root.contains("path")) {
            auto path = std::move(state.directory);
            path.append(root["path"].get<std::string>());

            ImageAsset asset(false, path.generic_string(), description);
            index.insert(std::move(state.parentName), std::move(asset));
        } else {
            auto color = root["color"].get<std::string>();

            ImageAsset asset(true, std::move(color), description);
            index.insert(std::move(state.parentName), std::move(asset));
        }
    }

    void Parser::shader(detail::ParserState &&state, const json &root) {
        const auto &stages = root["stages"];
        ShaderAsset shaderAsset;

        if (stages.contains("vertex"))
            shaderAsset.set(ShaderType::Vertex, resolveRef(state, stages["vertex"]));

        if (stages.contains("fragment"))
            shaderAsset.set(ShaderType::Fragment, resolveRef(state, stages["fragment"]));

        index.insert(std::move(state.parentName), std::move(shaderAsset));
    }

    void Parser::block(detail::ParserState &&state, const nlohmann::json &root) {
        //bool translucent = root.contains("translucent") && root["translucent"].getId<bool>();
        const auto &faces = root["faces"];

        BlockAsset blockAsset = BlockAsset::builder()
                .setFace(Face::Right, blockFace(state, faces["right"]))
                .setFace(Face::Left, blockFace(state, faces["left"]))
                .setFace(Face::Top, blockFace(state, faces["top"]))
                .setFace(Face::Bottom, blockFace(state, faces["bottom"]))
                .setFace(Face::Front, blockFace(state, faces["front"]))
                .setFace(Face::Back, blockFace(state, faces["back"]))
                .build();

        index.insert(std::move(state.parentName), std::move(blockAsset));
    }

    BlockFace Parser::blockFace(const detail::ParserState &state, const nlohmann::json &root) {
        return BlockFace(resolveRef(state, root["texture"]));
    }

    std::string Parser::resolveRef(const detail::ParserState &state, const nlohmann::json &value) {
        if (value.type() == nlohmann::json::value_t::string)
            return value.get<std::string>();
        else
            return asset(state, value).name;
    }
} // asset



