#include "RenderInfo.h"

namespace Engine
{
	namespace Rendering {

		
	

		RenderInfo::RenderInfo(SmartPntr<GameObject> i_pgObj, const char * i_pfilename):gObj(i_pgObj)
		{
			gSprite = CreateSprite(i_pfilename);
		}


		
		void *RenderInfo::LoadFile(const char * i_pFilename, size_t & o_sizeFile)
		 {
			 assert(i_pFilename != NULL);

			 FILE * pFile = NULL;

			 errno_t fopenError = fopen_s(&pFile, i_pFilename, "rb");
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

		 GLib::Sprites::Sprite * RenderInfo::CreateSprite(const char * i_pFilename)
		 {
			 assert(i_pFilename);

			 size_t sizeTextureFile = 0;

			 void * pTextureFile = LoadFile(i_pFilename, sizeTextureFile);
			 //LoadFile(i_pFilename, sizeTextureFile);

			 GLib::Texture * pTexture = pTextureFile ? GLib::CreateTexture(pTextureFile, sizeTextureFile) : nullptr;

			 if (pTextureFile)
				 delete[] pTextureFile;

			 if (pTexture == nullptr)
				 return NULL;

			 unsigned int width = 0;
			 unsigned int height = 0;
			 unsigned int depth = 0;

			 bool result = GLib::GetDimensions(pTexture, width, height, depth);
			 assert(result == true);
			 assert((width > 0) && (height > 0));

			 GLib::Sprites::SpriteEdges	Edges = { -float(width / 2.0f), float(height), float(width / 2.0f), 0.0f };
			 GLib::Sprites::SpriteUVs	UVs = { { 0.0f, 0.0f },{ 1.0f, 0.0f },{ 0.0f, 1.0f },{ 1.0f, 1.0f } };
			 GLib::RGBA							Color = { 255, 255, 255, 255 };
			 GLib::Sprites::Sprite* sprite = GLib::Sprites::CreateSprite(Edges, 0.1f, Color, UVs);
			 if (sprite == nullptr)
			 {
				 Release(pTexture);
				 return nullptr;
			 }

			 SetTexture(*sprite, *pTexture);

			 return sprite;
		 }

		
		 void RenderInfo::Draw()
		 {
			 GLib::Sprites::RenderSprite(*gSprite, gObj->getPosition(), gObj->getRotation());
		 }


	/*	 void RenderInfo::DrawImage(RenderInfo * entity1, RenderInfo * entity2)
		 {
			 GLib::BeginRendering();
			 GLib::Sprites::BeginRendering();

			 if (entity1->getSprite())
			 {
				 GLib::Sprites::RenderSprite(*(entity1->getSprite()), entity1->getGameObject()->getPosition());
			 }
			 if (entity2->getSprite())
			 {
				 GLib::Sprites::RenderSprite(*(entity2->getSprite()), entity2->getGameObject()->getPosition());
			 }
			 GLib::Sprites::EndRendering();
			 GLib::EndRendering();
		 }*/

		}
}