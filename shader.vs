attribute vec3 position;
attribute vec2 texcoord;
attribute vec4 boneindices;
attribute vec4 boneweights;

uniform mat4 model;
uniform mat4 viewProjection;
uniform mat4 bones[100];
uniform mat4 textureMatrix;

varying vec2 v_texcoord;

void main()
{
    vec4 animatedPosition;
    animatedPosition = (bones[int(boneindices.x * 255.0)] * vec4(position, 1.0)) * (boneweights.x);
    animatedPosition += (bones[int(boneindices.y * 255.0)] * vec4(position, 1.0)) * (boneweights.y);
    animatedPosition += (bones[int(boneindices.z * 255.0)] * vec4(position, 1.0)) * (boneweights.z);
    animatedPosition += (bones[int(boneindices.w * 255.0)] * vec4(position, 1.0)) * (boneweights.w);
    
    gl_Position = viewProjection * model * vec4(animatedPosition.xyz, 1.0);

    v_texcoord = (textureMatrix * vec4(texcoord, 1.0, 1.0)).xy;
}
