//\====================================================================================
//\ Author: Christopher.Allport
//\ About : InputHandler.h - 
//\ 
//\ Defines the InputHandler Singleton Class. This class is used to process all inputs
//\ for the game, it is used to return boolean values to other classes about whether 
//\ or not a key/button has been pressed.
//\====================================================================================
#ifndef _INPUTHANDLER_H_
#define _INPUTHANDLER_H_


#include "glfw.h"
#include "MidiInputhandler.h"
#include "MathHelper.h"
#include <list>
#include <string>


// Global Definitions, so that the variables can be used in other .cpp files
// The defintions do not affect anything, instead just gives a name to these numbers
// which are used for joystick input.
#define JoystickButtons_A					0
#define JoystickButtons_B					1
#define JoystickButtons_X					2
#define JoystickButtons_Y					3
#define JoystickButtons_LB					4
#define JoystickButtons_RB					5
#define JoystickButtons_Select				6
#define JoystickButtons_Start				7
#define JoystickButtons_LeftStickClick		8
#define JoystickButtons_RightStickClick		9


#define JoystickDPadButtons_Up				1
#define JoystickDPadButtons_Right			2
#define JoystickDPadButtons_Up_Right		3
#define JoystickDPadButtons_Down			4
#define JoystickDPadButtons_Down_Right		6
#define JoystickDPadButtons_Left			8
#define JoystickDPadButtons_Up_Left			9
#define JoystickDPadButtons_Down_Left		12








typedef std::pair<int, int> MouseCoords;


class InputHandler
{
//////////////////////////////////////////
public:
//////////////////////////////////////////

	//===============================================
	//			Constructor & Destructor
	//===============================================
	InputHandler();
	~InputHandler();
	//===============================================
	//			Static Functions
	//===============================================
	static InputHandler* GetInstance();
	//===============================================
	//			Public Functions
	//===============================================
	void OnLoad();
	void OnUnload();
	void Update();
	//===============================================
	//			Getter Functions
	//===============================================
	std::list<unsigned int>& GetCurrentlyTriggeredKeyboardKeys		(												);
	std::list<unsigned int>& GetCurrentlyTriggeredMouseKeys			(												);
	std::list<unsigned int>& GetCurrentlyTriggeredMidiKeyboardKeys	(												);

	std::list<unsigned int>& GetCurrentlyReleasedKeyboardKeys		(												);
	std::list<unsigned int>& GetCurrentlyReleasedMouseKeys			(												);
	std::list<unsigned int>& GetCurrentlyReleasedMidiKeyboardKeys	(												);

	bool		KeyTriggered										( unsigned int Key								);
	bool		KeyPressed											( unsigned int Key								);
	bool		KeyHeld												( unsigned int Key,		float HeldTimer = 1.0f	);

	bool		MouseButtonTriggered								( unsigned int Button							);
	bool		MouseButtonPressed									( unsigned int Button							);
	bool		MouseButtonHeld										( unsigned int Button,	float HeldTimer = 1.0f	);

	bool		GetJoyButtonState									( unsigned int Button							);
	bool		GetJoyDPadState										( unsigned int Hat								);

	bool		MidiKeyboardKeyTriggered							( KeyboardInfo::KeyboardOctaves::Octaves	Octave,	 KeyboardInfo::KeyboardKeys::Keys Key					);
	bool		MidiKeyboardKeyTriggered							( unsigned int		Key																						);
	bool		MidiKeyboardKeyPressed								( KeyboardInfo::KeyboardOctaves::Octaves	Octave,	 KeyboardInfo::KeyboardKeys::Keys Key					);
	bool		MidiKeyboardKeyPressed								( unsigned int		Key																						);
	bool		MidiKeyboardKeyHeld									( KeyboardInfo::KeyboardOctaves::Octaves	Octave,	 KeyboardInfo::KeyboardKeys::Keys Key,	float HeldTimer	);
	bool		MidiKeyboardKeyHeld									( unsigned int		Key,							 float HeldTimer										);

	MouseCoords GetMousePositionAsPair								(												);
	MouseCoords GetMouseMovementFromPreviousFrameAsPair				(												);
	MouseCoords GetMouseDeltaPositionAsPair							(												);
	Vector2		GetMousePositionAsVector2							(												);
	Vector2		GetMouseMovementFromPreviousFrameAsVector2			(												);
	Vector2		GetMouseDeltaPositionAsVector2						(												);
	int			GetMousePositionX									(												);
	int			GetMousePositionY									(												);
	int			GetMouseMovementFromPreviousFrameX					(												);
	int			GetMouseMovementFromPreviousFrameY					(												);
	int			GetMouseDeltaPositionX								(												);
	int			GetMouseDeltaPositionY								(												);

	int			GetMouseWheelMovementFromPreviousFrame				(												);
	int			GetMouseWheelDeltaPosition							(												);

	std::string GetKeyPressedCharacter								(												);
	//===============================================
	//			Setter Functions
	//===============================================
	void SetMouseCoords( MouseCoords XY );
	void SetMouseCoords( Vector2& XY );
	void SetMouseCoords( int X, int Y );

	void SetMouseWheelPosition( int Position );
	//===============================================
	//			Public Declarations
	//===============================================
	



//////////////////////////////////////////
private:
//////////////////////////////////////////

	//===============================================
	//			   Private Declarations
	//===============================================
	struct KeyInput
	{
		bool KeyTriggered;		// True if Just Hit, False if either Held down or not Hit.
		bool KeyDown;			// True if Just Hit or Held Down, False if not hit.

		float CurrentHoldTime;	// Current Time this Key is being Held for. Used for the 'Hold()' function. Mainly used for options, like adjusting an option by one then up at a constant rate after key was held for a second.

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



	struct MouseInput
	{
		KeyInput		MouseInputArray[GLFW_MOUSE_BUTTON_LAST];
		MouseCoords		PreviousMousePos,		CurrentMousePos;
		int				PreviousMouseWheelPos,	CurrentMouseWheelPos;

		MouseInput() 
			: PreviousMousePos(0, 0), CurrentMousePos(0, 0), PreviousMouseWheelPos(0), CurrentMouseWheelPos(0)
		{
		}
	};


	//===============================================
	//			Private Instance Variables
	//===============================================
	KeyInput			m_aKeyInputArray[GLFW_KEY_LAST];				// Keyboard Input Array
	MouseInput			m_MouseInputInfo;								// Mouse Button Input
	MidiInputHandler	m_MidiInput;									// Midi Keyboard Input

	bool JoyButtonInputArray[JoystickButtons_RightStickClick + 1];		// Joypad Button Array, on an Xbox Controller this would be: A, B, X, Y, LB, RB, Start, Select
	bool JoyDPadInputArray[JoystickDPadButtons_Down_Left + 1];			// Joypad D-Pad Direction Press Array

	std::list<KeyInput*>	m_lpCurrentlyPressedKeys;					// Currently Pressed Mouse/Keyboard Keys. Used for Updating Held Timers
	std::list<unsigned int> m_lCurrentlyTriggeredKeyboardKeys;			// Keyboard Keys That Were 'Just Triggered' On This Frame
	std::list<unsigned int> m_lCurrentlyTriggeredMouseKeys;				// Mouse Keys That Were 'Just Triggered' On This Frame

	std::list<unsigned int> m_lCurrentlyReleasedKeyboardKeys;			// Keyboard Keys That Were 'Just Released' On This Frame
	std::list<unsigned int> m_lCurrentlyReleasedMouseKeys;				// Mouse Keys That Were 'Just Released' On This Frame

	//===============================================
	//			Private Functions
	//===============================================
	void UpdateInput();
	void UpdateKeyboardInput();
	void UpdateMouseInput();
	void UpdateMouseButtonInput();
	void UpdateMousePosition();
	void UpdateMouseWheelPosition();
	void UpdateHoldTimer();
	
	std::string FindCharacterKeyPressed();
	std::string ShiftKeyCharacterUpdate( std::string& sCharacter );

	void SetJoyHatArray_toFalse();
	void SetJoyButtonArray_toFalse();
	//===============================================
	//			Private Pointers
	//===============================================
	static InputHandler*	m_Instance;
};

#endif