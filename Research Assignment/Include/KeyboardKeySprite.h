//#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//#					Keyboard Key Sprite
//#					Author: Christopher Diamond
//#					Date: October 20, 2013
//#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#	  Description:
//#		
//#		This class maintain individual key sprites for the Keyboard at the bottom
//#		edge of the screen. Including colour changes.
//#
//#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#ifndef _KEYBOARDKEYSPRITE_H_
#define _KEYBOARDKEYSPRITE_H_



#include "MathHelper.h"

class RenderQuad;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Class: KeyboardKeySprite
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class KeyboardKeySprite
{
////////////////////////////////////////////////////////////
public:
////////////////////////////////////////////////////////////

	//===============================================
	//			Constructors & Destructor
	//===============================================
	KeyboardKeySprite(unsigned int KeyID, const char* TextureName, const TransformDetails2D& Transform);
	KeyboardKeySprite(unsigned int KeyID, const char* TextureName, const Vector2& Position, const Vector2& Size);
	~KeyboardKeySprite();
	//===============================================
	//			Public Functions
	//===============================================
	void Update();
	void Draw();
	//===============================================
	//			Getter Functions
	//===============================================
	TransformDetails2D	GetTransform();
	Vector2				GetPosition();
	Vector2				GetSize();
	Vector4				GetColour();
	//===============================================
	//			Setter Functions
	//===============================================
	void SetTransform(const TransformDetails2D& Transform);
	void SetPosition(const Vector2& Position);
	void SetSize(const Vector2& Size);
	void SetColour(const Vector4& Colour);



////////////////////////////////////////////////////////////
private:
////////////////////////////////////////////////////////////
	
	//===============================================
	//			Private Declarations
	//===============================================
	static const Vector4 sm_vUnpressedKeyColour;
	static const Vector4 sm_vPressedKeyColour;
	//===============================================
	//			Private Instance Variables
	//===============================================
	RenderQuad*		m_pRenderQuad;
	unsigned int	m_uiKeyID;
	bool			m_bKeyPressed;
};


#endif