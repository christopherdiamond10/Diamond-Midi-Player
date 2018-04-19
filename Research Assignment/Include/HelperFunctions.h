//\====================================================================================
//\ Author: Christopher.Allport
//\ About : HelperFunctions.h - 
//\ 
//\ Just a whole bunch of functions that 
//\====================================================================================
#ifndef _HELPERFUNCTIONS_H_
#define _HELPERFUNCTIONS_H_


#include "MathHelper.h"
#include "DeltaTime.h"
#include <ctime>
#include <iostream>
#include <list>
#include <locale>
#include <regex>
#include <sstream>
#include <string>
#include <vector>


// Disable Warning On Using Deprecated Functions
#pragma warning( disable : 4996 )

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				NameSpace: HelperFunctions
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
namespace HelperFunctions
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Get Specific Element in List
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	template<class T>
	static T* GetSpecificElementInList(std::list<T>& List, unsigned int Element)
	{
		std::list<T>::iterator iter = GetListIteratorStartAtSpecificElement( List, Element );
		return (iter != List.end()) ? &(*iter) : NULL; 
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Start List Iterator At Specific Element
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	template<class T>
	static typename std::list<T>::iterator GetListIteratorStartAtSpecificElement(std::list<T>& List, unsigned int Element)
	{
		if (List.size() > Element)
		{
			std::list<T>::iterator iter = List.begin();
			std::advance(iter, Element);
			return iter;
		}
		else
		{
			return List.end();
		}
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Does List Include Item?
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	template<class T>
	static bool DoesListInclude(std::list<T>& a_List, T Variable)
	{
		for(std::list<T>::iterator iter = a_List.begin(); iter != a_List.end(); ++iter )
			if( *iter == Variable)
				return true;

		return false;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Get Specific Element in Vector
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	template<class T>
	static T* GetSpecificElementInVector(std::vector<T>& a_Vector, unsigned int Element)
	{
		return (Element < a_Vector.size()) ? &a_Vector[Element] : NULL; 
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Start Vector Iterator At Specific Element
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	template<class T>
	static typename std::vector<T>::iterator GetVectorIteratorStartAtSpecificElement(std::vector<T>& a_Vector, unsigned int Element)
	{
		if (a_Vector.size() > Element)
		{
			std::vector<T>::iterator iter = a_Vector.begin();
			std::advance(iter, Element);
			return iter;
		}
		else
		{
			return a_Vector.end();
		}
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Does Vector Include Item?
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	template<class T>
	static bool DoesVectorInclude(std::vector<T>& a_Vector, T Variable)
	{
		for(std::vector<T>::iterator iter = a_Vector.begin(); iter != a_Vector.end(); ++iter )
			if( *iter == Variable)
				return true;

		return false;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Clone Vector Array
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	template<class T>
	static void CloneVectorArray(std::vector<T>& Source, std::vector<T>& Destination)
	{
		std::copy( Source.begin(), Source.end(), std::back_inserter(Destination));
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Copy List Into Vector Array
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	template<class T>
	static void CopyListIntoVectorArray(std::list<T>& Source, std::vector<T>& Destination)
	{
		Destination.resize( Source.size() );
		unsigned int uiCurrentIndex = 0;
		for( std::list<T>::iterator iter = Source.begin(); iter != Source.end(); ++iter )
		{
			Destination[ uiCurrentIndex ] = (*iter);
			uiCurrentIndex += 1;
		}
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: UpCase an Entire String
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static std::string String_To_UpperCase( const std::string& cString )
	{
		std::locale loc;
		std::string sReturnString;

		for( unsigned int i = 0; i < cString.length(); ++i)
		{
			sReturnString += std::toupper( (unsigned char)cString[i], loc );
		}

		return sReturnString;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: LowerCase an Entire String
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static std::string String_To_LowerCase( const std::string& cString )
	{
		std::locale loc;
		std::string sReturnString;

		for( unsigned int i = 0; i < cString.length(); ++i)
		{
			sReturnString += std::tolower( (unsigned char)cString[i], loc );
		}

		return sReturnString;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Remove Characters in a String
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static std::string RemoveStringCharacters( const std::string& OriginalText, const std::string& CharactersToRemove, bool CheckBothUpperAndLowerCase = true )
	{
		std::list<unsigned int> lSkipElements;
		std::string OriginalTextUpsized			= CheckBothUpperAndLowerCase ? String_To_UpperCase(OriginalText)		: OriginalText;
		std::string CharactersToRemoveUpsized	= CheckBothUpperAndLowerCase ? String_To_UpperCase(CharactersToRemove)  : CharactersToRemove;

		for(unsigned int i = 0; i < OriginalTextUpsized.length(); ++i )
		{
			if(OriginalTextUpsized[i] == CharactersToRemoveUpsized[0])
			{
				for(unsigned int j = 0; j < CharactersToRemoveUpsized.length(); ++j)
				{
					if(OriginalTextUpsized.length() > (i + j))
					{
						if(OriginalTextUpsized[(i + j)] != CharactersToRemoveUpsized[j])
							break;

						if(j == (CharactersToRemoveUpsized.length() - 1))
							for( unsigned int k = 0; k < CharactersToRemoveUpsized.length(); ++k)
								lSkipElements.push_back((i + k));
					}
					else
					{
						break;
					}
				}
			}
		}
		std::string ReturnString;
		for(unsigned int i = 0; i < OriginalText.length(); ++i )
		{
			if(DoesListInclude(lSkipElements, i))
				continue;

			ReturnString += OriginalText[i];
		}

		return ReturnString;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Replace Characters in a String
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static std::string ReplaceStringCharacters( const std::string& OriginalText, const std::string& CharactersToReplace, const std::string& ReplacingCharacters, bool CheckBothUpperAndLowerCase = true, bool RedoCheckOnEndResult = false )
	{
		std::string ReturnString = RemoveStringCharacters(OriginalText, CharactersToReplace, CheckBothUpperAndLowerCase);

		unsigned int uiOriginalTextIndex = 0;
		for(unsigned int i = 0; i < OriginalText.length(); ++i, ++uiOriginalTextIndex )
		{
			// If Current Char is the same as Original Char, continue.
			// Otherwise it is possible that the text we want to replace was removed at the end of the string, making it smaller than the original, in this case we will simply add the replacing text at the end. 
			if( (ReturnString.length() > i) && (ReturnString[i] == OriginalText[uiOriginalTextIndex]) )
				continue;

			ReturnString.insert((i), &ReplacingCharacters[0]);
			i += ReplacingCharacters.length();
		}

		// If Redoing Check on End Result, we are basically going to scan the ReturnString to See if the Replaced Characters Also Caused the Text to contain 
		// another piece of text that we want to replace. Pretty Much cause a recursive function until No Text is Replaced.
		if( RedoCheckOnEndResult && (ReturnString != OriginalText) )
			ReturnString = ReplaceStringCharacters(ReturnString, CharactersToReplace, ReplacingCharacters, CheckBothUpperAndLowerCase, true);

		return ReturnString;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: WordWrap Text
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static std::string GetWordWrapping(std::string Text, unsigned int StartLineSpaceCount = 0, bool SpaceOutStartLine = false, unsigned int MaxCharactersPerLine = 80)
	{
		unsigned int iCurrentTextPosition = 0;

		// Space Out Text Begin
		if( SpaceOutStartLine )
		{
			for( unsigned int i = 0; i < StartLineSpaceCount; ++i )
			{
				Text.insert(0, " ");
			}
		}
    
		// Go Through the Text, One Char at a Time, keeping track of where we are.
		for( unsigned int i = 0; i < Text.length(); ++i )
		{
			++iCurrentTextPosition;
		
			// If Exceeding Text Limit on a Line
			if( iCurrentTextPosition > MaxCharactersPerLine )
			{
				// Find Most Recent WhiteSpace
				int iCurrentElement = i;
				while( (Text[iCurrentElement] != ' ') && (iCurrentElement != 0) )
				{
					--iCurrentElement;
				}

				// If The Current Char Element We're Testing Is the Start Element, then this word is simply too long to fix, gonna have to leave it as is.
				if( iCurrentElement == 0 )
				{
					for( unsigned int j = 0; j < StartLineSpaceCount; ++j )
					{
						Text.insert(i, " ");
					}
					Text.insert(i, "\n"); // Insert New Line
					iCurrentTextPosition = 0;
				}
				// Otherwise Push the Conflicting Word onto a New Line
				else
				{
					for( unsigned int j = 0; j < StartLineSpaceCount; ++j )
					{
						Text.insert(iCurrentElement, " ");
					}
					Text.insert(iCurrentElement, "\n"); // Insert New Line
					iCurrentTextPosition = 0;
				}
			}
		}

		return Text;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Get Filename Extension
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static std::string GetFilenameExtension( const std::string& Filename )
	{
		std::string ReturnString = "";

		unsigned int PopAmount = Filename.length();
		for(auto RIter = Filename.rbegin(); RIter != Filename.rend(); ++RIter )
		{
			--PopAmount;
			
			if(*RIter == '.')
				break;
		}

		if( Filename.length() > PopAmount)
		{
			for( unsigned int i = PopAmount; i < Filename.length(); ++i )
			{
				ReturnString += Filename[i];
			}
		}
		return ReturnString;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Is Character Valid?
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static bool IsCharacterValid( char c )
	{
		return( c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f' || c == 'g' || c == 'h' || c == 'i' ||
				c == 'j' || c == 'k' || c == 'l' || c == 'm' || c == 'n' || c == 'o' || c == 'p' || c == 'q' || c == 'r' || 
				c == 's' || c == 't' || c == 'u' || c == 'v' || c == 'w' || c == 'x' || c == 'y' || c == 'z' || c == '1' || 
				c == '2' ||	c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' || c == '0' || 
				c == '`' ||	c == '-' || c == '=' || c == ';' || c == ',' || c == '.' || c == '/' || c == 91  || c == ' ' ||	
				c == 93  || c == 39 );	// 92 == '\', 91 == '[', 93 == ']', 39 == '
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Convert Degrees To Radians
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static float DegreesToRadians( float deg )
	{
		return (0.01745329251994329577f * deg);	//	(PI / 180)  * Degrees
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Convert Radians To Degrees
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static float RadiansToDegress( float rad )
	{
		return (float)tan(rad * 3.14159265359f / 180.0f);
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Move Vector4 Towards Another Vector4
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static void MoveVector4TowardsTargetVector4( Vector4& Vector4ToMove, const Vector4& TargetPosition, float SpeedToMoveWith )
	{
		Vector4ToMove += ( TargetPosition - Vector4ToMove  ).Normalise() * SpeedToMoveWith * DeltaTime::GetInstance()->GetDeltaTime();
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Move Position Towards Target Position (Individual X, Y, Z)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static void MovePositionTowardsTargetPosition( float& CurrentPosition, float TargetPosition, float SpeedToMoveWith )
	{
		CurrentPosition += (TargetPosition - CurrentPosition) * SpeedToMoveWith * DeltaTime::GetInstance()->GetDeltaTime();
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Is Position Close Enough to Target Position?
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static bool IsVector4PositionCloseEnoughToTargetVector4( const Vector4& CurrentPosition, const Vector4& TargetPosition, float MovementSpeed )
	{
		return ( (TargetPosition - CurrentPosition).Magnitude() < (MovementSpeed * DeltaTime::GetInstance()->GetDeltaTime()));
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Check Sphere Collision
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static bool CheckSphereCollision( const Vector4& Object1Position, float Object1Radius, const Vector4& Object2Position, float Object2Radius )
	{
		float sqrRadius = (Object1Radius + Object2Radius) * (Object1Radius + Object2Radius);
		float distSqr	= ( Object1Position - Object2Position ).MagnitudeSqr();
		return (distSqr <= sqrRadius);
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Convert Item To String
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	template< class T >
	static std::string ConvertItemToString( const T& ItemToConvert )
	{
		std::stringstream ReturnString (std::stringstream::in | std::stringstream::out);
		ReturnString << ItemToConvert;
		return ReturnString.str();
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Get Valid Int Value From User Console Input
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static int GetValidIntValueFromUserConsoleInput( int Min = INT_MIN, int Max = INT_MAX )
	{
		int Val = 0;

		do
		{
			// Get User Input
			//////////////////////////////////////////////////////////////
			do
			{
				std::cout << "Insert Integer Value:     ";

				if( std::cin >> Val )
					break;
				else
				{
					std::cout << "Invalid Input, please try again\n\n\n";
					std::cin.clear();
					while (std::cin.get() != '\n') ;
				}
			} while( true );
			//////////////////////////////////////////////////////////////

			// Check to Make Sure User Input Is Valid
			if( Val < Min || Val > Max )
				std::cout << "ERROR: You Did Not Input A Valid Value, Please Try Again\n\n";
			else
				break;

		}while( true );


		std::cin.get();
		return Val;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Get User Console Input
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static std::string GetUserConsoleInput(unsigned int MinCharacters = 0, unsigned int MaxCharacters = UINT_MAX, unsigned int NewlineCount = 2)
	{
		std::string Input;
		while( true )
		{
			Input = "";
			std::getline(std::cin, Input);

			if( (Input.length() >= MinCharacters) && (Input.length() <= MaxCharacters) )
				break;

			if( Input.length() < MinCharacters )
			{
				std::cout << "ERROR: You have Entered Too Few Characters. You must enter at least " << MinCharacters << " characters.\n\n";
			}
			else
			{
				std::cout << "ERROR: You have Entered Too Many Characters. You may only enter up to " << MaxCharacters << " characters at a time.\n\n";
			}
			std::cout << "Please Try Again:  ";
		}

		if( NewlineCount > 0 )
			std::cout << std::string(NewlineCount, '\n');

		return Input;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Get Current Time As String
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static std::string GetCurrentTimeAsString( bool TwentyFourHourClock = false )
	{
		time_t timeObj = time(&timeObj);
		tm* pTime = localtime(&timeObj);

		// Get Current Hour. If Not Using 24 Hour Clock, Minus 12 Hours From 24 Hour Cycle if necessary.
		int iHour = (!TwentyFourHourClock && pTime->tm_hour > 12) ? (pTime->tm_hour - 12) : (pTime->tm_hour);

		std::string Hour		= ConvertItemToString( iHour );
		std::string Min			= ConvertItemToString( pTime->tm_min );
		std::string Sec			= ConvertItemToString( pTime->tm_sec );
		std::string Meridiem	= (TwentyFourHourClock) ? "" : (pTime->tm_hour > 11) ? "PM" : "AM";
	
		// Putting Zeros in front of Single Digit Numbers
		if( Hour.length() < 2 ) { Hour	= "0" + Hour;	}
		if( Min.length() < 2 )	{ Min	= "0" + Min;	}
		if( Sec.length() < 2 )	{ Sec	= "0" + Sec;	}


		return ( Hour + ":" + Min + ":" + Sec + Meridiem );
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Get Current Time As UInt
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static unsigned int GetCurrentTimeAsUInt()
	{
		return ( (unsigned int)time(0) );
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Seed Random With Time
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static void SeedRandomWithTime()
	{
		static unsigned int uiPreviousTime	= 0;
		unsigned int uiCurrentTime			= GetCurrentTimeAsUInt();

		if( uiPreviousTime != uiCurrentTime )
		{
			uiPreviousTime = uiCurrentTime;
			srand( uiCurrentTime );
		}
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Clear Console Window
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static void ClearConsoleWindow()
	{
		system( "CLS" );
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Erase Characters in Console Window
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static void EraseConsoleCharacters( unsigned int BackspaceAmount = 1 )
	{
		const char* Command = "\b \b";
		std::cout << std::string( BackspaceAmount, *Command );
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Methods: Print Console Message
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static void PrintConsoleMessage( const std::string& Message, unsigned int NewLineCount = 1 )
	{
		std::cout << Message << ((NewLineCount > 0) ? std::string(NewLineCount, '\n') : "");
	}

	static void PrintConsoleMessage( const char* Message, unsigned int NewLineCount = 1 )
	{
		std::cout << Message << ((NewLineCount > 0) ? std::string(NewLineCount, '\n') : "");
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Compare C String For Equality
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static bool CompareCStringsForEquality(const char* Var1, const char* Var2)
	{
		return strcmp(Var1, Var2) == 0;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Get Random Colour
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static Vector4 GetRandomColour()
	{
		SeedRandomWithTime();
		return Vector4( (float)(rand() % 256) / 255.0f, (float)(rand() % 256) / 255.0f, (float)(rand() % 256) / 255.0f, 1.0f );
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Matched String With Regular Expression?
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static bool MatchedStringWithRegex( const std::string& TestString, const std::regex& RegExpr, std::regex_constants::match_flag_type Flag = std::regex_constants::match_default )
	{
		std::cmatch Match;
		return std::regex_search( TestString.c_str(), Match, RegExpr, Flag );
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Test String For Valid Server IP Address
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static bool TestStringForValidServerIPAddress( const std::string& TestString )
	{
		return MatchedStringWithRegex( TestString, std::regex("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b") );
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	* New Method: Get Server Address from User Input
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static std::string GetServerAddressFromUserInput(unsigned int NewlineCount = 2)
	{
		std::string Input;
		
		while( true )
		{
			Input = GetUserConsoleInput(0, UINT_MAX, NewlineCount);

			if( TestStringForValidServerIPAddress(Input) )
			{
				return Input;
			}

			std::cout << "Error: You have not Inserted a Valid Server IP Address, please try again" << "\n\n";
		}
	}
}


#endif