#pragma once

#ifndef _LUAFILE_H_
#define _LUAFILE_H_


#include "lua.hpp"
#include "RenderInfo.h"
#include "Physics.h"

namespace Engine
{

	class LUA
	{
	public:
		uint8_t * LoadLuaFile(const char * i_pFileName, size_t & o_sizeFile)
		{

			assert(i_pFileName != NULL);

			FILE *				pFile = NULL;

			errno_t fopenError = fopen_s(&pFile, i_pFileName, "rb");
			if (fopenError != 0)
				return NULL;

			assert(pFile != NULL);

			int FileIOError = fseek(pFile, 0, SEEK_END);
			assert(FileIOError == 0);

			long FileSize = ftell(pFile);
			assert(FileSize >= 0);

			FileIOError = fseek(pFile, 0, SEEK_SET);
			assert(FileIOError == 0);

			uint8_t * pBuffer = new uint8_t[FileSize];
			assert(pBuffer);

			size_t FileRead = fread(pBuffer, 1, FileSize, pFile);
			assert(FileRead == FileSize);

			fclose(pFile);

			o_sizeFile = FileSize;

			return pBuffer;

		}

	static	Vector3 LuaGetVector3(lua_State * i_pLuaState, const char * i_pVectorName)
		{
			Vector3 newVector;

			float tempVector[3] = { 0.0f };

			int result = 0;

			lua_pushstring(i_pLuaState, i_pVectorName);
			result = lua_gettable(i_pLuaState, -2);
			assert(result == LUA_TTABLE);

			lua_pushnil(i_pLuaState);
			for (int i = 0; i < 3; i++) {

				lua_next(i_pLuaState, -2);
				assert(lua_type(i_pLuaState, -1) == LUA_TNUMBER);

				tempVector[i] = (float)lua_tonumber(i_pLuaState, -1);

				lua_pop(i_pLuaState, 1);

			}
			lua_pop(i_pLuaState, 2);
			newVector.setvector(tempVector[0], tempVector[1], tempVector[2]);
			return newVector;
		}

	SmartPntr<GameObject> LuaGameObjectLoad(const char *i_pfilename)
		{
			//create a lua_State
			lua_State * pLuaState = luaL_newstate();
			assert(pLuaState);

			luaL_openlibs(pLuaState);
			// Read in a file
			size_t			sizeFile = 0;
			void *		pFileContents = LoadLuaFile(i_pfilename, sizeFile);
			assert(pFileContents);
			if (pFileContents  && sizeFile)
			{
				int	result = 0;

				// Do the initial buffer parsing
				result = luaL_loadbuffer(pLuaState, reinterpret_cast<char *>(pFileContents), sizeFile, nullptr);
				assert(result == 0);
				lua_pcall(pLuaState, 0, 0, 0);
				assert(result == 0);

				int			type = LUA_TNIL;

				// Find the global variable named "Player" and push it onto stack.
				// If it doesn't find it it pushes a nil value instead

				result = lua_getglobal(pLuaState, "Actor");
				assert(result == LUA_TTABLE);

				// Get the Actor name from the Player table
				// 1. Push the key string
				lua_pushstring(pLuaState, "name");
				// 2. Get the value associated with the key
				type = lua_gettable(pLuaState, -2);
				assert(type == LUA_TSTRING);
				const char * pName = lua_tostring(pLuaState, -1);  // remeber to free this
				assert(pName != NULL);
				if (pName)
					pName = _strdup(pName);
				// 3. Remove the value from the stack now that we're done with it
				lua_pop(pLuaState, 1);

				// Get the Actors position from the Player table
				Vector3 initialPosition = LuaGetVector3(pLuaState, "initial_position");
				// Get the Actors velocity from the Player table
				Vector3 initialVelocity = LuaGetVector3(pLuaState, "initial_velocity");
				//push "initialRotation" onto the stack to request from table
				lua_pushstring(pLuaState, "initialRotation");
				result = lua_gettable(pLuaState, -2);
				assert(result == LUA_TNUMBER);
				float initialRotation = (float)lua_tonumber(pLuaState, -1);
				//pop the stack to clear the rotation stuff off
				lua_pop(pLuaState, 1);

				// let's process the "physics_settings" sub table
				float p_mass;
				lua_pushstring(pLuaState, "physics_settings");
				type = lua_gettable(pLuaState, -2);
				// It either needs to be a table or not there (no physics applied)
				assert((type == LUA_TNIL) || (type == LUA_TTABLE));

				if (type == LUA_TTABLE)
				{
					lua_pushstring(pLuaState, "mass");
					type = lua_gettable(pLuaState, -2);
					assert(type == LUA_TNUMBER);

					p_mass = float(lua_tonumber(pLuaState, -1));

					// pop the "mass" and "drag" values together
					lua_pop(pLuaState, 1); // n is the number of items you want to pop. 1 because i have only mass
				}
				
				// we're done with the "physics_settings" table - pop it
				// NOTE: If it didn't exist Lua would have pushed a nil value so still need the pop
				lua_pop(pLuaState, 1);
				
				// let's process the "bounding_box" sub table
				Vector3 center, extents;
				lua_pushstring(pLuaState, "bounding_box");
				type = lua_gettable(pLuaState, -2);
				assert((type == LUA_TNIL) || (type == LUA_TTABLE));

				if (type == LUA_TTABLE)
				{
					center= LuaGetVector3(pLuaState, "center");
					extents= LuaGetVector3(pLuaState, "extents");
				}
				// we're done with the "bounding_box" table - pop it
				
				lua_pop(pLuaState, 1);
				
				BoundingBox bound(extents, center);
				SmartPntr<GameObject> gObj = new GameObject(pName, initialPosition, initialVelocity, bound,initialRotation);

				delete pName;


				// pop "Player" table
				lua_pop(pLuaState, 1);

				int stack_items = lua_gettop(pLuaState);
				assert(stack_items == 0);

				delete[] pFileContents;
				lua_close(pLuaState);
				return gObj;

			}



		}

		void * LuaImageLoad(SmartPntr<GameObject>gObj, const char *i_pFile)
		{
			lua_State * pLuaState = luaL_newstate();
			assert(pLuaState);

			luaL_openlibs(pLuaState);
			// Read in a file
			size_t			sizeFile = 0;
			void *		pFileContents = LoadLuaFile(i_pFile, sizeFile);
			assert(pFileContents);
			if (pFileContents  && sizeFile)
			{
				int	result = 0;

				// Do the initial buffer parsing
				result = luaL_loadbuffer(pLuaState, reinterpret_cast<char *>(pFileContents), sizeFile, nullptr);
				assert(result == 0);
				lua_pcall(pLuaState, 0, 0, 0);
				assert(result == 0);

				int			type = LUA_TNIL;

				// Find the global variable named "Player" and push it onto stack.
				// If it doesn't find it it pushes a nil value instead

				result = lua_getglobal(pLuaState, "Actor");
				assert(result == LUA_TTABLE);

				// let's process the "render_settings" sub table
				lua_pushstring(pLuaState, "render_settings");
				type = lua_gettable(pLuaState, -2);
				assert((type == LUA_TNIL) || (type == LUA_TTABLE));
				lua_pushstring(pLuaState, "spriteFile");
				type = lua_gettable(pLuaState, -2);
				assert(type == LUA_TSTRING);

				const char * mySprite = lua_tostring(pLuaState, -1);
				assert(mySprite);
				if (mySprite)
				{
					mySprite = _strdup(mySprite);
				}
				lua_pop(pLuaState, 1);

				// we're done with the "render_settings" table - pop it
				lua_pop(pLuaState, 1);

				Rendering::RenderInfo *rInfo = new Rendering::RenderInfo(gObj, mySprite);
				delete mySprite;

				// pop "Player" table
				lua_pop(pLuaState, 1);

				int stack_items = lua_gettop(pLuaState);
				assert(stack_items == 0);

				delete[] pFileContents;
				lua_close(pLuaState);

				return rInfo;

			}

		}

		void * LuaPhysicsLoad(SmartPntr<GameObject>gObj, const char *i_pfilename)
		{
			//create a lua_State
			lua_State * pLuaState = luaL_newstate();
			assert(pLuaState);

			luaL_openlibs(pLuaState);
			// Read in a file
			size_t			sizeFile = 0;
			void *		pFileContents = LoadLuaFile(i_pfilename, sizeFile);
			assert(pFileContents);
			if (pFileContents  && sizeFile)
			{
				int	result = 0;

				// Do the initial buffer parsing
				result = luaL_loadbuffer(pLuaState, reinterpret_cast<char *>(pFileContents), sizeFile, nullptr);
				assert(result == 0);
				lua_pcall(pLuaState, 0, 0, 0);
				assert(result == 0);

				int			type = LUA_TNIL;

				// Find the global variable named "Player" and push it onto stack.
				// If it doesn't find it it pushes a nil value instead

				result = lua_getglobal(pLuaState, "Actor");
				assert(result == LUA_TTABLE);

				// let's process the "physics_settings" sub table
				float p_mass;
				lua_pushstring(pLuaState, "physics_settings");
				type = lua_gettable(pLuaState, -2);
				// It either needs to be a table or not there (no physics applied)
				assert((type == LUA_TNIL) || (type == LUA_TTABLE));

				if (type == LUA_TTABLE)
				{
					lua_pushstring(pLuaState, "mass");
					type = lua_gettable(pLuaState, -2);
					assert(type == LUA_TNUMBER);

					p_mass = float(lua_tonumber(pLuaState, -1));

					// pop the "mass"  values 
					lua_pop(pLuaState, 1); // n is the number of items you want to pop. 1 because i have only mass
				}
				// we're done with the "physics_settings" table - pop it
				// NOTE: If it didn't exist Lua would have pushed a nil value so still need the pop
				lua_pop(pLuaState, 1);

				physics *pObj = new physics(gObj, p_mass);

				// pop "Player" table
				lua_pop(pLuaState, 1);

				int stack_items = lua_gettop(pLuaState);
				assert(stack_items == 0);

				delete[] pFileContents;
				lua_close(pLuaState);
				return pObj;

			}

		}

	};
}

#endif