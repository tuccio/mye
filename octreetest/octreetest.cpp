#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include <cstdlib>
#include <iostream>
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

struct Ball
{
	Vector3 position;
	float   radius;
	Vector4 color;
};

void Reshape(int, int);
void Render(int);
void Render(void);
void Keyboard(unsigned char, int, int);
void Mouse(int, int, int, int);

void DrawAABB(const AABB & aabb);
void DrawBall(const Ball & ball);

void CreateBall(void);
void ColorPicked(int x, int y);
void ColorReset(void);

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

	}

}

LooseOctree<Ball *> g_octree;

glm::vec3           g_cameraPosition;
glm::quat           g_cameraOrientation;

glm::mat4           g_projMatrix;

bool                g_drawLooseOctree = true;
bool                g_drawHalfLooseOctree = false;

std::vector<Ball*>  g_balls;

float               g_near = 0.1f;
float               g_far  = 100.0f;;

glm::vec2           g_viewportSize;

int main(int argc, char * argv[])
{

	AABB aabb = AABB::FromCenterHalfExtents(Vector3(0), Vector3(0.5f));
	Frustum frustum(-5, Vector3(0, 0, 1), Vector3(0, 1, 0), Vector3(1, 0, 0), 0.1f, 100.0f, 120, 120);
	VolumeSide side;

	Intersect(frustum, aabb, side);

	std::cout << sizeof(mye::algorithms::detail::LooseOctreeNode<Ball*>) << std::endl;

	std::cout << static_cast<int>(side) << std::endl;

	g_octree.Create(Vector3(0), 5.0f, 2);

	g_cameraPosition    = glm::vec3(0, 0, -25.0f);
	g_cameraOrientation = glm::quat(1, 0, 0, 0);

	glutInit(&argc, argv);	

	glutInitWindowSize(1280, 720);
	glutCreateWindow("Octree test");

	glutReshapeFunc(Reshape);
	glutDisplayFunc(Render);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);

	glutShowWindow();

	glutTimerFunc(12, Render, 0);
	glutMainLoop();

	return 0;

}

void Render(int) { Render(); glutTimerFunc(12, Render, 0); }

void Render(void)
{

	glm::mat4 viewMatrix = GetViewMatrix();

	glLoadMatrixf(glm::value_ptr(viewMatrix));

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


	glm::vec3 forward = g_cameraOrientation * glm::vec3(0, 0, 1);
	glm::vec3 left    = g_cameraOrientation * glm::vec3(1, 0, 0);
	glm::vec3 up      = g_cameraOrientation * glm::vec3(0, -1, 0);

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
		g_cameraOrientation = glm::normalize(glm::rotate(g_cameraOrientation, aSpeed, up));
	}
		break;

	case 'c':
	{
		g_cameraOrientation = glm::normalize(glm::rotate(g_cameraOrientation, -aSpeed, up));
	}
		break;

	case 'b':
		CreateBall();
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

	glm::mat4 viewMatrix = glm::translate(glm::mat4(1), -g_cameraPosition) *
		glm::mat4_cast(g_cameraOrientation);


	return glm::inverse(viewMatrix);

}

void CreateBall(void)
{

	Ball * ball = new Ball;
	
	ball->position =  Vector3(std::rand(), std::rand(), std::rand()) * (10.0f / RAND_MAX) - Vector3(5);
	ball->radius   = (float) std::rand() / RAND_MAX;
	ball->color    = Vector4(1, 1, 1, 1);

	if (g_octree.Insert(ball))
	{
		std::cout << "Inserted ball at (" << ball->position.x() << ", " << ball->position.y() << ", " << ball->position.z() << ")" << std::endl;
		g_balls.push_back(ball);
	}
	else
	{
		delete ball;
	}

}

Ray CreateCameraRay(int x, int y)
{

	glm::mat4 viewMatrix = GetViewMatrix();
	glm::mat4 invViewProjMatrix = glm::inverse(g_projMatrix * viewMatrix);

	glm::vec2 deviceCoords;

	deviceCoords.x = x / (g_viewportSize.x * 0.5f) - 1.0f;
	deviceCoords.y = - ( y / (g_viewportSize.y * 0.5f) - 1.0f);

	glm::vec4 nearH = invViewProjMatrix * glm::vec4(deviceCoords.x, deviceCoords.y, -1, 1);
	glm::vec4 farH  = invViewProjMatrix * glm::vec4(deviceCoords.x, deviceCoords.y,  1, 1);

	glm::vec3 n = glm::vec3(nearH) / nearH.w;
	glm::vec3 f = glm::vec3(farH) / farH.w;

	glm::vec3 dir = glm::normalize(f - n);

	return Ray(Vector3(g_cameraPosition.x, g_cameraPosition.y, g_cameraPosition.z),
			   Vector3(dir.x, dir.y, - dir.z)); 

}

void ColorPicked(int x, int y)
{

	Ray ray = CreateCameraRay(x, y);

	g_octree.Query(ray,
				   [] (Ball * b)
	{
		b->color = Vector4(1, 0, 0, 1);
	});

}

void ColorReset(void)
{

	for (Ball * b : g_balls)
	{
		b->color = Vector4(1, 1, 1, 1);
	}

}