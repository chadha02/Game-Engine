#include "Physics.h"

namespace Engine
{
	physics::physics(SmartPntr <GameObject> i_gObj, float i_mass) :
		gObj(i_gObj), g_Mass(i_mass)
	{
		hascollided = false;
	}

	void physics::ApplyPhysics( float dt)
	{
		Vector3 accerlation = g_force / g_Mass;
		gObj->setVelocity((gObj->getVelociy() + (dt* accerlation)));
		gObj->setPosition(gObj->getPosition() + (dt * gObj->getVelociy()));
	}

	 physics::~physics()
	{

	}
}