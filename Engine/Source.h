#ifndef SOURCE_H
# define SOURCE_H

#include <iostream>
#include <string.h>
#include <time.h>
#include <vector>
#include "../GLib/Glib.h"
#include "../Engine/Vector3.h"
#include "BoundingBox.h"
#include "ISerializable.h"

#include "Timer.h"

using namespace std;
namespace Engine
{
	class GameObject:public Iserialisable
	{
	private:
		Vector3 position;
		Vector3 velocity;

	
		char *m_name;
		BoundingBox boundry;
		float zrotation;

	public:
		GameObject();
		GameObject(Vector3, Vector3);
		GameObject(const char * name);
		GameObject(const char* name, Vector3, Vector3, BoundingBox,float);


		inline Vector3 getPosition(void) const;
		inline Vector3 getVelociy(void) const;
		inline float getRotation(void) const;

		inline char* getname()const;
		inline BoundingBox getBoundingBox() const;
		
		inline void setPosition(const Vector3 i_x);
		inline void setVelocity(const Vector3 i_y);
		inline void setRotation(const float i_angle);
		inline void setGameObject(const char* name, Vector3, Vector3);

		inline HeashedString GameObject::getSerializableType();
		inline HeashedString GameObject::getSerializableId();
	

		inline size_t GameObject::serializePosition(uint8_t* buffer);
		inline size_t GameObject::deserializePosition(uint8_t* buffer);
		inline size_t GameObject::serializeRotation(uint8_t* buffer);
		inline size_t GameObject::deserializeRotation(uint8_t* buffer);
		inline size_t GameObject::serializeVelocity(uint8_t* buffer);
		inline size_t GameObject::deserializeVelocity(uint8_t* buffer);

		//	inline  bool _isnan(float a);
		//inline bool AlmostEqualUlps(float A, float B, int maxUlps);

		~GameObject()
		{
			delete m_name;
		};

	

	};




}

#include "Source-inl.h"
#endif