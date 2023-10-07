//
// Created by egorv on 6/29/2023.
//

#ifndef SHITCRAFT_GLRESOURCE_H
#define SHITCRAFT_GLRESOURCE_H

#include <utility>
#include "glad/glad.h"

#include "asset/Image.h"
#include "GlTypes.h"
#include "TextureDefinitions.h"

template<gl_id_type BindTarget>
struct GlTextureResource;

template<gl_id_type BindTarget>
struct GlBufferResource;

struct GlVertexArrayResource;

struct GlFramebufferResource;

class GlResourceIdHolder {
public:
    constexpr GlResourceIdHolder() = default;

    constexpr explicit GlResourceIdHolder(gl_id_type id) : id(id) {}

    constexpr GlResourceIdHolder(const GlResourceIdHolder &) = default;

    constexpr GlResourceIdHolder(GlResourceIdHolder &&other) noexcept {
        *this = std::move(other);
    }

    constexpr GlResourceIdHolder &operator=(const GlResourceIdHolder &) = default;

    constexpr GlResourceIdHolder &operator=(GlResourceIdHolder &&other) noexcept {
        id = other.id, other.id = 0;
        return *this;
    };

    [[nodiscard]] constexpr gl_id_type getId() const {
        return id;
    }

protected:
    gl_id_type id = 0;
};

template<typename ResourceType>
struct GlResourceLifecycle {
    static gl_id_type create() = delete;

    static void destroy(gl_id_type) = delete;
};

template<gl_enum_type TextureBindTarget>
struct GlResourceLifecycle<GlTextureResource<TextureBindTarget>> {
    static inline gl_id_type create() {
        gl_id_type id = 0;
        glGenTextures(1, &id);
        return id;
    }

    static inline void destroy(gl_id_type id) {
        glDeleteTextures(1, &id);
    }
};

template<gl_enum_type BufferBindTarget>
struct GlResourceLifecycle<GlBufferResource<BufferBindTarget>> {
    static inline gl_id_type create() {
        gl_id_type id = 0;
        glGenBuffers(1, &id);
        return id;
    }

    static inline void destroy(gl_id_type id) {
        glDeleteBuffers(1, &id);
    }
};

template<>
struct GlResourceLifecycle<GlVertexArrayResource> {
    static inline gl_id_type create() {
        gl_id_type id = 0;
        glGenVertexArrays(1, &id);
        return id;
    }

    static inline void destroy(gl_id_type id) {
        glDeleteVertexArrays(1, &id);
    }
};

template<>
struct GlResourceLifecycle<GlFramebufferResource> {
    static inline gl_id_type create() {
        gl_id_type id = 0;
        glGenFramebuffers(1, &id);
        return id;
    }

    static inline void destroy(gl_id_type id) {
        glDeleteFramebuffers(1, &id);
    }
};

#define GL_RESOURCE_OPERATIONS_BOILERPLATE \
public:                                    \
GlResourceOperations() = default;          \
GlResourceOperations(GlResourceOperations &&other) noexcept { \
    *this = std::move(other); \
};                                         \
GlResourceOperations &operator=(GlResourceOperations &&other) noexcept { \
    ownerId = other.ownerId, other.ownerId = 0;               \
    return *this;                                           \
}\
protected: \
explicit GlResourceOperations(gl_id_type ownerId) : ownerId(ownerId) {} \
private:                                   \
gl_id_type ownerId = 0;  \
public:

template<typename ResourceType>
struct GlResourceOperations;

template<gl_enum_type TextureBindTarget>
struct GlResourceOperations<GlTextureResource<TextureBindTarget>> {
GL_RESOURCE_OPERATIONS_BOILERPLATE

    inline void bind() const {
        glBindTexture(TextureBindTarget, ownerId);
    }

    static inline void unbind() {
        glBindTexture(TextureBindTarget, 0);
    }

    inline void setImage2d(const Image &image, int target = TextureBindTarget) {
        glTexImage2D(target,
                     0,
                     getTextureInternalFormat(image.getChannelCount()),
                     image.getWidth(),
                     image.getHeight(),
                     0,
                     getTextureFormat(image.getChannelCount()),
                     GL_UNSIGNED_BYTE,
                     image.getPixels());
    }
};

template<gl_enum_type BufferBindTarget>
struct GlResourceOperations<GlBufferResource<BufferBindTarget>> {
GL_RESOURCE_OPERATIONS_BOILERPLATE

    inline void bind() const {
        glBindBuffer(BufferBindTarget, ownerId);
    }

    static inline void unbind() {
        glBindBuffer(BufferBindTarget, 0);
    }
};

template<>
struct GlResourceOperations<GlVertexArrayResource> {
GL_RESOURCE_OPERATIONS_BOILERPLATE

    inline void bind() const {
        glBindVertexArray(ownerId);
    }

    static inline void unbind() {
        glBindVertexArray(0);
    }
};

template<>
struct GlResourceOperations<GlFramebufferResource> {
GL_RESOURCE_OPERATIONS_BOILERPLATE

    inline void bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, ownerId);
    }

    inline void bindDraw() const {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, ownerId);
    }

    inline void bindRead() const {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, ownerId);
    }

    static inline void unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    static inline void unbindDraw() {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }

    static inline void unbindRead() {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    }
};

#undef GL_RESOURCE_OPERATIONS_BOILERPLATE

template<typename ResourceType>
class GlResourceDelegate : public GlResourceIdHolder, public GlResourceOperations<ResourceType> {
public:
    GlResourceDelegate() = default;

    inline explicit GlResourceDelegate(gl_id_type id)
            : GlResourceIdHolder(id), GlResourceOperations<ResourceType>(id) {}

protected:
    using Lifecycle = GlResourceLifecycle<ResourceType>;

    static constexpr bool EnableCreate = std::is_function_v<decltype(Lifecycle::create)>;
    static constexpr bool EnableDestroy = std::is_function_v<decltype(Lifecycle::destroy)>;

    static inline GlResourceDelegate create(typename std::enable_if<EnableCreate, int>::type = 0) {
        return GlResourceDelegate(Lifecycle::create());
    }

    inline void destroy() {
        if constexpr (EnableDestroy)
            Lifecycle::destroy(getId());
    }
};

template<typename ResourceType>
class GlResourceRef : public GlResourceDelegate<ResourceType> {
public:
    GlResourceRef() = default;

    inline explicit GlResourceRef(gl_id_type id) : GlResourceDelegate<ResourceType>(id) {}

    GlResourceRef(const GlResourceRef &) = default;

    GlResourceRef(GlResourceRef &&) noexcept = default;

    GlResourceRef &operator=(const GlResourceRef &) = default;

    GlResourceRef &operator=(GlResourceRef &&) noexcept = default;
};

template<typename ResourceType>
class GlResource : public GlResourceDelegate<ResourceType> {
public:
    using ref_type = GlResourceRef<ResourceType>;

    static inline GlResource create() {
        return GlResource(GlResourceDelegate<ResourceType>::create());
    }

    GlResource() = default;

    constexpr explicit GlResource(gl_id_type id) : GlResourceDelegate<ResourceType>(id) {}

    GlResource(const GlResource &) = delete;

    GlResource &operator=(const GlResource &) = delete;

    constexpr GlResource(GlResource &&other) noexcept = default;

    constexpr GlResource &operator=(GlResource &&other) noexcept = default;

    inline ~GlResource() {
        this->destroy();
    }

private:
    inline explicit GlResource(GlResourceDelegate<ResourceType> &&delegate)
            : GlResourceDelegate<ResourceType>(std::move(delegate)) {}
};

template<typename ResourceType>
GlResourceRef<ResourceType> ref(const GlResource<ResourceType> &res) {
    return GlResourceRef<ResourceType>(res.getId());
}

using GlVertexBuffer = GlResource<GlBufferResource<GL_ARRAY_BUFFER>>;
using GlVertexBufferRef = GlVertexBuffer::ref_type;

using GlElementBuffer = GlResource<GlBufferResource<GL_ELEMENT_ARRAY_BUFFER>>;
using GlElementBufferRef = GlElementBuffer::ref_type;

using GlVertexArray = GlResource<GlVertexArrayResource>;
using GlVertexArrayRef = GlVertexArray::ref_type;

template<gl_enum_type GlTextureBindTarget>
using GlTexture = GlResource<GlTextureResource<GlTextureBindTarget>>;

template<gl_enum_type GlTextureBindTarget>
using GlTextureRef = typename GlTexture<GlTextureBindTarget>::ref_type;

using GlFramebuffer = GlResource<GlFramebufferResource>;
using GlFramebufferRef = GlFramebuffer::ref_type;

#endif //SHITCRAFT_GLRESOURCE_H
