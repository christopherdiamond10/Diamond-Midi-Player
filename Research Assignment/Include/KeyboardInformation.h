//#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//#					Keyboard Information
//#					Author: Christopher Diamond
//#					Date: October 20, 2013
//#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#	  Description:
//#		
//#		This File Simply Holds Necessary information about the Midi Keyboard 
//#		System
//#
//#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#ifndef _KEYBOARDINFORMATION_H_
#define _KEYBOARDINFORMATION_H_

#include "MathHelper.h"
#include "Screen.h"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Namespace: KeyboardInfo
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
namespace KeyboardInfo
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	** Keyboard Keys
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct KeyboardKeys
	{
		enum Keys
		{
			C = 9,		Cb = 8,		CFlat = 8,		CS = 10,	CSharp = 10,
			D = 11,		Db = 10,	DFlat = 10,		DS = 12,	DSharp = 12,
			E = 13,		Eb = 12,	EFlat = 12,		ES = 14,	ESharp = 14,
			F = 14,		Fb = 13,	FFlat = 13,		FS = 15,	FSharp = 15,
			G = 16,		Gb = 15,	GFlat = 15,		GS = 17,	GSharp = 17,
			A = 18,		Ab = 17,	AFlat = 17,		AS = 19,	ASharp = 19,
			B = 20,		Bb = 19,	BFlat = 19,		BS = 9,		BSharp = 9,
		};

		enum KeyType
		{
			LEFT,
			MIDDLE,
			RIGHT,
			BLACK,
		};
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	** Keys Info
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct KeysInfo
	{
		enum Info
		{
			KeysPerOctave		= 12,
			WhiteKeysPerOctave	= 7,
			BlackKeysPerOctave	= 5,

			TotalWhiteKeys		= 36,
			TotalBlackKeys		= 25,
			TotalOctaves		= 5,

			BeginningNoteID		= 36,					// Edited To Suit My Current Keyboard Setup
			HighestKeyID		= 96,					// Edited To Suit My Current Keyboard Setup
			KeysArraySize		= HighestKeyID + 1,		// For Arrays.
		};
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//	* New Method: Get Note ID
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		static KeyboardKeys::Keys GetNoteID(unsigned int NoteID)
		{
			KeyboardInfo::KeyboardKeys::Keys eCurrentKey = KeyboardInfo::KeyboardKeys::C;
			for( unsigned int i = KeysInfo::BeginningNoteID; i < NoteID; ++i )
			{
				eCurrentKey = GetNextNote(eCurrentKey);
			}
			return eCurrentKey;
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//	* New Methods: Get Next Note ID
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		static KeyboardKeys::Keys GetNextNote(unsigned int uiNoteID)
		{
			return GetNextNote( GetNoteID(uiNoteID) );
		}

		static KeyboardKeys::Keys GetNextNote(KeyboardKeys::Keys eCurrentNote)
		{
			switch(eCurrentNote)
			{
			case KeyboardInfo::KeyboardKeys::C:			return KeyboardInfo::KeyboardKeys::CSharp;
			case KeyboardInfo::KeyboardKeys::CSharp:	return KeyboardInfo::KeyboardKeys::D;
			case KeyboardInfo::KeyboardKeys::D:			return KeyboardInfo::KeyboardKeys::DSharp;
			case KeyboardInfo::KeyboardKeys::DSharp:	return KeyboardInfo::KeyboardKeys::E;
			case KeyboardInfo::KeyboardKeys::E:			return KeyboardInfo::KeyboardKeys::F;
			case KeyboardInfo::KeyboardKeys::F:			return KeyboardInfo::KeyboardKeys::FSharp;
			case KeyboardInfo::KeyboardKeys::FSharp:	return KeyboardInfo::KeyboardKeys::G;
			case KeyboardInfo::KeyboardKeys::G:			return KeyboardInfo::KeyboardKeys::GSharp;
			case KeyboardInfo::KeyboardKeys::GSharp:	return KeyboardInfo::KeyboardKeys::A;
			case KeyboardInfo::KeyboardKeys::A:			return KeyboardInfo::KeyboardKeys::ASharp;
			case KeyboardInfo::KeyboardKeys::ASharp:	return KeyboardInfo::KeyboardKeys::B;
			default:									return KeyboardInfo::KeyboardKeys::C;
			}
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//	* New Methods: Get Key Note As String
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		static const char* GetKeyNoteAsString(unsigned int uiNoteID)
		{
			return GetKeyNoteAsString( GetNoteID(uiNoteID) );
		}

		static const char* GetKeyNoteAsString(KeyboardKeys::Keys eCurrentNote)
		{
			switch(eCurrentNote)
			{
			case KeyboardInfo::KeyboardKeys::C:			return "C";
			case KeyboardInfo::KeyboardKeys::CSharp:	return "C#";
			case KeyboardInfo::KeyboardKeys::D:			return "D";
			case KeyboardInfo::KeyboardKeys::DSharp:	return "C#";
			case KeyboardInfo::KeyboardKeys::E:			return "E";
			case KeyboardInfo::KeyboardKeys::F:			return "F";
			case KeyboardInfo::KeyboardKeys::FSharp:	return "F#";
			case KeyboardInfo::KeyboardKeys::G:			return "G";
			case KeyboardInfo::KeyboardKeys::GSharp:	return "G#";
			case KeyboardInfo::KeyboardKeys::A:			return "A";
			case KeyboardInfo::KeyboardKeys::ASharp:	return "A#";
			default:									return "B";
			}
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//	* New Methods: Get Key Type
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		static KeyboardKeys::KeyType GetKeyType(unsigned int NoteID)
		{
			return GetKeyType( GetNoteID(NoteID) );
		}

		static KeyboardKeys::KeyType GetKeyType(KeyboardInfo::KeyboardKeys::Keys eCurrentKey)
		{
			switch(eCurrentKey)
			{
			case KeyboardInfo::KeyboardKeys::C:			return KeyboardKeys::LEFT;
			case KeyboardInfo::KeyboardKeys::CSharp:	return KeyboardKeys::BLACK;
			case KeyboardInfo::KeyboardKeys::D:			return KeyboardKeys::MIDDLE;
			case KeyboardInfo::KeyboardKeys::DSharp:	return KeyboardKeys::BLACK;
			case KeyboardInfo::KeyboardKeys::E:			return KeyboardKeys::RIGHT;
			case KeyboardInfo::KeyboardKeys::F:			return KeyboardKeys::LEFT;
			case KeyboardInfo::KeyboardKeys::FSharp:	return KeyboardKeys::BLACK;
			case KeyboardInfo::KeyboardKeys::G:			return KeyboardKeys::MIDDLE;
			case KeyboardInfo::KeyboardKeys::GSharp:	return KeyboardKeys::BLACK;
			case KeyboardInfo::KeyboardKeys::A:			return KeyboardKeys::MIDDLE;
			case KeyboardInfo::KeyboardKeys::ASharp:	return KeyboardKeys::BLACK;
			default:									return KeyboardKeys::RIGHT;
			}
		}
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	** Keyboard Octaves
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct KeyboardOctaves
	{
		enum Octaves
		{
			OctaveOne	= 0,
			OctaveTwo	= 1,
			OctaveThree	= 2,
			OctaveFour	= 3,
			OctaveFive	= 4,
			OctaveSix	= 5,
			OctaveSeven	= 6,
		};
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	** Keyboard Draw Info
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct KeyboardDrawInfo
	{
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//	* New Method: Get Note Position
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		static Vector2 GetNotePosition(unsigned int uiNoteID)
		{
			Vector2 vWhiteKeyPosition = Screen::GetKeyboardDrawArea().Position;					// Key Position
			KeyboardInfo::KeyboardKeys::Keys eCurrentKey = KeyboardInfo::KeyboardKeys::C;		// Starting KeyID
			for( unsigned int i = KeysInfo::BeginningNoteID; i < uiNoteID; ++i )				// Go Through Each Note Until ID is Reached
			{
				if( KeysInfo::GetKeyType(eCurrentKey) != KeyboardInfo::KeyboardKeys::BLACK )	// If Current Note is not a Black Key
					vWhiteKeyPosition.x += KeyboardInfo::KeyboardDrawInfo::WhiteKeySize().x;	//	Increase White Key Position

				eCurrentKey = KeyboardInfo::KeysInfo::GetNextNote(eCurrentKey);					// Move on to Next Key
			}

			// Return Position for Key Type
			if( KeysInfo::GetKeyType(eCurrentKey) == KeyboardInfo::KeyboardKeys::BLACK )
			{
				return (vWhiteKeyPosition + KeyboardInfo::KeyboardDrawInfo::BlackKeyPositionOffset());
			}
			else
			{
				return vWhiteKeyPosition;
			}
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//	* New Method: Get Note Size
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		static Vector2 GetNoteSize(unsigned int uiNoteID)
		{
			return (KeysInfo::GetKeyType(uiNoteID) == KeyboardInfo::KeyboardKeys::BLACK) ? BlackKeySize() : WhiteKeySize();
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//	* New Method: Get Note Colour
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		static Vector4 GetNoteColour(unsigned int uiNoteID)
		{
			return (KeysInfo::GetKeyType(uiNoteID) == KeyboardInfo::KeyboardKeys::BLACK) ? GetBlackKeyColour() : GetWhiteKeyColour();
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//	* New Method: Black Key Position Offset
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		static Vector2 BlackKeyPositionOffset()
		{
			return Vector2( -(WhiteKeySize().x * 0.15f), 0.0f );
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//	* New Method: Get White Key Size
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		static Vector2 WhiteKeySize()
		{
			return Vector2( ((float)Screen::GetWidth() / KeyboardInfo::KeysInfo::TotalWhiteKeys), ((float)Screen::GetHeight() * 0.2f) );
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//	* New Method: Get Black Key Size
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		static Vector2 BlackKeySize()
		{
			return Vector2( WhiteKeySize().x * 0.3f, ((float)Screen::GetHeight() * 0.14f) );
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//	* New Method: Get White Key Colour
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		static Vector4 GetWhiteKeyColour()
		{
			return Vector4( 1.0f, 1.0f, 1.0f, 1.0f );
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//	* New Method: Get Black Key Colour
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		static Vector4 GetBlackKeyColour()
		{
			return Vector4( 0.0f, 0.0f, 0.0f, 1.0f );
		}
	};
}

#endif