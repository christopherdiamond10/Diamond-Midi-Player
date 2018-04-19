#include "glfw.h"
#include "RenderQuad.h"
#include "TextureLoader.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Constructors
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
RenderQuad::RenderQuad()
{
	_Setup(Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), -1);
}

RenderQuad::RenderQuad( const TransformDetails2D& a_rTransform, const Vector4& a_rvMainColour, const Vector4& a_rvGradientColour )
{
	_Setup(a_rTransform.Position, a_rTransform.Size, a_rvMainColour, a_rvGradientColour, -1);
}

RenderQuad::RenderQuad( const Vector2& a_rvPosition, const Vector2& a_rvSize, const Vector4& a_rvMainColour, const Vector4& a_rvGradientColour )
{
	_Setup(a_rvPosition, a_rvSize, a_rvMainColour, a_rvGradientColour, -1);
}

RenderQuad::RenderQuad(	std::string& a_sTextureName, const Vector2& a_rvPosition, const Vector2& a_rvSize, const Vector4& a_rvMainColour, const Vector4& a_rvGradientColour )
{
	_Setup(a_rvPosition, a_rvSize, a_rvMainColour, a_rvGradientColour, TextureLoader::LoadTexture(a_sTextureName));
}

RenderQuad::RenderQuad(	std::string& a_sTextureName, const TransformDetails2D& a_rTransform, const Vector4& a_rvMainColour, const Vector4& a_rvGradientColour )
{
	_Setup(a_rTransform.Position, a_rTransform.Size, a_rvMainColour, a_rvGradientColour, TextureLoader::LoadTexture(a_sTextureName));
}

RenderQuad::RenderQuad(	const char* a_sTextureName, const Vector2& a_rvPosition, const Vector2& a_rvSize, const Vector4& a_rvMainColour, const Vector4& a_rvGradientColour )
{
	_Setup(a_rvPosition, a_rvSize, a_rvMainColour, a_rvGradientColour, TextureLoader::LoadTexture(a_sTextureName));
}

RenderQuad::RenderQuad(	const char* a_sTextureName, const TransformDetails2D& a_rTransform, const Vector4& a_rvMainColour, const Vector4& a_rvGradientColour )
{
	_Setup(a_rTransform.Position, a_rTransform.Size, a_rvMainColour, a_rvGradientColour, TextureLoader::LoadTexture(a_sTextureName));
}

RenderQuad::RenderQuad(	int a_iTextureID, const Vector2& a_rvPosition, const Vector2& a_rvSize, const Vector4& a_rvMainColour, const Vector4& a_rvGradientColour )
{
	_Setup(a_rvPosition, a_rvSize, a_rvMainColour, a_rvGradientColour, a_iTextureID);
}

RenderQuad::RenderQuad(	int a_iTextureID, const TransformDetails2D& a_Transform, const Vector4& a_rvMainColour, const Vector4& a_rvGradientColour )
{
	_Setup(a_Transform.Position, a_Transform.Size, a_rvMainColour, a_rvGradientColour, a_iTextureID);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	*~ Destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
RenderQuad::~RenderQuad()
{
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Draw
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void RenderQuad::Draw()
{
	if( m_iTextureID > 0 )
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_iTextureID);
		_DrawQuad();
		glDisable(GL_TEXTURE_2D);
	}
	else
	{
		_DrawQuad();
	}
}

void RenderQuad::Draw(Vector2& a_vPosition, Vector2& a_vSize)
{
	Vector2 vPosition	= GetPosition();
	Vector2 vSize		= GetSize();

	SetPosition(a_vPosition);
	SetSize(a_vSize);

	Draw();

	SetPosition(vPosition);
	SetSize(vSize);
}

void RenderQuad::Draw(Vector4& uvCoords)
{
	if( m_iTextureID > 0 )
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_iTextureID);
		_DrawQuad(uvCoords);
		glDisable(GL_TEXTURE_2D);
	}
	else
	{
		_DrawQuad(uvCoords);
	}
}

void RenderQuad::Draw(Vector2& a_vPosition, Vector2& a_vSize, Vector4& a_uvCoords)
{
	Vector2 vPosition	= GetPosition();
	Vector2 vSize		= GetSize();

	SetPosition(a_vPosition);
	SetSize(a_vSize);

	Draw(a_uvCoords);

	SetPosition(vPosition);
	SetSize(vSize);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Draw Quad
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void RenderQuad::_DrawQuad(Vector4& a_uvCoords)
{
	glBegin(GL_QUADS);
	{
		// Top Half
		glColor4f( m_vMainColour.x, m_vMainColour.y, m_vMainColour.z, m_vMainColour.w );
		glTexCoord2f(a_uvCoords.x, a_uvCoords.y);
		glVertex2f( m_Rect.x,  m_Rect.y  );
		
		//glColor4f( m_vGradientColour.x, m_vGradientColour.y, m_vGradientColour.z, m_vGradientColour.w );
		glTexCoord2f(a_uvCoords.z, a_uvCoords.y);
		glVertex2f( m_Rect.xw, m_Rect.y  );
		
		glColor4f( m_vGradientColour.x, m_vGradientColour.y, m_vGradientColour.z, m_vGradientColour.w );
		glTexCoord2f(a_uvCoords.z, a_uvCoords.w);
		glVertex2f( m_Rect.xw, m_Rect.yh );
		
		glColor4f( m_vMainColour.x, m_vMainColour.y, m_vMainColour.z, m_vMainColour.w );
		glTexCoord2f(a_uvCoords.x, a_uvCoords.w);
		glVertex2f( m_Rect.x,  m_Rect.yh );
	}
	glEnd();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Transform
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TransformDetails2D RenderQuad::GetTransform()
{
	TransformDetails2D TD;
	TD.Position = GetPosition();
	TD.Size		= GetSize();
	return TD;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Position
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Vector2 RenderQuad::GetPosition()
{
	return Vector2(m_Rect.x, m_Rect.y);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Size
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Vector2 RenderQuad::GetSize()
{
	return Vector2(m_Rect.w, m_Rect.h);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get End Position
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Vector2 RenderQuad::GetEndPosition()
{
	return (GetPosition() + GetSize());
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Main Colour
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Vector4 RenderQuad::GetMainColour()
{
	return m_vMainColour;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Gradient Colour
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Vector4 RenderQuad::GetGradientColour()
{
	return m_vGradientColour;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Texture ID
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int RenderQuad::GetTextureID()
{
	return m_iTextureID;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Transform
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void RenderQuad::SetTransform(const TransformDetails2D& a_Transform)
{
	SetPosition(a_Transform.Position);
	SetSize(a_Transform.Size);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Position
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void RenderQuad::SetPosition(const Vector2& a_vPosition)
{
	m_Rect.x = a_vPosition.x;
	m_Rect.y = a_vPosition.y;

	m_Rect.xw = m_Rect.x + m_Rect.w;
	m_Rect.yh = m_Rect.y + m_Rect.h;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Size
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void RenderQuad::SetSize(const Vector2& a_vSize)
{
	m_Rect.w = a_vSize.x;
	m_Rect.h = a_vSize.y;

	m_Rect.xw = m_Rect.x + m_Rect.w;
	m_Rect.yh = m_Rect.y + m_Rect.h;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Main Colour
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void RenderQuad::SetMainColour(const Vector4& a_vColour)
{
	m_vMainColour	= a_vColour;
	m_bUseGradient	= (m_vGradientColour != m_vMainColour);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Gradient Colour
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void RenderQuad::SetGradientColour(const Vector4& a_vColour)
{
	m_vGradientColour	= a_vColour;
	m_bUseGradient		= (m_vGradientColour != m_vMainColour);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Blend Gradient Colour With Main Colour
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void RenderQuad::BlendGradientColourWithMainColour()
{
	m_vGradientColour	= (m_vMainColour * 0.5f);
	m_bUseGradient		= true;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Set Texture
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void RenderQuad::SetTexture(int a_iTextureID)
{
	m_iTextureID = a_iTextureID;
}

void RenderQuad::SetTexture(std::string& a_sTextureName)
{
	m_iTextureID = TextureLoader::LoadTexture(a_sTextureName);
}

void RenderQuad::SetTexture(const char* a_sTextureName)
{
	m_iTextureID = TextureLoader::LoadTexture(a_sTextureName);
}

void RenderQuad::SetTexture(std::string& a_sTexturePath, std::string& a_sTextureName)
{
	m_iTextureID = TextureLoader::LoadTexture(a_sTexturePath, a_sTextureName);
}

void RenderQuad::SetTexture(const char* a_sTexturePath, const char* a_sTextureName)
{
	m_iTextureID = TextureLoader::LoadTexture(a_sTexturePath, a_sTextureName);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Setup
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void RenderQuad::_Setup(const Vector2& a_rvPosition, const Vector2& a_rvSize, const Vector4& a_rvMainColour, const Vector4& a_rvGradientColour, int a_iTextureID)
{
	SetPosition(a_rvPosition);
	SetSize(a_rvSize);
	SetMainColour(a_rvMainColour);
	SetGradientColour(a_rvGradientColour);
	SetTexture(a_iTextureID);
}