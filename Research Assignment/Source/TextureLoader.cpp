#include "glew.h"
#include "FreeImage.h"
#include "TextureLoader.h"
#include <map>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	*: Global Variables
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::map<std::string, unsigned int> g_ImportedImagesHashMap;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Check For Imported Image
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
unsigned int CheckForImportedImage( const char* PathToTexture )
{
	auto elem = g_ImportedImagesHashMap.find(PathToTexture);
	return (elem == g_ImportedImagesHashMap.end()) ? UINT_MAX : (*elem).second;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Add Image To Hash
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddImageToHash(const char* PathToTexture, unsigned int TextureID)
{
	g_ImportedImagesHashMap[ (std::string)PathToTexture ] = TextureID;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Create Texture
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
unsigned int CreateTexture( const char* a_sPathToTexture )
{
	// Check For Already Imported Image
	unsigned int uiImportedTextureID = CheckForImportedImage(a_sPathToTexture);
	if( uiImportedTextureID != UINT_MAX )
	{
		return uiImportedTextureID;
	}



	FIBITMAP* pBitmap = NULL;

	// Check the File Signature, Deduce its Format and Load it
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(a_sPathToTexture, 0);
	if (fif != FIF_UNKNOWN && FreeImage_FIFSupportsReading(fif)) 
	{
		pBitmap = FreeImage_Load(fif, a_sPathToTexture);
	}

	if (pBitmap == NULL) 
	{
		printf("Error: Failed to load image '%s'!\n", a_sPathToTexture);
		return UINT_MAX;
	}

	// Force the Image to RGBA
	unsigned int bpp = FreeImage_GetBPP(pBitmap);
	FREE_IMAGE_COLOR_TYPE fi_colourType = FreeImage_GetColorType(pBitmap);
	if (fi_colourType != FIC_RGBALPHA ) 
	{
		FIBITMAP* ndib = FreeImage_ConvertTo32Bits(pBitmap);
		FreeImage_Unload(pBitmap);
		pBitmap = ndib;
		bpp = FreeImage_GetBPP(pBitmap);
		fi_colourType = FreeImage_GetColorType(pBitmap);
	}

	// Get the Pixel Data
	BYTE* pData = FreeImage_GetBits(pBitmap);

	// Try to Determine Data Type of File (Bytes / Floats)
	FREE_IMAGE_TYPE fit = FreeImage_GetImageType(pBitmap);
	GLenum eType = (fit == FIT_RGBF || fit == FIT_FLOAT) ? GL_FLOAT : GL_UNSIGNED_BYTE;

	// Create GL Texture
	GLuint textureID;
	glGenTextures( 1, &textureID );
	glBindTexture( GL_TEXTURE_2D, textureID );
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(pBitmap), FreeImage_GetHeight(pBitmap), 0, GL_RGBA, eType, pData);

	// Specify Default Filtering and Wrapping
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	// Unbind Texture
	glBindTexture( GL_TEXTURE_2D, 0 );

	// Delete data
	FreeImage_Unload(pBitmap);

	// Add Texture To Imported List
	AddImageToHash( a_sPathToTexture, textureID );

	// Return Image ID
	return textureID;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Load Texture
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
unsigned int TextureLoader::LoadTexture( std::string& CustomPath, std::string& TextureFilename )
{
	return CreateTexture( (CustomPath + TextureFilename).c_str() ); 
}

unsigned int TextureLoader::LoadTexture( const char* CustomPath, const char* TextureFilename )
{
	return CreateTexture( ((std::string)CustomPath + TextureFilename).c_str() );
}

unsigned int TextureLoader::LoadTexture( std::string& TextureFilename )
{
	return CreateTexture( ((std::string)DEFAULT_TEXTURE_PATH + TextureFilename).c_str() );
}

unsigned int TextureLoader::LoadTexture( const char* TextureFilename )
{
	return CreateTexture( ((std::string)DEFAULT_TEXTURE_PATH + TextureFilename).c_str() );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Destroy All Image Memory
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TextureLoader::DestroyAllImageMemory()
{
	for( auto Iter = g_ImportedImagesHashMap.begin(); Iter != g_ImportedImagesHashMap.end(); ++Iter )
	{
		glDeleteTextures( 1, &(*Iter).second );
	}
	g_ImportedImagesHashMap.clear();
}