//#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//#					Texture Loader
//#					Author: Christopher Diamond
//#					Date: October 20, 2013
//#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#	  Description:
//#		
//#		This Namespace Loads in Texture files and Manages their IDs.
//#
//#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#ifndef _TEXTURELOADER_H_
#define _TEXTURELOADER_H_

#include <string>

#define DEFAULT_TEXTURE_PATH	"./Textures/"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Namespace: TextureLoader
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
namespace TextureLoader
{
	unsigned int LoadTexture( std::string& CustomPath, std::string& TextureFilename );
	unsigned int LoadTexture( const char*  CustomPath, const char*  TextureFilename );
	unsigned int LoadTexture( std::string& TextureFilename );
	unsigned int LoadTexture( const char*  TextureFilename );

	void DestroyAllImageMemory();
}


#endif