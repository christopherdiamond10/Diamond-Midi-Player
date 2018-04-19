//#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//#					Audio Manager
//#					Author: Christopher Diamond
//#					Date: August 6, 2013
//#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#	  Description:
//#		
//#	  ~	This class handles all audio in this project. Audio is split into three 
//#		categories:
//#
//#			* BGM: Background Music, Only one of these can be playing at a time.
//#					
//#			* BGS: Background Sound, Only one of these can be playing at a time.
//#					These sounds files are typically used as a constant noise,
//#					Such as a fire sound, or people talking.
//#
//#			* SFX: Sound Effects, can use more than one at a time.
//#
//#
//#	  ~ So far this class can only play Sounds as 2D.
//#
//#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#ifndef _AUDIOMANAGER_H_
#define _AUDIOMANAGER_H_


#include <fmod.hpp>
#include <string>
#include <list>
#include <vector>


#define NUMBER_OF_AUDIO_CHANNELS			 5
#define AUDIO_ROOT_DIRECTORY_FOLDER_NAME	"Audio/"
#define BACKGROUND_MUSIC_FOLDER_NAME		"BGM/"
#define BACKGROUND_SOUNDS_FOLDER_NAME		"BGS/"
#define SOUND_EFFECTS_FOLDER_NAME			"SE/"


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Class: Audio Manager
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class AudioManager
{
//////////////////////////////////////////
public:
//////////////////////////////////////////	

	//===============================================
	//			Public Declarations
	//===============================================
	enum AudioTypes
	{
		BGM					= 0,
		BACKGROUND_MUSIC	= 0,
		STAGE_MUSIC			= 0,
		BGS					= 1,
		BACKGROUND_SOUND	= 1,
		ENVIRONMENTAL_SOUND = 1,
		SE					= 2,
		SFX					= 2,
		SOUND_EFFECT		= 2,
		
		NONE				= -1,
	};


	//===============================================
	//			Constructor & Destructor
	//===============================================
	AudioManager();
	~AudioManager();




	//===============================================
	//			Static Functions
	//===============================================

	// Get the one time instance of the AudioManager class
	static AudioManager* GetInstance();


	//===============================================
	//			Public Functions
	//===============================================
	void OnLoad();
	void OnUnload();

	// As a rule, update must be called every frame
	void Update();

	// Play a Sound, pass in the alias name of a sound you have imported, if you know what soundtype it is (BGM, BGS, SFX) you can pass that in as an argument to speed up the process
	void PlayAudio( std::string& AliasNameofSound, AudioTypes SoundType = NONE );
	void PlayAudio( unsigned int AudioID );

	// Stop a Channel (including any and all sounds it is playing), pass in the alias name of a sound you have imported, Channel Types: BGM, BGS, SFX
	void StopSound(AudioTypes ChannelType);

	// Fadein Sound, pass in the amount of time you want to use until the sound is fully faded in. Use this function after using the PlaySound function
	void FadeinSound(AudioTypes ChannelType, float SecondsToFadeIn);

	// Fadeout Sound, pass in the amount of time you want to use until the sound is fully faded in. Use this function after using the PlaySound function
	void FadeoutSound(AudioTypes ChannelType, float SecondsToFadeOut);

	// Import New Audio Files into the AudioManager
	// Filename: Filename (including extension) of the Audio File you are importing, path is included with the directory string you have set earlier
	// AliasName: You may give this sound file an alias name, this alias name will be used to identify the sound for other functions. For Example name the file "Title Theme"
	// Volume: Volume of the sound file, can be between 5..100. The volume is offset by the channel volume, so 80 volume for this sound and 80 volume for the channel volume will yield a 64 total volume when played
	// Tempo: Speed of the Sound File, can be between 5..200. This ONLY affects MIDI files, does not affect other sound types, they will still play at normal speeds
	// LoopStart: Position to start Looping Audio via SampleRates, if you don't know what this is, best leave it as 0
	// LoopEnd: Position to end Audio and restart from Loopstart, again if you don't know what this is, leave it as 0
	unsigned int ImportLocalAudio( std::string& sFileName, AudioTypes AudioType, unsigned int Volume = 80, unsigned int Tempo = 100, unsigned int LoopStart = 0, unsigned int LoopEnd = 0 );
	unsigned int ImportLocalAudio( std::string& sFileName, std::string& sAliasName, AudioTypes AudioType, unsigned int Volume = 80, unsigned int Tempo = 100, unsigned int LoopStart = 0, unsigned int LoopEnd = 0 );
	unsigned int ImportAudioFromPath( std::string& sFilePath, AudioTypes AudioType, unsigned int Volume = 80, unsigned int Tempo = 100, unsigned int LoopStart = 0, unsigned int LoopEnd = 0 );
	unsigned int ImportAudioFromPath( std::string& sFilePath, std::string& sAliasName, AudioTypes AudioType, unsigned int Volume = 80, unsigned int Tempo = 100, unsigned int LoopStart = 0, unsigned int LoopEnd = 0 );
	
	// Restore Default Sound Options, sets all channel volumes back to 80
	void RestoreDefaults();

	// Prints out Audio Loop Information. Use for debugging
	void PrintOutAudioLoopInfo();

	//===============================================
	//			Getter Functions
	//===============================================

	// Get Volume for a Sound, pass in the alias name of a sound you have imported, if you know what soundtype it is (BGM, BGS, SFX) you can pass that in as an argument to speed up the process
	unsigned int GetSoundVolume( std::string& AliasName, AudioTypes SoundType = NONE );
	unsigned int GetSoundVolume( unsigned int AudioID );

	// Get Tempo for a Sound, pass in the alias name of a sound you have imported, if you know what soundtype it is (BGM, BGS, SFX) you can pass that in as an argument to speed up the process
	unsigned int GetSoundTempo( std::string& AliasName, AudioTypes SoundType = NONE );
	unsigned int GetSoundTempo( unsigned int AudioID );

	// See if a channel is mute. Channel Types: BGM, BGS, SFX
	bool IsChannelMuted( AudioTypes ChannelType );

	//===============================================
	//			Setter Functions
	//===============================================

	// Set Volume For Entire System, including BGM, BGS & SFX
	void SetMasterVolume( unsigned int Volume );	

	// Set Individual Channel Volume, Channel Types: BGM, BGS, SFX
	void SetChannelVolume( AudioTypes ChannelType, unsigned int Volume );

	// Set Volume for an individual sound, the channel sound still reflects off of this, so an 80 volume sound mixed in with an 80 channel volume sound will yield a 64 volume sound when played
	// If you know what soundtype it is (BGM, BGS, SFX) you can pass that in as an argument to speed up the process
	void SetSoundVolume( std::string& AliasName, unsigned int Volume, AudioTypes SoundType );
	void SetSoundVolume( unsigned int AudioID, unsigned int Volume );

	// Set Tempo for an individual sound, only works for MIDI Files, if you know what soundtype it is (BGM, BGS, SFX) you can pass that in as an argument to speed up the process
	void SetSoundTempo( std::string& AliasName, unsigned int Tempo, AudioTypes SoundType );
	void SetSoundTempo( unsigned int AudioID, unsigned int Tempo );
	
	// Set Sound Pause for a Channel, basically you can mute all sounds in that channel
	void SetChannelPause( AudioTypes ChannelType, bool SoundPaused );

	// Set Mute, you may mute a channel with this function. Channel Types: BGM, BGS, SFX
	void ToggleMute( AudioTypes ChannelType );


//////////////////////////////////////////
private:
//////////////////////////////////////////
	
	
	//===============================================
	//			Private Declarations
	//===============================================
	typedef std::pair<bool, unsigned int> ImportedAndID;


	struct AudioInfo
	{
		unsigned int			AudioID;
		std::string				FilePath;
		AudioTypes				SoundType;
		unsigned int			Volume;
		unsigned int			Tempo;
		float					ActualVolume;
		float					ActualTempo;
		unsigned int			LoopStart;
		unsigned int			LoopEnd;
		FMOD::Sound*			Sound_ptr;
		std::list<std::string>  AliasNames;

		void AddAlias( std::string& Alias );
		bool DoesAliasesInclude( std::string& Alias );
	};

	struct AudioHolder
	{
		std::vector< AudioInfo* > ImportedAudio;
		unsigned int			  EmptySlotID;

		AudioInfo* operator [] ( unsigned int index ) { return ImportedAudio[index]; }
	};

	struct ChannelInfo
	{
		FMOD::Channel**			ChannelPTR;
		AudioInfo*				CurrentSound;
		AudioTypes				ChannelType;
		float					Volume;
		bool					Paused;
		bool					Fadein;
		bool					Fadeout;
		float					FadeTime;
		float					CurrentFadeTime;
		std::list<AudioInfo*>	ImportedAudioList;
	};


	//===============================================
	//		   Private Instance Variables
	//===============================================
	ChannelInfo			 m_ChannelInfos[3];
	AudioHolder			 m_AudioHolder;

	AudioInfo*			 m_pBGMReservedAudio;
	AudioInfo*			 m_pBGSReservedAudio;

	bool				 m_bMuteSoundEffects;


	FMOD_RESULT			 FMOD_Result;
	FMOD::System*		 m_pFMODSystem;
	FMOD::Channel*		 m_pBGMChannel;
	FMOD::Channel*		 m_pBGSChannel;
	FMOD::Channel*		 m_pSEChannel;


	//===============================================
	//			Static Declarations
	//===============================================
	static AudioManager* m_Instance;


	//===============================================
	//			Private Functions
	//===============================================
	void			InitSound							(													);
	void			DeleteSounds						(													);
	void			SetupAudioHolder					(													);
	void			SetupAudioChannels					(													);
	void			AddNewAudioInfoToHolder				( AudioInfo* pAudInfo								);
	void			CorrectChannelVolume				( ChannelInfo&  a_Channel, float fVolume			);
	void			UpdateChannelFade					( ChannelInfo&  a_Channel							);

	bool			BGMWasFoundAndPlaying				( std::string&  AliasName							);
	bool			BGSWasFoundAndPlaying				( std::string&  AliasName							);
	bool			SEWasFoundAndPlaying				( std::string&  AliasName							);

	void			PlayBGMSound						( AudioInfo* pAudInfo								);
	void			PlayBGSSound						( AudioInfo* pAudInfo								);
	void			PlaySFXSound						( AudioInfo* pAudInfo								);
	
	ImportedAndID	CheckIfAlreadyImported				( std::string&  sFilePath, AudioTypes SoundType		);
	unsigned int	GetValidVolume						( unsigned int  iVolume								);
	unsigned int	GetValidTempo						( unsigned int  iTempo								);

	AudioInfo*		FindSoundFromPTR					( FMOD::Sound*  pSound, AudioTypes SoundType		);
	AudioInfo*		FindAudioInfoInBGMImportedList		( std::string&  AliasName							);
	AudioInfo*		FindAudioInfoInBGSImportedList		( std::string&  AliasName							);
	AudioInfo*		FindAudioInfoInSEImportedList		( std::string&  AliasName							);
	AudioInfo*		ConvertAliasIntoAudioInfoObject		( std::string&  AliasName, AudioTypes SoundType		);
	ChannelInfo*	ConvertChannelTypeToChannelInfo		( AudioTypes ChannelType							);
	std::string		ConvertSoundTypeToAudioFolder		( AudioTypes AudioType								);

	bool			IsBGMArgument						( AudioTypes SoundType								);
	bool			IsBGSArgument						( AudioTypes SoundType								);
	bool			IsSFXArgument						( AudioTypes SoundType								);
};

#endif