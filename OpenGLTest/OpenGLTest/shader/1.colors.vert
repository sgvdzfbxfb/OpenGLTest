#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexture;
layout (location = 2) in vec3 aNormal;

out vec3 FragPos;
out vec2 Texture;
out vec3 Normal;

uniform mat4 model;
uniform mat4 model2;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    if (gl_InstanceID == 0) {
        FragPos = vec3(model * vec4(aPos, 1.0));
        Normal = mat3(transpose(inverse(model))) * aNormal;
        Texture = aTexture;
    }
    else if (gl_InstanceID == 1) {
        FragPos = vec3(model2 * vec4(aPos, 1.0));
        Normal = mat3(transpose(inverse(model2))) * aNormal;
        Texture = aTexture;
    }
    gl_Position = projection * view * vec4(FragPos, 1.0);
}