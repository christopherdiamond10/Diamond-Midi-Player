#include "MidiInputHandler.h"

#include "DeltaTime.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MidiInputHandler::MidiInputHandler()
{
	m_uiMidiDeviceID	= 0;
	m_pMidiDevice		= new MidiDevice();
	m_aKeyInputArray	= new KeyInput[KeyboardInfo::KeysInfo::KeysArraySize];
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	*~ Deconstructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MidiInputHandler::~MidiInputHandler()
{
	delete		m_pMidiDevice;
	delete[]	m_aKeyInputArray;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: On Load
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiInputHandler::OnLoad()
{
	m_pMidiDevice->open(m_uiMidiDeviceID);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: On Unload
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiInputHandler::OnUnload()
{
	m_pMidiDevice->close();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Reset
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiInputHandler::Reset()
{
	for( int i = 0; i < KeyboardInfo::KeysInfo::KeysArraySize; ++i )
	{
		m_aKeyInputArray[i].ResetInput();
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Update
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiInputHandler::Update()
{
	UpdateTriggeredKeys();
	UpdateInput();
	UpdateHoldTimers();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Update Triggered Keys
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiInputHandler::UpdateTriggeredKeys()
{
	for( std::list< unsigned int >::iterator Iter = m_lCurrentlyTriggeredKeys.begin(); Iter != m_lCurrentlyTriggeredKeys.end(); ++Iter )
	{
		m_aKeyInputArray[ *Iter ].TurnOffTrigger();
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Update Input
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiInputHandler::UpdateInput()
{
	// Reset Triggered Keys List
	m_lCurrentlyTriggeredKeys.clear();
	m_lCurrentlyReleasedKeys.clear();

	// Go Through Keys List
	unsigned int uiPollCount = m_pMidiDevice->poll(m_aMidiBuffer);
	for( unsigned int i = 0; i < uiPollCount; ) 
	{
        unsigned int uiLength = (unsigned int)((m_aMidiBuffer[i] & 0xff00) >> 8);

        if((unsigned char)(m_aMidiBuffer[i] & 0x00ff) != 63) 
		{
            continue;
        }

        int iType	 = (int)((m_aMidiBuffer[i + 1] & 0x00ff));
        int iChannel = (int)((m_aMidiBuffer[i + 1] & 0xff00) >> 8);

		if( iType == MidiInputHandler::PRESSED )
		{
			m_aKeyInputArray[iChannel].StartInput();
			m_lCurrentlyTriggeredKeys.push_back( iChannel );
		}
		else if( iType == MidiInputHandler::RELEASED )
		{
			m_aKeyInputArray[iChannel].ResetInput();
			m_lCurrentlyReleasedKeys.push_back( iChannel );
		}

        i += uiLength;
    }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Update Hold Timers
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiInputHandler::UpdateHoldTimers()
{
	for( unsigned int i = 0; i < KeyboardInfo::KeysInfo::KeysArraySize; ++i )
	{
		if( m_aKeyInputArray[i].KeyDown )
		{
			m_aKeyInputArray[i].CurrentHoldTime += DeltaTime::GetInstance()->GetDeltaTime();
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Currently Triggered Keys
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::list<unsigned int>& MidiInputHandler::GetCurrentlyTriggeredKeys()
{
	return m_lCurrentlyTriggeredKeys;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Currently Released Keys
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::list<unsigned int>& MidiInputHandler::GetCurrentlyReleasedKeys()
{
	return m_lCurrentlyReleasedKeys;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Key ID
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
unsigned int MidiInputHandler::GetKeyID( KeyboardInfo::KeyboardOctaves::Octaves Octave, KeyboardInfo::KeyboardKeys::Keys Key )
{
	return ( (unsigned int)Key + ((unsigned int)Octave * (unsigned int)KeyboardInfo::KeysInfo::KeysPerOctave) );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Key Triggered?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool MidiInputHandler::KeyTriggered( KeyboardInfo::KeyboardOctaves::Octaves Octave, KeyboardInfo::KeyboardKeys::Keys Key )
{
	return KeyTriggered( GetKeyID(Octave, Key) );
}

bool MidiInputHandler::KeyTriggered( unsigned int Key )
{
	if( Key < KeyboardInfo::KeysInfo::KeysArraySize )
		return m_aKeyInputArray[ Key ].KeyTriggered;
	return false;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Key Pressed?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool MidiInputHandler::KeyPressed( KeyboardInfo::KeyboardOctaves::Octaves Octave, KeyboardInfo::KeyboardKeys::Keys Key )
{
	return KeyPressed( GetKeyID(Octave, Key) );
}

bool MidiInputHandler::KeyPressed( unsigned int Key )
{
	if( Key < KeyboardInfo::KeysInfo::KeysArraySize )
		return m_aKeyInputArray[ Key ].KeyDown;
	return false;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Key Held?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool MidiInputHandler::KeyHeld( KeyboardInfo::KeyboardOctaves::Octaves Octave, KeyboardInfo::KeyboardKeys::Keys Key, float a_fHeldTimer )
{
	return KeyHeld( GetKeyID(Octave, Key), a_fHeldTimer );
}

bool MidiInputHandler::KeyHeld( unsigned int Key, float a_fHeldTimer )
{
	if( Key < KeyboardInfo::KeysInfo::KeysArraySize )
		return (m_aKeyInputArray[ Key ].KeyDown) && (m_aKeyInputArray[ Key ].CurrentHoldTime > a_fHeldTimer);
	return false;
}