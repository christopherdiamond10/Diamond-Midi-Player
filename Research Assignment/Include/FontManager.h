//\====================================================================================
//\ Author: Christopher.Allport
//\ About : Font.h - 
//\ 
//\ Defines the Font Singleton Class. This class is used to show font throughout the 
//\ game, this class is used for the menu, instruction, textboxes and highscores.
//\====================================================================================
#ifndef _FONTMANAGER_H_
#define _FONTMANAGER_H_
	
#include <map>
#include "MathHelper.h"


class FontManager
{
//////////////////////////////////////////
public:
//////////////////////////////////////////

	enum FontStyle
	{
		NORMAL,
		DANCING,
	};
	//===============================================
	//			Constructor & Destructor
	//===============================================
	FontManager();
	~FontManager();
	//===============================================
	//			Public Functions
	//===============================================
	static FontManager* GetInstance();

	float GetTotalDrawLength(std::string& Text, const Vector2& FontSize);
	float GetTotalDrawLength(const char* Text, const Vector2& FontSize);
	Vector2 GetTotalDrawLengthAsVector2(std::string& Text, const Vector2& FontSize);
	Vector2 GetTotalDrawLengthAsVector2(const char* Text, const Vector2& FontSize);

	// Print Font. Standalone
	template< class T >
	void Print( T Value,			const Vector2& Position, const Vector2& Size = Vector2(20.0f, 20.0f), const Vector4& Colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f), FontStyle eStyle = NORMAL );
	void Print( std::string& Value,	const Vector2& Position, const Vector2& Size = Vector2(20.0f, 20.0f), const Vector4& Colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f), FontStyle eStyle = NORMAL );
	void Print( const char* Value,	const Vector2& Position, const Vector2& Size = Vector2(20.0f, 20.0f), const Vector4& Colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f), FontStyle eStyle = NORMAL );


//////////////////////////////////////////	
private:
//////////////////////////////////////////

	//===============================================
	//			Private Declarations
	//===============================================
	struct FontSheetCharacter
	{
		union
		{
			float f[4];
			struct{ float sx, sy, ex, ey; };
			struct{ Vector4 UVs; };
		};
	};
	//===============================================
	//		   Private Instance Variables
	//===============================================
	static FontManager* m_Instance;

	int m_iTextureID;
	float m_fSpacing;
	std::map<char, FontSheetCharacter> m_hmCharCoords;
	//===============================================
	//			Private Functions
	//===============================================
	void			_InitialiseInstanceVariables	();
	const Vector4   _GetUVCoords					( char cLetterNum );
	bool			_DoesCharacterNeedToBeAdjusted	( char c );
	void			_DrawFont						(std::string& Text, const Vector2& Position, const Vector2& Size, const Vector4& Colour);
	void			_DrawFontDancing				(std::string& Text, const Vector2& Position, const Vector2& Size, const Vector4& Colour);
	void			_DrawCharacter					( const Vector2& Position, const Vector2& Size, const Vector4& UVCoords );

};


#endif