#ifndef _ENGINE_H
#define _ENGINE_H


#include "GameController.h"
#include "Allocator.h"

#include "../Engine/SmartPntr.h"
#include "../GLib/_Console/ConsolePrint.h"
//#include "RenderInfo.h"
//#include "LuaFile.h"
#include "Rendering.h"
#include "LuaThreading.h"
#include "PhysicsInfo.h"


namespace Engine
{
	class EngineMain
	{
		//GameObject * smart_nObj, *smart_mObj;
		GameController * pCont;
		//MosterController *mCont;
		GameController *mCont;
		physics *p_physicsPntr;
		physics *m_physicsPntr;
		
		SmartPntr<GameObject> smart_nObj;
		SmartPntr<GameObject> smart_mObj;
		Rendering::RenderInfo *rInfo1;
		Rendering::RenderInfo *rInfo2;
		Mutex* fileMutex;
		Physicsinfo *pInfo;
		Time::Timer *a_Time;
		Serialize *serial;

	public:
		EngineMain()
		{
					
			fileMutex= new Mutex("Processor");
			a_Time = new Time::Timer();
			pCont = new PlayerController();
			mCont = new MosterController();
			pInfo = new Physicsinfo();
			serial = new Serialize();
			
		}

	
		
		void LoadObjects()
		{
			
			a_Time->Init();
			const char* pFile = "data\\Player.lua";
		
			LuaThreading::LoadFile(pFile, fileMutex);

			LuaThreading::ProcessFile(pFile, fileMutex);
			
			Engine::LUA *luaobj = new Engine::LUA();
		
			smart_nObj = (luaobj->LuaGameObjectLoad(pFile));
			Rendering::Init();
			pInfo->InitPhysicalbjects();
			
			//rInfo1 = new Rendering::RenderInfo(smart_nObj, "data\\GoodGuy.dds");
			rInfo1 = reinterpret_cast<Rendering::RenderInfo*>(luaobj->LuaImageLoad(smart_nObj, pFile));
			Rendering::RegisterRenderInfo(rInfo1);
			p_physicsPntr= reinterpret_cast<physics *>(luaobj->LuaPhysicsLoad(smart_nObj, pFile));
			
			
			const char* mFile = "data\\Monster.lua";
			LuaThreading::LoadFile(mFile, fileMutex);

			LuaThreading::ProcessFile(mFile, fileMutex);

			smart_mObj =(luaobj->LuaGameObjectLoad(mFile));
			//rInfo2 = new Rendering::RenderInfo(smart_mObj, "data\\BadGuy.dds");
			rInfo2 = reinterpret_cast<Rendering::RenderInfo*>(luaobj->LuaImageLoad(smart_mObj, mFile));
			Rendering::RegisterRenderInfo(rInfo2);
			m_physicsPntr= reinterpret_cast<physics *>(luaobj->LuaPhysicsLoad(smart_mObj, pFile));
			
			pInfo->RegisterPhysicalbjects(p_physicsPntr);
			pInfo->RegisterPhysicalbjects(m_physicsPntr);
			delete luaobj;
			serial->Initialize(1024 * 1024);

			serial->AddSerializableObject(smart_nObj.getObject());
			serial->AddSerializableObject(smart_mObj.getObject());
		}

		bool InitControllers()
		{
			
			pCont->init(smart_nObj);
			mCont->init(smart_mObj);
			
			return true;
		}

		bool quit()
		{
			bool done = false;
			GLib::Service(done);
			return done;
		}

		void run()
		{
			while (1)
			{
				if (quit())
					break;
				
				a_Time->Update();
				if (smart_nObj != nullptr)
				{
					
					pCont->UpdatePositionSmart(p_physicsPntr, m_physicsPntr, a_Time->deltaTime,serial);
					
				}
				if (smart_mObj != nullptr)
				{
					mCont->UpdatePositionSmart(m_physicsPntr, p_physicsPntr, a_Time->deltaTime,serial);
					
				}
			
			//	pInfo->ApplyCollisionInfo(a_Time->deltaTime);
				pInfo->CheckCollision(a_Time->deltaTime);
				Rendering::DrawRenderedObjects();
				DEBUG_PRINT("velocity x = %f\n", p_physicsPntr->GetGameObject()->getVelociy().getx());
				if (a_Time->totalTime > 2.0f && fabs(p_physicsPntr->GetForce().getx()) < 1.0f)
					break;

			/*	if (p_physicsPntr->hascollided && p_physicsPntr->GetGameObject()->getVelociy().getx()==1.0f )
				{
					
						break;
				}
				*/
				if ((p_physicsPntr ->GetGameObject()->getPosition().getx() < -400))
				{
					DEBUG_PRINT("You LOOSE");
					break;
				}

				if ((m_physicsPntr->GetGameObject()->getPosition().getx() > 420))
				{
					DEBUG_PRINT("You WIN");
					break;
				}
			}
		}

		void ShutDown()
		{
			//GLib::Sprites::Release(smart_nObj->getSprite());
			GLib::Sprites::Release(rInfo1->getSprite());

			//GLib::Sprites::Release(smart_mObj->getSprite());
			GLib::Sprites::Release(rInfo2->getSprite());
			GLib::Shutdown();
			mCont->shutdown();
			pCont->shutdown();
			Rendering::ShutDown();
			pInfo->ShutPhysicalObjects();
			serial->ShutDown();
//			delete luaThread;
			Engine::ThreadedFileProcessor::Shutdown();
			
		}

		~EngineMain()
		{
			delete pCont;
		  delete mCont;
		  delete rInfo1;
		  delete rInfo2;
		  delete pInfo;
		  delete p_physicsPntr;
		  delete m_physicsPntr;
		  delete a_Time;
		  delete fileMutex;
		  delete serial;
		}
	};
}

#endif




