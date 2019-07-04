#include <GL/glew.h>
#include <GL/freeglut.h>
#include "include/Angel.h"
#include "Mesh_Painter.h"
#include "mesh.h"
#pragma comment(lib, "glew32.lib")

#define PI 3.1415926535898
#define WINDOW_HEIGHT 700
#define WINDOW_WIDTH 700
#define REFLESH 1				// ˢ�µ�ʱ����

My_Mesh* my_mesh_ball;
vec3 lightPos = vec3(100, 100, -30);
float window_cent_w = WINDOW_WIDTH / 2;
float window_cent_h = WINDOW_HEIGHT / 2;
float mouse_sense_x = 0.05;
float mouse_sense_y = 0.05;
float pitch = 0;					// ����ĸ�����
float yaw = 0;						// �����ƫ����
float move_speed = 1.0;				// �ƶ��ٶ�

std::vector<My_Mesh*> my_meshs;
Mesh_Painter* mp_;


// ���Ӳ���
vec3 scale_rate_desk = vec3(20.5, 20.5, 20.5);	// ������
vec3 translate_desk = vec3(3.5, 0.0, 0.65);		// ƫ�ƣ�x�������ʼ���Զ����y���������ƶ���z��������
vec3 rotate_angel_desk = vec3(-90, 0, 0);		// ģ�͵���ת��
// ���˲���
vec3 scale_rate_mingren = vec3(0.2, 0.2, 0.2);
vec3 translate_mingren = vec3(10.0, 330.0, 0.0); // z��������,y����ǰ��
vec3 rotate_angel_mingren = vec3(-90, -90, 0);
// è����
vec3 scale_rate_cat = vec3(0.2, 0.2, 0.2);
vec3 translate_cat = vec3(-200.0, 22.0, -110.0); // y�������£�x�������ң�z����ǰ��
vec3 rotate_angel_cat = vec3(0, 90, 0);
//�����
vec3 scale_rate_ball = vec3(0.15, 0.15, 0.15);
vec3 translate_ball = vec3(50.0, 18.5, 270.0); 
vec3 rotate_angel_ball = vec3(0, 0, 0);
// �������
vec3 scale_rate_ground = vec3(20, 1, 20);
vec3 translate_ground = vec3(0, -0.01, 0);		// z���Ƶ������ң�y���Ƶ�������
vec3 rotate_angel_ground = vec3(0, 0, 0);
// ǰǽ����
vec3 scale_rate_frontwall = vec3(20, 1, 20);
vec3 translate_frontwall = vec3(4, 95, 0);		// y����ǰǽ���ʼ���Զ��
vec3 rotate_angel_frontwall = vec3(180, 0, 90);
// ��ǽ����
vec3 scale_rate_leftwall = vec3(20, 1, 20);
vec3 translate_leftwall = vec3(0, 90, 0);		// x���������ƶ���y���������ƶ�
vec3 rotate_angel_leftwall = vec3(-90, 0, 90);
// ��ǽ����
vec3 scale_rate_rightwall = vec3(20, 1, 20);
vec3 translate_rightwall = vec3(0, -90, 0);		// x���������ƶ���y���������ƶ�
vec3 rotate_angel_rightwall = vec3(-90, 0, 90);
// ��ǽ����
vec3 scale_rate_backwall = vec3(20, 1, 20);
vec3 translate_backwall = vec3(0, -95, 0);		// y����ǰǽ���ʼ���Զ��
vec3 rotate_angel_backwall = vec3(180, 0, 90);
// ��ղ���
vec3 scale_rate_sky = vec3(20, 1, 20);
vec3 translate_sky = vec3(0, 100, 0);			// z���������ƶ�
vec3 rotate_angel_sky = vec3(0, 0, 0);

// ���λ������
float eye_x = 0;
float eye_y = 20;
float eye_z = 0;
// ������Ϸ���
float up_x = 0;
float up_y = 1;
float up_z = 0;
// �������ķ���
float at_x = 0;
float at_y = 0;
float at_z = 1;
// �ı����������delta
float eye_x_delta = 1.5;
float eye_y_delta = 1.5;
float eye_z_delta = 1.5;
vec4 eye_delta;
float at_theta_delta = 0.05 * PI;
// �������Ҫ��
vec4 eye = vec4(eye_x, eye_y, eye_z, 1);	// �����������
vec4 at = vec4(at_x, at_y, at_z, 1);		// �������ķ���
vec4 up = vec4(up_x, up_y, up_z, 0);		// ������Ϸ���

// ==========================������������任����============================
namespace Camera {
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projMatrix;
	// ��������ͶӰ����
	mat4 ortho(const GLfloat left, const GLfloat right,
		const GLfloat bottom, const GLfloat top,
		const GLfloat zNear, const GLfloat zFar) {
		vec4 m1 = vec4(2 / (right - left), 0, 0, -1 * (right + left) / (right - left));
		vec4 m2 = vec4(0, 2 / (top - bottom), 0, -1 * (top + bottom) / (top - bottom));
		vec4 m3 = vec4(0, 0, -1 * 2 / (zFar - zNear), -1 * (zFar + zNear) / (zFar - zNear));
		vec4 m4 = vec4(0, 0, 0, 1);
		mat4 matrix = mat4(m1, m2, m3, m4);
		return matrix;
	}
	// ����͸��ͶӰ����
	mat4 perspective(const GLfloat fovy, const GLfloat aspect,
		const GLfloat zNear, const GLfloat zFar)
	{
		// TODO �밴��ʵ������ݲ�ȫ����۲����ļ���
		// ͸��ͶӰ������ppt
		GLfloat top = zNear*tan(fovy * DegreesToRadians);
		GLfloat bottom = -1 * top;
		GLfloat right = top*aspect;
		GLfloat left = -1 * right;

		vec4 m1 = vec4(zNear / right, 0, 0, 0);
		vec4 m2 = vec4(0, zNear / top, 0, 0);
		vec4 m3 = vec4(0, 0, -1 * (zFar + zNear) / (zFar - zNear), -1 * 2 * zFar*zNear / (zFar - zNear));
		vec4 m4 = vec4(0, 0, -1, 0);
		return mat4(m1, m2, m3, m4);
	}
	// ��������۲����
	mat4 lookAt(const vec4& eye, const vec4& at, const vec4& up) {
		vec4 n = normalize(eye - at);
		vec4 u = normalize(vec4(cross(up, n), 0));
		vec4 v = normalize(vec4(cross(n, u), 0));
		vec4 t = vec4(0.0, 0.0, 0.0, 1.0);

		// ����۲�任����
		mat4 viewMatrix = mat4(u, v, n, t);
		// ƽ�ƾ��󣬽������ԭ���Ƶ��ӵ�
		mat4 eyeMatrix = Translate(-eye.x, -eye.y, -eye.z);
		// ����۲������ƽ�ƾ���͹۲�任���������
		mat4 matrix = viewMatrix*eyeMatrix;

		return matrix;
	}
}
// ==========================������������任����============================

// ============================�ӿڡ������б�==============================
void init();
void display();
void printInfo();
void mouse1(int button, int state, int x, int y);
void mouse2(int x, int y);
void keyboard(unsigned char key, int x, int y);
void idle();
void reshape(GLsizei w, GLsizei h);
void clean();
void camera_at_rotate(float xoffset, float yoffset);
// ============================�ӿڡ������б�==============================

void init() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glClearColor(1, 1, 1, 1.0);
	//ShowCursor(FALSE);

	vec4 tempat = vec4(vec3(at.x, at.y, at.z) + vec3(eye.x, eye.y, eye.z), 1);
	Camera::modelMatrix = mat4(1.0);
	Camera::viewMatrix = Camera::lookAt(eye, tempat, up);
	Camera::projMatrix = Camera::perspective(40, 1.0, 0.1, 250);
	//Camera::projMatrix = Camera::ortho(-30, 30, -30, 30, 0.1, 100);

	glutWarpPointer(window_cent_w, window_cent_h);

	// OpenGL��Ӧ״̬����
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glutSetCursor(GLUT_CURSOR_NONE);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// ���¼�����ͼ����
	vec4 tempat = vec4(vec3(at.x, at.y, at.z) + vec3(eye.x, eye.y, eye.z), 1);
	Camera::viewMatrix = Camera::lookAt(eye, tempat, up);
	mp_->draw_meshes(Camera::viewMatrix);
	glutSwapBuffers();
};


// ��������������
void mouse1(int button, int state, int x, int y) {
	
}

// ������
void mouse2(int x, int y) {
	float x_delta = x - window_cent_w;
	float y_delta = y - window_cent_h;
	x_delta *= mouse_sense_x;
	y_delta *= mouse_sense_y;
	// ��ת
	camera_at_rotate(x_delta, y_delta);
	// ʹ������
	glutWarpPointer(window_cent_w, window_cent_h);
}

// ���̼���
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 033:
		exit(EXIT_SUCCESS);
		break;
	case 'q':
		exit(EXIT_SUCCESS);
		break;
	case 'w':
		eye_delta = vec4(normalize(vec3(at.x, 0, at.z))* move_speed, 0);
		eye += eye_delta;
		break;
	case 's':
		eye_delta = vec4((-normalize(vec3(at.x, 0, at.z)))*move_speed, 0);
		eye += eye_delta;
		break;
	case 'a':
		eye_delta = vec4(-normalize(cross(vec3(at.x, 0, at.z), vec3(up.x, up.y, up.z))) * move_speed, 0);
		eye += eye_delta;
		break;
	case 'd':
		eye_delta = vec4(normalize(cross(vec3(at.x, 0, at.z), vec3(up.x, up.y, up.z))) * move_speed, 0);
		eye += eye_delta;
		break;
	case 'p':
		my_mesh_ball->set_is_bounce(1);
		break;
	case 'o':
		my_mesh_ball->set_is_bounce(0);
		break;
	}
}

// ���лص�����
void idle() {
	glutPostRedisplay();
}

void update_window(int) {
	glutPostRedisplay();
	glutTimerFunc(REFLESH, update_window, 0);
}

void camera_at_rotate(float xoffset, float yoffset) {
	pitch += yoffset;
	yaw += xoffset;
	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	if (yaw > 180.0f) {
		yaw = -180.0f;
	}
	if (yaw < -180.0f) {
		yaw = 180.0f;
	}

	at.x = cos(pitch * DegreesToRadians) * cos(yaw * DegreesToRadians);
	at.y = -sin(pitch * DegreesToRadians);
	at.z = cos(pitch * DegreesToRadians) * sin(yaw * DegreesToRadians);
	at = normalize(at);
}

// �������ô���
void reshape(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);
}

void clean() {
	
}

// �����ʾ��Ϣ
void printInfo() {

}

// ��ʼ�������Լ�����
void initAndDrawMesh() {
	mp_ = new Mesh_Painter;
	mp_->set_lightPos(lightPos);
	mp_->set_matrix(Camera::modelMatrix, Camera::projMatrix);

	// ����
	My_Mesh* my_mesh_ground = new My_Mesh;
	my_mesh_ground->load_obj("data/ground.obj");
	my_mesh_ground->set_texture_file("data/grass.jpg");
	my_mesh_ground->set_scale_rate(scale_rate_ground);
	my_mesh_ground->set_translate(translate_ground);
	my_mesh_ground->set_rotate_angel(rotate_angel_ground);
	my_mesh_ground->set_is_need_shadow(0);
	my_mesh_ground->set_is_bounce(0);
	my_mesh_ground->set_shader("temp_vshader.glsl", "temp_fshader.glsl");
	my_meshs.push_back(my_mesh_ground);
	mp_->add_mesh(my_mesh_ground);
	// ���
	My_Mesh* my_mesh_sky = new My_Mesh;
	my_mesh_sky->load_obj("data/ground.obj");
	my_mesh_sky->set_texture_file("data/sky.jpg");
	my_mesh_sky->set_scale_rate(scale_rate_sky);
	my_mesh_sky->set_translate(translate_sky);
	my_mesh_sky->set_rotate_angel(rotate_angel_sky);
	my_mesh_sky->set_is_need_shadow(0);
	my_mesh_sky->set_is_bounce(0);
	my_mesh_sky->set_shader("temp_vshader.glsl", "temp_fshader.glsl");
	my_meshs.push_back(my_mesh_sky);
	mp_->add_mesh(my_mesh_sky);
	// ǰǽ
	My_Mesh* my_mesh_frontwall = new My_Mesh;
	my_mesh_frontwall->load_obj("data/ground.obj");
	my_mesh_frontwall->set_texture_file("data/wall.jpg");
	my_mesh_frontwall->set_scale_rate(scale_rate_frontwall);
	my_mesh_frontwall->set_translate(translate_frontwall);
	my_mesh_frontwall->set_rotate_angel(rotate_angel_frontwall);
	my_mesh_frontwall->set_is_need_shadow(0);
	my_mesh_frontwall->set_is_bounce(0);
	my_mesh_frontwall->set_shader("temp_vshader.glsl", "temp_fshader.glsl");
	my_meshs.push_back(my_mesh_frontwall);
	mp_->add_mesh(my_mesh_frontwall);
	// ��ǽ
	My_Mesh* my_mesh_backwall = new My_Mesh;
	my_mesh_backwall->load_obj("data/ground.obj");
	my_mesh_backwall->set_texture_file("data/wall.jpg");
	my_mesh_backwall->set_scale_rate(scale_rate_backwall);
	my_mesh_backwall->set_translate(translate_backwall);
	my_mesh_backwall->set_rotate_angel(rotate_angel_backwall);
	my_mesh_backwall->set_is_need_shadow(0);
	my_mesh_backwall->set_is_bounce(0);
	my_mesh_backwall->set_shader("temp_vshader.glsl", "temp_fshader.glsl");
	my_meshs.push_back(my_mesh_backwall);
	mp_->add_mesh(my_mesh_backwall);
	// ��ǽ
	My_Mesh* my_mesh_leftwall = new My_Mesh;
	my_mesh_leftwall->load_obj("data/ground.obj");
	my_mesh_leftwall->set_texture_file("data/leftwall.jpg");
	my_mesh_leftwall->set_scale_rate(scale_rate_leftwall);
	my_mesh_leftwall->set_translate(translate_leftwall);
	my_mesh_leftwall->set_rotate_angel(rotate_angel_leftwall);
	my_mesh_leftwall->set_is_need_shadow(0);
	my_mesh_leftwall->set_is_bounce(0);
	my_mesh_leftwall->set_shader("temp_vshader.glsl", "temp_fshader.glsl");
	my_meshs.push_back(my_mesh_leftwall);
	mp_->add_mesh(my_mesh_leftwall);
	// ��ǽ
	My_Mesh* my_mesh_rightwall = new My_Mesh;
	my_mesh_rightwall->load_obj("data/ground.obj");
	my_mesh_rightwall->set_texture_file("data/leftwall.jpg");
	my_mesh_rightwall->set_scale_rate(scale_rate_rightwall);
	my_mesh_rightwall->set_translate(translate_rightwall);
	my_mesh_rightwall->set_rotate_angel(rotate_angel_rightwall);
	my_mesh_rightwall->set_is_need_shadow(0);
	my_mesh_rightwall->set_is_bounce(0);
	my_mesh_rightwall->set_shader("temp_vshader.glsl", "temp_fshader.glsl");
	my_meshs.push_back(my_mesh_rightwall);
	mp_->add_mesh(my_mesh_rightwall);
	//// ����
	//My_Mesh* my_mesh_desk = new My_Mesh;
	//my_mesh_desk->load_obj("data/table.obj");
	//my_mesh_desk->set_texture_file("data/table.png");
	//my_mesh_desk->set_scale_rate(scale_rate_desk);
	//my_mesh_desk->set_translate(translate_desk);
	//my_mesh_desk->set_rotate_angel(rotate_angel_desk);
	//my_mesh_desk->set_is_need_shadow(1);
	//my_mesh_desk->set_is_bounce(0);
	//my_mesh_desk->set_shader("temp_vshader.glsl", "temp_fshader.glsl");
	//my_meshs.push_back(my_mesh_desk);
	//mp_->add_mesh(my_mesh_desk);
	// ����naludo
	My_Mesh* my_mesh_mingren = new My_Mesh;
	my_mesh_mingren->load_obj("data/mingren.obj");
	my_mesh_mingren->set_texture_file("data/mingren.png");
	my_mesh_mingren->set_scale_rate(scale_rate_mingren);
	my_mesh_mingren->set_translate(translate_mingren);
	my_mesh_mingren->set_rotate_angel(rotate_angel_mingren);
	my_mesh_mingren->set_is_need_shadow(1);
	my_mesh_mingren->set_is_bounce(0);
	my_mesh_mingren->set_shader("temp_vshader.glsl", "temp_fshader.glsl");
	my_meshs.push_back(my_mesh_mingren);
	mp_->add_mesh(my_mesh_mingren);
	// è
	My_Mesh* my_mesh_cat = new My_Mesh;
	my_mesh_cat->load_obj("data/mycat.obj");
	my_mesh_cat->set_texture_file("data/mycat.jpg");
	my_mesh_cat->set_scale_rate(scale_rate_cat);
	my_mesh_cat->set_translate(translate_cat);
	my_mesh_cat->set_rotate_angel(rotate_angel_cat);
	my_mesh_cat->set_is_need_shadow(1);
	my_mesh_cat->set_is_bounce(0);
	my_mesh_cat->set_shader("temp_vshader.glsl", "temp_fshader.glsl");
	my_meshs.push_back(my_mesh_cat);
	mp_->add_mesh(my_mesh_cat);
	// ��
	my_mesh_ball = new My_Mesh;
	my_mesh_ball->load_obj("data/basketball.obj");
	my_mesh_ball->set_texture_file("data/basketball.jpg");
	my_mesh_ball->set_scale_rate(scale_rate_ball);
	my_mesh_ball->set_translate(translate_ball);
	my_mesh_ball->set_rotate_angel(rotate_angel_ball);
	my_mesh_ball->set_is_need_shadow(1);
	my_mesh_ball->set_is_bounce(1);
	my_mesh_ball->set_bounce_max_height(25);
	my_mesh_ball->set_bounce_speed(0.21);
	my_mesh_ball->set_shader("temp_vshader.glsl", "temp_fshader.glsl");
	my_meshs.push_back(my_mesh_ball);
	mp_->add_mesh(my_mesh_ball);

	mp_->init_shaders();
	mp_->update_vertex_buffer();
	mp_->update_texture();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(700, 700);
	glutCreateWindow("��ĩ����ҵ_����_2016150051");
	glewInit();

	init();
	printInfo();

	initAndDrawMesh();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	//glutMotionFunc(mouse2); // �϶����������������
	//glutMouseFunc(mouse1);   // ����������������
	glutPassiveMotionFunc(mouse2);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(REFLESH, update_window, 0);
	glutMainLoop();

	clean();

	return 0;
}