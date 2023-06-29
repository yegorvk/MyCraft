//
// Created by egorv on 6/29/2023.
//

#ifndef SHITCRAFT_GLRESOURCE_H
#define SHITCRAFT_GLRESOURCE_H

#include <utility>
#include "glad/glad.h"

using gl_id_type = unsigned int;
using gl_enum_type = unsigned int;

template<gl_id_type BindTarget>
struct GlTextureResource;

template<gl_id_type BindTarget>
struct GlBufferResource;

struct GlVertexArrayResource;

template<typename ResourceType>
struct GlResourceOperation {
    static gl_id_type create() = delete;

    static void destroy(gl_id_type) = delete;

    static void bind(gl_id_type) = delete;

    static void unbind() = delete;
};

template<gl_enum_type TextureBindTarget>
struct GlResourceOperation<GlTextureResource<TextureBindTarget>> {
    static inline gl_id_type create() {
        gl_id_type id = 0;
        glGenTextures(1, &id);
        return id;
    }

    static inline void destroy(gl_id_type id) {
        glDeleteTextures(1, &id);
    }

    static inline void bind(gl_id_type id) {
        glBindTexture(TextureBindTarget, id);
    }

    static inline void unbind() {
        glBindTexture(TextureBindTarget, 0);
    }
};

template<gl_enum_type BufferBindTarget>
struct GlResourceOperation<GlBufferResource<BufferBindTarget>> {
    static inline gl_id_type create() {
        gl_id_type id = 0;
        glGenBuffers(1, &id);
        return id;
    }

    static inline void destroy(gl_id_type id) {
        glDeleteBuffers(1, &id);
    }

    static inline void bind(gl_id_type id) {
        glBindBuffer(BufferBindTarget, id);
    }

    static inline void unbind() {
        glBindBuffer(BufferBindTarget, 0);
    }
};

template<>
struct GlResourceOperation<GlVertexArrayResource> {
    static inline gl_id_type create() {
        gl_id_type id = 0;
        glGenVertexArrays(1, &id);
        return id;
    }

    static inline void destroy(gl_id_type id) {
        glDeleteVertexArrays(1, &id);
    }

    static inline void bind(gl_id_type id) {
        glBindVertexArray(id);
    }

    static inline void unbind() {
        glBindVertexArray(0);
    }
};

template<typename ResourceType>
class GlResourceDelegate {
public:
    GlResourceDelegate() = default;

    inline explicit GlResourceDelegate(gl_id_type id) : id(id) {}

    inline void bind() const {
        Operations::bind(id);
    }

    static inline void unbind() {
        Operations::unbind();
    }

    gl_id_type getId() {
        return id;
    }

protected:
    using Operations = GlResourceOperation<ResourceType>;

    static GlResourceDelegate create() {
        return GlResourceDelegate(Operations::create());
    }

    inline void destroy() {
        Operations::destroy(id);
        id = 0;
    }

    gl_id_type id = 0;
};

template<typename ResourceType>
class GlResource : public GlResourceDelegate<ResourceType> {
public:
    static inline GlResource create() {
        return GlResource(GlResourceDelegate<ResourceType>::create());
    }

    GlResource() = default;

    constexpr explicit GlResource(gl_id_type id) : GlResourceDelegate<ResourceType>(id) {}

    GlResource(const GlResource &) = delete;

    GlResource &operator=(const GlResource &) = delete;

    constexpr GlResource(GlResource &&other) noexcept {
        *this = std::move(other);
    }

    constexpr GlResource &operator=(GlResource &&other) noexcept {
        this->id = other.id, other.id = 0;
        return *this;
    }

    inline ~GlResource() {
        this->destroy();
    }

private:
    inline explicit GlResource(GlResourceDelegate<ResourceType> &&delegate)
    : GlResourceDelegate<ResourceType>(std::move(delegate)) {}
};

template<typename ResourceType>
class GlResourceRef : public GlResourceDelegate<ResourceType> {
public:
    GlResourceRef() = default;

    inline explicit GlResourceRef(gl_id_type id) : GlResourceDelegate<ResourceType>(id) {}

    inline explicit GlResourceRef(const GlResource<ResourceType> &resource)
    : GlResourceDelegate<ResourceType>(resource.id) {}

    GlResourceRef(const GlResourceRef &) = default;

    GlResourceRef(GlResourceRef &&) noexcept = default;

    GlResourceRef &operator=(const GlResourceRef &) = default;

    GlResourceRef &operator=(GlResourceRef &&) noexcept = default;
};

using GlVertexBuffer = GlResource<GlBufferResource<GL_ARRAY_BUFFER>>;

using GlVertexArray = GlResource<GlVertexArrayResource>;

template<gl_enum_type GlTextureBindTarget>
using GlTexture = GlResource<GlTextureResource<GlTextureBindTarget>>;

#endif //SHITCRAFT_GLRESOURCE_H
