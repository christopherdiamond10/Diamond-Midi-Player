#include "glfw.h"
#include <iostream>
#include "Screen.h"


unsigned int		Screen::m_uiWidth				= 0;
unsigned int		Screen::m_uiHeight				= 0;
const unsigned int	Screen::m_uiOriginalWidth		= 1280;
const unsigned int	Screen::m_uiOriginalHeight		= 720;
bool				Screen::m_bScreenSizeChanged	= false;
bool				Screen::m_bInitialisedWindow	= false;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Screen::Screen()
{
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	*~ Deconstructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Screen::~Screen()
{
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Create OpenGL Window
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Screen::CreateOpenGLWindow(unsigned int a_ScreenWidth, unsigned int a_ScreenHeight)
{
	glfwInit();
 
	bool bWindowOpened = glfwOpenWindow( a_ScreenWidth, a_ScreenHeight, 
																8,	// Red Bits
																8,	// Green Bits
																8,	// Blue Bits
																8,	// Alpha Bits
																0,	// Depth Bits
																0,	// Stencil Bits
																GLFW_WINDOW ) == 1;
	// Open The Window
	if(!bWindowOpened)
	{
		std::cout << "Failed to Open Window!\n\n";
		system( "PAUSE" );
		m_bInitialisedWindow = false;
		return;
    }
 
	glfwSetWindowTitle("Diamond MIDI Player");
 
	// Setup 2D Projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (double)a_ScreenWidth, (double)a_ScreenHeight, 0.0, 0.0, 1.0);

	// Disable Depth Testing, not really necessary
	//glDisable(GL_DEPTH_TEST);
 
	// Enable Blend (Alpha/Opacity)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Setup Instance Variables
	m_bScreenSizeChanged	= true;
	m_bInitialisedWindow	= true;
	m_uiWidth				= a_ScreenWidth;
	m_uiHeight				= a_ScreenHeight;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Update
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Screen::Update()
{
	if(ModifiedScreenSize())
	{
		m_bScreenSizeChanged = false;
	}

	else
	{
		int Width, Height;
		glfwGetWindowSize(&Width, &Height);
		if( Width != m_uiWidth || Height != m_uiHeight )
		{
			m_uiWidth  = (unsigned int)Width;
			m_uiHeight = (unsigned int)Height;
			m_bScreenSizeChanged = true;
			
			// Setup 2D Projection
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0.0, (double)m_uiWidth, (double)m_uiHeight, 0.0, 0.0, 1.0);
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Screen Width
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
unsigned int Screen::GetWidth()
{
	return m_uiWidth;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Screen Height
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
unsigned int Screen::GetHeight()
{
	return m_uiHeight;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Screen Size Changed This Frame?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Screen::ModifiedScreenSize()
{
	return m_bScreenSizeChanged;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: GL Window Opened?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Screen::HasOpenedWindow()
{
	return m_bInitialisedWindow;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Is GL Window Still Opened?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Screen::IsWindowStillOpened()
{
	return glfwGetWindowParam( GLFW_OPENED ) == GL_TRUE;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Keyboard Draw Area
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TransformDetails2D Screen::GetKeyboardDrawArea()
{
	TransformDetails2D Rect;
	Rect.startx	  = 0.0f;
	Rect.starty	  = (float)GetHeight() * 0.8f;
	Rect.endx	  = (float)GetWidth();
	Rect.endy	  = (float)GetHeight();
	return Rect;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Notes Draw Area
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TransformDetails2D Screen::GetNotesDrawArea()
{
	TransformDetails2D Rect;
	Rect.startx	  = 0.0f;
	Rect.starty	  = 0.0f;
	Rect.endx	  = (float)GetWidth();
	Rect.endy	  = GetKeyboardDrawArea().y;
	return Rect;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Scale Float To Screen Width 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
float Screen::ScaleToScreenWidth(float a)
{
	return (a * ScreenScaleWidth());
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Scale Int to Screen Width
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int Screen::ScaleToScreenWidth(int a)
{
	return (int)(a * ScreenScaleWidth());
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Scale Unsigned Int To Screen Width
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
unsigned int Screen::ScaleToScreenWidth(unsigned int a)
{
	return (unsigned int)(a * ScreenScaleWidth());
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Scale Float To Screen Height
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
float Screen::ScaleToScreenHeight(float a)
{
	return (a * ScreenScaleHeight());
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Scale Int To Screen Height
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int Screen::ScaleToScreenHeight(int a)
{
	return (int)(a * ScreenScaleHeight());
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Scale Unsigned Int To Screen Height
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
unsigned int Screen::ScaleToScreenHeight(unsigned int a)
{
	return (unsigned int)(a * ScreenScaleHeight());
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Scale Vector2 To Screen Size
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Vector2 Screen::ScaleToScreenSize(const Vector2& a)
{
	return Vector2( ScaleToScreenWidth(a.x), ScaleToScreenHeight(a.y) );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Scale Vector4 To Screen Size
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Vector4 Screen::ScaleToScreenSize(const Vector4& a)
{
	return Vector4( ScaleToScreenWidth(a.x), ScaleToScreenHeight(a.y), a.z, a.w );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Screen Width
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Screen::SetWidth(unsigned int a_Width)
{
	glfwSetWindowSize(a_Width, m_uiHeight);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Screen Height
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Screen::SetHeight(unsigned int a_Height)
{
	glfwSetWindowSize(m_uiWidth, a_Height);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Screen Scale Width
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
float Screen::ScreenScaleWidth()
{
	return (float)m_uiWidth / m_uiOriginalWidth;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Screen Scale Height
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
float Screen::ScreenScaleHeight()
{
	return (float)m_uiHeight / m_uiOriginalHeight;
}