#ifndef _SCENESELECT_H_
#define _SCENESELECT_H_

#include "Scene_Base.h"
#include "UIButton.h"
#include <string>

// Open File Dialog
#include <windows.h>
#include <shobjidl.h> 


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Class: Scene_Select
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class Scene_Select : public Scene_Base
{
////////////////////////////////////////////////////////////
public:
////////////////////////////////////////////////////////////

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// **~ Constructors & Destructor
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Scene_Select(SceneManager* SceneMan);
	~Scene_Select();
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
	typedef std::pair<int, COMDLG_FILTERSPEC*> FileExtensionFilter;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// *- Private Instance Variables
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool				m_bHasChangedMidiFiles;
	bool				m_bReadingMidiFile;

	std::string			m_sMidiDescription;
	std::string			m_sMidiFilenameDisplay;
	TransformDetails2D	m_tMidiDescriptionTransform;
	TransformDetails2D	m_tMidiFilenameTransform;
	Vector4				m_vMidiDescriptionColour;
	Vector4				m_vMidiFilenameColour;
	
	std::string			m_sAudioDescription;
	std::string			m_sAudioFilenameDisplay;
	TransformDetails2D	m_tAudioDescriptionTransform;
	TransformDetails2D	m_tAudioFilenameTransform;
	Vector4				m_vAudioDescriptionColour;
	Vector4				m_vAudioFilenameColour;

	std::string			m_sMidiFilename;
	std::string			m_sAudioFilename;

	UIButton			m_MidiFileChangeButton;
	UIButton			m_AudioFileChangeButton;
	UIButton			m_EnterNextSceneButton;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// *- Private Functions
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void		_SetupMidiFont();
	void		_SetupAudioFont();
	void		_SetupButtons();
	void		_SetNewMidiFileText(std::string& Text);
	void		_SetNewAudioFileText(std::string& Text);
	std::string _OpenDialogBox(FileExtensionFilter ExtensionFilter );
	void		_OpenMidiDialogBox();
	void		_OpenAudioDialogBox();

};

#endif