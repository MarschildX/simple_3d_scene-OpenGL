#pragma once

#include <vector>
#include <string>
#include "Angel.h"
struct  point3f
{
	float x;
	float y;
	float z;
	point3f()
	{
		x = 0;
		y = 0;
		z = 0;
	};
	point3f(float xx, float yy, float zz)
	{
		x = xx;
		y = yy;
		z = zz;
	};

	float distance(const point3f& p)
	{
		float d = (x - p.x)*(x - p.x);
		d += (y - p.y)*(y - p.y);
		d += (z - p.z)*(z - p.z);

		return sqrt(d);
	};
};

typedef std::vector<float> VertexList;
typedef std::vector<float> NormalList;
typedef std::vector<float> VtList;

typedef std::vector<float> STLVectorf;
typedef std::vector<int> STLVectori;

typedef std::vector<std::pair<int, int> > Edges;
typedef std::vector<unsigned int> FaceList;






class My_Mesh
{
public:
	My_Mesh();
	~My_Mesh();

	void load_obj(std::string obj_File);
	void set_texture_file(std::string s);
	std::string get_texture_file();

	const VertexList& get_vertices();
	const NormalList& get_normals();
	const FaceList&   get_faces();
	const STLVectorf&   get_colors();
	const VtList&   get_vts();

	int num_faces();
	int num_vertices();

	const point3f& get_center();
	void get_boundingbox(point3f& min_p, point3f& max_p) const;


	void generate_cylinder(int num_division = 100, float height = 2);

	static void normal_to_color(float, float, float, float&, float&, float&);
	void set_translate(float, float, float);
	void get_translate(float& x, float& y, float& z);

	void set_shader(std::string vshader_file_name, std::string fshader_file_name);

	std::string get_vshader_file_name();
	std::string get_fshader_file_name();

	void set_scale_rate(vec3 scale_rate);
	vec3 get_scale_rate();
	void set_translate(vec3 translate);
	vec3 get_translate();
	void set_rotate_angel(vec3 angel);
	vec3 get_rotate_angel();
	void set_is_need_shadow(int is_need);
	int get_is_need_shadow();
	void set_bounce_max_height(float bounce_max_height);
	float get_bounce_max_height();
	void set_bounce_speed(float bounce_speed);
	float get_bounce_speed();
	void set_current_height(float height);
	void set_bounce_dir(int dir);
	float get_current_height();
	int get_bounce_dir();
	void set_is_bounce(int is_bounce);
	int get_is_bounce();

private:
	void clear_data();
private:


	VertexList m_vertices_;
	NormalList m_normals_;
	FaceList   m_faces_;
	STLVectorf m_color_list_;
	VtList	m_vt_list_;
	point3f m_center_;
	point3f m_min_box_;
	point3f m_max_box_;

	std::string vshader_file_name_;
	std::string fshader_file_name_;

	vec3 scale_rate_;
	vec3 translate_;
	vec3 rotate_angel_;

	int is_need_shadow_;
	float bounce_max_height_;
	float bounce_speed_;
	float current_height_;
	int bounce_dir_;
	int is_bounce_;
public:
	void add_theta_step();
	void set_theta_step(float x, float y, float z);
	void set_theta(float x, float y, float z);

	void get_theta(float& x, float& y, float& z);

private:
	
	std::string texture_file_name;

	enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
	int      Axis = Xaxis;
	GLfloat  Theta[3];
	GLfloat  Theta_step[3];
	GLfloat vTranslation[3];

	
};

