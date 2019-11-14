#include <iostream>
#include <cmath>
#include "Force.h"
#include "Body.h"
#include "glm/ext.hpp"
glm::vec3 Force::apply(float mass, const glm::vec3& pos, const glm::vec3& vel) {
	return glm::vec3();
}
/*
** GRAVITY
*/
glm::vec3 Gravity::apply(float mass, const glm::vec3& pos, const glm::vec3& vel) {
	// complete . Should return the gravity force
	return m_gravity * mass;
}

glm::vec3 Wind::apply(float mass, const glm::vec3& pos, const glm::vec3& vel) {

	glm::vec3 velaverage = (m_bod1->getVel() + m_bod2->getVel() + m_bod3->getVel()) / 3;
	velaverage = velaverage - glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 cross = glm::cross((m_bod2->getPos() - m_bod1->getPos()), (m_bod3->getPos() - m_bod1->getPos()));
	glm::vec3 normal = cross / cross.length();
	float area = 0.5 * cross.length();

	float crossarea = area * (glm::dot(velaverage, normal) / velaverage.length());

	glm::vec3 force = 0.5f * m_air2 * (velaverage.length() * velaverage.length()) * m_cd2 * crossarea * normal;
	force = force / 3;
	return force;

}

/*
** DRAG
*/
glm::vec3 Drag::apply(float mass, const glm::vec3& pos, const glm::vec3& vel) {
	// complete . Should return the aerodynamic drag force
	glm::vec3 d = 0.5f * m_air * (m_bod->getVel().length() * m_bod->getVel().length()) * m_cd * (-m_bod->getVel() / m_bod->getVel().length());
	return d;
}
/*
** HOOKE
*/
glm::vec3 Hooke::apply(float mass, const glm::vec3& pos, const glm::vec3& vel) {

	glm::vec3 direc = m_b2->getPos() - m_b1->getPos();
	float length = glm::length(direc);

	glm::vec3 e = direc / length;

	float v1 = glm::dot(m_b1->getVel(), e);
	float v2 = glm::dot(m_b2->getVel(), e);

	float fsd = -m_ks * (m_rest - length) - m_kd * (v1 - v2);
	glm::vec3 f1 = fsd * e;

	if (m_b2->getPos() == pos)
	{
		f1 = -f1;
	}

	return f1;
}
