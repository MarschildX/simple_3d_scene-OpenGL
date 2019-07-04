#include "mesh.h"
#include<sstream>
#include <fstream>
#include <iosfwd>
#include <algorithm>
#include <gl/GL.h>
#include <math.h>
#include <algorithm>
#include <iostream>


My_Mesh::My_Mesh()
{

	vTranslation[0] = Theta[0] = 0;
	vTranslation[1] = Theta[1] = 0;
	vTranslation[2] = Theta[2] = 0;
	Theta[0] = 45;
	current_height_ = 0;
	bounce_dir_ = 1;	// 向上跳动
}


My_Mesh::~My_Mesh()
{
	
}

void My_Mesh::load_obj(std::string obj_File)
{
	//请在此添加代码实现对含有UV坐标的obj文件的读取
	if (obj_File.empty()) return;

	std::ifstream in;
	in.open(obj_File);
	std::string str;
	std::vector<vec2> uv;//暂存uv
	std::vector<vec3> vn;//暂存vn
	bool setVnSize = 0;//是否设置法向量和颜色数组的大小
	std::string head;//读取掉行头

					 //按行读取数据
	while (getline(in, str))
	{
		//防止使用str[0]和str[1]时越界
		if (str.size() < 2)
			continue;

		if (str[0] == 'v')
		{

			if (str[1] == 't')
			{
				std::istringstream inp(str);//从str读取字符

				vec2 point;
				inp >> head >> point.x >> point.y;
				uv.push_back(point);//暂存到uv数组中
			}
			else if (str[1] == 'n')
			{
				vec3 n;
				std::istringstream inp(str);
				inp >> head >> n.x >> n.y >> n.z;
				vn.push_back(n);//暂存法向量到vn数组中
			}
			else
			{
				float x, y, z;
				std::istringstream inp(str);
				inp >> head >> x >> y >> z;
				m_vertices_.push_back(x);//存储定点
				m_vertices_.push_back(y);
				m_vertices_.push_back(z);
			}

		}
		else if (str[0] == 'f')
		{
			//预先设定数组大小以便直接利用索引存储法向量、颜色
			if (setVnSize == 0)
			{
				m_normals_.resize(m_vertices_.size());
				m_color_list_.resize(m_vertices_.size());
				setVnSize = 1;
			}
			std::istringstream inp(str);

			inp >> head;
			for (int i = 0; i < 3; i++)
			{
				int VIndex, index;
				//点索引
				inp >> VIndex;
				VIndex--;//下标从0开始
				inp.get();
				m_faces_.push_back(VIndex);
				//vt索引
				inp >> index;
				inp.get();
				index--;
				//从数组uv中获得坐标
				m_vt_list_.push_back(uv[index].x);
				m_vt_list_.push_back(uv[index].y);
				//法向量索引
				inp >> index;
				index--;
				//从数组vn获得法向量
				m_normals_[VIndex * 3] = vn[index].x;
				m_normals_[VIndex * 3 + 1] = vn[index].y;
				m_normals_[VIndex * 3 + 2] = vn[index].z;
				//压入颜色
				float r;
				float g;
				float b;
				My_Mesh::normal_to_color(vn[index].x, vn[index].y, vn[index].z, r, g, b);
				m_color_list_[VIndex * 3] = r;
				m_color_list_[VIndex * 3 + 1] = g;
				m_color_list_[VIndex * 3 + 2] = b;

			}
		}
	}
	in.close();
	//设定box
	this->m_center_ = point3f(0, 0, 0);
	this->m_min_box_ = point3f(-5, -5, -5);
	this->m_max_box_ = point3f(5, 5, 5);
	//this->m_min_box_ = point3f(-1, -1, -1);
	//this->m_max_box_ = point3f(1, 1, 1);
};


void My_Mesh::normal_to_color(float nx, float ny, float nz, float& r, float& g, float& b)
{
	r = float(std::min(std::max(0.5 * (nx + 1.0), 0.0), 1.0));
	g = float(std::min(std::max(0.5 * (ny + 1.0), 0.0), 1.0));
	b = float(std::min(std::max(0.5 * (nz + 1.0), 0.0), 1.0));
};

const VtList&  My_Mesh::get_vts()
{
	return this->m_vt_list_;
};
void My_Mesh::clear_data()
{
	m_vertices_.clear();
	m_normals_.clear();
	m_faces_.clear();
	m_color_list_.clear();
	m_vt_list_.clear();
};
void My_Mesh::get_boundingbox(point3f& min_p, point3f& max_p) const
{
	min_p = this->m_min_box_;
	max_p = this->m_max_box_;
};
const STLVectorf&  My_Mesh::get_colors()
{
	return this->m_color_list_;
};
const VertexList& My_Mesh::get_vertices()
{
	return this->m_vertices_;
};
const NormalList& My_Mesh::get_normals()
{
	return this->m_normals_;
};
const FaceList&   My_Mesh::get_faces()
{
	return this->m_faces_;
};

int My_Mesh::num_faces()
{
	return this->m_faces_.size()/3;
};
int My_Mesh::num_vertices()
{
	return this->m_vertices_.size()/3;
};

const point3f& My_Mesh::get_center()
{
	return this->m_center_;
};


void My_Mesh::generate_cylinder(int num_division, float height)
{
	this->clear_data();
	this->m_center_ = point3f(0, 0, 0);
	this->m_min_box_ = point3f(-1, -1, -height);
	this->m_max_box_ = point3f(1, 1, height);

	int num_samples = num_division;
	float z = -height;
	float pi = 3.14159265;
	float step = 1.0 * 360 / num_samples;
	float rr = pi / 180;
	//圆柱体Z轴向上，按cos和sin生成x，y坐标
	for (int i = 0; i < num_samples; i++)
	{
		float r_r_r = i * step * rr;
		float x = cos(r_r_r);
		float y = sin(r_r_r);
		m_vertices_.push_back(x);
		m_vertices_.push_back(y);
		m_vertices_.push_back(z);

		m_normals_.push_back(x);
		m_normals_.push_back(y);
		m_normals_.push_back(0);
		//法线由里向外
		float r;
		float g;
		float b;
		My_Mesh::normal_to_color(x, y, z, r, g, b);
		//这里采用法线来生成颜色，学生可以自定义自己的颜色生成方式
		m_color_list_.push_back(r);
		m_color_list_.push_back(g);
		m_color_list_.push_back(b);
	}

	z = height;
	//圆柱体Z轴向上，按cos和sin生成x，y坐标，
	for (int i = 0; i < num_samples; i++)
	{
		float r_r_r = i * step * rr;
		float x = cos(r_r_r);
		float y = sin(r_r_r);
		m_vertices_.push_back(x);
		m_vertices_.push_back(y);
		m_vertices_.push_back(z);

		m_normals_.push_back(x);
		m_normals_.push_back(y);
		m_normals_.push_back(0);
		//法线由里向外
		float r;
		float g;
		float b;
		My_Mesh::normal_to_color(x, y, z, r, g, b);
		m_color_list_.push_back(r);
		m_color_list_.push_back(g);
		m_color_list_.push_back(b);
		//这里采用法线来生成颜色，学生可以自定义自己的颜色生成方式
	}

	for (int i = 0; i < num_samples; i++)
	{
		m_faces_.push_back(i);
		m_faces_.push_back((i + 1) % num_samples);
		m_faces_.push_back((i + num_samples) % (num_samples)+num_samples);

		m_faces_.push_back((i + num_samples) % (num_samples)+num_samples);
		m_faces_.push_back((i + 1) % num_samples);
		m_faces_.push_back((i + num_samples + 1) % (num_samples)+num_samples);
		//生成三角面片

		m_vt_list_.push_back(1.0 * i / num_samples);
		m_vt_list_.push_back(0.0);
		//纹理坐标
		m_vt_list_.push_back(1.0 * ((i + 1)) / num_samples);
		m_vt_list_.push_back(0.0);
		//纹理坐标
		m_vt_list_.push_back(1.0 * i / num_samples);
		m_vt_list_.push_back(1.0);
		//纹理坐标

		m_vt_list_.push_back(1.0 * i / num_samples);
		m_vt_list_.push_back(1.0);
		//纹理坐标
		m_vt_list_.push_back(1.0 * ((i + 1)) / num_samples);
		m_vt_list_.push_back(0.0);
		//纹理坐标
		m_vt_list_.push_back(1.0 * ((i + 1)) / num_samples);
		m_vt_list_.push_back(1.0);
		//纹理坐标
	}

};


void My_Mesh::set_texture_file(std::string s)
{
	this->texture_file_name = s;
};
std::string My_Mesh::get_texture_file()
{
	return this->texture_file_name;
};

void My_Mesh::set_translate(float x, float y, float z)
{
	vTranslation[0] = x;
	vTranslation[1] = y;
	vTranslation[2] = z;

};
void My_Mesh::get_translate(float& x, float& y, float& z)
{
	x = vTranslation[0];
	y = vTranslation[1];
	z = vTranslation[2];
};

void My_Mesh::set_theta(float x, float y, float z)
{
	Theta[0] = x;
	Theta[1] = y;
	Theta[2] = z;
};
void My_Mesh::get_theta(float& x, float& y, float& z)
{
	x = Theta[0];
	y = Theta[1];
	z = Theta[2];
};

void My_Mesh::set_theta_step(float x, float y, float z)
{
	Theta_step[0] = x;
	Theta_step[1] = y;
	Theta_step[2] = z;
};

void My_Mesh::add_theta_step()
{
	Theta[0] = Theta[0] + Theta_step[0];
	Theta[1] = Theta[1] + Theta_step[1];
	Theta[2] = Theta[2] + Theta_step[2];
};

void My_Mesh::set_shader(std::string vshader_file_name, std::string fshader_file_name) {
	vshader_file_name_ = vshader_file_name;
	fshader_file_name_ = fshader_file_name;
};

std::string My_Mesh::get_vshader_file_name() {
	return this->vshader_file_name_;
};

std::string My_Mesh::get_fshader_file_name() {
	return this->fshader_file_name_;
};

void My_Mesh::set_scale_rate(vec3 scale_rate) {
	this->scale_rate_ = scale_rate;
};

vec3 My_Mesh::get_scale_rate() {
	return this->scale_rate_;
};

void My_Mesh::set_translate(vec3 translate) {
	this->translate_ = translate;
}

vec3 My_Mesh::get_translate() {
	return this->translate_;
}

void My_Mesh::set_rotate_angel(vec3 angel) {
	this->rotate_angel_ = angel;
}

vec3 My_Mesh::get_rotate_angel() {
	return this->rotate_angel_;
}

void My_Mesh::set_is_need_shadow(int is_need) {
	this->is_need_shadow_ = is_need;
}

int My_Mesh::get_is_need_shadow() {
	return this->is_need_shadow_;
}

void My_Mesh::set_bounce_max_height(float bounce_max_height) {
	this->bounce_max_height_ = bounce_max_height;
}

float My_Mesh::get_bounce_max_height() {
	return this->bounce_max_height_;
}

void My_Mesh::set_bounce_speed(float bounce_speed) {
	this->bounce_speed_ = bounce_speed;
}

float My_Mesh::get_bounce_speed() {
	return this->bounce_speed_;
}

void My_Mesh::set_current_height(float height) {
	this->current_height_ = height;
}

float My_Mesh::get_current_height() {
	return this->current_height_;
}

void My_Mesh::set_bounce_dir(int dir) {
	this->bounce_dir_ = dir;
}

int My_Mesh::get_bounce_dir() {
	return this->bounce_dir_;
}

void My_Mesh::set_is_bounce(int is_bounce) {
	this->is_bounce_ = is_bounce;
}

int My_Mesh::get_is_bounce() {
	return this->is_bounce_;
}