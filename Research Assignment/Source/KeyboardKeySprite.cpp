#include "KeyboardKeySprite.h"
#include "RenderQuad.h"
#include "InputHandler.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	*: Initialising Variables
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const Vector4 KeyboardKeySprite::sm_vUnpressedKeyColour	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
const Vector4 KeyboardKeySprite::sm_vPressedKeyColour	= Vector4(0.0f, 0.0f, 0.0f, 1.0f);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Constructors
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
KeyboardKeySprite::KeyboardKeySprite(unsigned int a_uiKeyID, const char* a_sTextureName, const TransformDetails2D& a_rTransform)
{
	m_bKeyPressed	= false;
	m_uiKeyID		= a_uiKeyID;
	m_pRenderQuad	= new RenderQuad( a_sTextureName, a_rTransform.Position, a_rTransform.Size );
}

KeyboardKeySprite::KeyboardKeySprite(unsigned int a_uiKeyID, const char* a_sTextureName, const Vector2& a_rvPosition, const Vector2& a_rvSize)
{
	m_bKeyPressed	= false;
	m_uiKeyID		= a_uiKeyID;
	m_pRenderQuad	= new RenderQuad( a_sTextureName, a_rvPosition, a_rvSize );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	*~ Deconstructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
KeyboardKeySprite::~KeyboardKeySprite()
{
	delete m_pRenderQuad;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Update
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void KeyboardKeySprite::Update()
{
	// Change Colour if Pressed Bool has Changed
	if( m_bKeyPressed != InputHandler::GetInstance()->MidiKeyboardKeyPressed(m_uiKeyID) )
	{
		m_bKeyPressed = !m_bKeyPressed;

		m_pRenderQuad->SetMainColour( (m_bKeyPressed ? sm_vPressedKeyColour : sm_vUnpressedKeyColour) );
	}


	// Mouse Hover
	/*else
	{
		if(AIE::EqualWithinTolerance(InputHandler::GetInstance()->GetMousePositionAsVector2(), (this->GetPosition() + (this->GetSize() * 0.5f)), 20.0f))
		{
			m_pRenderQuad->SetMainColour( sm_vPressedKeyColour);
		}
		else
		{
			m_pRenderQuad->SetMainColour( sm_vUnpressedKeyColour);
		}
	}*/
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Draw
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void KeyboardKeySprite::Draw()
{
	m_pRenderQuad->Draw();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Transform
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TransformDetails2D KeyboardKeySprite::GetTransform()
{
	return m_pRenderQuad->GetTransform();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Position
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Vector2 KeyboardKeySprite::GetPosition()
{
	return m_pRenderQuad->GetPosition();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Size
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Vector2 KeyboardKeySprite::GetSize()
{
	return m_pRenderQuad->GetSize();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Colour
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Vector4 KeyboardKeySprite::GetColour()
{
	return m_pRenderQuad->GetMainColour();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Transform
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void KeyboardKeySprite::SetTransform(const TransformDetails2D& a_rTransform)
{
	m_pRenderQuad->SetTransform(a_rTransform);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Position
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void KeyboardKeySprite::SetPosition(const Vector2& a_vPosition)
{
	m_pRenderQuad->SetPosition( a_vPosition );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Size
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void KeyboardKeySprite::SetSize(const Vector2& a_vSize)
{
	m_pRenderQuad->SetSize( a_vSize );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Colour
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void KeyboardKeySprite::SetColour(const Vector4& a_vColour)
{
	m_pRenderQuad->SetMainColour( a_vColour );
}