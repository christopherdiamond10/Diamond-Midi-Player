#include "KeyboardDrawSystem.h"
#include "KeyboardKeySprite.h"
#include "KeyboardInformation.h"
#include "Screen.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	*: Initialising Variables
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const char* KeyboardDrawSystem::sm_sLeftKeySpriteName		= "LeftKeySprite.png";
const char* KeyboardDrawSystem::sm_sMiddleKeySpriteName		= "MiddleKeySprite.png";
const char* KeyboardDrawSystem::sm_sRightKeySpriteName		= "RightKeySprite.png";
const char* KeyboardDrawSystem::sm_sBlackKeySpriteName		= "BlackKeySprite.png";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
KeyboardDrawSystem::KeyboardDrawSystem()
{
	CreateKeySprites();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	*~ Deconstructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
KeyboardDrawSystem::~KeyboardDrawSystem()
{
	for( auto iter = m_lWhiteKeySprites.begin(); iter != m_lWhiteKeySprites.end(); ++iter )
	{
		delete (*iter);
	}

	for( auto iter = m_lBlackKeySprites.begin(); iter != m_lBlackKeySprites.end(); ++iter )
	{
		delete (*iter);
	}

	m_lWhiteKeySprites.clear();
	m_lBlackKeySprites.clear();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Update
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void KeyboardDrawSystem::Update()
{
	for( auto iter = m_lWhiteKeySprites.begin(); iter != m_lWhiteKeySprites.end(); ++iter )
	{
		(*iter)->Update();
	}

	for( auto iter = m_lBlackKeySprites.begin(); iter != m_lBlackKeySprites.end(); ++iter )
	{
		(*iter)->Update();
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Draw
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void KeyboardDrawSystem::Draw()
{
	for( auto iter = m_lWhiteKeySprites.begin(); iter != m_lWhiteKeySprites.end(); ++iter )
	{
		(*iter)->Draw();
	}

	// Black Keys need to be drawn after White keys, else texture crossovers happen and it looks unnatural.
	for( auto iter = m_lBlackKeySprites.begin(); iter != m_lBlackKeySprites.end(); ++iter )
	{
		(*iter)->Draw();
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Create Key Sprites
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void KeyboardDrawSystem::CreateKeySprites()
{	
	// Iterate Through Each Key, +1 for Aesthetic Purposes
	for( unsigned int i = KeyboardInfo::KeysInfo::BeginningNoteID; i < KeyboardInfo::KeysInfo::KeysArraySize+1; ++i )
	{
		KeyboardInfo::KeyboardKeys::KeyType eKType	= KeyboardInfo::KeysInfo::GetKeyType(i);
		const char* cSpriteName						= CurrentKeyTexture( eKType );
		Vector2 vKeyPosition						= KeyboardInfo::KeyboardDrawInfo::GetNotePosition(i);

		if( eKType == KeyboardInfo::KeyboardKeys::BLACK )
		{
			KeyboardKeySprite* Sprite = new KeyboardKeySprite(i, cSpriteName, vKeyPosition, KeyboardInfo::KeyboardDrawInfo::BlackKeySize());
			m_lBlackKeySprites.push_back(Sprite);
		}
		else
		{
			KeyboardKeySprite* Sprite = new KeyboardKeySprite(i, cSpriteName, vKeyPosition, KeyboardInfo::KeyboardDrawInfo::WhiteKeySize());
			m_lWhiteKeySprites.push_back(Sprite);
		}
	}

}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Current Key Texture Name
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const char* KeyboardDrawSystem::CurrentKeyTexture(KeyboardInfo::KeyboardKeys::KeyType KType)
{
	switch(KType)
	{
		case KeyboardInfo::KeyboardKeys::LEFT:		return KeyboardDrawSystem::sm_sLeftKeySpriteName;
		case KeyboardInfo::KeyboardKeys::MIDDLE:	return KeyboardDrawSystem::sm_sMiddleKeySpriteName;
		case KeyboardInfo::KeyboardKeys::RIGHT:		return KeyboardDrawSystem::sm_sRightKeySpriteName;
		default:									return KeyboardDrawSystem::sm_sBlackKeySpriteName;
	}
}