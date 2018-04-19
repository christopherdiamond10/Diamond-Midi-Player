//#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//#					Render Quad
//#					Author: Christopher Diamond
//#					Date: October 20, 2013
//#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#	  Description:
//#		
//#		This class handles the Draw Calls for any Renderable Quad Object(s)
//#
//#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#ifndef _RENDERQUAD_H_
#define _RENDERQUAD_H_

#include "MathHelper.h"
#include <string>



//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Class: RenderQuad
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class RenderQuad
{
////////////////////////////////////////////////////////////
public:
////////////////////////////////////////////////////////////

	//===============================================
	//			Constructors & Destructor
	//===============================================
	RenderQuad();
	RenderQuad(								const Vector2& Position,				const Vector2& Size, const Vector4& MainColour = Vector4(1.0f, 1.0f, 1.0f, 1.0f), const Vector4& GradientColour = Vector4(1.0f, 1.0f, 1.0f, 1.0f) );
	RenderQuad(								const TransformDetails2D& Transform,						 const Vector4& MainColour = Vector4(1.0f, 1.0f, 1.0f, 1.0f), const Vector4& GradientColour = Vector4(1.0f, 1.0f, 1.0f, 1.0f) );
	RenderQuad(	std::string& TextureName,	const Vector2& Position,				const Vector2& Size, const Vector4& MainColour = Vector4(1.0f, 1.0f, 1.0f, 1.0f), const Vector4& GradientColour = Vector4(1.0f, 1.0f, 1.0f, 1.0f) );
	RenderQuad(	std::string& TextureName,	const TransformDetails2D& Transform,						 const Vector4& MainColour = Vector4(1.0f, 1.0f, 1.0f, 1.0f), const Vector4& GradientColour = Vector4(1.0f, 1.0f, 1.0f, 1.0f) );
	RenderQuad(	const char*  TextureName,	const Vector2& Position,				const Vector2& Size, const Vector4& MainColour = Vector4(1.0f, 1.0f, 1.0f, 1.0f), const Vector4& GradientColour = Vector4(1.0f, 1.0f, 1.0f, 1.0f) );
	RenderQuad(	const char*  TextureName,	const TransformDetails2D& Transform,						 const Vector4& MainColour = Vector4(1.0f, 1.0f, 1.0f, 1.0f), const Vector4& GradientColour = Vector4(1.0f, 1.0f, 1.0f, 1.0f) );
	RenderQuad(	int TextureID,				const Vector2& Position,				const Vector2& Size, const Vector4& MainColour = Vector4(1.0f, 1.0f, 1.0f, 1.0f), const Vector4& GradientColour = Vector4(1.0f, 1.0f, 1.0f, 1.0f) );
	RenderQuad(	int TextureID,				const TransformDetails2D& Transform,						 const Vector4& MainColour = Vector4(1.0f, 1.0f, 1.0f, 1.0f), const Vector4& GradientColour = Vector4(1.0f, 1.0f, 1.0f, 1.0f) );
	~RenderQuad();
	//===============================================
	//			Public Functions
	//===============================================
	void Draw(Vector4& uvCoords);
	void Draw(Vector2& Position, Vector2& Size, Vector4& uvCoords);
	void Draw(Vector2& Position, Vector2& Size);
	void Draw();
	//===============================================
	//			Getter Functions
	//===============================================
	TransformDetails2D	GetTransform();
	Vector2				GetPosition();
	Vector2				GetSize();
	Vector2				GetEndPosition();
	Vector4				GetMainColour();
	Vector4				GetGradientColour();
	int					GetTextureID();
	//===============================================
	//			Setter Functions
	//===============================================
	void SetTransform(const TransformDetails2D& Transform);
	void SetPosition(const Vector2& Position);
	void SetSize(const Vector2& Size);
	void SetMainColour(const Vector4& Colour);
	void SetGradientColour(const Vector4& Colour);
	void BlendGradientColourWithMainColour();

	void SetTexture(int TextureID);
	void SetTexture(std::string& TextureName);
	void SetTexture(const char* TextureName);
	void SetTexture(std::string& TexturePath, std::string& TextureName);
	void SetTexture(const char* TexturePath, const char* TextureName);



////////////////////////////////////////////////////////////
private:
////////////////////////////////////////////////////////////
	
	//===============================================
	//			Private Declarations
	//===============================================
	struct Rect
	{
		float x, y, w, h, xw, yh; // X, Y, Width, Height, X + Width, Y + Height
	};
	//===============================================
	//			Private Instance Variables
	//===============================================
	Rect			m_Rect;
	Vector4			m_vMainColour;
	Vector4			m_vGradientColour;
	int				m_iTextureID;
	bool			m_bUseGradient;
	//===============================================
	//			Private Functions
	//===============================================
	void _Setup(const Vector2& Position, const Vector2& Size, const Vector4& MainColour, const Vector4& GradientColour, int TextureID); 
	void _DrawQuad(Vector4& uvCoords = Vector4(0.0f, 1.0f, 1.0f, 0.0f));
};


#endif