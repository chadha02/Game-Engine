#include "Source.h"

using namespace Engine;


GameObject::GameObject()
{
	m_name = NULL;
}
GameObject::GameObject(Vector3 a, Vector3 b) : position(a), velocity(b), m_name(NULL)
{	}

GameObject::GameObject(const char * name)
{
	m_name = new char[strlen(name) + 1];
	strcpy_s(m_name, strlen(name) + 1, name);
}

GameObject::GameObject(const char*name, Vector3 a, Vector3 b, BoundingBox c, float d)
:position(a), velocity(b),boundry(c),zrotation(d)
{
	m_name = new char[strlen(name) + 1];
	strcpy_s(m_name, strlen(name) + 1, name);
	//m_sprite = image;
}



