#version 330 core

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vFaceIndecies;
in vec3 vColor;


out vec3 N;
out vec3 V;
out vec2 texCoord;
out vec4 normal;


uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform int vMode;

void main() {
	if (vMode == 0) {
		gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(vPosition, 1.0);
		// TODO 将顶点变换到相机坐标系下
		vec4 vertPos_cameraspace = viewMatrix*modelMatrix*vec4(vPosition, 1.0);
		// 对顶点坐标做透视投影
		V = vertPos_cameraspace.xyz / vertPos_cameraspace.w;
		// TODO 将法向量变换到相机坐标系下并传入片元着色器
		N = (viewMatrix * modelMatrix * vec4(vNormal, 0.0)).xyz;
		texCoord = vTexCoord;
		normal = vec4(vNormal, 0);
	}
	else if(vMode == 1){
		gl_Position = projMatrix*viewMatrix*modelMatrix*vec4(vPosition, 1.0);
	}
}