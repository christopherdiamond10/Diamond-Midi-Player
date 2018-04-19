//#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//#					Keyboard Draw System
//#					Author: Christopher Diamond
//#					Date: October 20, 2013
//#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#	  Description:
//#		
//#		This class contains all inidividual Key Sprites for the Keyboard and 
//#		Draws them as needed.
//#
//#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#ifndef _KEYBOARDDRAWSYSTEM_H_
#define _KEYBOARDDRAWSYSTEM_H_

#include "KeyboardInformation.h"
#include <list>

class KeyboardKeySprite;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Class: KeyboardDrawSystem
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class KeyboardDrawSystem
{
////////////////////////////////////////////////////////////
public:
////////////////////////////////////////////////////////////

	//===============================================
	//			Constructors & Destructor
	//===============================================
	KeyboardDrawSystem();
	~KeyboardDrawSystem();
	//===============================================
	//			Public Functions
	//===============================================
	void Update();
	void Draw();


////////////////////////////////////////////////////////////
private:
////////////////////////////////////////////////////////////
	
	//===============================================
	//			Private Declarations
	//===============================================
	static const char* sm_sLeftKeySpriteName;
	static const char* sm_sMiddleKeySpriteName;
	static const char* sm_sRightKeySpriteName;
	static const char* sm_sBlackKeySpriteName;
	//===============================================
	//			Private Instance Variables
	//===============================================
	std::list<KeyboardKeySprite*> m_lWhiteKeySprites;
	std::list<KeyboardKeySprite*> m_lBlackKeySprites;
	//===============================================
	//			Private Functions
	//===============================================
	void			CreateKeySprites();
	const char*		CurrentKeyTexture(KeyboardInfo::KeyboardKeys::KeyType KType);
};


#endif