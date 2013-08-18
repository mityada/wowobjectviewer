attribute vec3 position;
attribute vec3 normal;
attribute vec2 texcoord;
attribute vec4 boneindices;
attribute vec4 boneweights;

uniform mat4 mvpMatrix;
uniform mat3 normalMatrix;
uniform mat4 bones[100];
uniform mat4 textureMatrix;

varying vec2 v_texcoord;
varying vec3 v_normal;

void main()
{
    vec4 animatedPosition;
    animatedPosition = (bones[int(boneindices.x * 255.0)] * vec4(position, 1.0)) * (boneweights.x);
    animatedPosition += (bones[int(boneindices.y * 255.0)] * vec4(position, 1.0)) * (boneweights.y);
    animatedPosition += (bones[int(boneindices.z * 255.0)] * vec4(position, 1.0)) * (boneweights.z);
    animatedPosition += (bones[int(boneindices.w * 255.0)] * vec4(position, 1.0)) * (boneweights.w);
    
    gl_Position = mvpMatrix * vec4(animatedPosition.xyz, 1.0);

    v_texcoord = (textureMatrix * vec4(texcoord, 1.0, 1.0)).xy;
    v_normal = normalMatrix * normal;
}
