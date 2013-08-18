#ifndef M2_STRUCTURES_H
#define M2_STRUCTURES_H

#include <QtGlobal>
#include <QVector3D>

#define M2_MAGIC "MD20"

#pragma pack(push, 1)

struct M2Header
{
    char magic[4];
    quint8 version[4];
    quint32 nameLength;
    quint32 nameOffset;
    quint32 flags;
    quint32 sequencesCount;
    quint32 sequencesOffset;
    quint32 animationsCount;
    quint32 animationsOffset;
    quint32 animationLookupCount;
    quint32 animationLookupOffset;
    quint32 dCount;
    quint32 dOffset;
    quint32 bonesCount;
    quint32 bonesOffset;
    quint32 skeletalBoneLookupCount;
    quint32 skeletalBoneLookupOffset;
    quint32 verticesCount;
    quint32 verticesOffset;
    quint32 viewsCount;
    quint32 viewsOffset;
    quint32 colorsCount;
    quint32 colorsOffset;
    quint32 texturesCount;
    quint32 texturesOffset;
    quint32 transparencyCount;
    quint32 transparencyOffset;
    quint32 iCount;
    quint32 iOffset;
    quint32 textureAnimationsCount;
    quint32 textureAnimationsOffset;
    quint32 textureReplaceCount;
    quint32 textureReplaceOffset;
    quint32 renderFlagsCount;
    quint32 renderFlagsOffset;
    quint32 boneLookupCount;
    quint32 boneLookupOffset;
    quint32 textureLookupCount;
    quint32 textureLookupOffset;
    quint32 textureUnitsCount;
    quint32 textureUnitsOffset;
    quint32 transparencyLookupCount;
    quint32 transparencyLookupOffset;
    quint32 textureAnimationLookupCount;
    quint32 textureAnimationLookupOffset;
    float values[14];
    quint32 boundingTrianglesCount;
    quint32 boundingTrianglesOffset;
    quint32 boundingVerticesCount;
    quint32 boundingVerticesOffset;
    quint32 boundingNormalsCount;
    quint32 boundingNormalsOffset;
    quint32 attachmentsCount;
    quint32 attachmentsOffset;
    quint32 attachmentLookupCount;
    quint32 attachmentLookupOffset;
    quint32 attachments2Count;
    quint32 attachments2Offset;
    quint32 lightsCount;
    quint32 lightsOffset;
    quint32 camerasCount;
    quint32 camerasOffset;
    quint32 cameraLookupCount;
    quint32 cameraLookupOffset;
    quint32 ribbonEmittersCount;
    quint32 ribbonEmittersOffset;
    quint32 particleEmittersCount;
    quint32 particleEmittersOffset;
};

struct M2Animation
{
    quint32 id;
    quint32 startTime;
    quint32 endTime;
    float moveSpeed;
    quint32 loopType;
    quint32 flags;
    quint32 unknown1;
    quint32 unknown2;
    quint32 unknown3;
    QVector3D bounding[2];
    float radius;
    qint16 nextAnimation;
    quint16 unknown4;
};

struct M2AnimationBlock
{
    quint16 type;
    qint16 sequence;
    quint32 rangesCount;
    quint32 rangesOffset;
    quint32 timesCount;
    quint32 timesOffset;
    quint32 keysCount;
    quint32 keysOffset;
};

struct M2Bone
{
    qint32 animationId;
    quint32 flags;
    qint16 parent;
    quint16 geoset;
    M2AnimationBlock translation;
    M2AnimationBlock rotation;
    M2AnimationBlock scaling;
    QVector3D pivot;
};

struct M2Vertex
{
    float position[3];
    quint8 boneWeights[4];
    quint8 boneIndices[4];
    float normal[3];
    float textureCoordinates[2];
    float unknown[2];
};

struct M2View
{
    quint32 indicesCount;
    quint32 indicesOffset;
    quint32 trianglesCount;
    quint32 trianglesOffset;
    quint32 vertexPropertyCount;
    quint32 vertexPropertyOffset;
    quint32 submeshCount;
    quint32 submeshOffset;
    quint32 textureUnitsCount;
    quint32 textureUnitsOffset;
    quint32 unknown;
};

struct M2Submesh
{
    quint32 part;
    quint16 startingVertex;
    quint16 verticesCount;
    quint16 startingTriangle;
    quint16 trianglesCount;
    quint16 bonesCount;
    quint16 startingBone;
    quint16 maxBones;
    quint16 rootBone;
    float vector[3];
};

struct M2TextureUnit
{
    quint16 flags;
    qint16 renderOrder;
    quint16 submeshIndex;
    quint16 submeshIndex2;
    qint16 colorIndex;
    quint16 renderFlagIndex;
    quint16 textureUnitNumber;
    quint16 mode;
    quint16 textureIndex;
    quint16 textureUnitNumber2;
    quint16 transparencyIndex;
    quint16 textureAnimationId;
};

enum RenderFlag
{
    RENDER_FLAG_UNLIT       = 0x01,
    RENDER_FLAG_UNFOGGED    = 0x02,
    RENDER_FLAG_TWO_SIDED   = 0x04,
    RENDER_FLAG_UNKNOWN     = 0x08,
    RENDER_FLAG_NO_Z_WRITE  = 0x10
};

enum BlendingMode
{
    BM_OPAQUE,
    BM_TRANSPARENT,
    BM_ALPHA_BLEND,
    BM_ADDITIVE,
    BM_ADDITIVE_ALPHA,
    BM_MODULATE
};

struct M2RenderFlags
{
    quint16 flags;
    quint16 blending;
};

struct M2Color
{
    M2AnimationBlock color;
    M2AnimationBlock opacity;
};

struct M2Texture
{
    quint32 type;
    quint16 unknown;
    quint16 flags;
    quint32 fileNameLength;
    quint32 fileNameOffset;
};

struct M2TextureAnimation
{
    M2AnimationBlock translation;
    M2AnimationBlock rotation;
    M2AnimationBlock scaling;
};

#pragma pack(pop)

#endif
