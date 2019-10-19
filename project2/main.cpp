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

Gravity *a = new Gravity(glm::vec3(0.0f, -9.8f, 0.0f));

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

	// TASK 1: 5 PARTICLES GOING DOWNWARDS
	std::vector<Particle>p;

	//Put particles in vector p
	for (int i = 0; i < 5; ++i)
	{
		Particle p1;
		p1.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
		p1.translate(glm::vec3(i, 5.0f, 0.0f));
		p1.scale(glm::vec3(1.0f, 1.0f, 1.0f));
		p1.setMass(0.1f);
		p1.setVel(glm::vec3(0.0f, 0.0f, 0.0f));
		p1.setAcc(glm::vec3(0.0f, 0.0f, 0.0f));
		p.push_back(p1);
	}

	//For loop to add springs to each particle
	for (int i = 1; i < p.size(); i++)
	{
		Hooke* hooke = new Hooke(&p[i], &p[i - 1], 6.0f, 2.0f, 0.5f);
		p[i].addForce(a);
		p[i].addForce(hooke);
		p[i - 1].addForce(hooke);	
	}

	//TASK 2: 10 PARTICLES WITH TWO FIXED EXTREMETIES
	std::vector<Particle>p2;

	for (int i = 0; i < 10; i++)
	{
		Particle p1;
		p1.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
		p1.translate(glm::vec3(i - 5.0f, 5.0f, 0.0f));
		p1.scale(glm::vec3(1.0f, 1.0f, 1.0f));
		p1.setMass(0.1f);
		p1.setVel(glm::vec3(0.0f, 0.0f, 0.0f));
		p1.setAcc(glm::vec3(0.0f, 0.0f, 0.0f));
		p2.push_back(p1);
	}

	for (int i = 1; i < p2.size(); i++)
	{
		Hooke* hooke = new Hooke(&p2[i], &p2[i - 1], 6.0f, 2.0f, 0.5f);
		p2[i].addForce(a);
		p2[i].addForce(hooke);
		p2[i - 1].addForce(hooke);
	}

	//TASK 3: 10 PARTICLES WITH TWO FIXED EXTREMETIES PLUS COLLISION
	std::vector<Particle>p3;

	for (int i = 0; i < 10; i++)
	{
		Particle p1;
		p1.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
		p1.translate(glm::vec3(i - 5.0f, 2.0f, 0.0f));
		p1.scale(glm::vec3(1.0f, 1.0f, 1.0f));
		p1.setMass(0.1f);
		p1.setVel(glm::vec3(0.0f, 0.0f, 0.0f));
		p1.setAcc(glm::vec3(0.0f, 0.0f, 0.0f));
		p3.push_back(p1);
	}

	for (int i = 1; i < p3.size(); i++)
	{
		Hooke* hooke = new Hooke(&p3[i], &p3[i - 1], 6.0f, 2.0f, 0.5f);
		p3[i].addForce(a);
		p3[i].addForce(hooke);
		p3[i - 1].addForce(hooke);
	}

	//TASK 4: CLOTH SIMULATION 1
	std::vector<std::vector<Particle>>p4;

	for (int i = 0; i < 10; i++)
	{
		std::vector<Particle> row;

		for (int r = 0; r < 10; r++)
		{
			Particle p1;
			p1.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_green.frag"));
			p1.translate(glm::vec3(i - 5.0f, 3.0f, r - 5.0f));
			p1.scale(glm::vec3(0.5f, 0.5f, 0.5f));
			p1.setMass(0.1f);
			p1.setVel(glm::vec3(0.0f, 0.0f, 0.0f));
			p1.setAcc(glm::vec3(0.0f, 0.0f, 0.0f));
			row.push_back(p1);
		}
		p4.push_back(row);
	}

	//Nested for loop to add forces to the double particle vector
	for (int i = 0; i < p4.size(); ++i)
	{
		for (int r = 0; r < p4.size(); r++)
		{
			p4[r][i].addForce(a);
			if (i != 0)
			{
				Hooke* hooke = new Hooke(&p4[i][r], &p4[i - 1][r], 2.0f, 2.0f, 0.5f);
				p4[i - 1][r].addForce(hooke);
				Drag* d = new Drag(&p4[i][r], 0.4f, 0.2f);
				p4[i][r].addForce(d);
			}
			if (r != 0)
			{
				Hooke* hooke = new Hooke(&p4[i][r], &p4[i][r - 1], 2.0f, 2.0f, 0.5f);
				p4[i][r - 1].addForce(hooke);
			}
			if (i != p4.size() - 1)
			{
				Hooke* hooke = new Hooke(&p4[i][r], &p4[i + 1][r], 2.0f, 2.0f, 0.5f);
				p4[i + 1][r].addForce(hooke);
			}
			if (r != p4.size() - 1)
			{
				Hooke* hooke = new Hooke(&p4[i][r], &p4[i][r + 1], 2.0f, 2.0f, 0.5f);
				p4[i][r + 1].addForce(hooke);
			}			
		}
	}

	//TASK 5 - VERTICAL CLOTH WITH WIND
	 
	std::vector<std::vector<Particle>>p5;

	for (int i = 0; i < 10; i++)
	{
		std::vector<Particle> row;

		for (int r = 0; r < 10; r++)
		{
			Particle p1;
			p1.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_green.frag"));
			p1.translate(glm::vec3(i - 5.0f, r, 0.0f));
			p1.scale(glm::vec3(0.5f, 0.5f, 0.5f));
			p1.setMass(0.1f);
			p1.setVel(glm::vec3(0.0f, 0.0f, 0.0f));
			p1.setAcc(glm::vec3(0.0f, 0.0f, 0.0f));
			row.push_back(p1);
		}
		p5.push_back(row);
	}

	//Nested for loop to add springs to particles
	for (int i = 0; i < p5.size(); ++i)
	{
		for (int r = 0; r < p5.size(); r++)
		{
			p5[r][i].addForce(a);
			if (i != 0)
			{
				Hooke* hooke = new Hooke(&p5[i][r], &p5[i - 1][r], 2.0f, 2.0f, 0.5f);
				p5[i - 1][r].addForce(hooke);
			}
			if (r != 0)
			{
				Hooke* hooke = new Hooke(&p5[i][r], &p5[i][r - 1], 2.0f, 2.0f, 0.5f);
				p5[i][r - 1].addForce(hooke);
			}
			if (i != p5.size() - 1)
			{
				Hooke* hooke = new Hooke(&p5[i][r], &p5[i + 1][r], 2.0f, 2.0f, 0.5f);
				p5[i + 1][r].addForce(hooke);
			}
			if (r != p5.size() - 1)
			{
				Hooke* hooke = new Hooke(&p5[i][r], &p5[i][r + 1], 2.0f, 2.0f, 0.5f);
				p5[i][r + 1].addForce(hooke);
			}

			//Adds drag to particle
			Drag* d = new Drag(&p5[i][r], 0.4f, 0.2f);
			p5[i][r].addForce(d);

			//Adds wind to particle
			if (i != p5.size() - 1 && r != p5.size() - 1)
			{
				Wind* wind = new Wind(&p5[i][r], &p5[i + 1][r], &p5[i][r + 1], 2.0f, 0.5f);
				p5[i][r].addForce(wind);
				p5[i + 1][r].addForce(wind);
				p5[i][r + 1].addForce(wind);
			}
			if(i != p5.size() - 1 && r != 0)
			{
				Wind* wind = new Wind(&p5[i][r], &p5[i + 1][r - 1], &p5[i + 1][r], 2.0f, 0.5f);
				p5[i][r].addForce(wind);
				p5[i + 1][r - 1].addForce(wind);
				p5[i + 1][r].addForce(wind);
			}	
			if (r == p5.size())
			{
				Wind* wind = new Wind(&p5[i - 1][r], &p5[i][r], &p5[i][r + 1], 2.0f, 0.5f);
				p5[i][r].addForce(wind);
				p5[i][r - 1].addForce(wind);
				p5[i][r + 1].addForce(wind);
			}
		}
	}
	
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
			//Do movement and get keyboard input
			app.doMovement(dt);
			task = app.task();

			if (task == 1)
			{
				for (unsigned int i = 1; i < p.size(); i++)
				{
					p[i].setAcc(p[i].applyForces(p[i].getPos(), p[i].getVel(), t, dt));
					p[i].setVel(p[i].getVel() + p[i].getAcc() * dt);
					p[i].translate(p[i].getVel() * dt);
				}
			}

			if (task == 2)
			{
				for (unsigned int i = 1; i < p2.size() - 1; i++)
				{
					p2[i].setAcc(p2[i].applyForces(p2[i].getPos(), p2[i].getVel(), t, dt));
					p2[i].setVel(p2[i].getVel() + p2[i].getAcc() * dt);
					p2[i].translate(p2[i].getVel() * dt);
				}
			}

			if (task == 3)
			{
				for (unsigned int i = 1; i < p3.size() - 1; i++)
				{
					p3[i].setAcc(p3[i].applyForces(p3[i].getPos(), p3[i].getVel(), t, dt));
					p3[i].setVel(p3[i].getVel() + p3[i].getAcc() * dt);
					p3[i].translate(p3[i].getVel() * dt);
				}

				for (unsigned int i = 1; i < p3.size(); i++)
				{
					if (p3[i].getPos().y <= 0)
					{
						p3[i].setVel(glm::vec3(p3[i].getVel().x, p3[i].getVel().y *= dropoff, p3[i].getVel().z));
						p3[i].setPos(glm::vec3(p3[i].getPos().x, 0.0f, p3[i].getPos().z));
					}
				}
			}

			if (task == 4)
			{
				for (unsigned int i = 1; i < p4.size() - 1; i++)
				{
					for (unsigned int r = 1; r < p4[i].size() - 1; r++)
					{
						p4[i][r].setAcc(p4[i][r].applyForces(p4[i][r].getPos(), p4[i][r].getVel(), t, dt));
						p4[i][r].setVel(p4[i][r].getVel() + p4[i][r].getAcc() * dt);
						p4[i][r].translate(p4[i][r].getVel() * dt);
						if (p4[i][r].getPos().y <= 0)
						{
							p4[i][r].setVel(glm::vec3(p4[i][r].getVel().x, p4[i][r].getVel().y *= dropoff, p4[i][r].getVel().z));
							p4[i][r].setPos(glm::vec3(p4[i][r].getPos().x, 0.0f, p4[i][r].getPos().z));
						}
					}
				}
			}

			if (task == 5)
			{
				for (unsigned int i = 0; i < p5.size() ; i++)
				{
					for (unsigned int r = 0; r < p5[i].size() - 1; r++)
					{
						p5[i][r].setAcc(p5[i][r].applyForces(p5[i][r].getPos(), p5[i][r].getVel(), t, dt));
						p5[i][r].setVel(p5[i][r].getVel() + p5[i][r].getAcc() * dt);
						p5[i][r].translate(p5[i][r].getVel() * dt);
						if (p5[i][r].getPos().y <= 0)
						{
							p5[i][r].setVel(glm::vec3(p5[i][r].getVel().x, p5[i][r].getVel().y *= dropoff, p5[i][r].getVel().z));
							p5[i][r].setPos(glm::vec3(p5[i][r].getPos().x, 0.0f, p5[i][r].getPos().z));
						}
					}
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
				for (int i = 0; i < p.size(); i++)
				{
					app.draw(p[i].getMesh());
				}
			}

			if (task == 2)
			{
				for (int i = 0; i < p2.size(); i++)
				{
					app.draw(p2[i].getMesh());
				}
			}

			if (task == 3)
			{
				for (int i = 0; i < p3.size(); i++)
				{
					app.draw(p3[i].getMesh());
				}
			}
			
			if (task == 4)
			{
				for (int i = 0; i < p4.size(); i++)
				{					
					for (int n = 0; n < p4[i].size(); n++)
					{
						app.draw(p4[i][n].getMesh());
					}
				}
			}
			
			if (task == 5)
			{
				for (int i = 0; i < p5.size(); i++)
				{
					for (int n = 0; n < p5[i].size(); n++)
					{
						app.draw(p5[i][n].getMesh());
					}
				}
			}
			app.display();
		}
	}
	app.terminate();

	return EXIT_SUCCESS;
}
