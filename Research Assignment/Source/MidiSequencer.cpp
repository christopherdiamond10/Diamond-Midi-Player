#include "MidiSequencer.h"

#include "AudioManager.h"
#include "EditableOptions.h"
#include "FontManager.h"
#include "HelperFunctions.h"
#include "InputHandler.h"
#include "Screen.h"
#include "TextureLoader.h"

#include <math.h>
#include <assert.h>
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Class: MidiSequencer
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
MidiSequencer* MidiSequencer::m_Instance = NULL;
MidiSequencer* MidiSequencer::GetInstance()
{
	return m_Instance;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MidiSequencer::MidiSequencer()
{
	m_pMidiChannelsHolder	= NULL;
	m_pMidiNoteBuckets		= NULL;
	m_fTotalMidiDuration	= 0.0f;
	m_uiAudioID				= 0;

	if( m_Instance == NULL )
		m_Instance = this;
	else
		assert(0);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Deconstructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MidiSequencer::~MidiSequencer()
{
	DestroyPointers();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Destroy Pointers
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::DestroyPointers()
{
	if( m_pMidiChannelsHolder != NULL )
	{
		delete m_pMidiChannelsHolder;
		m_pMidiChannelsHolder = NULL;
	}

	if( m_pMidiNoteBuckets != NULL )
	{
		delete m_pMidiNoteBuckets;
		m_pMidiNoteBuckets = NULL;
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Reset Self
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::Reset()
{
	DestroyPointers();

	m_pMidiChannelsHolder	= new MidiChannelsHolder();
	m_pMidiNoteBuckets		= new MidiNoteBuckets();
	m_fTotalMidiDuration	= 0.0f;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Update
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::Update()
{
	m_pMidiNoteBuckets->Update();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Draw
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::Draw()
{
	m_pMidiNoteBuckets->Draw();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Setup Sequencer
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::ReadMidiFile(std::string& a_sMidiFilename)
{
	ReadMidiFile( a_sMidiFilename.c_str() );
}

void MidiSequencer::ReadMidiFile(const char* a_sMidiFilename)
{
	Reset();

	jdksmidi::MIDIFileReadStreamFile	rs			 ( a_sMidiFilename );
    jdksmidi::MIDIMultiTrack			tracks		 ( 64 );
    jdksmidi::MIDIFileReadMultiTrack	track_loader ( &tracks );
    jdksmidi::MIDIFileRead				reader		 ( &rs, &track_loader );
    reader.Parse();

	SetupMidiEvents(&tracks);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Setup Midi Events
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::SetupMidiEvents(jdksmidi::MIDIMultiTrack* a_pMultiTrack)
{
	// Create JDKsMidi Sequencer Which will read through the tracks
	jdksmidi::MIDISequencer seq( a_pMultiTrack );
    seq.GoToZero();

	// Get Total Midi Time
	m_fTotalMidiDuration = (float)seq.GetMisicDurationInSeconds();
	seq.GoToZero();
	
	// In the next Function, these will both be changed
	int iTrack = 0;
	jdksmidi::MIDITimedBigMessage MidiEventMessage;
	

	// Get All Events
	while ( seq.GetNextEvent( &iTrack, &MidiEventMessage ) )
	{
		if ( MidiEventMessage.IsBeatMarker() ) 
		{
			continue;
		}

		// If Note Message (We don't really care about any other messages)
		if ( (MidiEventMessage.status & 0xf0) == jdksmidi::NOTE_ON || (MidiEventMessage.status & 0xf0) == jdksmidi::NOTE_OFF)
		{ 
			unsigned int uiNote			= (unsigned int)MidiEventMessage.byte1;
			unsigned int uiChannel		= MidiEventMessage.GetChannel();


			// Midi Files can allow Notes that Go Higher than the Standard Key Set on a Keyboard, so make sure this event can be used
			if( (uiNote >= KeyboardInfo::KeysInfo::BeginningNoteID) && (uiNote < KeyboardInfo::KeysInfo::KeysArraySize) )
			{
				MidiEvent* pMEvent			= new MidiEvent();
				pMEvent->uiNoteID			= uiNote;																							// Get Note ID
				pMEvent->bNoteOn			= !(((MidiEventMessage.status & 0xf0) == jdksmidi::NOTE_OFF) || (MidiEventMessage.IsNoteOnV0()));	// If Velocity of Zero, That Means this note just turned off, else any other velocity means the note is on.
				pMEvent->fActivationTime	= (float)(seq.GetCurrentTimeInMs() * 0.001) + EditableOptions::fSongBeginDelay();					// Convert Milliseconds to Seconds.

				// Create Channel Information
				if(GetChannel(uiChannel)->eChannelState != MidiSequencer::ACTIVE)
				{
					GetChannel(uiChannel)->sChannelName = seq.GetTrackState(iTrack)->track_name;
					GetChannel(uiChannel)->eChannelState = MidiSequencer::ACTIVE;
				}

				GetChannel(uiChannel)->AddMidiEvent( pMEvent );
			}
		}
	}

	// Sort Midi Events into Notes
	for( unsigned int i = 0; i < m_pMidiChannelsHolder->uiMaxChannels; ++i )
	{
		GetChannel(i)->SortMidiEvents();
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Store Audio File
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::StoreAudioFile(unsigned int a_uiAudioID)
{
	m_uiAudioID = a_uiAudioID;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Play Audio File
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::PlayAudioFile()
{
	AudioManager::GetInstance()->PlayAudio(m_uiAudioID);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Channel
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MidiSequencer::MidiChannel*	MidiSequencer::GetChannel(unsigned int a_uiChannel)
{
	return (*m_pMidiChannelsHolder)[a_uiChannel];
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Create Bucket System
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::CreateBucketSystem()
{
	m_pMidiNoteBuckets->CreateBuckets(m_pMidiChannelsHolder, m_fTotalMidiDuration);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Song Completed?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool MidiSequencer::SongCompleted()
{
	return m_pMidiNoteBuckets->Finished();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Channel Colour
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Vector4 MidiSequencer::GetChannelColour(unsigned int ChannelID)
{
	return GetChannel(ChannelID)->v4ChannelColour;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Channel Activity
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MidiSequencer::ChannelState MidiSequencer::GetChannelActivity(unsigned int ChannelID)
{
	return GetChannel(ChannelID)->eChannelState;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Channel Name
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::string MidiSequencer::GetChannelName(unsigned int ChannelID)
{
	return GetChannel(ChannelID)->sChannelName;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Channel Activity
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::SetChannelActivity(unsigned int ChannelID, ChannelState eActivity)
{
	GetChannel(ChannelID)->eChannelState = eActivity;
}








//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Struct: MidiEvent
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MidiSequencer::MidiEvent::MidiEvent()
	: uiNoteID(0)
	, bNoteOn(true)
	, fActivationTime(0.0f)
{

}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Deconstructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MidiSequencer::MidiEvent::~MidiEvent()
{

}





//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Struct: MidiEventHolder
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MidiSequencer::MidiEventHolder::MidiEventHolder()
	: lNoteOnEvents()
	, lNoteOffEvents()
{

}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Deconstructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MidiSequencer::MidiEventHolder::~MidiEventHolder()
{
	for( MidiSequencer::MidiEventList::iterator iter = lNoteOnEvents.begin(); iter != lNoteOnEvents.end(); ++iter )
	{
		delete (*iter);
	}

	for( MidiSequencer::MidiEventList::iterator iter = lNoteOffEvents.begin(); iter != lNoteOffEvents.end(); ++iter )
	{
		delete (*iter);
	}
}




//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Struct: MidiChannelEventsHolder
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MidiSequencer::MidiChannelEventsHolder::MidiChannelEventsHolder()
{

}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	*~ Deconstructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MidiSequencer::MidiChannelEventsHolder::~MidiChannelEventsHolder()
{

}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* Operator Overload: []
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MidiSequencer::MidiEventHolder* MidiSequencer::MidiChannelEventsHolder::operator [] (unsigned int index)
{
	return &aEvents[index];
}






//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Struct: MidiNote
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
const Vector4 MidiSequencer::MidiNote::v4HitColour	= Vector4( 100.0f / 255.0f, 100.0f / 255.0f, 200.0f / 255.0f, 1.0f );
const Vector4 MidiSequencer::MidiNote::v4MissColour = Vector4( 10.0f  / 255.0f, 10.0f  / 255.0f, 10.0f  / 255.0f, 1.0f );
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MidiSequencer::MidiNote::MidiNote(const Vector4& a_rvColour, unsigned int a_uiNoteID, MidiSequencer::MidiEvent* a_pEventStart, MidiSequencer::MidiEvent* a_pEventEnd)
	: v4Colour		(a_rvColour)
	, uiNoteID		(a_uiNoteID)
	, bReachedKey	(false)
	, pEventStart	(a_pEventStart)
	, pEventEnd		(a_pEventEnd)
	, eStatus		(MidiSequencer::NOT_REACHED)
{
	iHitTextureID = TextureLoader::LoadTexture("HitSprite.png");

	TransformDetails2D Transform;
	Transform.Size.x = KeyboardInfo::KeyboardDrawInfo::GetNoteSize(a_uiNoteID).x * 0.8f;																					// Shorten the Horizontal Size so that it better fits into the Key as it drops.
	Transform.Size.y = ceil((((a_pEventEnd->fActivationTime - a_pEventStart->fActivationTime) / EditableOptions::fSeeAheadLength()) * Screen::GetNotesDrawArea().height));	// Create Pixel Length Vertical Size. Using the Start and End Time as Length
	
	Transform.Position = KeyboardInfo::KeyboardDrawInfo::GetNotePosition(a_uiNoteID);																						// Start Position Where the Drawn_Key Will Be
	Transform.Position.x += Transform.Size.x * 0.1f;																														// Offset Position a little, to centre the note to the key.
	Transform.Position.y  = -ceil(((a_pEventEnd->fActivationTime / EditableOptions::fSeeAheadLength()) * Screen::GetNotesDrawArea().height));								// Get Vertical Position. This needs to be negative since I'm drawing with y-Zero at the top of the screen rather than the bottom.

	Vector2 vEndPosition = Transform.Position;																																// Place The Endx Variable at the same pos where startx will be.
	vEndPosition.y = Screen::GetKeyboardDrawArea().y;																														// Endy Position is where the NoteKey begins to draw.

	pRenderQuad = new RenderQuad( Transform, a_rvColour, KeyboardInfo::KeyboardDrawInfo::GetNoteColour(a_uiNoteID) );														// Create a RenderQuad for the Note
	pMBOTDrop	= new HelperClasses::MovementBasedOnTime(Transform.Position, vEndPosition, a_pEventEnd->fActivationTime);													// Create a MovementBasedOnTime Handler for Dropping into Position
	pMBOTLeave	= new HelperClasses::MovementBasedOnTime(Vector2(Transform.Position.x, Screen::GetKeyboardDrawArea().y),													// Create a MovementBasedOnTime Handler for Leaving the Screen
														 Vector2(Transform.Position.x, (float)Screen::GetHeight()), 0.5f);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	*~ Deconstructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MidiSequencer::MidiNote::~MidiNote()
{
	delete pRenderQuad;
	delete pMBOTDrop;
	delete pMBOTLeave;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Update
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::MidiNote::Update()
{
	if( !bReachedKey )
	{
		// Update Draw Position
		pMBOTDrop->Update();
		pRenderQuad->SetPosition( pMBOTDrop->GetCurrentPosition() );

		// If can hit the Key
		if( CanBeHit() && WithinRange() )
		{
			eStatus = MidiSequencer::REACHED;
			if( InputHandler::GetInstance()->MidiKeyboardKeyTriggered(uiNoteID) || InputHandler::GetInstance()->KeyPressed( ' ' ))
			{
				eStatus = MidiSequencer::HIT;
				pRenderQuad->SetMainColour(v4HitColour);
				pRenderQuad->SetGradientColour(v4Colour);
			}
		}

		// If You Could Hit it and now you can't. You've missed it.
		else if( eStatus == MidiSequencer::REACHED )
		{
			eStatus = MidiSequencer::MISSED;
			pRenderQuad->SetMainColour(v4MissColour);
			pRenderQuad->SetGradientColour(v4Colour);
			AudioManager::GetInstance()->FadeinSound( AudioManager::BGM, 0.1f );
		}

		// Change Update State?
		bReachedKey = pMBOTDrop->HasReachedDestination();
	}

	else
	{
		pMBOTLeave->Update();
		pRenderQuad->SetPosition( pMBOTLeave->GetCurrentPosition() );
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Draw
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::MidiNote::Draw()
{
	pRenderQuad->Draw();
	FontManager::GetInstance()->Print( KeyboardInfo::KeysInfo::GetKeyNoteAsString(uiNoteID), pRenderQuad->GetEndPosition(), Vector2(10.0f, 10.0f), v4Colour);

	if( IsBeingHit() )
	{
		Vector4 Colour(1.0f, 1.0f, 1.0f,1.0f);
		Colour *= abs(sin(DeltaTime::GetInstance()->GetTrackTime() - pEventEnd->fActivationTime)); 
		Vector2 Size = KeyboardInfo::KeyboardDrawInfo::GetNoteSize(uiNoteID);
		Size.y = Screen::ScaleToScreenHeight(20.0f);
		Vector2 Position = KeyboardInfo::KeyboardDrawInfo::GetNotePosition(uiNoteID);
		Position.y -= Size.y;

		RenderQuad RQ(iHitTextureID, Position, Size, Colour, Colour);
		RQ.Draw();
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Reset
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::MidiNote::Reset()
{
	bReachedKey = false;
	eStatus = MidiSequencer::NOT_REACHED;

	pRenderQuad->SetMainColour(v4Colour);
	pRenderQuad->SetGradientColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	pMBOTLeave->Reset();
	pMBOTDrop->Reset();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Activate
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::MidiNote::Activate()
{
	pMBOTDrop->GetTimeInstance().SetCurrentTime( DeltaTime::GetInstance()->GetTrackTime() );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Is Within Range?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool MidiSequencer::MidiNote::WithinRange()
{
	static const float sfDetectionRange			= ((float)Screen::GetHeight() * (EditableOptions::fHitDetectionRange() / EditableOptions::fSeeAheadLength())) * 0.5f;
	static const float sfDetectionHitPosition	= Screen::GetKeyboardDrawArea().y;

	float fBeginRangeY	= pRenderQuad->GetEndPosition().y + sfDetectionRange;
	float fEndRangeY	= pRenderQuad->GetEndPosition().y - sfDetectionRange;

	return (fBeginRangeY > sfDetectionHitPosition && fEndRangeY < sfDetectionHitPosition);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Can Be Hit?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool MidiSequencer::MidiNote::CanBeHit()
{
	return (eStatus == MidiSequencer::REACHED) || (eStatus == MidiSequencer::NOT_REACHED);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Is Being Hit?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool MidiSequencer::MidiNote::IsBeingHit()
{
	return (eStatus == MidiSequencer::HIT) && (InputHandler::GetInstance()->MidiKeyboardKeyPressed(uiNoteID) || InputHandler::GetInstance()->KeyPressed(' '));
}





//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Struct: MidiNoteBuckets
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MidiSequencer::MidiNoteBuckets::MidiNoteBuckets()
{
	fBucketLength = (EditableOptions::fSeeAheadLength());
	plAvailableMidiNotes = new std::list<MidiSequencer::MidiNoteList*>();
	
	Reset();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	*~ Deconstructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MidiSequencer::MidiNoteBuckets::~MidiNoteBuckets()
{
	DestroyBuckets();
	delete plAvailableMidiNotes;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Create Buckets
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::MidiNoteBuckets::CreateBuckets(MidiSequencer::MidiChannelsHolder* a_pChannels, float a_fTotalMidiDuration)
{ 
	Reset();

	ImportNotes(a_pChannels, a_fTotalMidiDuration);		// Create Buckets
	RemoveEmptyEndBuckets();							// Remove Empty End Entries (Remember that we doubled the Timer Length because it's not 100% Accurate. This is to filter out uneeded buckets
	CreateMidiDuration(a_pChannels);					// Get Midi Duration
	ResetAllNotes();									// Reset All Notes in the Bucket, Just in case we exited the playgame scene and returned to play once more.
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Destroy Buckets
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::MidiNoteBuckets::DestroyBuckets()
{
	for( auto iter = plAvailableMidiNotes->begin(); iter != plAvailableMidiNotes->end(); ++iter )
	{
		delete (*iter);
	}
	plAvailableMidiNotes->clear();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Reset
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::MidiNoteBuckets::Reset()
{
	fCurrentTime = 0.0f;
	fTotalMidiDuration = 0.0f;
	DestroyBuckets();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Update
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::MidiNoteBuckets::Update()
{
	// Update Current Bucket
	fCurrentTime += DeltaTime::GetInstance()->GetDeltaTime();
	if( fCurrentTime > fBucketLength )
	{
		fCurrentTime -= fBucketLength;
		if( ValidElement(plAvailableMidiNotes->begin()) )
		{
			RemoveFrontElement();
		}
	}

	// Update Objects in Current Bucket
	auto itMaster = plAvailableMidiNotes->begin();
	if( ValidElement(itMaster) )
	{
		for( auto iter = (*itMaster)->begin(); iter != (*itMaster)->end(); ++iter )
		{
			(*iter)->Update();
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Draw
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::MidiNoteBuckets::Draw()
{
	auto itMaster = plAvailableMidiNotes->begin();
	if( ValidElement(itMaster) )
	{
		for( auto iter = (*itMaster)->begin(); iter != (*itMaster)->end(); ++iter )
		{
			(*iter)->Draw();
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Is Valid Element?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool MidiSequencer::MidiNoteBuckets::ValidElement(std::list<MidiSequencer::MidiNoteList*>::iterator& iter)
{
	return (iter != plAvailableMidiNotes->end());
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Has Finished?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool MidiSequencer::MidiNoteBuckets::Finished()
{
	return (plAvailableMidiNotes->begin() == plAvailableMidiNotes->end());
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Remove Front Element
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::MidiNoteBuckets::RemoveFrontElement()
{
	// Remove First Element
	auto itMaster = plAvailableMidiNotes->begin();
	if( ValidElement(itMaster) )
	{
		delete (*itMaster);
		plAvailableMidiNotes->pop_front();
	}

	// If Next Element Exists, Activate Midi Notes in that Segment
	itMaster = plAvailableMidiNotes->begin();
	if( ValidElement(itMaster) )
	{
		for( auto iter = (*itMaster)->begin(); iter != (*itMaster)->end(); ++iter )
		{
			(*iter)->Activate();
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Remove Back Element
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::MidiNoteBuckets::RemoveBackElement()
{
	delete (*plAvailableMidiNotes->rbegin());
	plAvailableMidiNotes->pop_back();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Import Notes
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::MidiNoteBuckets::ImportNotes(MidiSequencer::MidiChannelsHolder* a_pChannels, float a_fTotalMidiDuration)
{
	for( float f = 0.0f; f < (a_fTotalMidiDuration * 2.0f); f += fBucketLength )
	{
		MidiSequencer::MidiNoteList* plBucket = new MidiSequencer::MidiNoteList();

		for( unsigned int i = 0; i < a_pChannels->uiMaxChannels; ++i )
		{
			if( (*a_pChannels)[i]->eChannelState == MidiSequencer::ACTIVE )
			{
				(*a_pChannels)[i]->GetNotesWithinRange( plBucket, f, (f + fBucketLength * 2.0f) );
			}
		}

		plAvailableMidiNotes->push_back( plBucket );
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Remove Empty End Bucket Entries
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::MidiNoteBuckets::RemoveEmptyEndBuckets()
{
	unsigned int iCount = 0;
	for( auto iter = plAvailableMidiNotes->rbegin(); iter != plAvailableMidiNotes->rend(); ++iter )
	{
		if( !(*iter)->empty() )
			break;
		else
			iCount += 1;
	}

	for(unsigned int i = 0; i < iCount; ++i)
	{
		RemoveBackElement();
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Create Midi Duration
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::MidiNoteBuckets::CreateMidiDuration(MidiSequencer::MidiChannelsHolder* a_pChannels)
{
	for( unsigned int i = 0; i < a_pChannels->uiMaxChannels; ++i )
	{
		float fTime = (*a_pChannels)[i]->GetFinalEventActivationTime();
		if( fTime > fTotalMidiDuration )
			fTotalMidiDuration = fTime;
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Reset All Notes
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::MidiNoteBuckets::ResetAllNotes()
{
	for( auto Masteriter = plAvailableMidiNotes->begin(); Masteriter != plAvailableMidiNotes->end(); ++Masteriter )
	{
		for( auto iter = (*Masteriter)->begin(); iter != (*Masteriter)->end(); ++iter )
		{
			(*iter)->Reset();
		}
	}
}
	




//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Struct: MidiChannel
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MidiSequencer::MidiChannel::MidiChannel(Vector4& a_rvChannelColour)
	: aMidiNotesOnChannel()
	, eChannelState(MidiSequencer::NOT_AVAILABLE)
	, lTimeSortedMidiNotes()
	, MidiEvents()
	, sChannelName("")
	, v4ChannelColour(a_rvChannelColour)
{
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	*~ Deconstructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MidiSequencer::MidiChannel::~MidiChannel()
{
	for( auto iter = lTimeSortedMidiNotes.begin(); iter != lTimeSortedMidiNotes.end(); ++iter)
		delete (*iter);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Add Midi Event
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::MidiChannel::AddMidiEvent( MidiSequencer::MidiEvent* a_pEvent )
{
	if( a_pEvent->bNoteOn )
	{
		MidiEvents.aEvents[ a_pEvent->uiNoteID ].lNoteOnEvents.push_back( a_pEvent );
	}
	else
	{
		MidiEvents.aEvents[ a_pEvent->uiNoteID ].lNoteOffEvents.push_back( a_pEvent );
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Sort Midi Events
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::MidiChannel::SortMidiEvents()
{
	// Sort Events into Notes
	MidiSequencer::MidiNoteList lNoteList;

	for( unsigned int i = 0; i < KeyboardInfo::KeysInfo::KeysArraySize; ++i )
	{
		if( !MidiEvents.aEvents[i].lNoteOnEvents.empty() )
		{
			MidiSequencer::MidiEventList::iterator	itNoteOn  = MidiEvents.aEvents[i].lNoteOnEvents.begin();
			MidiSequencer::MidiEventList::iterator	itNoteOff = MidiEvents.aEvents[i].lNoteOffEvents.begin();

			while( itNoteOn != MidiEvents.aEvents[i].lNoteOnEvents.end() )
			{
				MidiSequencer::MidiNote* pMidiNote = new MidiSequencer::MidiNote( v4ChannelColour, i, *itNoteOn, *itNoteOff );
				lNoteList.push_back( pMidiNote );

				++itNoteOn;
				++itNoteOff;
			}
		}
	}

	HelperFunctions::CopyListIntoVectorArray( lNoteList, aMidiNotesOnChannel );


	// Sort Notes into Order They Appear
	while( true )
	{
		MidiSequencer::MidiNoteList::iterator itCurrentNote = lNoteList.begin();
		MidiSequencer::MidiNoteList::iterator itSortedIter	= lTimeSortedMidiNotes.begin();

		if( itCurrentNote == lNoteList.end() )
		{
			break;
		}

		while( true )
		{
			if( itSortedIter == lTimeSortedMidiNotes.end() )
			{
				lTimeSortedMidiNotes.push_back( *itCurrentNote );
				break;
			}

			if( (*itSortedIter)->pEventStart->fActivationTime > (*itCurrentNote)->pEventStart->fActivationTime )
			{
				lTimeSortedMidiNotes.insert(itSortedIter, *itCurrentNote);
				break;
			}

			++itSortedIter;
		}

		lNoteList.pop_front();
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Notes Within Range
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MidiSequencer::MidiChannel::GetNotesWithinRange(MidiSequencer::MidiNoteList* a_plNoteList, float a_fStartRange, float a_fEndRange)
{
	for( unsigned int i = 0; i < aMidiNotesOnChannel.size(); ++i )
	{
		float fEventStart	= aMidiNotesOnChannel[i]->pEventStart->fActivationTime;
		float fEventEnd		= aMidiNotesOnChannel[i]->pEventEnd->fActivationTime;
		
		if( ((fEventStart	> a_fStartRange) && (fEventStart < a_fEndRange)) ||	// If Note Starts Within Range
			((fEventEnd		> a_fStartRange) && (fEventEnd	 < a_fEndRange)) || // Or Note Ends Within Range
			((fEventStart	< a_fStartRange) && (fEventEnd	 > a_fEndRange)) )  // Or Note Starts Before and Ends after Range
		{
			a_plNoteList->push_back( aMidiNotesOnChannel[i] );
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Notes Within Range
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
float MidiSequencer::MidiChannel::GetFinalEventActivationTime()
{
	return (lTimeSortedMidiNotes.rbegin() != lTimeSortedMidiNotes.rend()) ? (*lTimeSortedMidiNotes.rbegin())->pEventEnd->fActivationTime : 0.0f;
}





//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Struct: MidiChannelHolder
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MidiSequencer::MidiChannelsHolder::MidiChannelsHolder()
	: uiMaxChannels(16)
{
	pChannelOne			= new MidiSequencer::MidiChannel( Vector4(   115.0f / 255.0f,    138.0f / 255.0f,    255.0f / 255.0f,    1.0f   ) );
	pChannelTwo			= new MidiSequencer::MidiChannel( Vector4(   175.0f / 255.0f,    95.0f  / 255.0f,    138.0f / 255.0f,    1.0f   ) );
	pChannelThree		= new MidiSequencer::MidiChannel( Vector4(   255.0f / 255.0f,    171.0f / 255.0f,    191.0f / 255.0f,    1.0f   ) );
	pChannelFour		= new MidiSequencer::MidiChannel( Vector4(   231.0f / 255.0f,    66.0f  / 255.0f,    62.0f  / 255.0f,    1.0f   ) );
	pChannelFive		= new MidiSequencer::MidiChannel( Vector4(   251.0f / 255.0f,    135.0f / 255.0f,    13.0f  / 255.0f,    1.0f   ) );
	pChannelSix			= new MidiSequencer::MidiChannel( Vector4(   182.0f / 255.0f,    187.0f / 255.0f,    140.0f / 255.0f,    1.0f   ) );
	pChannelSeven		= new MidiSequencer::MidiChannel( Vector4(   107.0f / 255.0f,    164.0f / 255.0f,    153.0f / 255.0f,    1.0f   ) );
	pChannelEight		= new MidiSequencer::MidiChannel( Vector4(   77.0f  / 255.0f,    247.0f / 255.0f,    41.0f  / 255.0f,    1.0f   ) );
	pChannelNine		= new MidiSequencer::MidiChannel( Vector4(   139.0f / 255.0f,    230.0f / 255.0f,    252.0f / 255.0f,    1.0f   ) );
	pChannelTen			= new MidiSequencer::MidiChannel( Vector4(   43.0f  / 255.0f,    138.0f / 255.0f,    255.0f / 255.0f,    1.0f   ) );
	pChannelEleven		= new MidiSequencer::MidiChannel( Vector4(   122.0f / 255.0f,    136.0f / 255.0f,    207.0f / 255.0f,    1.0f   ) );
	pChannelTwelve		= new MidiSequencer::MidiChannel( Vector4(   211.0f / 255.0f,    129.0f / 255.0f,    188.0f / 255.0f,    1.0f   ) );
	pChannelThirteen	= new MidiSequencer::MidiChannel( Vector4(   128.0f / 255.0f,    108.0f / 255.0f,    193.0f / 255.0f,    1.0f   ) );
	pChannelFourteen	= new MidiSequencer::MidiChannel( Vector4(   149.0f / 255.0f,    132.0f / 255.0f,    84.0f  / 255.0f,    1.0f   ) );
	pChannelFiveteen	= new MidiSequencer::MidiChannel( Vector4(   105.0f / 255.0f,    136.0f / 255.0f,    122.0f / 255.0f,    1.0f   ) );
	pChannelSixteen		= new MidiSequencer::MidiChannel( Vector4(   132.0f / 255.0f,    155.0f / 255.0f,    105.0f / 255.0f,    1.0f   ) );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	*~ Deconstructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MidiSequencer::MidiChannelsHolder::~MidiChannelsHolder()
{
	for( unsigned int i = 0; i < uiMaxChannels; ++i )
	{
		delete (*this)[i];
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Active Channels
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MidiSequencer::MidiChannelList MidiSequencer::MidiChannelsHolder::GetActiveChannels()
{
	MidiSequencer::MidiChannelList lActiveChannel;

	for( unsigned int i = 0; i < uiMaxChannels; ++i )
	{
		if( (*this)[i]->eChannelState == MidiSequencer::ACTIVE )
		{
			lActiveChannel.push_back( (*this)[i] );
		}
	}

	return lActiveChannel;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* Operator Overload: []
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MidiSequencer::MidiChannel* MidiSequencer::MidiChannelsHolder::operator [] (unsigned int index)
{
	switch(index)
	{
	case 0:  return pChannelOne;
	case 1:  return pChannelTwo;
	case 2:  return pChannelThree;
	case 3:  return pChannelFour;
	case 4:  return pChannelFive;
	case 5:  return pChannelSix;
	case 6:  return pChannelSeven;
	case 7:  return pChannelEight;
	case 8:  return pChannelNine;
	case 9:  return pChannelTen;
	case 10: return pChannelEleven;
	case 11: return pChannelTwelve;
	case 12: return pChannelThirteen;
	case 13: return pChannelFourteen;
	case 14: return pChannelFiveteen;
	case 15: return pChannelSixteen;
	default: return pChannelOne;
	}
}