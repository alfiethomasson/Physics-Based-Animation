#pragma once
// Math constants
#define _USE_MATH_DEFINES
#include <cmath>  
#include <random>

// Std. Includes
#include <string>
#include <time.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include "glm/ext.hpp"

// Other Libs
#include "SOIL2/SOIL2.h"

// project includes
#include "Application.h"
#include "Shader.h"
#include "Mesh.h"
#include "Particle.h"
#include "RigidBody.h"

Gravity* a = new Gravity(glm::vec3(0.0f, -9.8f, 0.0f));

double task = 0;

double t = 0.0;
const double dt = 0.01;
double currentTime;
double accumulator = 0.00;

glm::vec3 airforce;
glm::vec3 force;
double dropoff = -0.8;
double rad = 2;
double bigrad = 4;
float h = 5.0f;
float wforce = 5.0f;
float e = 1.0f;
bool collision = false;

//Defines cube 
glm::vec3 cube = glm::vec3(5);
glm::vec3 edge = glm::vec3(-5.0f, 0.0f, -5.0f);

// main function
int main()
{
	// create application
	Application app = Application::Application();
	app.initRender();
	Application::camera.setCameraPosition(glm::vec3(0.0f, 5.0f, 15.0f));

	// create ground plane
	Mesh plane = Mesh::Mesh(Mesh::QUAD);
	// scale it up x5
	plane.scale(glm::vec3(5.0f, 5.0f, 5.0f));
	Shader lambert = Shader("resources/shaders/physics.vert", "resources/shaders/physics.frag");
	plane.setShader(lambert);

		


	RigidBody rb1 = RigidBody();
	 Mesh m = Mesh::Mesh(Mesh::CUBE);
	rb1.setMesh(m);
	Shader rbShader = Shader("resources/shaders/physics.vert", "resources/shaders/physics.frag");
	rb1.getMesh().setShader(rbShader);
	rb1.scale(glm::vec3(1.0f, 3.0f, 1.0f));
	rb1.setMass(2.0f);

	rb1.translate(glm::vec3(0.0f, 5.0f, 0.0f));
	rb1.setVel(glm::vec3(2.0f, 0.0f, 0.0f));
	rb1.setAngVel(glm::vec3(0.0f, 0.0f, 0.0f));
	//rb.addForce(a);


	RigidBody rb = RigidBody();
	m = Mesh::Mesh(Mesh::CUBE);
	rb.setMesh(m);
	rbShader = Shader("resources/shaders/physics.vert", "resources/shaders/physics.frag");
	rb.getMesh().setShader(rbShader);
	rb.scale(glm::vec3(1.0f, 3.0f, 1.0f));
	rb.setMass(2.0f);
	rb.setElasticity(1.0f);

	rb.translate(glm::vec3(0.0f, 5.0f, 0.0f));
	rb.setVel(glm::vec3(0.0f, 2.0f, 0.0f));
	rb.setAngVel(glm::vec3(1.0f, 0.0f, 0.0f));

	rb.addForce(a);

	RigidBody rb2 = RigidBody();
	m = Mesh::Mesh(Mesh::CUBE);
	rb2.setMesh(m);
	rbShader = Shader("resources/shaders/physics.vert", "resources/shaders/physics.frag");
	rb2.getMesh().setShader(rbShader);
	rb2.scale(glm::vec3(1.0f, 3.0f, 1.0f));
	rb2.setMass(2.0f);
	rb2.setElasticity(1.0f);

	rb2.translate(glm::vec3(0.0f, 5.0f, 0.0f));
	rb2.setVel(glm::vec3(0.0f, 0.0f, 0.0f));
	rb2.setAngVel(glm::vec3(0.0f, 0.0f, 0.5f));

	rb2.addForce(a);

	RigidBody rb3 = RigidBody();
	m = Mesh::Mesh(Mesh::CUBE);
	rb3.setMesh(m);
	rbShader = Shader("resources/shaders/physics.vert", "resources/shaders/physics.frag");
	rb3.getMesh().setShader(rbShader);
	rb3.scale(glm::vec3(1.0f, 3.0f, 1.0f));
	rb3.setMass(2.0f);
	rb3.setElasticity(0.7f);

	rb3.translate(glm::vec3(0.0f, 5.0f, 0.0f));
	rb3.setVel(glm::vec3(0.0f, 0.0f, 0.0f));
	rb3.setAngVel(glm::vec3(0.1f, 0.1f, 0.1f));

	rb3.addForce(a);
	
float timeforimpulse = 2.0f;
glm::vec3 impulse = glm::vec3(-4.0f, 0.0f, 0.0f);
bool impulseadded = false;
std::vector<glm::vec3> edges;

	// Game loop
	while (!glfwWindowShouldClose(app.getWindow()))
	{
		//Time stuff
		double newTime = (GLfloat)glfwGetTime();
		double frameTime = newTime - currentTime;
		currentTime = newTime;

		accumulator += frameTime;

		while (accumulator >= dt)
		{
			task = app.doTask();
			//Do movement and get keyboard input
			
			if (task == 1)
			{
				rb1.setAcc(rb1.applyForces(rb1.getPos(), rb1.getVel(), t, dt));
				rb1.setVel(rb1.getVel() + rb1.getAcc() * dt);
				rb1.translate(rb1.getVel() * dt);

				// integration ( rotation )
				rb1.setAngVel(rb1.getAngVel() + dt * rb1.getAngAcc());
				// create skew symmetric matrix for w
				glm::mat3 angVelSkew = glm::matrixCross3(rb1.getAngVel());
				// create 3x3 rotation matrix from rb rotation matrix
				glm::mat3 R = glm::mat3(rb1.getRotate());
				// update rotation matrix
				R += dt * angVelSkew * R;
				R = glm::orthonormalize(R);
				rb1.setRotate(glm::mat4(R));

				if ((currentTime > timeforimpulse) && (impulseadded == false))
				{
					Vertex impulsepoint = Vertex(rb1.getPos() + glm::vec3(0.0f, 2.0f, 0.0f));
					rb1.setVel(rb1.getVel() + (impulse / rb1.getMass()));
					std::cout << "Inverse Inertia Matrix: " << glm::to_string(rb1.getInvInertia()) << "\n";
					rb1.setAngVel(rb1.getAngVel() + (rb1.getInvInertia() * glm::cross(impulse, impulsepoint.getCoord() - rb1.getPos())));
					impulseadded = true;
				}
			}

			if (task == 2)
			{


				rb.setAcc(rb.applyForces(rb.getPos(), rb.getVel(), t, dt));
				rb.setVel(rb.getVel() + rb.getAcc() * dt);
				rb.translate(rb.getVel() * dt);

				// integration ( rotation )
				rb.setAngVel(rb.getAngVel() + dt * rb.getAngAcc());
				// create skew symmetric matrix for w
				glm::mat3 angVelSkew = glm::matrixCross3(rb.getAngVel());
				// create 3x3 rotation matrix from rb rotation matrix
				glm::mat3 R = glm::mat3(rb.getRotate());
				// update rotation matrix
				R += dt * angVelSkew * R;
				R = glm::orthonormalize(R);
				rb.setRotate(glm::mat4(R));

				std::vector<glm::vec3> edges = {};

				std::vector<Vertex> vertices = rb.getMesh().getVertices();

				for (Vertex vertex : vertices)
				{
					glm::vec3 v = glm::mat3(rb.getMesh().getModel()) * vertex.getCoord() + rb.getPos();

					if (v.y < plane.getPos().y)
					{
						std::cout << "Vertex Collision at: " << glm::to_string(v) << "\n";
						edges.push_back(v);
						collision = true;
					}
				}

				if (collision == true && edges.size() != 0)
				{
					glm::vec3 avg;
					if (edges.size() == 1)
					{
						std::cout << "COLLISION ON VERTEX\n";
						avg = edges[0];
					}
					else if (edges.size() == 2)
					{
						std::cout << "COLLISION ON EDGE\n";
						avg = (edges[0] + edges[1]) / 2;
					}
					else if (edges.size() == 4)
					{
						std::cout << "COLLISION ON FACE\n";
						avg = (edges[0] + edges[1] + edges[2] + edges[3]) / 4;
					}

					rb.setPos(rb.getPos() + glm::vec3(0.0f, 0.0f - avg.y, 0.0f));

					glm::vec3 r = avg - rb.getPos();
					glm::vec3 n = glm::vec3(0.0f, 1.0f, 0.0f);
					n = glm::normalize(n);
					glm::vec3 vr = rb.getVel() + glm::cross(rb.getAngVel(), r);

					float jr = (-(1.0f + rb.getElasticity()) * glm::dot(vr, n)) / ((1 / rb.getMass()) + glm::dot(n, (glm::cross(rb.getInvInertia() * (glm::cross(r, n)), r))));

					rb.setVel(rb.getVel() + (jr / rb.getMass()) * n);
					rb.setAngVel(rb.getAngVel() + (jr * rb.getInvInertia() * glm::cross(r, n)));

					edges.clear();
					collision = false;
				}
			}

			if (task == 3)
			{
				rb2.setAcc(rb2.applyForces(rb2.getPos(), rb2.getVel(), t, dt));
				rb2.setVel(rb2.getVel() + rb2.getAcc() * dt);
				rb2.translate(rb2.getVel()* dt);

				// integration ( rotation )
				rb2.setAngVel(rb2.getAngVel() + dt * rb2.getAngAcc());
				// create skew symmetric matrix for w
				glm::mat3 angVelSkew = glm::matrixCross3(rb2.getAngVel());
				// create 3x3 rotation matrix from rb rotation matrix
				glm::mat3 R = glm::mat3(rb2.getRotate());
				// update rotation matrix
				R += dt * angVelSkew * R;
				R = glm::orthonormalize(R);
				rb2.setRotate(glm::mat4(R));

				std::vector<glm::vec3> edges = {};

				std::vector<Vertex> vertices = rb2.getMesh().getVertices();

				for (Vertex vertex : vertices)
				{
					glm::vec3 v = glm::mat3(rb2.getMesh().getModel()) * vertex.getCoord() + rb2.getPos();

					if (v.y < plane.getPos().y)
					{
						edges.push_back(v);
						collision = true;
					}
				}

				if (collision == true && edges.size() != 0)
				{
					glm::vec3 avg;
					if (edges.size() == 1)
					{
						avg = edges[0];
					}
					else if (edges.size() == 2)
					{
						avg = (edges[0] + edges[1]) / 2;
					}
					else if (edges.size() == 4)
					{
						avg = (edges[0] + edges[1] + edges[2] + edges[3]) / 4;
					}

					rb2.setPos(rb2.getPos() + glm::vec3(0.0f, 0.0f - avg.y, 0.0f));

					glm::vec3 r = avg - rb2.getPos();
					glm::vec3 n = glm::vec3(0.0f, 1.0f, 0.0f);
					n = glm::normalize(n);
					glm::vec3 vr = rb2.getVel() + glm::cross(rb2.getAngVel(), r);

					float jr = (-(1.0f + rb2.getElasticity()) * glm::dot(vr, n)) / ((1 / rb2.getMass()) + glm::dot(n, (glm::cross(rb2.getInvInertia() * (glm::cross(r, n)), r))));

					rb2.setVel(rb2.getVel() + (jr / rb2.getMass()) * n);
					rb2.setAngVel(rb2.getAngVel() + (jr * rb2.getInvInertia() * glm::cross(r, n)));

					edges.clear();
					collision = false;
				}
			}

			if (task == 4)
			{
				rb3.setAcc(rb3.applyForces(rb3.getPos(), rb3.getVel(), t, dt));
				rb3.setVel(rb3.getVel() + rb3.getAcc() * dt);
				rb3.translate(rb3.getVel() * dt);

				// integration ( rotation )
				rb3.setAngVel(rb3.getAngVel() + dt * rb3.getAngAcc());
				// create skew symmetric matrix for w
				glm::mat3 angVelSkew = glm::matrixCross3(rb3.getAngVel());
				// create 3x3 rotation matrix from rb rotation matrix
				glm::mat3 R = glm::mat3(rb3.getRotate());
				// update rotation matrix
				R += dt * angVelSkew * R;
				R = glm::orthonormalize(R);
				rb3.setRotate(glm::mat4(R));

				std::vector<glm::vec3> edges = {};

				std::vector<Vertex> vertices = rb3.getMesh().getVertices();

				for (Vertex vertex : vertices)
				{

					glm::vec3 v = glm::mat3(rb3.getMesh().getModel()) * vertex.getCoord() + rb3.getPos();

					if (v.y < plane.getPos().y)
					{
						edges.push_back(v);
						collision = true;
					}
				}

				if (collision == true && edges.size() != 0)
				{
					glm::vec3 avg;
					if (edges.size() == 1)
					{
						avg = edges[0];
					}
					else if (edges.size() == 2)
					{
						avg = (edges[0] + edges[1]) / 2;
					}
					else if (edges.size() == 4)
					{
						avg = (edges[0] + edges[1] + edges[2] + edges[3]) / 4;
					}

				
					rb3.setPos(rb3.getPos() + glm::vec3(0.0f, 0.0f - avg.y, 0.0f));

					glm::vec3 r = avg - rb3.getPos();
					glm::vec3 n = glm::vec3(0.0f, 1.0f, 0.0f);
					n = glm::normalize(n);
					glm::vec3 vr = rb3.getVel() + glm::cross(rb3.getAngVel(), r);

					float jr = (-(1.0f + rb3.getElasticity()) * glm::dot(vr, n)) / ((1 / rb3.getMass()) + glm::dot(n, (glm::cross(rb3.getInvInertia() * (glm::cross(r, n)), r))));

					rb3.setVel(rb3.getVel() + (jr / rb3.getMass()) * n);
					rb3.setAngVel(rb3.getAngVel() + (jr * rb3.getInvInertia() * glm::cross(r, n)));

					edges.clear();
					collision = false;
				}
			}

			accumulator -= dt;
			t += dt;

			// clear buffer
			app.clear();
			// draw groud plane
			app.draw(plane);
			// draw particles

			if (task == 1)
			{
				app.draw(rb1.getMesh());
			}
			if (task == 2)
			{
				app.draw(rb.getMesh());
			}
			if (task == 3)
			{
				app.draw(rb2.getMesh());
			}
			if (task == 4)
			{
				app.draw(rb3.getMesh());
			}
			app.doMovement(dt);
			task = app.task();

			app.display();
		}
	}
	app.terminate();

	return EXIT_SUCCESS;
}
