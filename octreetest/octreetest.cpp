#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/freeglut.h>

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>

#include <mye/algorithms/LooseOctree.h>
#include <mye/core/Camera.h>
#include <mye/math/Geometry.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace mye::algorithms;
using namespace mye::math;
using namespace mye::core;

struct __MYE_ALIGNED(16) Ball
{

	__MYE_DECLARE_ALIGNED_16

	Vector3 position;
	float   radius;
	Vector4 color;
	Vector3 velocity;
};

void Reshape(int, int);
void MainLoop(int);
void Render(void);
void Keyboard(unsigned char, int, int);
void Mouse(int, int, int, int);

void DrawAABB(const AABB & aabb);
void DrawBall(const Ball & ball);

bool CreateBall(void);
void ColorPicked(int x, int y);
void ColorReset(void);
void CreateObjects(void);
void DestroyObjects(void);
void MoveBalls(void);

glm::mat4 GetViewMatrix(void);

namespace mye
{

	namespace algorithms
	{

		template <>
		struct Boundings<Ball*, AABB>
		{

			inline AABB operator() (const Ball * b)
			{
				return AABB::FromCenterHalfExtents(b->position, b->radius);
			}

		};

		template <>
		struct Boundings<Ball*, Sphere>
		{

			inline Sphere operator() (const Ball * b)
			{
				return Sphere(b->position, b->radius);
			}

		};

	}

}

LooseOctree<Ball *, Sphere> g_octree;

glm::vec3           g_cameraPosition;
glm::quat           g_cameraOrientation;

glm::mat4           g_projMatrix;

bool                g_drawLooseOctree     = true;
bool                g_drawHalfLooseOctree = false;
bool                g_drawRay             = false;;
Ray                 g_ray;

bool                g_createObjects       = true;
bool                g_destroyObjects      = false;

std::vector<Ball*>  g_balls;

float               g_near = 0.1f;
float               g_far  = 100.0f;;

glm::vec2           g_viewportSize;


int main(int argc, char * argv[])
{

	g_octree.Create(Vector3(0), 5.0f, 2);

	g_cameraPosition    = glm::vec3(0, 0, -25.0f);
	g_cameraOrientation = glm::quat(0, 0, 1, 0);

	glutInit(&argc, argv);	

	glutInitWindowSize(1280, 720);
	glutCreateWindow("Octree test");

	glutReshapeFunc(Reshape);
	glutDisplayFunc(Render);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);

	glutShowWindow();

	glutTimerFunc(12, MainLoop, 0);
	glutMainLoop();

	return 0;

}

void MainLoop(int)
{

	if (g_createObjects)
	{
		CreateObjects();
	}

	if (g_destroyObjects)
	{
		DestroyObjects();
	}

	MoveBalls();
	Render();

	glutTimerFunc(12, MainLoop, 0);

}

void Render(void)
{

	glm::mat4 viewMatrix = GetViewMatrix();

	glLoadMatrixf(glm::value_ptr(viewMatrix));

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (g_drawLooseOctree)
	{

		g_octree.TraverseAABBs([] (const AABB & aabb)
		{
			DrawAABB(aabb);
		});

	}

	g_octree.Query(g_octree.GetAABB(),
				   [] (const Ball * ball)
	{
		DrawBall(*ball);
	});

	if (g_drawRay)
	{
		
		glColor3f(1, 0, 1);

		glBegin(GL_LINES);
		glVertex3fv(g_ray.GetOrigin().Data());
		glVertex3fv(g_ray.Evaluate(70.0f).Data());
		glEnd();

	}
	
	glDisable(GL_DEPTH_TEST);

	glColor3f(1, 1, 1);


	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0f, g_viewportSize.x, 0.0f, g_viewportSize.y);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	std::stringstream ss;

	ss << "Number of objects: " << g_octree.GetSize() << std::endl;

	glRasterPos2f(0.0f, g_viewportSize.y - 14);
	glutBitmapString(GLUT_BITMAP_HELVETICA_12, (unsigned char *) ss.str().c_str());

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glutSwapBuffers();

}

void Reshape(int w, int h)
{

	glViewport(0, 0, w, h);

	g_projMatrix = glm::perspective(glm::radians(90.0f), (float) w / h, g_near, g_far);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(g_projMatrix));

	glMatrixMode(GL_MODELVIEW);

	g_viewportSize = glm::vec2(w, h);

	glutPostRedisplay();

}

void Keyboard(unsigned char key, int x, int y)
{

	float speed  = 0.35f;
	float aSpeed = 0.2f;


	glm::vec3 forward = g_cameraOrientation * glm::vec3(0, 0, -1);
	glm::vec3 left    = g_cameraOrientation * glm::vec3(1, 0, 0);
	glm::vec3 up      = g_cameraOrientation * glm::vec3(0, 1, 0);

	switch (key)
	{

	case 'l':
		g_drawLooseOctree = !g_drawLooseOctree;
		break;

	case 'h':
		g_drawHalfLooseOctree = !g_drawHalfLooseOctree;
		break;

	case 'w':
	{
		
		g_cameraPosition = g_cameraPosition + speed * forward;
	}
		break;

	case 's':
	{
		g_cameraPosition = g_cameraPosition - speed * forward;
	}

		break;

	case 'a':
	{
		g_cameraPosition = g_cameraPosition + speed * left;
	}
		break;


	case 'd':
	{
		g_cameraPosition = g_cameraPosition - speed * left;
	}
		break;

	case 'q':
	{
		
		g_cameraPosition = g_cameraPosition - speed * up;
	}
		break;

	case 'e':
	{
		g_cameraPosition = g_cameraPosition + speed * up;
	}
		break;

	case 'x':
	{
		g_cameraOrientation = glm::normalize(glm::rotate(g_cameraOrientation, -aSpeed, up));
	}
		break;

	case 'c':
	{
		g_cameraOrientation = glm::normalize(glm::rotate(g_cameraOrientation, aSpeed, up));
	}
		break;

	case 'b':
		while (!CreateBall());
		break;

	case 'r':
		g_drawRay = true;
		break;

	case ' ':
		g_createObjects = !g_createObjects;
		break;

	case '-':
		g_destroyObjects = !g_destroyObjects;
		break;

	case '.':
		g_octree.Shrink();
		break;

	}

}

void Mouse(int button, int state, int x, int y)
{

	switch (button)
	{

	case GLUT_LEFT_BUTTON:

		if (state == GLUT_DOWN)
		{

			ColorReset();
			ColorPicked(x, y);
			break;

		}

	}

}
void DrawAABB(const AABB & aabb)
{

	glColor4f(0, 1, 0, 1);

	Vector3 aabbMin = aabb.GetMinimum();
	Vector3 aabbMax = aabb.GetMaximum();
	

	if (g_drawHalfLooseOctree)
	{

		Vector3 center = aabb.GetCenter();

		aabbMin = (aabbMin - center) * 0.5f + center;
		aabbMax = (aabbMax - center) * 0.5f + center;
		
	}

	glBegin(GL_LINE_LOOP);
	glNormal3f(0, 0, 1);
	glVertex3f(aabbMin.x(), aabbMin.y(), aabbMax.z());
	glNormal3f(0, 0, 1);
	glVertex3f(aabbMax.x(), aabbMin.y(), aabbMax.z());
	glNormal3f(0, 0, 1);
	glVertex3f(aabbMax.x(), aabbMax.y(), aabbMax.z());
	glNormal3f(0, 0, 1);
	glVertex3f(aabbMin.x(), aabbMax.y(), aabbMax.z());
	glEnd();

	glBegin(GL_LINE_LOOP);
	glNormal3f(1, 0, 0);
	glVertex3f(aabbMax.x(), aabbMin.y(), aabbMax.z());
	glNormal3f(1, 0, 0);
	glVertex3f(aabbMax.x(), aabbMin.y(), aabbMin.z());
	glNormal3f(1, 0, 0);
	glVertex3f(aabbMax.x(), aabbMax.y(), aabbMin.z());
	glNormal3f(1, 0, 0);
	glVertex3f(aabbMax.x(), aabbMax.y(), aabbMax.z());
	glEnd();

	glBegin(GL_LINE_LOOP);
	glNormal3f(0, 1, 0);
	glVertex3f(aabbMin.x(), aabbMax.y(), aabbMax.z());
	glNormal3f(0, 1, 0);
	glVertex3f(aabbMax.x(), aabbMax.y(), aabbMax.z());
	glNormal3f(0, 1, 0);
	glVertex3f(aabbMax.x(), aabbMax.y(), aabbMin.z());
	glNormal3f(0, 1, 0);
	glVertex3f(aabbMin.x(), aabbMax.y(), aabbMin.z());
	glEnd();

	glBegin(GL_LINE_LOOP);
	glNormal3f(0, 0, -1);
	glVertex3f(aabbMin.x(), aabbMin.y(), aabbMin.z());
	glNormal3f(0, 0, -1);
	glVertex3f(aabbMin.x(), aabbMax.y(), aabbMin.z());
	glNormal3f(0, 0, -1);
	glVertex3f(aabbMax.x(), aabbMax.y(), aabbMin.z());
	glNormal3f(0, 0, -1);
	glVertex3f(aabbMax.x(), aabbMin.y(), aabbMin.z());
	glEnd();

	glBegin(GL_LINE_LOOP);
	glNormal3f(0, -1, 0);
	glVertex3f(aabbMin.x(), aabbMin.y(), aabbMin.z());
	glNormal3f(0, -1, 0);
	glVertex3f(aabbMax.x(), aabbMin.y(), aabbMin.z());
	glNormal3f(0, -1, 0);
	glVertex3f(aabbMax.x(), aabbMin.y(), aabbMax.z());
	glNormal3f(0, -1, 0);
	glVertex3f(aabbMin.x(), aabbMin.y(), aabbMax.z());
	glEnd();

	glBegin(GL_LINE_LOOP);
	glNormal3f(-1, 0, 0);
	glVertex3f(aabbMin.x(), aabbMin.y(), aabbMin.z());
	glNormal3f(-1, 0, 0);
	glVertex3f(aabbMin.x(), aabbMin.y(), aabbMax.z());
	glNormal3f(-1, 0, 0);
	glVertex3f(aabbMin.x(), aabbMax.y(), aabbMax.z());
	glNormal3f(-1, 0, 0);
	glVertex3f(aabbMin.x(), aabbMax.y(), aabbMin.z());
	glEnd();

}

void DrawBall(const Ball & ball)
{

	static GLUquadric * quadric = gluNewQuadric();

	glPushMatrix();

	glm::vec3 position = glm::vec3(ball.position.x(), ball.position.y(), ball.position.z());
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1), position);

	glMultMatrixf(glm::value_ptr(modelMatrix));

	glColor4fv(ball.color.Data());
	gluSphere(quadric, ball.radius, 32, 32);

	glPopMatrix();

}

glm::mat4 GetViewMatrix(void)
{

	glm::mat4 view;

	glm::vec3 forward = glm::normalize(g_cameraOrientation * glm::vec3(0, 0, -1));
	glm::vec3 up      = glm::vec3(0, 1, 0);
	glm::vec3 left    = glm::cross(up, forward);

	view[0][0] = left.x;
	view[1][0] = left.y;
	view[2][0] = left.z;
	view[0][1] = up.x;
	view[1][1] = up.y;
	view[2][1] = up.z;
	view[0][2] = - forward.x;
	view[1][2] = - forward.y;
	view[2][2] = - forward.z;
	view[3][0] = - glm::dot(left, g_cameraPosition);
	view[3][1] = - glm::dot(up, g_cameraPosition);
	view[3][2] =   glm::dot(forward, g_cameraPosition);

	return view;

}

template <typename T, int N>
void WriteVector(const Matrix<T, N, 1> & v)
{

	std::cout << "(";

	for (int i = 0; i < N; i++)
	{
		if (i > 0) std::cout << ", ";
		std::cout << v[i];
	}

	std::cout << ")";

}

bool CreateBall(void)
{

	Ball * ball = new Ball;
	
	ball->position = Vector3(std::rand(), std::rand(), std::rand()) * (10.0f / RAND_MAX) - Vector3(5);
	ball->radius   = (float) std::rand() / RAND_MAX;
	ball->velocity = Vector3(std::rand(), std::rand(), std::rand()) * (1.0f / RAND_MAX) - Vector3(0.5f);
	ball->color    = Vector4(ball->velocity * 0.5f + 1, 1);

	if (g_octree.Insert(ball))
	{

		/*std::cout << "Inserted ball at ";
		WriteVector(ball->position);
		std::cout << std::endl;*/
		
		g_balls.push_back(ball);
		return true;
	}
	else
	{
		delete ball;
		return false;
	}

}

Ray CreateCameraRay(int x, int y)
{

	glm::mat4 viewMatrix = GetViewMatrix();
	glm::mat4 invViewProjMatrix = glm::inverse(g_projMatrix * viewMatrix);

	glm::vec2 deviceCoords;

	deviceCoords.x = x / (g_viewportSize.x * 0.5f) - 1.0f;
	deviceCoords.y = 1.0f - y / (g_viewportSize.y * 0.5f);

	glm::vec4 nearH = invViewProjMatrix * glm::vec4(deviceCoords.x, deviceCoords.y, -1 + g_near, 1);
	glm::vec4 farH  = invViewProjMatrix * glm::vec4(deviceCoords.x, deviceCoords.y,  1, 1);

	glm::vec3 n = glm::vec3(nearH) / nearH.w;
	glm::vec3 f = glm::vec3(farH) / farH.w;

	glm::vec3 dir = glm::normalize(f - n);

	return Ray(Vector3(g_cameraPosition.x, g_cameraPosition.y, g_cameraPosition.z),
			   Vector3(dir.x, dir.y, dir.z)); 

}

void ColorPicked(int x, int y)
{

	Ray ray = CreateCameraRay(x, y);

	g_ray = ray;

	std::cout << x << ", " << y << ": ";
	WriteVector(ray.GetOrigin());
	WriteVector(ray.GetDirection());
	std::cout << std::endl;

	Ball * b;
	Real t;

	if (g_octree.Pick(ray, b, t))
	{
		b->color = Vector4(1, 0, 0, 1);
	}

	

}

void ColorReset(void)
{

	for (Ball * b : g_balls)
	{
		b->color = Vector4(b->velocity * 0.5f + 1, 1);
	}

}

void MoveBalls(void)
{

	for (Ball * b : g_balls)
	{
		
		Vector3 oldPosition = b->position;

		g_octree.Remove(b);
		b->position = b->position + b->velocity * 0.012f;
		
		if (!g_octree.Insert(b))
		{

			b->position = oldPosition;
			b->velocity = Vector3(0);

			g_octree.Insert(b);

		}

	}

}

void CreateObjects(void)
{

	for (int i = 0; i < 5; i++)
	{
		while (!CreateBall());
	}

}

void DestroyObjects(void)
{

	size_t size = g_octree.GetSize();

	for (int i = 0; i < 5; i++)
	{
		
		if (size > 0)
		{

			unsigned int index = (float) (size * std::rand()) / RAND_MAX;

			auto it = g_balls.begin() + index;

			g_octree.Remove(*it);
			g_balls.erase(it);

			size--;

		}

	}

}