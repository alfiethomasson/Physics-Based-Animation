#include "RigidBody.h"

RigidBody::RigidBody()
{
}

RigidBody::~RigidBody()
{
}

void RigidBody::invInertia()
{

	

	float w = getMesh().getScale()[0][0];
	float h = getMesh().getScale()[1][1];
	float d = getMesh().getScale()[2][2];
	
	glm::mat3 inertia = glm::mat3(0.0f);
	inertia[0][0] = (getMass() * ((h * h) + (d * d))) / 12;
	inertia[1][1] = (getMass() * ((w * w) + (d * d))) / 12;
	inertia[2][2] = (getMass() * ((w * w) + (h * h))) / 12;

	m_invInertia = glm::inverse(inertia) * getRotate() * glm::transpose(getRotate());
}
