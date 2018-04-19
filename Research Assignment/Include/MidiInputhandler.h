//#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//#					Midi Input Handler
//#					Author: Christopher Diamond
//#					Date: August 28, 2013
//#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#	  Description:
//#		
//#		This class handles the Input from a Midi Keyboard.
//#		All Keys are inputted into an array and return boolean values
//#		based on whatever test it was asked. They are:
//#			
//#		  *	KeyTriggered:	Key was Pressed on this frame?
//#		
//#		  *	KeyPressed:		Key is Pressed?
//#		  
//#		  *	KeyHeld:		This function also takes in a parameter of a float,
//#							this float represents seconds. If the key has been 
//#							pressed and held for that amount of time, function
//#							returns true.
//#
//#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#ifndef _MIDIINPUTHANDLER_H_
#define _MIDIINPUTHANDLER_H_



#include "MidiInput.h"
#include "MidiDevice.h"
#include "KeyboardInformation.h"
#include <list>




//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Class: Midi Input Handler
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class MidiInputHandler
{
//////////////////////////////////////////
public:
//////////////////////////////////////////

	//===============================================
	//			Constructor & Destructor
	//===============================================
	MidiInputHandler();
	~MidiInputHandler();
	//===============================================
	//			Public Functions
	//===============================================
	void OnLoad();
	void OnUnload();

	void Reset();
	void Update();
	//===============================================
	//			Getter Functions
	//===============================================
	std::list<unsigned int>& GetCurrentlyTriggeredKeys	(																											);
	std::list<unsigned int>& GetCurrentlyReleasedKeys	(																											);
	unsigned int			 GetKeyID					( KeyboardInfo::KeyboardOctaves::Octaves	Octave,	 KeyboardInfo::KeyboardKeys::Keys Key					);
	bool					 KeyTriggered				( KeyboardInfo::KeyboardOctaves::Octaves	Octave,	 KeyboardInfo::KeyboardKeys::Keys Key					);
	bool					 KeyTriggered				( unsigned int								Key																);
	bool					 KeyPressed					( KeyboardInfo::KeyboardOctaves::Octaves	Octave,	 KeyboardInfo::KeyboardKeys::Keys Key					);
	bool					 KeyPressed					( unsigned int								Key																);
	bool					 KeyHeld					( KeyboardInfo::KeyboardOctaves::Octaves	Octave,	 KeyboardInfo::KeyboardKeys::Keys Key,	float HeldTimer	);
	bool					 KeyHeld					( unsigned int								Key,	 float HeldTimer										);




//////////////////////////////////////////
private:
//////////////////////////////////////////

	//===============================================
	//			   Private Declarations
	//===============================================
	enum KeyboardPressed
	{
		PRESSED		= 144,
		RELEASED	= 128,
	};


	struct KeyInput;


	//===============================================
	//			Private Instance Variables
	//===============================================
	KeyInput*					m_aKeyInputArray;

	std::list<unsigned int>		m_lCurrentlyTriggeredKeys;
	std::list<unsigned int>		m_lCurrentlyReleasedKeys;

	unsigned int				m_uiMidiDeviceID;
	MidiEvent					m_MidiEventBuffer[MAX_MIDI_EVENTS];
	MidiDevice*					m_pMidiDevice;

	unsigned short				m_aMidiBuffer[MIDI_BUFFERSIZE];
	//===============================================
	//			Private Functions
	//===============================================
	void UpdatePressedKeys();
	void UpdateTriggeredKeys();
	void UpdateInput();
	void UpdateHoldTimers();
};




//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Struct: Key Input
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
struct MidiInputHandler::KeyInput
{
	bool KeyTriggered;
	bool KeyDown;

	float CurrentHoldTime;

	KeyInput()
	{
		ResetInput();
	}
	void ResetInput()
	{
		KeyTriggered	= false;
		KeyDown			= false;
	}
	void StartInput()
	{
		KeyTriggered	= true;
		KeyDown			= true;
		CurrentHoldTime = 0.0f;
	}
	void TurnOffTrigger()
	{
		KeyTriggered	= false;
	}
};


#endif