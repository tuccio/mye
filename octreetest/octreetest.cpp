#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include <iostream>

#include <mye/algorithms/LooseOctree.h>
#include <mye/math/Geometry.h>

using namespace mye::algorithms;
using namespace mye::math;

void Reshape(int, int);
void Render(void);

struct Ball
{
	Vector3 position;
	float   radius;
	Vector4 color;
};

namespace mye
{

	namespace algorithms
	{

		template <>
		struct Boundings<Ball, AABB>
		{

			inline AABB operator() (const Ball & b)
			{
				return AABB::FromCenterHalfExtents(b.position, b.radius);
			}

		};

	}

}

int main(int argc, char * argv[])
{

	AABB aabb = AABB::FromCenterHalfExtents(Vector3(0), Vector3(1, 1, 1));
	Ray ray(Vector3(-5, -5, -5), Vector3(0.57735f, 0.57735f, 0.57735f));

	float t1, t2;

	bool intersection1 = Intersect(aabb, ray, t1);
	bool intersection3 = Intersect(aabb, ray);

	bool intersection2 = Intersect(ray, aabb, t2);
	bool intersection4 = Intersect(ray, aabb);

	std::cout << std::boolalpha << intersection1 << " " << t1 << std::endl;
	std::cout << std::boolalpha << intersection3 << std::endl;

	std::cout << std::boolalpha << intersection2 << " " << t2 << std::endl;
	std::cout << std::boolalpha << intersection4 << std::endl;

	bool intersection5 = Intersect(aabb, aabb);

	std::cout << std::boolalpha << intersection5 << std::endl;

	//LooseOctree<Ball, AABB> octree(Vector3(0), 50.0f);

	glutInit(&argc, argv);	

	glutInitWindowSize(800, 600);
	glutCreateWindow("Octree test");

	glutReshapeFunc(Reshape);
	glutDisplayFunc(Render);

	glutShowWindow();

	glutMainLoop();

	return 0;

}

void Render(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float triangles[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	glBegin(GL_TRIANGLES);

	glVertex3fv(triangles);
	glVertex3fv(triangles + 3);
	glVertex3fv(triangles + 6);

	glEnd();
	
	glutSwapBuffers();

}

void Reshape(int w, int h)
{

	glViewport(0, 0, w, h);

	glutPostRedisplay();

}