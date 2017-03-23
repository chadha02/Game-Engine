#include"GameController.h"
#include "../GLib/_Console/ConsolePrint.h"

namespace Engine
{
	

	bool MosterController::init(SmartPntr <GameObject> i_gObj)
	{
		//Engine::LUA *luaobj = new Engine::LUA();
		//physicsPntr = reinterpret_cast<physics *>(luaobj->LuaPhysicsLoad(i_gObj, "data\\Monster.lua"));
		//delete luaobj;
		deltaTime = new Time::Timer();
	//	if (!deltaTime->Init())
		//	return false;
		return true;
	}

	
	void MosterController::UpdatePositionSmart(physics *physicsPntr1, physics *physicsPntr2, float dt, Serialize *s)
	{

		Vector3 moveX(-45.0f, 0.0f, 0.0f);
		float angleInRadian = 1.0f;
		//	Vector3 moveY(0.0f, 1.0f, 0.0f);
		//	Vector3 moveZ(0.0f, 0.0f, 1.0f);

		Vector3 moveY(0.0f, -15.0f, 0.0f);
		Vector3 movereverseY(0.0f, 15.0f, 0.0f);
	
		if (!(physicsPntr1->hascollided))
		{
		//	float monpos_y = round(physicsPntr1->GetGameObject()->getPosition().gety());
		//	float playerpos_y = round(physicsPntr2->GetGameObject()->getPosition().gety());

		//	float monpos_x = fabs(round(physicsPntr1->GetGameObject()->getPosition().getx()));
		//	float playerpos_x =	fabs( round(physicsPntr2->GetGameObject()->getPosition().getx()));
				
		//	DEBUG_PRINT("player pos = %f", playerpos_x);
			//DEBUG_PRINT("monster pos = %f", monpos_x);

		//	if ((monpos_x - playerpos_x) > 150)
			{
				physicsPntr1->SetForce(moveX);
				physicsPntr1->ApplyPhysics(dt);
			}
			
		}
		
		else
		{
			//physicsPntr->ApplyPhysics(-dt);
			return;
		}


		//physicsPntr->ApplyRotation(angleInRadian);

	}

	void MosterController::shutdown()
	{
		//delete physicsPntr;
		delete deltaTime;
	}

	bool PlayerController::init(SmartPntr <GameObject> i_gObj)
	{
		//Engine::LUA *luaobj = new Engine::LUA();
		//physicsPntr = reinterpret_cast<physics *>(luaobj->LuaPhysicsLoad(i_gObj, "data\\Player.lua"));
		//delete luaobj;
		deltaTime = new Time::Timer();
		if (!deltaTime->Init())
			return false;
		return true;
	}

	void PlayerController::UpdatePositionSmart(physics *physicsPntr1, physics *physicsPntr2, float dt,  Serialize * s)
	{
		Vector3 moveX(40.0f, 0.0f, 0.0f);
		Vector3 moveY(0.0f, 20.0f, 0.0f);
		float time = dt;
	//	DEBUG_PRINT("deltaTime= %f\n", time);
			

		int key = GLib::Input::GetKey();
	//	if (!physicsPntr->hascollided)
		{
		//	physicsPntr->SetForce(moveX);
		//	physicsPntr->ApplyPhysics(dt);
			switch (key)
			{
		/*	case 0x57:
				physicsPntr1->SetForce(moveY);
				physicsPntr1->ApplyPhysics(time);

				break;*/

			case 0x41:
				physicsPntr1->SetForce(-1.0f * moveX);
				physicsPntr1->ApplyPhysics(time);//A
				break;

			/*case 0x53:
				physicsPntr1->SetForce(-1.0f * moveY);
				physicsPntr1->ApplyPhysics(time);//S
				break;*/
			case 0x44:
				physicsPntr1->SetForce(moveX);
				physicsPntr1->ApplyPhysics(time);//D
				break;
			case 0x46:
				s->SerialiseObjects(); //F
				break;
			case 0x4C:
				s->DeSerialiseObjects();//L
				break;
			case -1: physicsPntr1->GetGameObject()->setVelocity(physicsPntr1->GetGameObject()->getVelociy() - (0.10f * physicsPntr1->GetGameObject()->getVelociy()));// key is up reset velocity
				break;
			}
			
	
		}
		//else
		//	return;
	}
	void PlayerController::shutdown()
	{
		//delete physicsPntr;
		delete deltaTime;
	}
}

//deltaTime->Update()