#include "GameEngine.h"

#include "AudioManager.h"
#include "DeltaTime.h"
#include "EditableOptions.h"
#include "FontManager.h"
#include "InputHandler.h"
#include "MidiSequencer.h"
#include "Screen.h"
#include "TextureLoader.h"
#include "XMLHandler.h"

#include "SceneManager.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
GameEngine::GameEngine()
{
	Screen::CreateOpenGLWindow( EditableOptions::uiScreenWidth(), EditableOptions::uiScreenHeight());	// Initialise OpenGL
	m_bGameEngineRunning = Screen::HasOpenedWindow();													// Screen Opened?
	ActivateSingletons();																				// Creates Singleton Classes
	m_pSceneManager = new SceneManager(this);															// Create SceneManager, which itself will create all necessary scenes
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	*~ Destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
GameEngine::~GameEngine()
{
	DeactivateSingletons();
	TextureLoader::DestroyAllImageMemory();

	delete m_pSceneManager;

	TerminateGLFW();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: On Load
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void GameEngine::OnLoad()
{
	// Just Set the Clear Colour Before entering the game loop. If a scene wants to change it, that's fine. We just want to have some clear colour besides black if that's not the case; Security Measures...
	glClearColor(0.25f, 0.25f ,0.25f, 1.f);

	OnLoadSingletons();

	// Load Scene Manager and Her Scenes
	m_pSceneManager->OnLoad();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: On Unload
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void GameEngine::OnUnload()
{
	OnUnloadSingletons();

	m_pSceneManager->OnUnload();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Run~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void GameEngine::Run()
{
	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	glEnable(GL_DEPTH);
	glDepthFunc(GL_LEQUAL);

	// Update DeltaTime Before Loop. To keep away spiked results from loading everything.
	DeltaTime::GetInstance()->UpdateDeltaTime();


	while( IsRunning() )
	{
		// Update Singletons
		Screen::Update();
		AudioManager::GetInstance()	->Update();
		DeltaTime::GetInstance()	->UpdateDeltaTime();
		InputHandler::GetInstance()	->Update();

		// Update Scene Manager
		m_pSceneManager->Update();

		// Check Input (GameEngine)
		CheckInput();

		// Update Scene
		m_pSceneManager->SceneUpdate();

		// Clear the Screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Draw Scene
		m_pSceneManager->SceneDraw();

		// Bring the Back-Buffer to the Monitor
		glfwSwapBuffers();
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Initialise Shutdown
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void GameEngine::InitialiseShutdown()
{
	m_bGameEngineRunning = false;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Create Singletons
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void GameEngine::ActivateSingletons()
{
	new AudioManager();
	new DeltaTime();
	new FontManager();
	new InputHandler();
	new MidiSequencer();
	new XMLHandler("XML/");
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Destroy Singletons
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void GameEngine::DeactivateSingletons()
{
	delete AudioManager::GetInstance();
	delete DeltaTime::GetInstance();
	delete FontManager::GetInstance();
	delete InputHandler::GetInstance();
	delete MidiSequencer::GetInstance();
	delete XMLHandler::GetInstance();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Load Singleton Classes
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void GameEngine::OnLoadSingletons()
{
	AudioManager::GetInstance()	->OnLoad();
	DeltaTime::GetInstance()	->OnLoad();
	InputHandler::GetInstance()	->OnLoad();
	XMLHandler::GetInstance()	->OnLoad();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Unload Singleton Classes
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void GameEngine::OnUnloadSingletons()
{
	AudioManager::GetInstance()	->OnUnload();
	DeltaTime::GetInstance()	->OnUnload();
	InputHandler::GetInstance()	->OnUnload();
	XMLHandler::GetInstance()	->OnUnload();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Is Game Running
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool GameEngine::IsRunning()
{
	return m_bGameEngineRunning && Screen::IsWindowStillOpened();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Check Input
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void GameEngine::CheckInput()
{
	// Switch From GL_FILL to GL_LINES?
	if( InputHandler::GetInstance()->KeyTriggered(GLFW_KEY_TAB) )
	{
		static bool bDrawLines = false;
		bDrawLines = !bDrawLines;
		glPolygonMode( GL_FRONT_AND_BACK, (bDrawLines ? GL_LINE : GL_FILL) );
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Terminate GLFW
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void GameEngine::TerminateGLFW()
{
	glfwTerminate();
}