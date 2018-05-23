#shader vertex
// a simple shader
#version 410

layout(location = 0) in vec4 Position;
layout(location = 2) in vec2 TexCoord;

out vec2 vTexCoord;

uniform mat4 ProjectionViewModel;

void main() {
	vTexCoord = TexCoord;
	gl_Position = ProjectionViewModel * Position;
}

#shader fragment
// a simple flat colour shader
#version 410

in vec2 vTexCoord;

uniform sampler2D diffuseTexture;

out vec4 FragColour;

void main() {
	FragColour = texture(diffuseTexture, vTexCoord);
}
