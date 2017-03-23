#pragma once
#ifndef _RENDERING_H_
#define _RENDERING_H_
#include"RenderInfo.h"

namespace Engine
{
	namespace Rendering
	{
		std::vector<RenderInfo *> *RenderedObjects;
	
		void Init()
		{
			RenderedObjects = new std::vector<RenderInfo*>();
		}

		void RegisterRenderInfo(RenderInfo *rinfo)
		{
			RenderedObjects->push_back(rinfo);
		}

		void DrawRenderedObjects()
		{
			GLib::BeginRendering();
			GLib::Sprites::BeginRendering();
			for (std::vector<RenderInfo*>::iterator it = RenderedObjects->begin(); it != RenderedObjects->end(); it++)
			{
				(*it)->Draw();
				
			}
			GLib::Sprites::EndRendering();
			GLib::EndRendering();
		}

		void ShutDown()
		{
		
			delete RenderedObjects;
		}
	}
}


#endif
