#ifndef _UIBUTTON_H_
#define _UIBUTTON_H_


#include "MathHelper.h"
#include "RenderQuad.h"
#include <string>

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Class: Scene_Select
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class UIButton
{
////////////////////////////////////////////////////////////
public:
////////////////////////////////////////////////////////////

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// **~ Constructors & Destructor
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	UIButton();
	~UIButton();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// *+ Public Functions
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void Update();
	void Draw();
	void DrawReversed();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// *+ Getter Functions
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool				WasClicked();

	TransformDetails2D	GetButtonTransform();
	Vector2				GetButtonPosition();
	Vector2				GetButtonSize();

	TransformDetails2D	GetTextTransform();
	Vector2				GetTextPosition();
	Vector2				GetTextSize();

	Vector4				GetButtonMainColour();
	Vector4				GetButtonGradientColour();
	Vector4				GetButtonHoverColour();
	Vector4				GetButtonTextColour();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// *+ Setter Functions
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void SetButtonTransform(const TransformDetails2D& Transform);
	void SetButtonPosition(const Vector2& Position);
	void SetButtonSize(const Vector2& Size);

	void ResetButtonTextSize();
	void SetButtonText(const std::string& Text, bool CentreText = true, bool KeepTextWithinButtonBoundaries = true);
	void SetButtonTextTransform(const TransformDetails2D& Transform);
	void SetButtonTextPosition(const Vector2& Position);
	void SetButtonTextSize(const Vector2& Size, bool CentreText = true, bool KeepTextWithinButtonBoundaries = true);

	void SetButtonMainColour(const Vector4& Colour);
	void SetButtonGradientClour(const Vector4& Colour);
	void SetButtonHoverColour(const Vector4& Colour);
	void SetButtonTextColour(const Vector4& Colour);

	void SetButtonTexture(int TextureID);
	void SetButtonTexture(std::string& TextureName);
	void SetButtonTexture(const char* TextureName);
	void SetButtonTexture(std::string& TexturePath, std::string& TextureName);
	void SetButtonTexture(const char* TexturePath, const char* TextureName);

////////////////////////////////////////////////////////////
private:
////////////////////////////////////////////////////////////
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// *- Private Instance Variables
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool				m_bHovered;

	std::string			m_sButtonText;
	TransformDetails2D	m_tTextTransform;

	Vector4				m_vTextColour;
	Vector4				m_vMainColour;
	Vector4				m_vButtonGradient;
	Vector4				m_vHoverColour;

	RenderQuad*			m_pRenderableQuad;
};

#endif