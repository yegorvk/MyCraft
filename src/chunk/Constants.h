//
// Created by egorv on 5/17/2023.
//

#ifndef SHITCRAFT_CONSTANTS_H
#define SHITCRAFT_CONSTANTS_H

#include <cstdint>

constexpr int CHUNK_SIDE_BLOCK_COUNT = 16;
constexpr double BLOCK_SIDE_SCALE = 0.1;
constexpr float BLOCK_SIDE_SCALE_F = static_cast<float>(BLOCK_SIDE_SCALE);

constexpr double CHUNK_SIDE_SCALE = BLOCK_SIDE_SCALE * CHUNK_SIDE_BLOCK_COUNT;
constexpr float CHUNK_SIDE_SCALE_F = static_cast<float>(CHUNK_SIDE_SCALE);

using BlockId = uint16_t;

namespace Axis {
    constexpr int X = 0;
    constexpr int Y = 1;
    constexpr int Z = 2;

    enum class AxisEnum : int {
        X = X,
        Y = Y,
        Z = Z
    };

    constexpr AxisEnum toEnum(int axis) {
        return static_cast<AxisEnum>(axis);
    }

    constexpr int toValue(AxisEnum axisEnum) {
        return static_cast<int>(axisEnum);
    }

    constexpr glm::ivec3 getPositiveDirection(int axis) {
        switch (toEnum(axis)) {
            case AxisEnum::X:
                return {1, 0, 0};
            case AxisEnum::Y:
                return {0, 1, 0};
            case AxisEnum::Z:
                return {0, 0, 1};
        }
    }

    constexpr glm::ivec3 getDirection(int axis, bool positiveOrientated) {
        return positiveOrientated ? getPositiveDirection(axis) : -getPositiveDirection(axis);
    }
}

namespace Face {
    constexpr int Right = 0;
    constexpr int Left = 1;
    constexpr int Top = 2;
    constexpr int Bottom = 3;
    constexpr int Front = 4;
    constexpr int Back = 5;

    constexpr int fromAxis(int axis, bool positiveOrientated) {
        return 2 * axis + !positiveOrientated;
    }

    // true if face exterior normal points in positive direction
    constexpr bool isPositiveOrientated(int face) {
        return face % 2 == 0;
    }

    constexpr int getNormalAxis(int face) {
        return face / 2;
    }

    constexpr glm::ivec3 getNormalAxisDirection(int face) {
        return Axis::getDirection(getNormalAxis(face), isPositiveOrientated(face));
    }

    enum class FaceEnum : int {
        Right = Right,
        Left = Left,
        Top = Top,
        Bottom = Bottom,
        Front = Front,
        Back = Back
    };

    constexpr FaceEnum toEnum(int face) {
        return static_cast<FaceEnum>(face);
    }

    constexpr int toValue(FaceEnum faceEnum) {
        return static_cast<int>(faceEnum);
    }

    constexpr glm::ivec3 getOriginNormalized(int face) {
        switch (toEnum(face)) {
            case FaceEnum::Right:
                return {0, 0, 1};
            case FaceEnum::Left:
                return {0, 0, 0};
            case FaceEnum::Top:
                return {0, 0, 1};
            case FaceEnum::Bottom:
            case FaceEnum::Front:
                return {0, 0, 0};
            case FaceEnum::Back:
                return {1, 0, 0};
        }
    }

    constexpr glm::ivec3 getRightDirection(int face) {
        switch (toEnum(face)) {
            case FaceEnum::Right:
                return {0, 0, -1};
            case FaceEnum::Left:
                return {0, 0, 1};
            case FaceEnum::Top:
            case FaceEnum::Bottom:
            case FaceEnum::Front:
                return {1, 0, 0};
            case FaceEnum::Back:
                return {-1, 0, 0};
        }
    }

    constexpr glm::ivec3 getTopDirection(int face) {
        switch (toEnum(face)) {
            case FaceEnum::Right:
            case FaceEnum::Left:
            case FaceEnum::Front:
            case FaceEnum::Back:
                return {0, 1, 0};
            case FaceEnum::Top:
                return {0, 0, -1};
            case FaceEnum::Bottom:
                return {0, 0, 1};
        }
    }
}

#endif //SHITCRAFT_CONSTANTS_H
