#ifndef SOURCE_INL_H
# define SOURCE_INL_H

#include <assert.h>


namespace Engine
{
	inline Vector3 GameObject::getPosition(void) const
	{
		return position;
		
	}

	inline Vector3 GameObject::getVelociy(void) const
	{
		return velocity;
	}
	
	inline float GameObject::getRotation(void) const
	{
		return zrotation;
	}

	inline char * GameObject::getname()const
	{
		return m_name;
	}

	inline BoundingBox GameObject::getBoundingBox() const
	{
		return boundry;
	}
	/*inline float GameObject::getMass()const
	{
		return m_mass;
	}

/*	inline GLib::Sprites::Sprite *GameObject::getSprite()
	{
		return m_sprite;
	}*/

	inline void GameObject::setPosition(const Vector3 i_x)
	{
		position = i_x;
	}
	inline void GameObject::setVelocity(const Vector3 i_y)
	{
		velocity = i_y;
	}

	inline void GameObject::setRotation(const float i_angle)
	{
		zrotation = i_angle;
	}

	inline void GameObject::setGameObject(const char* i_name, Vector3 i_position, Vector3 i_velocity)
	{
		m_name = new char[strlen(i_name) + 1];
		strcpy_s(m_name, strlen(i_name) + 1, i_name);
		position = i_position;
		velocity = i_velocity;

	}

	inline HeashedString GameObject::getSerializableType()
	{
		return "GameObject";
	}
	inline HeashedString GameObject::getSerializableId()
	{
		return m_name;
	}

	inline size_t GameObject::serializePosition(uint8_t* buffer)
	{
		assert(buffer != nullptr);

		memcpy(buffer, &position, sizeof(position));

		return sizeof(position);
	}
	inline size_t GameObject::deserializePosition(uint8_t* buffer)
	{

		memcpy(&(position), buffer, sizeof(position));
		return sizeof(position);
	}
	
	inline size_t GameObject::serializeRotation(uint8_t* buffer)
	{
		assert(buffer != nullptr);

		memcpy(buffer, &zrotation, sizeof(zrotation));

		return sizeof(zrotation);
	}

	inline size_t GameObject::deserializeRotation(uint8_t* buffer)
	{

		memcpy(&(zrotation), buffer, sizeof(zrotation));
		return sizeof(zrotation);
	}
	
	inline size_t GameObject::serializeVelocity(uint8_t* buffer)
	{
		assert(buffer != nullptr);

		memcpy(buffer, &velocity, sizeof(velocity));

		return sizeof(velocity);
	}

	inline size_t GameObject::deserializeVelocity(uint8_t* buffer)
	{

		memcpy(&(velocity), buffer, sizeof(velocity));
		return sizeof(velocity);
	}

	/*inline void GameObject::setMass(const float i_mass)
	{
		m_mass = i_mass;
	}*/
	/*inline void GameObject::setSprite( GLib::Sprites::Sprite *sprite)
	{
		m_sprite = sprite;
	}/*

/*	inline bool GameObject::_isnan(float a)
	{

		//if (isnan(a))
		if (a!=a)
			return true;
		else
		return false;
	}
	inline bool GameObject::AlmostEqualUlps(float A, float B, int maxUlps)
	{
		assert(sizeof(float) == sizeof(int));

		if (A == B)

			return true;

		int intDiff = abs(*(int*)&A - *(int*)&B);

		if (intDiff <= maxUlps)

			return true;

		return false;
	}*/


}
#endif