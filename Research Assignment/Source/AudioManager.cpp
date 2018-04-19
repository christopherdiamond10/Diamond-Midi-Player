//\====================================================================================
//\ Author: Christopher Diamond
//\ About : AudioManager.cpp - 
//\
//\ Creates the AudioManager source file. This cpp defines the functions made in 
//\ the header file.
//\====================================================================================

//////////////////////////////////////////////////
//	Includes some header files which we will  ////
//	be using in this .cpp file				  ////
//////////////////////////////////////////////////
#include "AudioManager.h"					  ////
#include "DeltaTime.h"						  ////
#include <assert.h>							  ////
#include <iostream>							  ////
//////////////////////////////////////////////////


AudioManager* AudioManager::m_Instance = NULL;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Static Method: Get Audio Manager Instance
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AudioManager* AudioManager::GetInstance()
{
	return m_Instance;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AudioManager::AudioManager()
	: m_pFMODSystem(NULL)
{
	if( m_Instance == NULL )	
	{ 
		m_Instance = this; 
	}
	else						
	{ 
		std::cout << "ERROR: Only One Instance of the AudioManager can exist at any one time! \n\n\n\n\n";
		assert(0); 
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	*~ Deconstructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AudioManager::~AudioManager()
{
	DeleteSounds();

	if( m_pFMODSystem != NULL )
	{
		m_pFMODSystem->close();
		m_pFMODSystem->release();
	}
	
	m_Instance = NULL;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Intialise Sound
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::InitSound()
{
	FMOD_Result = System_Create( &m_pFMODSystem );
	FMOD_Result = m_pFMODSystem->init( NUMBER_OF_AUDIO_CHANNELS, FMOD_INIT_NORMAL, 0 );	
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Setup Audio Holder
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::SetupAudioHolder()
{
	m_AudioHolder.ImportedAudio.resize( 10, NULL );
	m_AudioHolder.EmptySlotID = 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Setup Audio Channels
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::SetupAudioChannels()
{
	for(int i = 0; i < 3; ++i)
	{
		m_ChannelInfos[i].Volume			= 0.8f;
		m_ChannelInfos[i].Paused			= false;
		m_ChannelInfos[i].Fadein			= false;
		m_ChannelInfos[i].Fadeout			= false;
		m_ChannelInfos[i].FadeTime			= 0.f;
		m_ChannelInfos[i].CurrentFadeTime	= 0.f;
		m_ChannelInfos[i].CurrentSound		= NULL;
	}

	m_ChannelInfos[0].ChannelPTR = &m_pBGMChannel;
	m_ChannelInfos[1].ChannelPTR = &m_pBGSChannel;
	m_ChannelInfos[2].ChannelPTR = &m_pSEChannel;

	m_ChannelInfos[0].ChannelType = AudioManager::BACKGROUND_MUSIC;
	m_ChannelInfos[1].ChannelType = AudioManager::BACKGROUND_SOUND;
	m_ChannelInfos[2].ChannelType = AudioManager::SOUND_EFFECT;

	m_pBGMReservedAudio  = NULL;
	m_pBGSReservedAudio  = NULL;
	m_bMuteSoundEffects  = false;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: On Load
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::OnLoad()
{
	InitSound();
	SetupAudioHolder();
	SetupAudioChannels();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: On Unload
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::OnUnload()
{

}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Delete Sounds
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::DeleteSounds()
{
	for( unsigned int i = 0; i < m_AudioHolder.EmptySlotID; ++i )
	{
		m_AudioHolder[i]->Sound_ptr->release();
		delete m_AudioHolder[i];
	}

	m_AudioHolder.ImportedAudio.clear();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Get Sound Volume
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
unsigned int AudioManager::GetSoundVolume( std::string& AliasName, AudioTypes SoundType )
{
	AudioInfo* AudInfo = ConvertAliasIntoAudioInfoObject(AliasName, SoundType);
	if(AudInfo != NULL)
	{
		return AudInfo->Volume; 
	}

	return 80; // 80 is default volume sound, return this if the Sound cannot be found
}

unsigned int AudioManager::GetSoundVolume( unsigned int AudioID )
{
	return m_AudioHolder[ AudioID ]->Volume;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Get Sound Tempo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
unsigned int AudioManager::GetSoundTempo( std::string& AliasName, AudioTypes SoundType )
{
	AudioInfo* AudInfo = ConvertAliasIntoAudioInfoObject(AliasName, SoundType);
	if(AudInfo != NULL)
	{
		return AudInfo->Tempo; 
	}

	return 100; // 100 is default Tempo of sound, return this if the Sound cannot be found
}

unsigned int AudioManager::GetSoundTempo( unsigned int AudioID )
{
	return m_AudioHolder[ AudioID ]->Tempo;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Is Channel Muted?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool AudioManager::IsChannelMuted( AudioTypes ChannelType )
{
	bool bMute = false;

	if( IsBGMArgument( ChannelType ) )
	{ 
		m_pBGMChannel->getMute( &bMute ); 
	}
	else if( IsBGSArgument( ChannelType ) )
	{ 
		m_pBGSChannel->getMute( &bMute ); 
	}
	else if( IsSFXArgument( ChannelType ) )
	{ 
		bMute = m_bMuteSoundEffects; 
	}
	
	return bMute;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Master Volume
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::SetMasterVolume( unsigned int iVolume )
{
	float fVolume = ( (float)GetValidVolume( iVolume ) * 0.01f );	// Volume / 100

	m_ChannelInfos[0].Volume = fVolume;
	m_ChannelInfos[1].Volume = fVolume;
	m_ChannelInfos[2].Volume = fVolume;

	// Correct Volume for all Channels
	CorrectChannelVolume(m_ChannelInfos[0], fVolume);
	CorrectChannelVolume(m_ChannelInfos[1], fVolume);
	CorrectChannelVolume(m_ChannelInfos[2], fVolume);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Channel Volume
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::SetChannelVolume( AudioTypes ChannelType, unsigned int iVolume )
{
	ChannelInfo* CI = ConvertChannelTypeToChannelInfo(ChannelType);
	if(CI != NULL)
	{
		float fVolume = ( (float)GetValidVolume( iVolume ) * 0.01f );
		CorrectChannelVolume(*CI, fVolume );
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Set Sound Volume
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::SetSoundVolume( std::string& AliasName, unsigned int iVolume, AudioTypes ChannelType )
{
	AudioInfo* AudInfo = ConvertAliasIntoAudioInfoObject(AliasName, ChannelType);
	if(AudInfo != NULL)
	{
		AudInfo->Volume			= GetValidVolume( iVolume );
		AudInfo->ActualVolume	= (AudInfo->Volume * 0.01f);
	}
}

void AudioManager::SetSoundVolume( unsigned int AudioID, unsigned int iVolume )
{
	m_AudioHolder[ AudioID ]->Volume		= GetValidVolume( iVolume );
	m_AudioHolder[ AudioID ]->ActualVolume	= (m_AudioHolder[ AudioID ]->Volume * 0.01f);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Set Sound Tempo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::SetSoundTempo( std::string& AliasName, unsigned int iTempo, AudioTypes ChannelType )
{
	AudioInfo* AudInfo = ConvertAliasIntoAudioInfoObject(AliasName, ChannelType);
	if(AudInfo != NULL)
	{
		AudInfo->Tempo			= GetValidTempo( iTempo );
		AudInfo->ActualTempo	= (AudInfo->Tempo * 0.01f);
	}
}

void AudioManager::SetSoundTempo( unsigned int AudioID, unsigned int iTempo )
{
	m_AudioHolder[ AudioID ]->Tempo			= GetValidTempo( iTempo );
	m_AudioHolder[ AudioID ]->ActualTempo	= (m_AudioHolder[ AudioID ]->Tempo * 0.01f);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Channel Pause State
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::SetChannelPause( AudioTypes ChannelType, bool bSoundPaused )
{
	ChannelInfo* CI = ConvertChannelTypeToChannelInfo(ChannelType);
	if(CI != NULL)
	{
		CI->Paused = bSoundPaused;
		(*CI->ChannelPTR)->setPaused(bSoundPaused);
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Toggle Channel Mute
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::ToggleMute( AudioTypes ChannelType )
{
	if( IsBGMArgument(ChannelType) )
	{
		bool bMute = false;
		m_pBGMChannel->getMute( &bMute );

		if( !bMute )
		{ 
			m_pBGMChannel->setMute( true ); 
		}
		else
		{ 
			m_pBGMChannel->setMute( false ); 

			// Play Reserved Sound if you're no longer mute
			if( m_pBGMReservedAudio != NULL )
			{
				PlayBGMSound( m_pBGMReservedAudio );
				m_pBGMReservedAudio = NULL;
			}
		}
	}
	
	else if( IsBGSArgument(ChannelType) )
	{
		bool bMute = false;
		m_pBGSChannel->getMute( &bMute );

		if( !bMute )
		{ 
			m_pBGSChannel->setMute( true ); 
		}
		else
		{ 
			m_pBGSChannel->setMute( false ); 

			if( m_pBGSReservedAudio != NULL )
			{
				PlayBGSSound( m_pBGSReservedAudio );
				m_pBGSReservedAudio = NULL;
			}
		}
	}
	
	else if( IsSFXArgument(ChannelType) )
	{
		m_bMuteSoundEffects = !m_bMuteSoundEffects;
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Restore Defaults
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::RestoreDefaults()
{
	// Correct Volume for all Channels
	CorrectChannelVolume(m_ChannelInfos[0], 0.8f);
	CorrectChannelVolume(m_ChannelInfos[1], 0.8f);
	CorrectChannelVolume(m_ChannelInfos[2], 0.8f);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Update
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::Update()
{
	// Update Sound Fade
	for(int i = 0; i < 3; ++i)
	{
		if(m_ChannelInfos[i].Fadein || m_ChannelInfos[i].Fadeout)
		{
			UpdateChannelFade( m_ChannelInfos[i] );
		}
	}

	// Update SoundSystem
	FMOD_Result = m_pFMODSystem->update();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Update Channel Fade
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::UpdateChannelFade( ChannelInfo& a_Channel )
{	
	 a_Channel.CurrentFadeTime += DeltaTime::GetInstance()->GetDeltaTime();

	 // Get Current Channel Volume
	 float fVolume;
	 (*a_Channel.ChannelPTR)->getVolume(&fVolume);


	 // Get Update Volume Via DeltaTime
	 float fTempVolume = ((DeltaTime::GetInstance()->GetDeltaTime() * a_Channel.Volume * a_Channel.CurrentSound->ActualVolume) / a_Channel.FadeTime);
	
	 // Add or Deduct Volume from Current Channel Volume
	 fVolume += a_Channel.Fadein ? fTempVolume : -fTempVolume;

	 // Set Volume
	 (*a_Channel.ChannelPTR)->setPaused(true);
	 (*a_Channel.ChannelPTR)->setVolume(fVolume);
	 (*a_Channel.ChannelPTR)->setPaused(false);

	 // If Fully Faded in/out, set fade to false
	 if(a_Channel.CurrentFadeTime > a_Channel.FadeTime)
	 {
		 a_Channel.Fadein = false;
		 a_Channel.Fadeout = false;
		 a_Channel.CurrentFadeTime = 0.0f;
	 }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Print Out Audio Loop Info
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::PrintOutAudioLoopInfo()
{
	//////////////////////////////////////
	unsigned int iSoundPosition, iLoopStart, iLoopEnd;
	//////////////////////////////////////
	m_pBGMChannel->getPosition( &iSoundPosition, FMOD_TIMEUNIT_PCM );
	m_pBGMChannel->getLoopPoints( &iLoopStart, FMOD_TIMEUNIT_PCM, &iLoopEnd, FMOD_TIMEUNIT_PCM );
	//////////////////////////////////////
	std::cout << "\n\nCurrent Sound Position: " << iSoundPosition << std::endl;
	//////////////////////////////////////
	if( iSoundPosition > iLoopStart )
		std::cout << "Sound has Entered LoopStart\n";	
	else
		std::cout << "Sound has not Entered LoopStart\n";
	//////////////////////////////////////
	if( iSoundPosition == iLoopEnd )
		std::cout << "Sound has hit LoopEnd\n";	
	else
		std::cout << "Sound has not hit LoopEnd\n";
	//////////////////////////////////////
	std::cout << "LoopStart = " << iLoopStart << std::endl;
	std::cout << "LoopEnd = " << iLoopEnd << "\n\n\n\n\n";
	//////////////////////////////////////
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Import Audio
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
unsigned int AudioManager::ImportLocalAudio( std::string& sFileName, AudioTypes eSoundType, unsigned int iVolume, unsigned int iTempo, unsigned int iSampleRate_LoopStart, unsigned int iSampleRate_LoopEnd )
{
	return ImportLocalAudio( sFileName, (std::string)"", eSoundType, iVolume, iTempo, iSampleRate_LoopStart, iSampleRate_LoopEnd );
}

unsigned int AudioManager::ImportLocalAudio( std::string& sFileName, std::string& sAliasName, AudioTypes eSoundType, unsigned int iVolume, unsigned int iTempo, unsigned int iSampleRate_LoopStart, unsigned int iSampleRate_LoopEnd )
{
	std::string sPathtoFile = (AUDIO_ROOT_DIRECTORY_FOLDER_NAME + ConvertSoundTypeToAudioFolder(eSoundType) + sFileName); // Create Path to Audio Clip
	return ImportAudioFromPath(sPathtoFile, sAliasName, eSoundType, iVolume, iTempo, iSampleRate_LoopStart, iSampleRate_LoopEnd);
}

unsigned int AudioManager::ImportAudioFromPath( std::string& sFilePath, AudioTypes eSoundType, unsigned int iVolume, unsigned int iTempo, unsigned int iSampleRate_LoopStart, unsigned int iSampleRate_LoopEnd )
{
	return ImportAudioFromPath(sFilePath, (std::string)"", eSoundType, iVolume, iTempo, iSampleRate_LoopStart, iSampleRate_LoopEnd);
}

unsigned int AudioManager::ImportAudioFromPath( std::string& sFilePath, std::string& sAliasName, AudioTypes eSoundType, unsigned int iVolume, unsigned int iTempo, unsigned int iSampleRate_LoopStart, unsigned int iSampleRate_LoopEnd )
{
	if( eSoundType == AudioManager::NONE )
	{
		return 0;
	}	

	// Check to make sure the Audio hasn't already been imported
	ImportedAndID prWasImported = CheckIfAlreadyImported( sFilePath, eSoundType );
	if( prWasImported.first )
	{
		m_AudioHolder[prWasImported.second]->AddAlias( sAliasName );
		return prWasImported.second;
	}

	// Create Info for the Sound
	AudioInfo* pAudInfo		= new AudioInfo();
	pAudInfo->FilePath		= sFilePath;
	pAudInfo->SoundType		= eSoundType;
	pAudInfo->Volume		= (GetValidVolume(iVolume));
	pAudInfo->Tempo			= (GetValidTempo(iTempo));
	pAudInfo->ActualVolume	= (pAudInfo->Volume * 0.01f);
	pAudInfo->ActualTempo	= (pAudInfo->Tempo  * 0.01f);
	pAudInfo->LoopStart		= iSampleRate_LoopStart;
	pAudInfo->LoopEnd		= iSampleRate_LoopEnd;
	pAudInfo->AddAlias		  ( sAliasName );

	// Add to Holder
	AddNewAudioInfoToHolder( pAudInfo );
	
	// Create either Stream or Memory for the sound
	if( IsBGMArgument(eSoundType) )
	{
		FMOD_Result = m_pFMODSystem->createStream( pAudInfo->FilePath.c_str(), FMOD_LOOP_NORMAL, 0, &pAudInfo->Sound_ptr );
		m_ChannelInfos[0].ImportedAudioList.push_back(pAudInfo);
	}

	else if( IsBGSArgument(eSoundType) )
	{
		FMOD_Result = m_pFMODSystem->createSound( pAudInfo->FilePath.c_str(), FMOD_LOOP_NORMAL, 0, &pAudInfo->Sound_ptr);
		m_ChannelInfos[1].ImportedAudioList.push_back(pAudInfo);
	}

	else
	{
		FMOD_Result = m_pFMODSystem->createSound( pAudInfo->FilePath.c_str(), FMOD_DEFAULT, 0, &pAudInfo->Sound_ptr);
		m_ChannelInfos[2].ImportedAudioList.push_back(pAudInfo);
	}

	// Return AudioID
	return pAudInfo->AudioID;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Add New Audio Info To Holder
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::AddNewAudioInfoToHolder( AudioInfo* pAudInfo )
{
	pAudInfo->AudioID = m_AudioHolder.EmptySlotID;

	// If Reached Max Capacity, Add 10 Elements
	if( m_AudioHolder.ImportedAudio.size() == m_AudioHolder.EmptySlotID )
	{
		m_AudioHolder.ImportedAudio.resize( m_AudioHolder.ImportedAudio.size() + 10, NULL );
	}

	m_AudioHolder.ImportedAudio[m_AudioHolder.EmptySlotID] = pAudInfo;
	m_AudioHolder.EmptySlotID += 1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: FadeIn Sound
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::FadeinSound(AudioTypes ChannelType, float fSecondsToFadeIn)
{
	ChannelInfo* CI = ConvertChannelTypeToChannelInfo(ChannelType);
	if(CI != NULL)
	{
		if(CI->CurrentSound != NULL )
		{
			CI->Paused		= false;
			CI->Fadein		= true;
			CI->Fadeout		= false;
			CI->FadeTime	= fSecondsToFadeIn;

			// Set Low Volume
			(*CI->ChannelPTR)->setPaused(true);
			(*CI->ChannelPTR)->setVolume(0.001f);
			(*CI->ChannelPTR)->setPaused(false);
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: FadeOut Sound
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::FadeoutSound(AudioTypes ChannelType, float fSecondsToFadeOut)
{
	ChannelInfo* CI = ConvertChannelTypeToChannelInfo(ChannelType);
	if(CI != NULL)
	{
		if(CI->CurrentSound != NULL )
		{
			CI->Paused		= false;
			CI->Fadein		= false;
			CI->Fadeout		= true;
			CI->FadeTime	= fSecondsToFadeOut;
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Stop Sound	(Channel)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::StopSound(AudioTypes ChannelType)
{
	ChannelInfo* CI = ConvertChannelTypeToChannelInfo(ChannelType);
	if(CI != NULL)
	{
		FMOD_Result = (*CI->ChannelPTR)->stop();
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Play Sound
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::PlayAudio( std::string& AliasNameofSound, AudioTypes SoundType )
{	
	AudioInfo* pAudInfo = ConvertAliasIntoAudioInfoObject(AliasNameofSound, SoundType);

	if( pAudInfo != NULL )
	{
		PlayAudio( pAudInfo->AudioID );
	}
	else
	{
		// Sound Was Not Found, Show Error in Console
		std::cout << "Alias SoundFileName: " << AliasNameofSound << " was not found. It either does not exist or you have not imported it.\n";
	}
}

void AudioManager::PlayAudio( unsigned int AudioID )
{
	if( IsBGMArgument(m_AudioHolder[ AudioID ]->SoundType) )
	{
		PlayBGMSound( m_AudioHolder[ AudioID ] );
	}
	else if( IsBGSArgument(m_AudioHolder[ AudioID ]->SoundType) )
	{
		PlayBGSSound( m_AudioHolder[ AudioID ] );
	}
	else
	{
		PlaySFXSound( m_AudioHolder[ AudioID ] );
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: BGM Was Found and is now Playing?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool AudioManager::BGMWasFoundAndPlaying(std::string& AliasNameofSound)
{
	AudioInfo* TempAudioInfo = FindAudioInfoInBGMImportedList(AliasNameofSound);
	if(TempAudioInfo != NULL)
	{
		// Found Sound in this List
		PlayBGMSound( TempAudioInfo );
		return true;
	}
	// Didn't find Sound in this List
	return false;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: BGS Was Found and is now Playing?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool AudioManager::BGSWasFoundAndPlaying(std::string& AliasNameofSound)
{
	AudioInfo* TempAudioInfo = FindAudioInfoInBGSImportedList(AliasNameofSound);
	if(TempAudioInfo != NULL)
	{
		PlayBGSSound( TempAudioInfo );
		return true;
	}
	return false;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: SFX Was Found and is now Playing?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool AudioManager::SEWasFoundAndPlaying(std::string& AliasNameofSound)
{
	AudioInfo* TempAudioInfo = FindAudioInfoInSEImportedList(AliasNameofSound);
	if(TempAudioInfo != NULL)
	{
		PlaySFXSound( TempAudioInfo );
		return true;
	}
	return false;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Play BGM Sound
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::PlayBGMSound( AudioInfo* pAudInfo )
{
	// If Muted, Leave
	if( IsChannelMuted(AudioManager::BACKGROUND_MUSIC) ) 
	{ 
		m_pBGMReservedAudio = pAudInfo;
		return; 
	}

	// Return if already playing this sound
	FMOD::Sound* pFSound;
	m_pBGMChannel->getCurrentSound(&pFSound);
	if(pFSound == pAudInfo->Sound_ptr) 
	{ 
		return; 
	}


	// Setup Sound: Stop Whatever is currently playing, set loop points, play sound, then adjust volume & tempo 
	// (FMOD reconfigures volume and Tempo when it begins playing a sound, so to modify it we have to set it after we start playing)
	FMOD_Result = m_pBGMChannel->stop();
		
	// Set Loop Points (If Any)
	if( pAudInfo->LoopEnd > pAudInfo->LoopStart )
	{ 
		FMOD_Result = pAudInfo->Sound_ptr->setLoopPoints( pAudInfo->LoopStart, FMOD_TIMEUNIT_PCM, pAudInfo->LoopEnd, FMOD_TIMEUNIT_PCM ); 
	}
		
	FMOD_Result = m_pFMODSystem->playSound( FMOD_CHANNEL_REUSE, pAudInfo->Sound_ptr, false, &m_pBGMChannel );

	FMOD_Result = m_pBGMChannel->setPaused(true);
	FMOD_Result = m_pBGMChannel->setVolume(pAudInfo->ActualVolume * m_ChannelInfos[0].Volume);
	FMOD_Result = pAudInfo->Sound_ptr->setMusicSpeed(pAudInfo->ActualTempo);
	FMOD_Result = m_pBGMChannel->setPaused(false);
		
	m_ChannelInfos[0].Paused = false;
	m_ChannelInfos[0].CurrentSound = pAudInfo;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Play BGS Sound
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::PlayBGSSound( AudioInfo* pAudInfo )
{
	if( IsChannelMuted( AudioManager::BACKGROUND_SOUND)) 
	{ 
		m_pBGSReservedAudio = pAudInfo;
		return; 
	}

	// Return if already playing this sound
	FMOD::Sound* pFSound;
	m_pBGSChannel->getCurrentSound(&pFSound);
	if(pFSound == pAudInfo->Sound_ptr) 
	{ 
		return; 
	}

	FMOD_Result = m_pBGSChannel->stop();
	FMOD_Result = m_pFMODSystem->playSound( FMOD_CHANNEL_REUSE, pAudInfo->Sound_ptr, false, &m_pBGSChannel );
	FMOD_Result = m_pBGSChannel->setPaused(true);
	FMOD_Result = m_pBGSChannel->setVolume( pAudInfo->ActualVolume * m_ChannelInfos[1].Volume);
	FMOD_Result = pAudInfo->Sound_ptr->setMusicSpeed(pAudInfo->ActualTempo);
	FMOD_Result = m_pBGSChannel->setPaused(false);

	m_ChannelInfos[1].Paused		= false;
	m_ChannelInfos[1].CurrentSound	= pAudInfo;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Play SFX Sound
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::PlaySFXSound( AudioInfo* pAudInfo )
{
	if( IsChannelMuted(AudioManager::SOUND_EFFECT) ) 
	{ 
		return; 
	}

	FMOD_Result = m_pFMODSystem->playSound( FMOD_CHANNEL_FREE, pAudInfo->Sound_ptr, false, &m_pSEChannel );
	FMOD_Result = m_pSEChannel->setVolume( pAudInfo->ActualVolume * m_ChannelInfos[2].Volume);
	FMOD_Result = pAudInfo->Sound_ptr->setMusicSpeed(pAudInfo->ActualTempo);
		
	m_ChannelInfos[2].Paused		= false;
	m_ChannelInfos[2].CurrentSound	= pAudInfo;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Check If Audio Was Already Imported
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AudioManager::ImportedAndID AudioManager::CheckIfAlreadyImported( std::string& sFilePath, AudioTypes eSoundType )
{
	// If the filename matches up to one that's already been imported, return true
	if( IsBGMArgument(eSoundType) )
	{
		for( std::list<AudioInfo*>::iterator Iter = m_ChannelInfos[0].ImportedAudioList.begin(); Iter != m_ChannelInfos[0].ImportedAudioList.end(); ++Iter )
		{
			if( (*Iter)->FilePath == sFilePath )
			{
				return ImportedAndID(true, (*Iter)->AudioID);
			}
		}
	}

	else if( IsBGSArgument(eSoundType) )
	{
		for( std::list<AudioInfo*>::iterator Iter = m_ChannelInfos[1].ImportedAudioList.begin(); Iter != m_ChannelInfos[1].ImportedAudioList.end(); ++Iter )
		{
			if( (*Iter)->FilePath == sFilePath )
			{
				return ImportedAndID(true, (*Iter)->AudioID);
			}
		}
	}

	else if( IsSFXArgument(eSoundType) )
	{
		for( std::list<AudioInfo*>::iterator Iter = m_ChannelInfos[2].ImportedAudioList.begin(); Iter != m_ChannelInfos[2].ImportedAudioList.end(); ++Iter )
		{
			if( (*Iter)->FilePath == sFilePath )
			{
				return ImportedAndID(true, (*Iter)->AudioID);
			}
		}
	}

	return ImportedAndID(false, 0);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Valid Volume
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
unsigned int AudioManager::GetValidVolume( unsigned int iVolume )
{
	return (iVolume > 100 ? 100 : (iVolume < 5 ? 5 : iVolume));
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Valid Tempo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
unsigned int AudioManager::GetValidTempo( unsigned int iTempo )
{
	return (iTempo > 200 ? 200 : (iTempo < 5 ? 5 : iTempo));
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Find AudioInfo From Sound PTR
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AudioManager::AudioInfo* AudioManager::FindSoundFromPTR( FMOD::Sound* pSound, AudioTypes eSoundType )
{
	if( IsBGMArgument(eSoundType) )
	{
		for( std::list<AudioInfo*>::iterator iter = m_ChannelInfos[0].ImportedAudioList.begin(); iter != m_ChannelInfos[0].ImportedAudioList.end(); ++iter )
		{
			if( (*iter)->Sound_ptr == pSound )
			{
				return (*iter);
			}
		}
	}

	else if( IsBGSArgument(eSoundType) )
	{
		for( std::list<AudioInfo*>::iterator iter = m_ChannelInfos[1].ImportedAudioList.begin(); iter != m_ChannelInfos[1].ImportedAudioList.end(); ++iter )
		{
			if( (*iter)->Sound_ptr == pSound )
			{
				return (*iter);
			}
		}
	}

	else
	{
		for( std::list<AudioInfo*>::iterator iter = m_ChannelInfos[2].ImportedAudioList.begin(); iter != m_ChannelInfos[2].ImportedAudioList.end(); ++iter )
		{
			if( (*iter)->Sound_ptr == pSound )
			{
				return (*iter);
			}
		}
	}

	return NULL;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Correct Channel Volume
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::CorrectChannelVolume( ChannelInfo& a_Channel, float fVolume )
{
	FMOD::Sound* TempSoundPTR = NULL;
	FMOD_Result = (*a_Channel.ChannelPTR)->getCurrentSound(&TempSoundPTR);
	
	// If Sound is playing, adjust volume to the sound's individual volume combined with the channel's volume
	if( TempSoundPTR != NULL ) 
	{ 
		FMOD_Result = (*a_Channel.ChannelPTR)->setPaused(true);
		FMOD_Result = (*a_Channel.ChannelPTR)->setVolume( FindSoundFromPTR(TempSoundPTR, a_Channel.ChannelType)->ActualVolume * fVolume ); 
		FMOD_Result = (*a_Channel.ChannelPTR)->setPaused(a_Channel.Paused);
	}
	// Otherwise just set the channel straight to that specific volume
	else
	{
		FMOD_Result = (*a_Channel.ChannelPTR)->setVolume( fVolume ); 
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Find Audio Info in BGM Imported List
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AudioManager::AudioInfo* AudioManager::FindAudioInfoInBGMImportedList( std::string& AliasName )
{
	for( std::list<AudioInfo*>::iterator iter = m_ChannelInfos[0].ImportedAudioList.begin(); iter != m_ChannelInfos[0].ImportedAudioList.end(); ++iter )
	{
		if( (*iter)->DoesAliasesInclude(AliasName) )
		{
			return (*iter);
		}
	}
	return NULL;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Find Audio Info in BGS Imported List
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AudioManager::AudioInfo* AudioManager::FindAudioInfoInBGSImportedList( std::string& AliasName )
{
	for( std::list<AudioInfo*>::iterator iter = m_ChannelInfos[1].ImportedAudioList.begin(); iter != m_ChannelInfos[1].ImportedAudioList.end(); ++iter )
	{
		if( (*iter)->DoesAliasesInclude(AliasName) )
		{
			return (*iter);
		}
	}
	return NULL;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Find Audio Info in SFX Imported List
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AudioManager::AudioInfo* AudioManager::FindAudioInfoInSEImportedList( std::string& AliasName )
{
	for( std::list<AudioInfo*>::iterator iter = m_ChannelInfos[2].ImportedAudioList.begin(); iter != m_ChannelInfos[2].ImportedAudioList.end(); ++iter )
	{
		if( (*iter)->DoesAliasesInclude(AliasName) )
		{
			return (*iter);
		}
	}
	return NULL;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Convert Alias Into Audio Info Object
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AudioManager::AudioInfo* AudioManager::ConvertAliasIntoAudioInfoObject( std::string& AliasName, AudioTypes eSoundType )
{
	// Give SFX Priority
	if( IsSFXArgument(eSoundType) )
	{
		AudioInfo* TempAudioInfo = FindAudioInfoInSEImportedList(AliasName);
		if(TempAudioInfo != NULL) { return TempAudioInfo; }

		TempAudioInfo = FindAudioInfoInBGSImportedList(AliasName);
		if(TempAudioInfo != NULL) { return TempAudioInfo; }

		return FindAudioInfoInBGMImportedList(AliasName);
	}

	// Give BGS Priority
	else if( IsBGSArgument(eSoundType) )
	{
		AudioInfo* TempAudioInfo = FindAudioInfoInBGSImportedList(AliasName);
		if(TempAudioInfo != NULL) { return TempAudioInfo; }

		TempAudioInfo = FindAudioInfoInBGMImportedList(AliasName);
		if(TempAudioInfo != NULL) { return TempAudioInfo; }

		return FindAudioInfoInSEImportedList(AliasName);
	}

	// Give BGM Priority
	else
	{
		AudioInfo* TempAudioInfo = FindAudioInfoInBGMImportedList(AliasName);
		if(TempAudioInfo != NULL) { return TempAudioInfo; }
		
		TempAudioInfo = FindAudioInfoInBGSImportedList(AliasName);
		if(TempAudioInfo != NULL) { return TempAudioInfo; }

		return FindAudioInfoInSEImportedList(AliasName);
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Convert Channel Type to Channel Info
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AudioManager::ChannelInfo* AudioManager::ConvertChannelTypeToChannelInfo( AudioTypes ChannelType )
{
	if(IsBGMArgument( ChannelType )) { return &m_ChannelInfos[0]; }
	if(IsBGSArgument( ChannelType )) { return &m_ChannelInfos[1]; }
	if(IsSFXArgument( ChannelType )) { return &m_ChannelInfos[2]; }
	return NULL;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Convert SOund Type to Audio Folder
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::string AudioManager::ConvertSoundTypeToAudioFolder( AudioTypes AudioType )
{
	if(IsBGMArgument( AudioType )) { return BACKGROUND_MUSIC_FOLDER_NAME; }
	if(IsBGSArgument( AudioType )) { return BACKGROUND_SOUNDS_FOLDER_NAME; }
	if(IsSFXArgument( AudioType )) { return SOUND_EFFECTS_FOLDER_NAME; }
	return "";
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Is A BGM Argument?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool AudioManager::IsBGMArgument( AudioTypes SoundType )
{
	return ( (int)SoundType == (int)AudioManager::BGM );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Is A BGS Argument?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool AudioManager::IsBGSArgument( AudioTypes SoundType )
{
	return ( (int)SoundType == (int)AudioManager::BGS );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Is A SFX Argument?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool AudioManager::IsSFXArgument( AudioTypes SoundType )
{
	return ( (int)SoundType == (int)AudioManager::SFX );
}









//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Add Alias		   (Audio Info Struct)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AudioManager::AudioInfo::AddAlias( std::string& Alias )
{
	if( Alias != "" )
	{
		for( std::list<std::string>::iterator Iter = AliasNames.begin(); Iter != AliasNames.end(); ++Iter )
		{
			if( Alias == (*Iter) )
			{
				return;
			}
		}

		AliasNames.push_back( Alias );
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Does Alias List Include?  (Audio Info Struct)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool AudioManager::AudioInfo::DoesAliasesInclude( std::string& Alias )
{
	for( std::list<std::string>::iterator Iter = AliasNames.begin(); Iter != AliasNames.end(); ++Iter )
	{
		if( Alias == (*Iter) )
		{
			return true;
		}
	}
	return false;
}