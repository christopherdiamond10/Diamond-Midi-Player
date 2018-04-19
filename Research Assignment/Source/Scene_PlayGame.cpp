#include "Scene_PlayGame.h"

#include "AudioManager.h"
#include "DeltaTime.h"
#include "EditableOptions.h"
#include "InputHandler.h"
#include "KeyboardDrawSystem.h"
#include "MidiSequencer.h"
#include "RenderQuad.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Scene_PlayGame::Scene_PlayGame(SceneManager* pSceneMan) 
				: Scene_Base()
				, m_TTSoundBegin(EditableOptions::fSongBeginDelay())
{
	m_pSceneManager = pSceneMan;

	m_pKeyboardDrawSystem	= new KeyboardDrawSystem();
	m_pBackgroundRenderer	= new RenderQuad("Background.jpg", Vector2(0.0f, 0.0f), Vector2((float)Screen::GetWidth(), (float)Screen::GetHeight()));

	Reset();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	*~ Destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Scene_PlayGame::~Scene_PlayGame()
{
	delete m_pKeyboardDrawSystem;
	delete m_pBackgroundRenderer;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* Redefined Method: On Load
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_PlayGame::OnLoad()
{	
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* Redefined Method: On Unload
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_PlayGame::OnUnload()
{
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Reset
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_PlayGame::Reset()
{
	m_TTSoundBegin.Reset();
	m_bPlayingAudio = false;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* Redefined Method: Scene Start
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_PlayGame::SceneStart()
{	
	Reset();
	AudioManager::GetInstance()->StopSound(AudioManager::BGM);
	MidiSequencer::GetInstance()->CreateBucketSystem();
	DeltaTime::GetInstance()->UpdateDeltaTime();
	DeltaTime::GetInstance()->ResetTrackTime();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* Redefined Method: Scene Terminate
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_PlayGame::SceneTerminate()
{
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* Redefined Method: Update
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_PlayGame::Update()
{
	MidiSequencer::GetInstance()->Update();
	m_pKeyboardDrawSystem->Update();


	if( MidiSequencer::GetInstance()->SongCompleted() )
	{
		m_pSceneManager->ChangeScene(SceneManager::MIDIOPTIONS_SCENE);
	}

	if( !m_bPlayingAudio )
	{
		m_TTSoundBegin.Update();
		m_bPlayingAudio = m_TTSoundBegin.TimeUp();
		if( m_bPlayingAudio )
		{
			MidiSequencer::GetInstance()->PlayAudioFile();
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* Redefined Method: Draw
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_PlayGame::Draw()
{
	m_pBackgroundRenderer->Draw();
	MidiSequencer::GetInstance()->Draw();
	m_pKeyboardDrawSystem->Draw();
}