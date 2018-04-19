//#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//#					Screen
//#					Author: Christopher Diamond
//#					Date: October 28, 2013
//#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#	  Description:
//#		
//#		This class holds the Screen Dimensions and Handles the Resizing of 
//#		Objects	Based on that Screen Size.
//#
//#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "MathHelper.h"



//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Class: Screen
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class Screen
{
////////////////////////////////////////////////////////////
public:
////////////////////////////////////////////////////////////

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// *+ Public Functions
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static void CreateOpenGLWindow(unsigned int Width, unsigned int Height);
	static void Update();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// *+ Getter Functions
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static unsigned int GetWidth();
	static unsigned int GetHeight();
	static bool			ModifiedScreenSize();
	static bool			HasOpenedWindow();
	static bool			IsWindowStillOpened();

	static TransformDetails2D	GetKeyboardDrawArea();
	static TransformDetails2D	GetNotesDrawArea();

	static float		ScaleToScreenWidth(float a);
	static int			ScaleToScreenWidth(int a);
	static unsigned int	ScaleToScreenWidth(unsigned int a);
	static float		ScaleToScreenHeight(float a);
	static int			ScaleToScreenHeight(int a);
	static unsigned int	ScaleToScreenHeight(unsigned int a);
	static Vector2		ScaleToScreenSize(const Vector2& a);
	static Vector4		ScaleToScreenSize(const Vector4& a);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// *+ Setter Functions
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static void SetWidth(unsigned int Width);
	static void SetHeight(unsigned int Height);



////////////////////////////////////////////////////////////
private:
////////////////////////////////////////////////////////////
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// **~ Constructor & Deconstructor
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Screen();
	~Screen();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// *- Private Instance Variables
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static unsigned int			m_uiWidth;
	static unsigned int			m_uiHeight;

	static const unsigned int	m_uiOriginalWidth;
	static const unsigned int	m_uiOriginalHeight;

	static bool					m_bScreenSizeChanged;
	static bool					m_bInitialisedWindow;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// *- Private Functions
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static float ScreenScaleWidth();
	static float ScreenScaleHeight();
};


#endif