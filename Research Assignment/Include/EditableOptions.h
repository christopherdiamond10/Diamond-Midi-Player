//#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//#					Editable Options
//#					Author: Christopher Diamond
//#					Date: Novemeber 10, 2013
//#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#	  Description:
//#		
//#		This class holds editable options for the Application
//#
//#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#ifndef _EDITABLEOPTIONS_H_
#define _EDITABLEOPTIONS_H_


namespace EditableOptions
{
	// Screen Dimensions
	static const unsigned int	uiScreenWidth()			{	return 1900;	}	// How Wide the Window Is?
	static const unsigned int	uiScreenHeight()		{	return 1000;	}	// How High the Window Is?
	
	// Game Options
	static const float			fSeeAheadLength()		{	return 2.0f;	}	// How Many Seconds Ahead Can the Notes Board See?
	static const float			fSongBeginDelay()		{	return 3.0f;	}	// How Many Seconds will the Song Begin AFTER Scene_PlayGame Activates?
	static const float			fHitDetectionRange()	{	return 0.3f;	}	// What is the Detection Range (in seconds) for 
}


#endif