#include "glfw.h"
#include "FontManager.h"

#include "DeltaTime.h"
#include "TextureLoader.h"
#include <sstream>
#include <assert.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Instance
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FontManager* FontManager::m_Instance = NULL;
FontManager* FontManager::GetInstance()
{
	return m_Instance;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FontManager::FontManager()
	: m_hmCharCoords()
{
	if( m_Instance == NULL )
	{ 
		m_Instance = this;
		_InitialiseInstanceVariables();
	}
	else
	{ 
		assert(0);
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	*~ Deconstructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FontManager::~FontManager()
{
	m_Instance = NULL;

	m_hmCharCoords.clear();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Initialise Instance Variables
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void FontManager::_InitialiseInstanceVariables()
{
	const int iVertzSize = 7;
	const int iHorzSize = 15;
	char cPushTempArray[iVertzSize][iHorzSize] =
	{
		{ '?', '?', '?', '?', '?', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', },
		{ ';', '"', 39,  '<', '>', ',', '.', '?', '?', '?', '?', '?', '?', '?', '?', }, // 39 == ' (Quotation Mark)
		{ '&', '*', '(', ')', '-', '_', '+', '=', '{', '}', 91,  93,  '|', 92,  ':', }, // 91 == '[', 92 == '\', 93 == ']'
		{ 't', 'u', 'v', 'w', 'x', 'y', 'z', '`', '~', '!', '@', '#', '$', '%', '^', },
		{ 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', },
		{ 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', },
		{ 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', },
	};
	
	// Push the Above 2D Array into the list
	for( int Y = 0; Y < iVertzSize; Y++ )
	{
		for( int X = 0; X < iHorzSize; X++ )
		{
			FontSheetCharacter FSCI = FontSheetCharacter();

			// Set the UV Coordinates
			FSCI.sx = ((float)X		/	 (float)iHorzSize			);
			FSCI.ex = ((FSCI.sx)	+	 (1.f /  (float)iHorzSize  ));
			FSCI.sy = ((float)Y		/	 (float)iVertzSize		    );
			FSCI.ey = ((FSCI.sy)	+	 (1.f /  (float)iVertzSize ));

			// Add it to the List
			m_hmCharCoords.insert( m_hmCharCoords.begin(), std::pair<char, FontSheetCharacter>(cPushTempArray[Y][X], FSCI) );
		}
	}

	m_iTextureID	= TextureLoader::LoadTexture("FontSheet.png");
	m_fSpacing		= 0.8f;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Get Total Font Length
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
float FontManager::GetTotalDrawLength(std::string& Text, const Vector2& FontSize)
{
	return ((float)Text.length() * FontSize.x * m_fSpacing);
}

float FontManager::GetTotalDrawLength(const char* Text, const Vector2& FontSize)
{
	return GetTotalDrawLength((std::string)Text, FontSize);
}

Vector2 FontManager::GetTotalDrawLengthAsVector2(std::string& Text, const Vector2& FontSize)
{
	return Vector2( GetTotalDrawLength(Text, FontSize), 0.0f );
}

Vector2 FontManager::GetTotalDrawLengthAsVector2(const char* Text, const Vector2& FontSize)
{
	return GetTotalDrawLengthAsVector2((std::string)Text, FontSize);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Print Font
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template< class T >
void FontManager::Print( T AnyValue, const Vector2& a_rvPosition, const Vector2& a_rvSize, const Vector4& a_rvColour, FontStyle a_eStyle )
{
	// Converting from any non-string value to a string ...
	std::stringstream cString (std::stringstream::in | std::stringstream::out);
	cString << AnyValue;

	switch(a_eStyle)
	{
	case DANCING: 
		{
			_DrawFontDancing(cString.str(), a_rvPosition, a_rvSize, a_rvColour);
			break;
		}

	default:
		{
			_DrawFont(cString.str(), a_rvPosition, a_rvSize, a_rvColour);
			break;
		}
	}
}

void FontManager::Print( std::string& Value, const Vector2& a_rvPosition, const Vector2& a_rvSize, const Vector4& a_rvColour, FontStyle a_eStyle )
{
	switch(a_eStyle)
	{
	case DANCING: 
		{
			_DrawFontDancing(Value, a_rvPosition, a_rvSize, a_rvColour);
			break;
		}

	default:
		{
			_DrawFont(Value, a_rvPosition, a_rvSize, a_rvColour);
			break;
		}
	}
}

void FontManager::Print( const char* Value,	const Vector2& a_rvPosition, const Vector2& a_rvSize, const Vector4& a_rvColour, FontStyle a_eStyle )
{
	switch(a_eStyle)
	{
	case DANCING: 
		{
			_DrawFontDancing((std::string)Value, a_rvPosition, a_rvSize, a_rvColour);
			break;
		}

	default:
		{
			_DrawFont((std::string)Value, a_rvPosition, a_rvSize, a_rvColour);
			break;
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Character UV Coords
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const Vector4 FontManager::_GetUVCoords( char cLetterNum )
{
	auto iter = m_hmCharCoords.find(cLetterNum);
	if( iter != m_hmCharCoords.end() )
	{
		return (*iter).second.UVs;
	}
	return Vector4(0.0f, 0.0f, 1.0f, 1.0f);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Does Character Need To Be Adjusted?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool FontManager::_DoesCharacterNeedToBeAdjusted( char c )
{
	return( c == 'g' || c == 'j' || c == 'p' || c == 'q' || c == 'y' );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Draw Font (Normal)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void FontManager::_DrawFont(std::string& a_sText, const Vector2& a_rvPosition, const Vector2& a_rvSize, const Vector4& a_rvColour)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_iTextureID);
	glColor4f( a_rvColour.x, a_rvColour.y, a_rvColour.z, a_rvColour.w );
	
	Vector2 vDrawPosition = a_rvPosition;

	for( unsigned int i = 0; i < a_sText.length(); ++i)
	{
		char cLetterNum = a_sText[i];

		if( cLetterNum != ' ' )
		{
			if( _DoesCharacterNeedToBeAdjusted( cLetterNum ) )
			{
				float yPos = vDrawPosition.y;
				vDrawPosition.y += (a_rvSize.y * 0.3f);
				_DrawCharacter( vDrawPosition, a_rvSize, _GetUVCoords(cLetterNum) );
				vDrawPosition.y = yPos;
			}

			else
			{
				_DrawCharacter( vDrawPosition, a_rvSize, _GetUVCoords(cLetterNum) );
			}
		}

		vDrawPosition.x += a_rvSize.x * m_fSpacing;
	}

	glDisable(GL_TEXTURE_2D);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Draw Font (Dancing)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void FontManager::_DrawFontDancing(std::string& a_sText, const Vector2& a_rvPosition, const Vector2& a_rvSize, const Vector4& a_rvColour)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_iTextureID);
	glColor4f( a_rvColour.x, a_rvColour.y, a_rvColour.z, a_rvColour.w );
	
	Vector2 vDrawPosition = a_rvPosition;

	for( unsigned int i = 0; i < a_sText.length(); ++i)
	{
		char cLetterNum = a_sText[i];

		if( cLetterNum != ' ' )
		{
			float fDanceAdjustx = cos(DeltaTime::GetInstance()->GetTotalTime() * (i % 2 == 0 ? -4.0f : 4.0f)) * 20.0f;
			float fDanceAdjusty = sin(DeltaTime::GetInstance()->GetTotalTime() * (i % 2 == 0 ? -4.0f : 4.0f)) * 5.0f;
			vDrawPosition.x += fDanceAdjustx;
			vDrawPosition.y += fDanceAdjusty;
			if( _DoesCharacterNeedToBeAdjusted( cLetterNum ) )
			{
				float yPos = vDrawPosition.y;
				vDrawPosition.y += (a_rvSize.y * 0.3f);
				_DrawCharacter( vDrawPosition, a_rvSize, _GetUVCoords(cLetterNum) );
				vDrawPosition.y = yPos;
			}

			else
			{
				_DrawCharacter( vDrawPosition, a_rvSize, _GetUVCoords(cLetterNum) );
			}
			vDrawPosition.x -= fDanceAdjustx;
			vDrawPosition.y -= fDanceAdjusty;
		}

		vDrawPosition.x += a_rvSize.x * m_fSpacing;
	}

	glDisable(GL_TEXTURE_2D);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Draw Character
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void FontManager::_DrawCharacter( const Vector2& a_rvPosition, const Vector2& a_rvSize, const Vector4& a_rvUVCoords )
{
	float xw = a_rvPosition.x + a_rvSize.x;
	float yh = a_rvPosition.y + a_rvSize.y;

	glBegin(GL_QUADS);
	{
		glTexCoord2f(a_rvUVCoords.x, a_rvUVCoords.y);
		glVertex2f( a_rvPosition.x,  yh );

		glTexCoord2f(a_rvUVCoords.z, a_rvUVCoords.y);
		glVertex2f( xw, yh );

		glTexCoord2f(a_rvUVCoords.z, a_rvUVCoords.w);
		glVertex2f( xw, a_rvPosition.y  );

		glTexCoord2f(a_rvUVCoords.x, a_rvUVCoords.w);
		glVertex2f( a_rvPosition.x,  a_rvPosition.y  );
	}
	glEnd();
}