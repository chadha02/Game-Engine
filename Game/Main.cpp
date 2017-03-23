#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>
#define _USE_MATH_DEFINES
#include <cmath>

#if defined _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif // _DEBUG

#include "GLib.h"
#include "../Engine/Source.h"
#include "../Engine/Allocator.h"
#include "../Engine/Engine.h"
#include "Game.h"
#include "../Engine/StringPool.h"
#include "../Engine/Messaging.h"
#include "../Engine/Matrix4x4.h"


class TestMessage : public Engine::MessagingSystem::IMessageHandler
{
	void HandleMessage(const Engine::HeashedString & i_Message, void *i_pHandle)
	{
		DEBUG_PRINT("Message Handler Unit test works as expected\n");
	}
};
void MessageHandlerUnitTest()
{
	using namespace Engine::MessagingSystem;

	Messaging *message = new Messaging();
	IMessageHandler *handle = new TestMessage();
	message->RegisterMessageHandler("Test", handle);
	message->NotifyMessage("Test", NULL);
	message->DeregisterMessageHandler("Test", handle);
	delete message;
	delete handle;
}
void MatrixUnitTest()
{
	using namespace Engine;
	Vector4 newvector = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	Matrix4x4 transmatrix = Matrix4x4::CreateTranslation(10.0f, 0.0f, 0.0f);
	Vector4 outvector = transmatrix.MultiplyRight(newvector);
	DEBUG_PRINT("Expected Output = 11.0 , Produced Output = %f\n",  outvector.getx());
	DEBUG_PRINT("Expected Output = 1.0 , Produced Output = %f\n", outvector.getw());

	Vector4 newvector2 = Vector4(0.0f, 1.0f, 0.0f, 0.0f); 
	Matrix4x4 rotmatrix = Matrix4x4::CreateZRotation((float)M_PI_2);
	outvector = rotmatrix.MultiplyRight(newvector2);
	DEBUG_PRINT("Expected Output = -1.0 , Produced Output = %f\n", outvector.getx());

	Matrix4x4 transrotmatrix = transmatrix * rotmatrix;
	outvector = transrotmatrix.MultiplyRight(newvector);
	DEBUG_PRINT("Expected Output = 10.0, 1.0 ,0.0, 1.0 , Produced Output = (%f, %f, %f, %f)\n", outvector.getx(), outvector.gety(), outvector.getz(), outvector.getw());

	Matrix4x4 transposeMatrix;
	transrotmatrix.Transpose();
	transposeMatrix = transrotmatrix;
	outvector= transposeMatrix.MultiplyRight(newvector);
	DEBUG_PRINT("Expected Output =-1.0, 0.0, 11.0f , Produced Output = %f, %f, %f\n",  outvector.gety(), outvector.getz(), outvector.getw());
}


void TestKeyCallback(unsigned int i_VKeyID, bool bWentDown)
{
	printf("%d went %s\n", i_VKeyID, bWentDown ? "down" : "up");
}

void TestStringPool()
{
	using namespace Engine;
	
	StringPool *pPool = StringPool::Create(1024);
		
	const char * pNeo = pPool->add("Arpit Chadha");

	const char * pdNeo = pPool->add("ArpitsChadha");
	const char * pdNeos = pPool->find("ArpitsChadha");

	if (pdNeo == pdNeos)
	{
	DEBUG_PRINT("StringPool works as Expected\n");
	}
	else
	DEBUG_PRINT("StringPool not working\n");


	delete pPool;
	
}


int WINAPI wWinMain(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow)
{
	
	
	Engine::EngineMain *Einstance = new Engine::EngineMain();
	
	bool bSuccess = GLib::Initialize(i_hInstance, i_nCmdShow, "HeadON", -1, 1200, 720);
	
	
	if (bSuccess)
	{
		GLib::SetKeyStateChangeCallback(TestKeyCallback);
		Einstance->LoadObjects();
	
		if (Einstance->InitControllers() && Game::Init())
		{
			Einstance->run();
			TestStringPool();
			MessageHandlerUnitTest();
			MatrixUnitTest();
		}
		Einstance->ShutDown();
			
	
	}
	delete Einstance;
	
	
	
#if defined _DEBUG
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

	return 0;
}



