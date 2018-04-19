#include "InputHandler.h"

#include "DeltaTime.h"
#include "HelperFunctions.h"

#include <assert.h>
#include <map>


InputHandler* InputHandler::m_Instance = NULL;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Static Method: Get Instance
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
InputHandler* InputHandler::GetInstance()
{
	return m_Instance;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
InputHandler::InputHandler()
{
	if( m_Instance == nullptr )
	{
		m_Instance = this;
	}
	else
	{	
		assert(0);	
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	*~ Deconstructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
InputHandler::~InputHandler()
{
	m_Instance = nullptr;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: On Load
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void InputHandler::OnLoad()
{
	// Make all Input values equal false before they are ever used.
	SetJoyHatArray_toFalse();
	SetJoyButtonArray_toFalse();

	m_MidiInput.OnLoad();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: On Unload
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void InputHandler::OnUnload()
{
	m_MidiInput.OnUnload();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Update
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void InputHandler::Update()
{	
	m_lpCurrentlyPressedKeys.clear();
	UpdateInput();

	m_MidiInput.Update();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Update Input
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void InputHandler::UpdateInput()
{
	UpdateKeyboardInput();
	UpdateMouseInput();
	UpdateHoldTimer();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Update Keyboard Input
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void InputHandler::UpdateKeyboardInput()
{
	// Clear Currently Triggered Keys List
	m_lCurrentlyTriggeredKeyboardKeys.clear();
	m_lCurrentlyReleasedKeyboardKeys.clear();

	for(unsigned int i = 0; i < GLFW_KEY_LAST; ++i )
	{
		if(glfwGetKey(i) == GLFW_PRESS)
		{
			m_lpCurrentlyPressedKeys.push_back( &m_aKeyInputArray[i] );
			
			if( m_aKeyInputArray[i].KeyDown )
			{
				m_aKeyInputArray[i].TurnOffTrigger();		// Trigger only occurs on the frame it is Pressed
			}
			else
			{
				m_aKeyInputArray[i].StartInput();
				m_lCurrentlyTriggeredKeyboardKeys.push_back( i ); // Add 'Just Now' Key(s) That Have Been Triggered 
			}
		}
		else
		{
			if( m_aKeyInputArray[i].KeyDown )
			{
				m_lCurrentlyReleasedKeyboardKeys.push_back( i );
			}
			m_aKeyInputArray[i].ResetInput();
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Update Mouse Input
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void InputHandler::UpdateMouseInput()
{
	UpdateMouseButtonInput();
	UpdateMousePosition();
	UpdateMouseWheelPosition();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Update Mouse Button Input
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void InputHandler::UpdateMouseButtonInput()
{
	m_lCurrentlyTriggeredMouseKeys.clear();
	m_lCurrentlyReleasedMouseKeys.clear();

	for(unsigned int i = 0; i < GLFW_MOUSE_BUTTON_LAST; ++i )
	{
		if(glfwGetMouseButton(i) == GLFW_PRESS)
		{
			m_lpCurrentlyPressedKeys.push_back( &m_MouseInputInfo.MouseInputArray[i] );

			if( m_MouseInputInfo.MouseInputArray[i].KeyDown )
			{
				m_MouseInputInfo.MouseInputArray[i].TurnOffTrigger();
			}
			else
			{
				m_MouseInputInfo.MouseInputArray[i].StartInput();
				m_lCurrentlyTriggeredMouseKeys.push_back( i );
			}
		}
		else
		{
			if(m_MouseInputInfo.MouseInputArray[i].KeyDown)
			{
				m_lCurrentlyReleasedMouseKeys.push_back( i );
			}
			m_MouseInputInfo.MouseInputArray[i].ResetInput();
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Update Mouse Position
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void InputHandler::UpdateMousePosition()
{
	m_MouseInputInfo.PreviousMousePos = m_MouseInputInfo.CurrentMousePos;
	glfwGetMousePos( &m_MouseInputInfo.CurrentMousePos.first, &m_MouseInputInfo.CurrentMousePos.second );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Update Mouse Wheel Position
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void InputHandler::UpdateMouseWheelPosition()
{
	m_MouseInputInfo.PreviousMouseWheelPos	= m_MouseInputInfo.CurrentMouseWheelPos;
	m_MouseInputInfo.CurrentMouseWheelPos	= glfwGetMouseWheel();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Update Hold Timer for all Currently Held Keys
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void InputHandler::UpdateHoldTimer()
{
	for( std::list<KeyInput*>::iterator Iter = m_lpCurrentlyPressedKeys.begin(); Iter != m_lpCurrentlyPressedKeys.end(); ++Iter )
	{
		(*Iter)->CurrentHoldTime += DeltaTime::GetInstance()->GetDeltaTime();
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Joy D-Pad Array to False
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void InputHandler::SetJoyHatArray_toFalse()
{
	for ( int i = 0; i < JoystickDPadButtons_Down_Left + 1; ++i )
	{
		JoyDPadInputArray[i] = false;
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Joy Button Array to False
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void InputHandler::SetJoyButtonArray_toFalse()
{
	for ( int i = 0; i < JoystickButtons_RightStickClick + 1; ++i )
	{
		JoyButtonInputArray[i] = false;
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Find Currently Pressed Character Key
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::string InputHandler::FindCharacterKeyPressed()
{
	for(unsigned int i = 0; i < GLFW_KEY_LAST; ++i)
	{
		if( i == GLFW_KEY_LSHIFT || i == GLFW_KEY_RSHIFT ) { continue; } // Shift Keys

		if( KeyTriggered(i) || KeyHeld(i) )
		{
			char c = i;
			if( i == 92 ) { c = '\\'; }
			if( HelperFunctions::IsCharacterValid(c) )
			{
				return (const char*)c;
			}
		}
	}
	return "";
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Upcase Character if Shift Key is Pressed
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::string InputHandler::ShiftKeyCharacterUpdate( std::string& str )
{
	if( !str.empty() )
	{
		if( KeyPressed( GLFW_KEY_LSHIFT ) || KeyPressed( GLFW_KEY_RSHIFT ) )
		{
			switch(str[0])
			{
				case 92 :	{ return "|";				}
				case 91 :	{ return "{";				}
				case 93 :	{ return "}";				}
				case 39 :	{ return (const char*)'"';	}
				case ';':	{ return ":";				}
				case '/':	{ return "?";				}
				case '.':	{ return ">";				}
				case ',':	{ return "<";				}
				case '`':	{ return "~";				}
				case '1':	{ return "!";				}
				case '2':	{ return "@";				}
				case '3':	{ return "#";				}
				case '4':	{ return "$";				}
				case '5':	{ return "%";				}
				case '6':	{ return "^";				}
				case '7':	{ return "&";				}
				case '8':	{ return "*";				}
				case '9':	{ return "(";				}
				case '0':	{ return ")";				}
				case '-':	{ return "_";				}
				case '=':	{ return "+";				}
				default :	{ return HelperFunctions::String_To_UpperCase(str); }
			}
		}
	}
	return str;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Currently Triggered Keyboard Keys
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::list<unsigned int>& InputHandler::GetCurrentlyTriggeredKeyboardKeys()
{
	return m_lCurrentlyTriggeredKeyboardKeys;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Currently Triggered Mouse Keys
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::list<unsigned int>& InputHandler::GetCurrentlyTriggeredMouseKeys()
{
	return m_lCurrentlyTriggeredMouseKeys;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Currently Triggered Midi Keyboard Keys
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::list<unsigned int>& InputHandler::GetCurrentlyTriggeredMidiKeyboardKeys()
{
	return m_MidiInput.GetCurrentlyTriggeredKeys();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Currently Released Keyboard Keys
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::list<unsigned int>& InputHandler::GetCurrentlyReleasedKeyboardKeys()
{
	return m_lCurrentlyReleasedKeyboardKeys;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Currently Released Mouse Keys
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::list<unsigned int>& InputHandler::GetCurrentlyReleasedMouseKeys()
{
	return m_lCurrentlyReleasedMouseKeys;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Currently Released Midi Keyboard Keys
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::list<unsigned int>& InputHandler::GetCurrentlyReleasedMidiKeyboardKeys()
{
	return m_MidiInput.GetCurrentlyReleasedKeys();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Key Triggered?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool InputHandler::KeyTriggered( unsigned int Key )
{
	return m_aKeyInputArray[ Key ].KeyTriggered;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Key Pressed?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool InputHandler::KeyPressed( unsigned int Key )
{
	return m_aKeyInputArray[ Key ].KeyDown;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Key Held?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool InputHandler::KeyHeld( unsigned int Key, float a_fHeldTimer )
{
	return (m_aKeyInputArray[ Key ].KeyDown) && (m_aKeyInputArray[ Key ].CurrentHoldTime > a_fHeldTimer);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Mouse Button Triggered?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool InputHandler::MouseButtonTriggered( unsigned int Button )
{
	return m_MouseInputInfo.MouseInputArray[ Button ].KeyTriggered;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Mouse Button Pressed?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool InputHandler::MouseButtonPressed( unsigned int Button )
{
	return m_MouseInputInfo.MouseInputArray[ Button ].KeyDown;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Mouse Button Held?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool InputHandler::MouseButtonHeld( unsigned int Button, float a_fHeldTimer )
{
	return (m_MouseInputInfo.MouseInputArray[ Button ].KeyDown) && (m_MouseInputInfo.MouseInputArray[ Button ].CurrentHoldTime > a_fHeldTimer);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Joy Button State
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool InputHandler::GetJoyButtonState( unsigned int Button )
{
	return JoyButtonInputArray[ Button ];
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Joy D-Pad State
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool InputHandler::GetJoyDPadState( unsigned int Hat )
{
	bool bReturnBool = JoyDPadInputArray[ Hat ];

	// Since JoyHatMotion doesn't have an SDL call to detect if the hat is released
	// We need to set it back to false after giving its value to whatever called it.
	//
	// The Input function will still do everything the same way it should even if we 
	// are sneakily setting the value to false.

	JoyDPadInputArray[ Hat ] = false;

	return bReturnBool;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Midi Keyboard Key Triggered?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool InputHandler::MidiKeyboardKeyTriggered( KeyboardInfo::KeyboardOctaves::Octaves Octave, KeyboardInfo::KeyboardKeys::Keys Key )
{
	return m_MidiInput.KeyTriggered( Octave, Key );
}

bool InputHandler::MidiKeyboardKeyTriggered( unsigned int Key )
{
	return m_MidiInput.KeyTriggered( Key );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Midi Keyboard Key Pressed?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool InputHandler::MidiKeyboardKeyPressed( KeyboardInfo::KeyboardOctaves::Octaves Octave, KeyboardInfo::KeyboardKeys::Keys Key )
{
	return m_MidiInput.KeyPressed( Octave, Key );
}

bool InputHandler::MidiKeyboardKeyPressed( unsigned int Key )
{
	return m_MidiInput.KeyPressed( Key );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Midi Keyboard Key Held?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool InputHandler::MidiKeyboardKeyHeld( KeyboardInfo::KeyboardOctaves::Octaves Octave, KeyboardInfo::KeyboardKeys::Keys Key, float a_fHeldTimer )
{
	return m_MidiInput.KeyHeld( Octave, Key, a_fHeldTimer );
}

bool InputHandler::MidiKeyboardKeyHeld( unsigned int Key, float a_fHeldTimer )
{
	return m_MidiInput.KeyHeld( Key, a_fHeldTimer );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Get Mouse Position
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MouseCoords InputHandler::GetMousePositionAsPair()
{
	return m_MouseInputInfo.CurrentMousePos;
}

Vector2 InputHandler::GetMousePositionAsVector2()
{
	return Vector2( (float)GetMousePositionX(), (float)GetMousePositionY() );
}

int InputHandler::GetMousePositionX()
{
	return m_MouseInputInfo.CurrentMousePos.first;
}

int InputHandler::GetMousePositionY()
{
	return m_MouseInputInfo.CurrentMousePos.second;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Get Mouse Movement From Previous Frame
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MouseCoords InputHandler::GetMouseMovementFromPreviousFrameAsPair()
{
	return MouseCoords(	GetMouseMovementFromPreviousFrameX(), GetMouseMovementFromPreviousFrameY() );
}

Vector2 InputHandler::GetMouseMovementFromPreviousFrameAsVector2()
{
	return Vector2( (float)GetMouseMovementFromPreviousFrameX(), (float)GetMouseMovementFromPreviousFrameY() );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Get Mouse Delta Position (Same As Above, Different Name)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MouseCoords InputHandler::GetMouseDeltaPositionAsPair()
{
	return GetMouseMovementFromPreviousFrameAsPair();
}

Vector2 InputHandler::GetMouseDeltaPositionAsVector2()
{
	return GetMouseMovementFromPreviousFrameAsVector2();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Get Mouse Movement From Previous Frame Positions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int InputHandler::GetMouseMovementFromPreviousFrameX()
{
	return (GetMousePositionX() - m_MouseInputInfo.PreviousMousePos.first);
}

int InputHandler::GetMouseMovementFromPreviousFrameY()
{
	return (GetMousePositionY() - m_MouseInputInfo.PreviousMousePos.second);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Get Mouse Delta Positions (Same As Above, Different Name)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int InputHandler::GetMouseDeltaPositionX()
{
	return GetMouseMovementFromPreviousFrameX();
}

int InputHandler::GetMouseDeltaPositionY()
{
	return GetMouseMovementFromPreviousFrameY();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Mouse Wheel Position From Previous Frame
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int InputHandler::GetMouseWheelMovementFromPreviousFrame()
{
	return (m_MouseInputInfo.CurrentMouseWheelPos - m_MouseInputInfo.PreviousMouseWheelPos);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Mouse Wheel Delta Position (Same As Above, Different Name)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int InputHandler::GetMouseWheelDeltaPosition()
{
	return GetMouseWheelMovementFromPreviousFrame();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Key Pressed Character
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::string InputHandler::GetKeyPressedCharacter()
{
	return ShiftKeyCharacterUpdate( FindCharacterKeyPressed() );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Set Mouse Coordinates
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void InputHandler::SetMouseCoords( MouseCoords XY )
{
	SetMouseCoords( XY.first, XY.second );
}

void InputHandler::SetMouseCoords( Vector2& XY )
{
	SetMouseCoords( (int)XY.x, (int)XY.y );
}

void InputHandler::SetMouseCoords( int X, int Y )
{
	glfwSetMousePos( X, Y );
	m_MouseInputInfo.PreviousMousePos = m_MouseInputInfo.CurrentMousePos = MouseCoords( X, Y );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Mouse Wheel Position
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void InputHandler::SetMouseWheelPosition( int Position )
{
	glfwSetMouseWheel( Position );
	m_MouseInputInfo.PreviousMouseWheelPos = m_MouseInputInfo.CurrentMouseWheelPos = Position;
}