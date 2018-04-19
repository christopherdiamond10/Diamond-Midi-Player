#include "SceneManager.h"


#include "Scene_Base.h"

#include "AudioManager.h"
#include "GameEngine.h"
#include "InputHandler.h"

#include "Scene_Select.h"
#include "Scene_MidiOptions.h"
#include "Scene_PlayGame.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SceneManager::SceneManager(GameEngine* p_GameEngine)
{
	// Assign the Pointers
	m_pOwningGame						= p_GameEngine;

	m_pSceneArray						= new Scene_Base*[TOTAL_SCENES];
	m_pSceneArray[SELECT_SCENE]			= new Scene_Select(this);
	m_pSceneArray[MIDIOPTIONS_SCENE]	= new Scene_MidiOptions(this);
	m_pSceneArray[PLAYGAME_SCENE]		= new Scene_PlayGame(this);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	*~ Destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SceneManager::~SceneManager()
{	
	for( int i = 0; i < TOTAL_SCENES; ++i )
	{
		delete m_pSceneArray[i];
	}
	delete[] m_pSceneArray;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: On Load
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SceneManager::OnLoad()
{			
	m_eCurrentState	 = SELECT_SCENE;
	m_ePreviousState = SELECT_SCENE;


	for( int i = 0; i < TOTAL_SCENES; ++i )
	{
		m_pSceneArray[i]->OnLoad();
	}

	// Init Starting Scene
	GetScene()->SceneStart();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: On Unload
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SceneManager::OnUnload()
{						
	for( int i = 0; i < TOTAL_SCENES; ++i )
	{
		m_pSceneArray[i]->OnUnload();
	}
} 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Change Scene
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SceneManager::ChangeScene( STATE_TYPES eType )
{
	m_ePreviousState			= m_eCurrentState;
	m_eCurrentState				= eType;

	GetScene(m_ePreviousState)	->SceneTerminate();
	GetScene()					->SceneStart();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Get Scene
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Scene_Base* SceneManager::GetScene()
{
	return m_pSceneArray[m_eCurrentState];
}

Scene_Base* SceneManager::GetScene(STATE_TYPES eType)
{
	return (eType >= 0 && eType < TOTAL_SCENES) ? m_pSceneArray[eType] : NULL;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Is This the Current Scene?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool SceneManager::Get_IsCurrentScene( int Scene )
{
	return (m_eCurrentState == Scene);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Update SceneManager
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SceneManager::Update()
{
	ProcessInput();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Update Scene
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SceneManager::SceneUpdate()
{
	GetScene()->Update();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Draw Scene
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SceneManager::SceneDraw()
{
	GetScene()->Draw();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Shutdown
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SceneManager::Shutdown()
{
	m_pOwningGame->InitialiseShutdown();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Process Input
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SceneManager::ProcessInput()
{
	if( InputHandler::GetInstance()->KeyTriggered( GLFW_KEY_ESC ) || InputHandler::GetInstance()->GetJoyButtonState( JoystickButtons_LB ) )
	{
		switch(m_eCurrentState)
		{
		case PLAYGAME_SCENE: 
			{
				ChangeScene(MIDIOPTIONS_SCENE);
				break;
			}
		case MIDIOPTIONS_SCENE:
			{
				ChangeScene(SELECT_SCENE);
				break;
			}
		default:
			{
				Shutdown();
				break;
			}
		}
	}	
}