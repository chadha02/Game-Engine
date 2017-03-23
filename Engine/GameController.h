#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "Physics.h"
#include "../GLib/_System/Input.h"
#include "LuaFile.h"
#include "Serialize.h"

namespace Engine
{

	class GameController
	{
	public:
		virtual bool init(SmartPntr <GameObject> i_gObj) = 0;//, float i_mass) = 0;
		virtual void UpdatePositionSmart(physics *physicsPntr1, physics *physicsPntr2,float dt, Serialize * ) = 0;
		virtual void shutdown() = 0;
	protected:
		//physics *physicsPntr;
		Time::Timer *deltaTime;
				
	};

	class MosterController : public GameController
	{
	public:
		virtual bool init(SmartPntr <GameObject> i_gObj);
	
		virtual void UpdatePositionSmart(physics *physicsPntr1, physics *physicsPntr2, float dt, Serialize *);
		void shutdown();

	};

	class PlayerController : public GameController
	{
	public:
		virtual bool init(SmartPntr <GameObject> i_gObj);
		virtual void UpdatePositionSmart(physics *physicsPntr1, physics *physicsPntr2, float dt, Serialize *);
		void shutdown();


	};
}
