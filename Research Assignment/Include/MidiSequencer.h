//#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//#					Midi Sequencer
//#					Author: Christopher Diamond
//#					Date: September 21, 2013
//#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#	  Description:
//#		
//#		This class handles the Input Sequence from a Midi File itself.
//#		Thus allowing us to gain information on MidiEvents from each channel.
//#
//#			* So Far the Class is incomplete.
//#
//#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#ifndef _MIDISEQUENCER_H_
#define _MIDISEQUENCER_H_





#include "jdksmidi/world.h"
#include "jdksmidi/filereadmultitrack.h"
#include "jdksmidi/manager.h"
#include "jdksmidi/driverdump.h"
#include "jdksmidi/driver.h"

#include "HelperClasses.h"
#include "KeyboardInformation.h"
#include "MathHelper.h"
#include "RenderQuad.h"
#include <string>
#include <list>
#include <vector>



//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Class: Midi Sequencer
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class MidiSequencer
{
////////////////////////////////////////////////////////////
public:
////////////////////////////////////////////////////////////

	//===============================================
	//			Public Declarations
	//===============================================
	enum NoteStatus
	{
		HIT,
		MISSED,
		REACHED,
		NOT_REACHED,
	};

	enum ChannelState
	{
		ACTIVE,
		INACTIVE,
		NOT_AVAILABLE,
	};
	//===============================================
	//			Constructors & Destructor
	//===============================================
	MidiSequencer();
	~MidiSequencer();
	static MidiSequencer* GetInstance();

	//===============================================
	//			Public Functions
	//===============================================
	void Reset();

	void Update();
	void Draw();

	void ReadMidiFile(std::string& MidiFilename);
	void ReadMidiFile(const char* MidiFilename);
	void StoreAudioFile(unsigned int AudioID);
	void PlayAudioFile();

	void CreateBucketSystem();
	bool SongCompleted();

	Vector4			GetChannelColour(unsigned int ChannelID);
	ChannelState	GetChannelActivity(unsigned int ChannelID);
	std::string		GetChannelName(unsigned int ChannelID);
	void			SetChannelActivity(unsigned int ChannelID, ChannelState Activity);


////////////////////////////////////////////////////////////
private:
////////////////////////////////////////////////////////////
	
	//===============================================
	//			Private Declarations
	//===============================================
	struct MidiEvent;
	struct MidiEventHolder;
	struct MidiChannelEventsHolder;
	struct MidiNote;
	struct MidiNoteBuckets;
	struct MidiChannel;
	struct MidiChannelsHolder;

	friend struct MidiEvent;
	friend struct MidiEventHolder;
	friend struct MidiNote;
	friend struct MidiNoteBuckets;
	friend struct MidiChannel;
	friend struct MidiChannelEventsHolder;
	friend struct MidiChannelsHolder;
	
	typedef std::list	<	MidiEvent*				>		MidiEventList;
	typedef std::list	<	MidiNote*				>		MidiNoteList;
	typedef std::vector	<	MidiNote*				>		MidiNoteVector;
	typedef std::list	<	MidiChannel*			>		MidiChannelList;
	typedef std::vector	<	MidiChannel*			>		MidiChannelVector;
	//===============================================
	//			Private Instance Variables
	//===============================================
	float			m_fTotalMidiDuration;
	unsigned int	m_uiAudioID;
	//===============================================
	//			Private Functions
	//===============================================
	void			DestroyPointers();
	void			SetupMidiEvents(jdksmidi::MIDIMultiTrack* MultiTrack);

	MidiChannel*	GetChannel(unsigned int Channel); 
	//===============================================
	//			Pointers
	//===============================================
	MidiChannelsHolder*		m_pMidiChannelsHolder;
	MidiNoteBuckets*		m_pMidiNoteBuckets;

	static MidiSequencer* m_Instance;

};





//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Struct: Midi Event
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
struct MidiSequencer::MidiEvent
{
	unsigned int							uiNoteID;				// ID of Note
	bool									bNoteOn;				// Start or End of Note
	float									fActivationTime;		// When To Activate? (RealTime) 


	MidiEvent();
	~MidiEvent();
};
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//			  Struct: Midi Event Holder
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
struct MidiSequencer::MidiEventHolder
{
	MidiSequencer::MidiEventList			lNoteOnEvents;	// Midi Events For When Note is Activated
	MidiSequencer::MidiEventList			lNoteOffEvents;	// Midi Events For When Note is Deactivated


	MidiEventHolder();
	~MidiEventHolder();
};
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//			Struct: Midi Channel Events Holder
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
struct MidiSequencer::MidiChannelEventsHolder
{
	MidiSequencer::MidiEventHolder			aEvents[KeyboardInfo::KeysInfo::KeysArraySize];


	MidiChannelEventsHolder();
	~MidiChannelEventsHolder();

	MidiSequencer::MidiEventHolder*			operator [] (unsigned int index);
};
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Struct: Midi Note
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
struct MidiSequencer::MidiNote
{
	static const Vector4					v4HitColour;		// Colour to change to if Hit
	static const Vector4					v4MissColour;		// Colour to change to if Missed
	const Vector4&							v4Colour;			// Channel Colour Representation

	unsigned int							uiNoteID;			// Keyboard Key ID
	int										iHitTextureID;		// ID of Texture to Show When Hit
	MidiSequencer::NoteStatus				eStatus;			// Status of Note. Has it been reached? Was it Hit?
	bool									bReachedKey;		// Has Reached the Key?

	MidiSequencer::MidiEvent*				pEventStart;		// Note On Event
	MidiSequencer::MidiEvent*				pEventEnd;			// Note Off Event
	RenderQuad*								pRenderQuad;		// Draw Quad
	HelperClasses::MovementBasedOnTime*		pMBOTDrop;			// Movement Class, Dropping into Key
	HelperClasses::MovementBasedOnTime*		pMBOTLeave;			// Movement Class, Leaving Screen



	MidiNote(const Vector4& Colour, unsigned int NoteID, MidiSequencer::MidiEvent* EventStart, MidiSequencer::MidiEvent* EventEnd);
	~MidiNote();
	void Update();
	void Draw();
	void Reset();
	void Activate();
	bool WithinRange();
	bool CanBeHit();
	bool IsBeingHit();
};
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Struct: Midi Note Buckets
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
struct MidiSequencer::MidiNoteBuckets
{
	std::list<MidiSequencer::MidiNoteList*>* plAvailableMidiNotes;
	float fCurrentTime;			// Seconds
	float fBucketLength;		// Seconds
	float fTotalMidiDuration;	// Seconds

	MidiNoteBuckets();
	~MidiNoteBuckets();

	void CreateBuckets(MidiSequencer::MidiChannelsHolder* pChannels, float TotalMidiDuration);
	void DestroyBuckets();
	void Reset();
	void Update();
	void Draw();

	
	bool ValidElement(std::list<MidiSequencer::MidiNoteList*>::iterator& iter);
	bool Finished();
	void RemoveFrontElement();
	void RemoveBackElement();

	void ImportNotes(MidiSequencer::MidiChannelsHolder* pChannels, float TotalMidiDuration);
	void RemoveEmptyEndBuckets();
	void CreateMidiDuration(MidiSequencer::MidiChannelsHolder* pChannels);
	void ResetAllNotes();
};
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Struct: Midi Channel
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
struct MidiSequencer::MidiChannel
{
	Vector4									v4ChannelColour;
	MidiSequencer::MidiNoteVector			aMidiNotesOnChannel;
	MidiSequencer::MidiChannelEventsHolder  MidiEvents;
	MidiSequencer::MidiNoteList				lTimeSortedMidiNotes;

	std::string								sChannelName;
	MidiSequencer::ChannelState				eChannelState;

	MidiChannel(Vector4& vChannelColour);
	~MidiChannel();

	void	AddMidiEvent( MidiSequencer::MidiEvent* Event );
	void	SortMidiEvents();
	void	GetNotesWithinRange(MidiSequencer::MidiNoteList* plNoteList, float StartRange, float EndRange);
	float	GetFinalEventActivationTime();
};
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Struct: Midi Channel Holder
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
struct MidiSequencer::MidiChannelsHolder
{
	const unsigned int						uiMaxChannels;
	MidiSequencer::MidiChannel*				pChannelOne;
	MidiSequencer::MidiChannel*				pChannelTwo;
	MidiSequencer::MidiChannel*				pChannelThree;
	MidiSequencer::MidiChannel*				pChannelFour;
	MidiSequencer::MidiChannel*				pChannelFive;
	MidiSequencer::MidiChannel*				pChannelSix;
	MidiSequencer::MidiChannel*				pChannelSeven;
	MidiSequencer::MidiChannel*				pChannelEight;
	MidiSequencer::MidiChannel*				pChannelNine;
	MidiSequencer::MidiChannel*				pChannelTen;
	MidiSequencer::MidiChannel*				pChannelEleven;
	MidiSequencer::MidiChannel*				pChannelTwelve;
	MidiSequencer::MidiChannel*				pChannelThirteen;
	MidiSequencer::MidiChannel*				pChannelFourteen;
	MidiSequencer::MidiChannel*				pChannelFiveteen;
	MidiSequencer::MidiChannel*				pChannelSixteen;

	MidiChannelsHolder();
	~MidiChannelsHolder();

	MidiSequencer::MidiChannelList			GetActiveChannels();
	MidiSequencer::MidiChannel*				operator [] (unsigned int index);
};

#endif