#ifndef _SCENE_BASE_H_
#define _SCENE_BASE_H_


// For Our Derived Classes
#include "SceneManager.h"
#include "MathHelper.h"

class Scene_Base
{
//////////////////////////////////////////
public:
//////////////////////////////////////////

	//===============================================
	//			Public Inherited Functions
	//===============================================
	virtual void OnLoad()				{	/* Just after Scene is Created into Memory  */	}
	virtual void OnUnload()				{	/* Just before Scene is Deleted from Memory */	}

	virtual void SceneStart()			{	/* When Scene Begins Running				*/	}
	virtual void SceneTerminate()		{	/* When Scene Stops Running					*/	}

	virtual void Update()				{	/* Update Scene Information					*/	}
	virtual void Draw()					{	/* Draw Scene Items							*/	}



//////////////////////////////////////////
protected:
//////////////////////////////////////////

	//===============================================
	//			Pointers
	//===============================================
	SceneManager* m_pSceneManager;

};


#endif