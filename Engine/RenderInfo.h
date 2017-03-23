#pragma once

#ifndef _DRAW_H_
#define _DRAW_H

#include "../GLib/BasicTypes.h"
//#include "Vector3.h"
#include "../GLib/GLib.h"
#include <assert.h>
#include "SmartPntr.h"
#include "Source.h"

namespace Engine
{
	namespace Rendering
	{
		typedef GLib::Sprites::Sprite DSprite;
		
		class RenderInfo
		{
			SmartPntr<GameObject> gObj;
			DSprite *gSprite;
			
		public:
			RenderInfo(SmartPntr<GameObject>, const char*);
		//	static void DrawImage(RenderInfo *, RenderInfo *);// place holder
			void Draw();

			inline DSprite * getSprite()
			{
				return gSprite;
			}

			inline void setSprite(DSprite * i_pSprite)
			{
				gSprite = i_pSprite;
			}

			inline SmartPntr<GameObject>  getGameObject()
			{
				return gObj;
			}
			

			static void * LoadFile(const char * i_pFilename, size_t & o_sizeFile);
			static	DSprite * CreateSprite(const char * i_pFilename);
			
			~RenderInfo()
			{
				
				//GLib::Sprites::Release(gSprite);
				//delete gSprite;
				
			}
			
		};

		

	}
}
//	}
//}

#endif