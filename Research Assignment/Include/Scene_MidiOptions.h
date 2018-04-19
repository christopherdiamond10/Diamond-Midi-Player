#ifndef _SCENEMIDIOPTIONS_H_
#define _SCENEMIDIOPTIONS_H_

#include "Scene_Base.h"
#include "MidiSequencer.h"
#include "UIButton.h"
#include <string> 


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Class: Scene_Select
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class Scene_MidiOptions : public Scene_Base
{
////////////////////////////////////////////////////////////
public:
////////////////////////////////////////////////////////////

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// **~ Constructors & Destructor
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Scene_MidiOptions(SceneManager* SceneMan);
	~Scene_MidiOptions();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// *+ Public Functions
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void OnLoad();
	void OnUnload();

	void SceneStart();
	void SceneTerminate();

	void Update();
	void Draw();

////////////////////////////////////////////////////////////
private:
////////////////////////////////////////////////////////////
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// *{} Private Declarations
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct MidiChannelButton
	{
		UIButton Button;
		MidiSequencer::ChannelState eChannelActivity;
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// *- Private Instance Variables
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	UIButton			m_ReturnSceneButton;
	UIButton			m_PlayGameButton;
	MidiChannelButton*	m_paChannelButtons;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// *- Private Functions
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void _SetupButtonPositions();
	void _SetupButtonChannelInfo();
	void _SetupButtonColours();
	void _SetupSceneChangingButtons();
	std::string _PlaceHolderTrackName(int i);

};

#endif