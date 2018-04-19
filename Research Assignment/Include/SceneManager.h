//\====================================================================================
//\ Author: Christopher.Allport
//\ About : SceneManager.h -
//\
//\ Keeps track of current in-game scenes and update & draws them accordingly
//\====================================================================================
#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_


class GameEngine;
class Scene_Base;

class SceneManager
{
//////////////////////////////////////////
public:
//////////////////////////////////////////

	//===============================================
	//			Scene Enum
	//===============================================
	enum STATE_TYPES
	{
		SELECT_SCENE,
		MIDIOPTIONS_SCENE,
		PLAYGAME_SCENE,

		TOTAL_SCENES, // Must Be Last Element in Enum
	};

	//===============================================
	//			Constructor & Destructor
	//===============================================
	SceneManager(GameEngine* p_GameEngine);
	~SceneManager();

	//===============================================
	//			Public Functions
	//===============================================
	void OnLoad();
	void OnUnload();

	void Update();
	void SceneUpdate();
	void SceneDraw();

	void Shutdown();
	void ChangeScene( STATE_TYPES eType );

	//===============================================
	//			Getter Functions
	//===============================================
	Scene_Base* GetScene();
	Scene_Base* GetScene(STATE_TYPES eType);

	bool Get_IsCurrentScene( int Scene );


//////////////////////////////////////////
private:
//////////////////////////////////////////
	
	//===============================================
	//			Private Declarations
	//===============================================
	STATE_TYPES m_eCurrentState;
	STATE_TYPES m_ePreviousState;

	//===============================================
	//			Private Functions
	//===============================================
	void ProcessInput();

	//===============================================
	//			Pointers
	//===============================================
	GameEngine*		m_pOwningGame;
	Scene_Base**	m_pSceneArray;
};

#endif