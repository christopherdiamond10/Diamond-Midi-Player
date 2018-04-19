//\====================================================================================
//\ Author: Christopher.Allport
//\ About : GameEngine.h - 
//\ 
//\ Defines the GameEngine Class. 
//\ This Class is used as the main handler for the game, ie Managing the game's 'while'
//\ loop as well as managing the singleton classes.
//\====================================================================================
#ifndef _GAMEENGINE_H_
#define _GAMEENGINE_H_


class SceneManager;

class GameEngine
{
////////////////////////////////////////////////////////////
public:
////////////////////////////////////////////////////////////

	//===============================================
	//			Constructors & Destructor
	//===============================================
	GameEngine();
	~GameEngine();



	//===============================================
	//			Public Functions
	//===============================================
	void OnLoad();
	void OnUnload();

	void Run();
	void InitialiseShutdown();
	


////////////////////////////////////////////////////////////
private:
////////////////////////////////////////////////////////////
	
	//===============================================
	//			Private Declarations
	//===============================================
	bool m_bGameEngineRunning;


	//===============================================
	//			Private Functons
	//===============================================
	void ActivateSingletons();
	void DeactivateSingletons();

	void OnLoadSingletons();
	void OnUnloadSingletons();

	bool IsRunning();
	void CheckInput();

	bool InitialiseGLFW();
	void TerminateGLFW();

	//===============================================
	//			Pointers
	//===============================================
	SceneManager* m_pSceneManager;
	
};

#endif