#pragma once

#include <string>
#include <vector>
#include "FreeImage.h"
#include "include/Angel.h"
#include "mesh.h"
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "FreeImage.lib")

class Mesh_Painter {
public:
	Mesh_Painter();
	~Mesh_Painter();
	void draw_meshes(mat4 viewMatrix);
	void update_vertex_buffer();
	void update_texture();

	void init_shaders();
	void add_mesh(My_Mesh*);
	void clear_mehs();
	void set_lightPos(vec3 lightPos);
	void set_matrix(mat4 modelMatrix, mat4 projMatrix);
	mat4 bounce(int index);
private:
	void load_texture_FreeImage(std::string file_name, GLuint& m_texName);

	std::vector<GLuint> textures_all;
	std::vector<GLuint> program_all;
	std::vector<GLuint> vao_all;
	std::vector<GLuint> buffer_all;
	std::vector<GLuint> vPosition_all;
	std::vector<GLuint> vColor_all;
	std::vector<GLuint> vTexCoord_all;
	std::vector<GLuint> vNormal_all;
	std::vector<My_Mesh*> m_my_meshes_;
	vec3 lightPos_;
	std::vector<GLuint> vao_shadow_all;
	mat4 modelMatrix_;
	mat4 viewMatrix_;
	mat4 projMatrix_;
};