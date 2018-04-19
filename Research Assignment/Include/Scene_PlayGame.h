//\====================================================================================
//\ Author: Christopher.Allport
//\ About : Scene_PlayGame.h - 
//\ 
//\ Defines the PlayGame Class
//\ This class holds our PlayGame State and manages the scene as well as the enities 
//\ involved.
//\====================================================================================

// Include Guards
#ifndef _SCENE_PLAYGAME_H_
#define _SCENE_PLAYGAME_H_

// Dependant Files
#include "Scene_Base.h"
#include "HelperClasses.h"
#include <list>

class KeyboardDrawSystem;
class RenderQuad;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Class: Scene_PlayGame
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class Scene_PlayGame : public Scene_Base
{
////////////////////////////////////////////////////////////
public:
////////////////////////////////////////////////////////////

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// **~ Constructors & Destructor
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Scene_PlayGame(SceneManager* SceneMan);
	~Scene_PlayGame();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// *+ Public Functions
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void OnLoad();
	void OnUnload();

	void Reset();

	void SceneStart();
	void SceneTerminate();

	void Update();
	void Draw();

////////////////////////////////////////////////////////////
private:
////////////////////////////////////////////////////////////
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// *- Private Instance Variables
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool m_bPlayingAudio;

	HelperClasses::TimeTracker	m_TTSoundBegin;
	
	KeyboardDrawSystem*			m_pKeyboardDrawSystem;
	RenderQuad*					m_pBackgroundRenderer;

};


#endif