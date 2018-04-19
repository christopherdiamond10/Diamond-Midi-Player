#include "UIButton.h"

#include "FontManager.h"
#include "InputHandler.h"
#include "Screen.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
UIButton::UIButton()
{
	m_pRenderableQuad = new RenderQuad();

	m_vTextColour		= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vMainColour		= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vHoverColour		= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vButtonGradient	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	m_tTextTransform.Position	= Screen::ScaleToScreenSize(Vector2(0.0f, 0.0f));
	ResetButtonTextSize();

	m_bHovered = false;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	*~ Deconstructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
UIButton::~UIButton()
{
	delete m_pRenderableQuad;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Update
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UIButton::Update()
{
	Vector2 vMousePosition				= InputHandler::GetInstance()->GetMousePositionAsVector2();
	TransformDetails2D tSelfTransform	= m_pRenderableQuad->GetTransform();
	
	bool bHovered = (vMousePosition.x > tSelfTransform.x) && (vMousePosition.x < tSelfTransform.x + tSelfTransform.endx) &&
					(vMousePosition.y > tSelfTransform.y) && (vMousePosition.y < tSelfTransform.y + tSelfTransform.endy);

	if( m_bHovered != bHovered )
	{
		m_bHovered = !m_bHovered;

		m_pRenderableQuad->SetGradientColour( m_bHovered ? m_vHoverColour : m_vButtonGradient );
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Draw
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UIButton::Draw()
{
	m_pRenderableQuad->Draw();

	if(m_sButtonText != "")
		FontManager::GetInstance()->Print(m_sButtonText, m_tTextTransform.Position, m_tTextTransform.Size, m_vTextColour);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Draw Reversed Button Texture
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UIButton::DrawReversed()
{
	m_pRenderableQuad->Draw(Vector4(1.0f, 1.0f, 0.0f, 0.0f));

	if(m_sButtonText != "")
		FontManager::GetInstance()->Print(m_sButtonText, m_tTextTransform.Position, m_tTextTransform.Size, m_vTextColour);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Was Clicked?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool UIButton::WasClicked()
{
	return m_bHovered && InputHandler::GetInstance()->MouseButtonTriggered(GLFW_MOUSE_BUTTON_1);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: get Button Transform
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TransformDetails2D UIButton::GetButtonTransform()
{
	return m_pRenderableQuad->GetTransform();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Button Position
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Vector2 UIButton::GetButtonPosition()
{
	return m_pRenderableQuad->GetPosition();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Button Size
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Vector2 UIButton::GetButtonSize()
{
	return m_pRenderableQuad->GetSize();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Text Transform
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TransformDetails2D UIButton::GetTextTransform()
{
	return m_tTextTransform;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Text Position
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Vector2 UIButton::GetTextPosition()
{
	return m_tTextTransform.Position;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Text Size
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Vector2 UIButton::GetTextSize()
{
	return m_tTextTransform.Size;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Button Main Colour
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Vector4 UIButton::GetButtonMainColour()
{
	return m_vMainColour;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Button Gradient Colour
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Vector4 UIButton::GetButtonGradientColour()
{
	return m_vButtonGradient;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Button Hover Colour
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Vector4 UIButton::GetButtonHoverColour()
{
	return m_vHoverColour;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Button Text Colour
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Vector4 UIButton::GetButtonTextColour()
{
	return m_vTextColour;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Button Position
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UIButton::SetButtonTransform(const TransformDetails2D& a_rTransform)
{
	m_pRenderableQuad->SetTransform(a_rTransform);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Button Position
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UIButton::SetButtonPosition(const Vector2& a_rvPosition)
{
	m_pRenderableQuad->SetPosition(a_rvPosition);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Button Size
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UIButton::SetButtonSize(const Vector2& a_rvSize)
{
	m_pRenderableQuad->SetSize(a_rvSize);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Reset Button Text Size
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UIButton::ResetButtonTextSize()
{
	m_tTextTransform.Size = Screen::ScaleToScreenSize(Vector2(20.0f, 20.0f));
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Button Text
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UIButton::SetButtonText(const std::string& a_sText, bool a_bCentreText, bool a_bKeepTextWithinButtonBoundaries)
{
	m_sButtonText = a_sText;

	if(a_bKeepTextWithinButtonBoundaries)
	{
		if( FontManager::GetInstance()->GetTotalDrawLength(m_sButtonText, m_tTextTransform.Size) > m_pRenderableQuad->GetSize().x )
		{
			m_tTextTransform.Size *= (m_pRenderableQuad->GetSize().x / FontManager::GetInstance()->GetTotalDrawLength(m_sButtonText, m_tTextTransform.Size));
		}
	}

	if(a_bCentreText)
	{
		Vector2 vCentre = (m_pRenderableQuad->GetPosition() + (m_pRenderableQuad->GetSize() * 0.5f));
		m_tTextTransform.Position.x = (vCentre.x - (FontManager::GetInstance()->GetTotalDrawLength(m_sButtonText, m_tTextTransform.Size) * 0.5f));
		m_tTextTransform.Position.y = vCentre.y - (m_tTextTransform.height * 0.5f);
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Button Text Transform
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UIButton::SetButtonTextTransform(const TransformDetails2D& a_rTransform)
{
	m_tTextTransform = a_rTransform;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Button Text Position
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UIButton::SetButtonTextPosition(const Vector2& a_rvPosition)
{
	m_tTextTransform.Position = a_rvPosition;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Button Text Size
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UIButton::SetButtonTextSize(const Vector2& a_rvSize, bool a_bCentreText, bool a_bKeepTextWithinButtonBoundaries)
{
	m_tTextTransform.Size = a_rvSize;
	SetButtonText(m_sButtonText, a_bCentreText, a_bKeepTextWithinButtonBoundaries);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Button Main Colour
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UIButton::SetButtonMainColour(const Vector4& a_rvColour)
{
	m_vMainColour = a_rvColour;
	m_pRenderableQuad->SetMainColour(m_vMainColour);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Button Gradient Colour
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UIButton::SetButtonGradientClour(const Vector4& a_rvColour)
{
	m_vButtonGradient = a_rvColour;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Button Hover Colour
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UIButton::SetButtonHoverColour(const Vector4& a_rvColour)
{
	m_vHoverColour = a_rvColour;

	if( m_bHovered )
		m_pRenderableQuad->SetGradientColour(m_vHoverColour);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Button Text Colour
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UIButton::SetButtonTextColour(const Vector4& a_rvColour)
{
	m_vTextColour = a_rvColour;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Button Texture
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UIButton::SetButtonTexture(int a_iTextureID)
{
	m_pRenderableQuad->SetTexture(a_iTextureID);
}

void UIButton::SetButtonTexture(std::string& a_sTextureName)
{
	m_pRenderableQuad->SetTexture(a_sTextureName);
}

void UIButton::SetButtonTexture(const char* a_sTextureName)
{
	m_pRenderableQuad->SetTexture(a_sTextureName);
}

void UIButton::SetButtonTexture(std::string& a_sTexturePath, std::string& a_sTextureName)
{
	m_pRenderableQuad->SetTexture(a_sTexturePath, a_sTextureName);
}

void UIButton::SetButtonTexture(const char* a_sTexturePath, const char* a_sTextureName)
{
	m_pRenderableQuad->SetTexture(a_sTexturePath, a_sTextureName);
}