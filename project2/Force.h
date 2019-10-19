#pragma once
#include <glm/glm.hpp>
#include <iostream>
class Body; // forward declaration to avoid circular dependencies
class Force
{
public:
	Force() {}
	~Force() {}
	virtual glm::vec3 apply(float mass, const glm::vec3& pos, const glm::vec3& vel);
};
/*
** GRAVITY CLASS
*/
class Gravity : public Force {
public:
	// constructors
	Gravity() {}
	Gravity(const glm::vec3& gravity) { m_gravity = gravity; }
	// get and set methods
	glm::vec3 getGravity() const { return m_gravity; }
	void setGravity(glm::vec3 gravity) { m_gravity = gravity; }
	// physics
	glm::vec3 apply(float mass, const glm::vec3& pos, const glm::vec3& vel);
private:
	glm::vec3 m_gravity = glm::vec3(0.0f, -9.8f, 0.0f);
};
/*
** WIND CLASS
*/
class Wind : public Force {
public:
	Wind() {}

	Wind(Body* bod1, Body* bod2, Body* bod3, float cd, float air)
	{
		m_bod1 = bod1;
		m_bod2 = bod2;
		m_bod3 = bod3;
		m_cd2 = cd;
		m_air2 = air;
	}
	// physics
	glm::vec3 apply(float mass, const glm::vec3& pos, const glm::vec3& vel);
private:
	Body* m_bod1;
	Body* m_bod2;
	Body* m_bod3;
	float m_cd2;
	float m_air2;
};

/*
** DRAG CLASS
*/
class Drag : public Force {
public:
	Drag() {}

	Drag(Body* bod, const float& air, const float& cd)
	{
		m_air = air;
		m_cd = cd;
		m_bod = bod;
	}
	// physics
	glm::vec3 apply(float mass, const glm::vec3 & pos, const glm::vec3 & vel);

private:
	float m_cd;
	float m_air;
	Body* m_bod;

};
/* 
** HOOKE CLASS
*/
class Hooke : public Force {
public:
	Hooke() {}
	Hooke(Body* b1, Body* b2, float ks, float kd, float rest) {
		m_ks = ks; m_kd = kd; m_rest = rest; m_b1 = b1; m_b2 = b2;
	}

	glm::vec3 apply(float mass, const glm::vec3& pos, const glm::vec3& vel);

private:
	float m_ks; // spring stiffness
	float m_kd; // damping coefficient
	float m_rest; // sprint rest length

	Body* m_b1; // pointer to the body connected to one extremity of the spring
	Body* m_b2; // pointer to the body connected to the other extremity
};
