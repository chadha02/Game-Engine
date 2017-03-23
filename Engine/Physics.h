#pragma once

#ifndef PHYSICS_H
#define PHYISCS_H

#include "Source.h"
#include "SmartPntr.h"

namespace Engine
{
	class physics
	{
		SmartPntr<GameObject> gObj;
		float g_Mass;
		Vector3 g_force;
		//float g_Drag;

	public:
		bool hascollided;
		physics(SmartPntr<GameObject>, float);
		/*static void UpdatePhysics(SmartPntr<GameObject> &i_gObj, const Vector3 &force, float dt)
		{
			Vector3 accerlation = force / i_gObj->getMass();
			i_gObj->setVelocity((i_gObj->getVelociy() + (dt* accerlation)));

			i_gObj->setPosition(i_gObj->getPosition() + (dt * i_gObj->getVelociy()));
		}*/

		void ApplyPhysics( float dt);
		inline void ApplyRotation(const float &angle)
		{
			gObj->setRotation(gObj->getRotation() + angle);
		}
		inline SmartPntr<GameObject>  GetGameObject()
		{
			return gObj;
		}
		
		inline void SetForce(const Vector3 i_force)
		{
			g_force = i_force;
		}
		inline Vector3 GetForce() const
		{
			return g_force;
		}
		inline float GetMass() const
		{
			return g_Mass;
		}

		~physics();
		

	};
}



#endif