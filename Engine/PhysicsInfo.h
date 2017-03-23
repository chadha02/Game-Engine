#pragma once
#ifndef PHYSICSINFO_H
#define PHYSICSINFO_H

#include "Physics.h"
#include "Matrix4x4.h"

namespace Engine
{
	/*struct Collision
	{
		physics * obj1;
		physics * obj2;
		Vector4 collisionNormal;
		float timeOfCollision;
	};*/

	
	class Physicsinfo
	{
	private:
		std::vector<physics *> *PhysicalObjects;
		Time::Timer *deltaTime;
		Vector4 collisionNormal;
		float timeOfCollision;

	public:

		Physicsinfo()
		{}

		void InitPhysicalbjects()
		{
			PhysicalObjects = new std::vector<physics *>;
			deltaTime = new Time::Timer();
			
		}

		void RegisterPhysicalbjects(physics *pObj)
		{
			PhysicalObjects->push_back(pObj);
		}

	


		bool checkForAxisCollisions(float start, float end, float center, float realativevelocity, float &axisClose, float &axisOpen)
		{
			if (start > end)
			{
				float swap = end;
				end = start;
				start = swap;
			}

			//If AVelocityInB.x is 0 we aren’t moving in the x direction so we’re either collided or non-collided depending on 
			//ACenterInB.x relative to 𝐵_𝑙𝑒𝑓𝑡 and 𝐵_𝑟𝑖𝑔ℎ𝑡
			if (realativevelocity == 0.0f)
			{
				if (start < center && end > center)
				return true;
				else
				{
					axisClose = 0.0f;
					axisOpen = 1.0f;
					return false;
				}

			}

			axisClose = (start - center) / realativevelocity; //tclose
			axisOpen = (end - center) / realativevelocity;//topen
		//If we calculate a 𝑡_𝑜𝑝𝑒𝑛 that’s less than 𝑡_𝑐𝑙𝑜𝑠𝑒 it mean A’s velocity is moving it away from B and we can simply swap 𝑡_𝑜𝑝𝑒𝑛 and 𝑡_𝑐𝑙𝑜𝑠𝑒
			
			if (axisOpen < axisClose)
			{
				float swap = axisOpen;
				axisOpen = axisClose;
				axisClose = swap;
			}
			//If we calculate a 𝑡_𝑐𝑙𝑜𝑠𝑒 that’s greater than 𝑡_(𝑒𝑛𝑑_𝑓𝑟𝑎𝑚𝑒) or 1.0f here that means that the separation will close after this frame or  no collision 
			//If we calculate a 𝑡_𝑜𝑝𝑒𝑛 that’s less than 0 that means we are A is past B and the collision, if it happened, happened in the past or  no collision 

			
			if ((axisClose >= 1.0f) || (axisOpen <= 0.0f))
				return false; // no collision
		}


		bool CheckCollision(physics * A, physics * B, float dt)//, Collision & collisionInfo)
		{
			// will be used to check earliest open and latest close
			float tClose = 0.0f;
			float tOpen = 1.0f;

			// will be used to check sepration close and open for a perticular axis
			float axisClose = 0.0f;
			float axisOpen = 1.0f;
			

			// transfroms from actors to world
			Matrix4x4 actorAToWorld = Matrix4x4::CreateTranslation(A->GetGameObject()->getPosition()) 
				* Matrix4x4::CreateZRotation(A->GetGameObject()->getRotation());
			Matrix4x4 actorBToWorld = Matrix4x4::CreateTranslation(B->GetGameObject()->getPosition()) 
				* Matrix4x4::CreateZRotation(B->GetGameObject()->getRotation());

			//transfroms to and from A and B
			Matrix4x4 AToB = actorBToWorld.GetInverse() * actorAToWorld;
			Matrix4x4 BToA = actorAToWorld.GetInverse() * actorBToWorld;

			//calculating centers in each others coordinate systems
			Vector4 ACenterInB = AToB * Vector4(A->GetGameObject()->getBoundingBox().GetCenter(),1.0f);
			Vector4 BCenterInA = BToA * Vector4(B->GetGameObject()->getBoundingBox().GetCenter(), 1.0f);

			//calculating relative velocity atob in b coordinate system
			Vector3 relativeAVelocityInWorld = A->GetGameObject()->getVelociy() - B->GetGameObject()->getVelociy();
			Vector4 relativeVeloctityAInB(relativeAVelocityInWorld, 1.0f);
			relativeVeloctityAInB =  AToB * relativeVeloctityAInB;
			//	relativeVeloctityAInB = actorBToWorld.GetInverse() * relativeVeloctityAInB;


			//calculating the extents of A in B coordinate system

			Vector4 BoxExtentsXAInB = AToB *  Vector4(A->GetGameObject()->getBoundingBox().GetExtent().getx(), 0.0f, 0.0f, 0.0f);
			Vector4 BoxExtentsYAInB = AToB * Vector4(0.0f, A->GetGameObject()->getBoundingBox().GetExtent().gety(), 0.0f, 0.0f);
			float projectionExtendsAOnB_x = fabs(BoxExtentsXAInB.getx()) + fabs(BoxExtentsYAInB.getx());
			float projectionExtendsAOnB_Y = fabs(BoxExtentsXAInB.gety()) + fabs(BoxExtentsYAInB.gety());

			// combining the extents of B with projection of A axes on B to get a line to point check
			float CombinedExtentsX = projectionExtendsAOnB_x + B->GetGameObject()->getBoundingBox().GetExtent().getx();
			float CombinedExtentsY = projectionExtendsAOnB_Y + B->GetGameObject()->getBoundingBox().GetExtent().gety() ;

			//start and end for x axis
			float start = B->GetGameObject()->getBoundingBox().GetCenter().getx() - CombinedExtentsX;
			float end = B->GetGameObject()->getBoundingBox().GetCenter().getx() + CombinedExtentsX;

			Vector4 collisionAxis;// = Matrix4x4::CreateZRotation(A->GetGameObject()->getRotation()).Col(0).getUnitVector();

			//Check for separation parallel to obj1 Y-axis
			if (!checkForAxisCollisions(start, end, ACenterInB.getx(), relativeVeloctityAInB.getx(), axisClose, axisOpen))
			{
				return false;
			}
			
			if (axisClose > tClose)
			{
				tClose = axisClose;
				collisionAxis = Matrix4x4::CreateZRotation(A->GetGameObject()->getRotation()).Col(0).getUnitVector();
			}
			if (axisOpen < tOpen)
			{
				tOpen = axisOpen;
				collisionAxis = Matrix4x4::CreateZRotation(A->GetGameObject()->getRotation()).Col(0).getUnitVector();
			}
		
			//start and end for y axis
			start = B->GetGameObject()->getBoundingBox().GetCenter().gety() - CombinedExtentsY;
			end = B->GetGameObject()->getBoundingBox().GetCenter().gety() + CombinedExtentsY;
		
			//Check for separation parallel to obj1 X-axis
		//	collisionAxis = Matrix4x4::CreateZRotation(A->GetGameObject()->getRotation()).Col(1).getUnitVector();
			if (!checkForAxisCollisions(start, end, ACenterInB.gety(), relativeVeloctityAInB.gety(), axisClose, axisOpen))
			{			return false;
			}
			
			if (axisClose > tClose)
			{
				tClose = axisClose;
				collisionAxis = Matrix4x4::CreateZRotation(A->GetGameObject()->getRotation()).Col(1).getUnitVector();
			}
			if (axisOpen < tOpen)
			{
				tOpen = axisOpen;
				collisionAxis = Matrix4x4::CreateZRotation(A->GetGameObject()->getRotation()).Col(1).getUnitVector();
			}
		

			//calculating the extents of B in A coordinate system
			Vector4 BoxExtentsXBInA = BToA *  Vector4(B->GetGameObject()->getBoundingBox().GetExtent().getx(), 0.0f, 0.0f, 0.0f);
			Vector4 BoxExtentsYBInA = BToA * Vector4(0.0f, B->GetGameObject()->getBoundingBox().GetExtent().gety(), 0.0f, 0.0f);
			float projectionExtendsBOnA_X = fabs(BoxExtentsXBInA.getx()) + fabs(BoxExtentsYBInA.getx());
			float projectionExtendsBOnA_Y = fabs(BoxExtentsXBInA.gety()) + fabs(BoxExtentsYBInA.gety());

			// combining the extents of A with B to get a line to point check
			CombinedExtentsX = projectionExtendsBOnA_X + A->GetGameObject()->getBoundingBox().GetExtent().getx();
			CombinedExtentsY = projectionExtendsBOnA_Y + A->GetGameObject()->getBoundingBox().GetExtent().gety() ;

			start = A->GetGameObject()->getBoundingBox().GetCenter().getx() - CombinedExtentsX;
			end = A->GetGameObject()->getBoundingBox().GetCenter().getx() + CombinedExtentsX;

			//calculating relative velocity BtoA in B coordinate system
			Vector3 relativeBVelocityInWorld = B->GetGameObject()->getVelociy() - A->GetGameObject()->getVelociy();
			Vector4 relativeVeloctityBInA(relativeBVelocityInWorld, 1.0f);
	
			relativeVeloctityBInA = BToA * relativeVeloctityBInA;
			//relativeVeloctityBInA = actorAToWorld.GetInverse() * relativeVeloctityAInB;


			//Check for separation parallel to obj2 Y-axis
		//	 collisionAxis = Matrix4x4::CreateZRotation(B->GetGameObject()->getRotation()).Col(0).getUnitVector();

			if (!checkForAxisCollisions(start, end, BCenterInA.getx(), relativeVeloctityBInA.getx(), axisClose, axisOpen))
			{
				return false;
			}

			if (axisClose > tClose)
			{
				tClose = axisClose;
				collisionAxis = Matrix4x4::CreateZRotation(B->GetGameObject()->getRotation()).Col(0).getUnitVector();
			}
			if (axisOpen < tOpen)
			{
				tOpen = axisOpen;
				collisionAxis = Matrix4x4::CreateZRotation(B->GetGameObject()->getRotation()).Col(0).getUnitVector();
			}

		

		//Check for separation parallel to obj2 X-axis
			start = A->GetGameObject()->getBoundingBox().GetCenter().gety() - CombinedExtentsY;
			end = A->GetGameObject()->getBoundingBox().GetCenter().gety() + CombinedExtentsY;
		//	collisionAxis = Matrix4x4::CreateZRotation(B->GetGameObject()->getRotation()).Col(1).getUnitVector();

			if (!checkForAxisCollisions(start, end, BCenterInA.gety(), relativeVeloctityBInA.gety(), axisClose, axisOpen))
			{
				return false;
			}

			if (axisClose > tClose)
			{
				tClose = axisClose;
				collisionAxis = Matrix4x4::CreateZRotation(B->GetGameObject()->getRotation()).Col(1).getUnitVector();
			}
			if (axisOpen < tOpen)
			{
				tOpen = axisOpen;
				collisionAxis = Matrix4x4::CreateZRotation(B->GetGameObject()->getRotation()).Col(1).getUnitVector();
			}

			if (tOpen > tClose)
			{
				timeOfCollision = tClose;
				collisionNormal = collisionAxis;
				
			}

		//If the latest 𝑡_𝑐𝑙𝑜𝑠𝑒 was BEFORE the earliest 𝑡_𝑜𝑝𝑒𝑛 that all separation closed there was a collision.

		return tOpen > tClose;
		}


		

		void CheckCollision(float dt)
		{
			for (unsigned int i = 0; i < PhysicalObjects->size() ; i++)
			{
				for (unsigned int j = i + 1; j < PhysicalObjects->size(); j++)
				{
					//Collision info;
					if (CheckCollision((*PhysicalObjects)[i], (*PhysicalObjects)[j], dt))//, info))
					{
						
					//	DEBUG_PRINT("\nCollison Happenned");
						(*PhysicalObjects)[i]->hascollided = true;
						(*PhysicalObjects)[j]->hascollided = true;
						AfterCollision((*PhysicalObjects)[i], (*PhysicalObjects)[j], dt);
						
					}
					else
					{
						(*PhysicalObjects)[i]->hascollided = false;
						(*PhysicalObjects)[j]->hascollided = false;
					}
				}
			}
		}

	/*	void ApplyCollisionInfo(float dt)
		{
			for (std::vector<physics *>::iterator it = PhysicalObjects->begin(); it != PhysicalObjects->end(); it++)
			{
				if ((*it)->hascollided)
				{
					(*it)->ApplyPhysics(-(dt);
				//	(*it)->ApplyPhysics(timeOfCollision);
				//	(*it)->ApplyPhysics(dt - timeOfCollision);
				}
				else
					return;
			}
		}*/

		void AfterCollision(physics *A, physics *B, float dt)
		{

		
			
		
			Vector3 velocityOne = ((A->GetMass() - B->GetMass()) / (A->GetMass() + B->GetMass())) * A->GetGameObject()->getVelociy() +
				((2 * B->GetMass() / (A->GetMass() + B->GetMass()))) * B->GetGameObject()->getVelociy();

			Vector3 velocityTwo = ((B->GetMass() - A->GetMass()) / (A->GetMass() + B->GetMass())) *B->GetGameObject()->getVelociy() +
				((2 * A->GetMass() / (A->GetMass() + B->GetMass()))) * A->GetGameObject()->getVelociy();

			A->GetGameObject()->setVelocity(velocityOne);
			B->GetGameObject()->setVelocity(velocityTwo);

			A->ApplyPhysics(dt - timeOfCollision);
			B->ApplyPhysics(dt-timeOfCollision);

			
		}

		void ShutPhysicalObjects()
		{
			delete PhysicalObjects;
			delete deltaTime;
		}

		~Physicsinfo()
		{

		}
	};
}

#endif