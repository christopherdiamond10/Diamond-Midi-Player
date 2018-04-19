#include "Scene_Select.h"

#include "AudioManager.h"
#include "DeltaTime.h"
#include "FontManager.h"
#include "MidiSequencer.h"
#include "Screen.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Scene_Select::Scene_Select(SceneManager* a_pSceneMan) 
				: Scene_Base()
				, m_MidiFileChangeButton()
				, m_AudioFileChangeButton()
				, m_EnterNextSceneButton()
{
	m_pSceneManager = a_pSceneMan;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	*~ Deconstructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Scene_Select::~Scene_Select()
{

}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* Redefined Method: On Load
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_Select::OnLoad()
{
	m_bHasChangedMidiFiles = false;
	m_bReadingMidiFile = false;

	_SetupMidiFont();
	_SetupAudioFont();
	_SetupButtons();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* Redefined Method: Unload
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_Select::OnUnload()
{

}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* Redefined Method: Scene Start
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_Select::SceneStart()
{

}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* Redefined Method: Scene Terminate
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_Select::SceneTerminate()
{

}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* Redefined Method: Update
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_Select::Update()
{
	// Load Midi, Switch Scene
	if( m_bReadingMidiFile )
	{
		unsigned int uiAudioID		= AudioManager::GetInstance()->ImportAudioFromPath(m_sAudioFilename, AudioManager::BGM);
		MidiSequencer::GetInstance()->ReadMidiFile(m_sMidiFilename);
		MidiSequencer::GetInstance()->StoreAudioFile(uiAudioID);
		m_bReadingMidiFile			= false;
		m_bHasChangedMidiFiles		= false;
		m_pSceneManager->ChangeScene(SceneManager::MIDIOPTIONS_SCENE);
	}

	else
	{
		m_MidiFileChangeButton.Update();
		m_AudioFileChangeButton.Update();
		m_EnterNextSceneButton.Update();

		if(m_MidiFileChangeButton.WasClicked())
		{
			_OpenMidiDialogBox();
		}
		else if(m_AudioFileChangeButton.WasClicked())
		{
			_OpenAudioDialogBox();
		}
		else if(m_EnterNextSceneButton.WasClicked())
		{
			if( m_sMidiFilename != "" )
			{
				if( m_bHasChangedMidiFiles )
					m_bReadingMidiFile = true;
				else
				{
					MidiSequencer::GetInstance()->StoreAudioFile( AudioManager::GetInstance()->ImportAudioFromPath(m_sAudioFilename, AudioManager::BGM) );
					m_pSceneManager->ChangeScene(SceneManager::MIDIOPTIONS_SCENE);
				}
			}
			else
			{
				MessageBox(NULL, L"You Must Select a Midi File to Read before you can continue!", L"ERROR", 64);
			}
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* Redefined Method: Draw
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_Select::Draw()
{
	// Draw Load Screen
	if( m_bReadingMidiFile )
	{
		std::string Text = "Reading Midi File";
		Vector2 vSize( Screen::ScaleToScreenSize(Vector2(80.0f, 80.0f)) );
		float fSizeLength = FontManager::GetInstance()->GetTotalDrawLength(Text, vSize);
		Vector2 vPosition( ((float)Screen::GetWidth() * 0.5f) - (fSizeLength * 0.5f), (((float)Screen::GetHeight() * 0.5f) - vSize.y) );
		FontManager::GetInstance()->Print("Reading Midi File", vPosition, vSize);
	}

	// Draw Scene
	else
	{
		FontManager::GetInstance()->Print(m_sMidiDescription,		m_tMidiDescriptionTransform.Position,	m_tMidiDescriptionTransform.Size,	m_vMidiDescriptionColour, FontManager::DANCING);	// Print Midi Description
		FontManager::GetInstance()->Print(m_sMidiFilenameDisplay,	m_tMidiFilenameTransform.Position,		m_tMidiFilenameTransform.Size,		m_vMidiFilenameColour);								// Print Midi Filename
		FontManager::GetInstance()->Print(m_sAudioDescription,		m_tAudioDescriptionTransform.Position,	m_tAudioDescriptionTransform.Size,	m_vAudioDescriptionColour, FontManager::DANCING);	// Print Audio Description
		FontManager::GetInstance()->Print(m_sAudioFilenameDisplay,	m_tAudioFilenameTransform.Position,		m_tAudioFilenameTransform.Size,		m_vAudioFilenameColour);							// Print Audio Filename

		m_MidiFileChangeButton.Draw();
		m_AudioFileChangeButton.Draw();
		m_EnterNextSceneButton.Draw();
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Setup Midi Font
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_Select::_SetupMidiFont()
{
	m_sMidiDescription						 = "Select a Midi File to Read From: ";
	m_sMidiFilenameDisplay					 = "(NONE SELECTED)";

	m_tMidiDescriptionTransform.Size		 = Screen::ScaleToScreenSize(Vector2(40.0f, 40.0f));
	m_tMidiDescriptionTransform.Position.x	 = ((float)Screen::GetWidth() * 0.5f) - (FontManager::GetInstance()->GetTotalDrawLength(m_sMidiDescription, m_tMidiDescriptionTransform.Size) * 0.5f);
	m_tMidiDescriptionTransform.Position.y	 = 50.0f;

	m_tMidiFilenameTransform.Position		 = m_tMidiDescriptionTransform.Position;
	m_tMidiFilenameTransform.Position.y		+= (m_tMidiDescriptionTransform.Size.y * 2.5f);
	m_tMidiFilenameTransform.Size			 = m_tMidiDescriptionTransform.Size * 0.8f;

	m_vMidiDescriptionColour				 = Vector4(200.0f / 255.0f, 0.0f / 255.0f, 200.0f / 255.0f, 1.0f);
	m_vMidiFilenameColour					 = Vector4(255.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 1.0f);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Setup Audio Font
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_Select::_SetupAudioFont()
{
	m_sAudioDescription						 = "Select an Audio File to Play: ";
	m_sAudioFilenameDisplay					 = "(NONE SELECTED)";

	m_tAudioDescriptionTransform.Size		 = Screen::ScaleToScreenSize(Vector2(40.0f, 40.0f));
	m_tAudioDescriptionTransform.Position.x	 = ((float)Screen::GetWidth() * 0.5f) - (FontManager::GetInstance()->GetTotalDrawLength(m_sAudioDescription, m_tAudioDescriptionTransform.Size) * 0.5f);
	m_tAudioDescriptionTransform.Position.y	 = 350.0f;

	m_tAudioFilenameTransform.Position		 = m_tAudioDescriptionTransform.Position;
	m_tAudioFilenameTransform.Position.y	+= (m_tAudioDescriptionTransform.Size.y * 2.5f);
	m_tAudioFilenameTransform.Size			 = m_tAudioDescriptionTransform.Size * 0.8f;

	m_vAudioDescriptionColour				 = Vector4(0.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f);
	m_vAudioFilenameColour					 = Vector4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Setup Textures
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_Select::_SetupButtons()
{
	Vector2 vEndOfMidiDescription  = (m_tMidiDescriptionTransform.Position + FontManager::GetInstance()->GetTotalDrawLengthAsVector2(m_sMidiDescription, m_tMidiDescriptionTransform.Size));
	Vector2 vEndOfAudioDescription = (m_tAudioDescriptionTransform.Position + FontManager::GetInstance()->GetTotalDrawLengthAsVector2(m_sAudioDescription, m_tAudioDescriptionTransform.Size));

	// Change Midi Button Setup
	TransformDetails2D tMidiFileChangeTransform;
	tMidiFileChangeTransform.Size = Vector2(Screen::ScaleToScreenWidth(150.0f), m_tMidiFilenameTransform.Size.y);
	tMidiFileChangeTransform.Position.x = (vEndOfMidiDescription - tMidiFileChangeTransform.Size).x;
	tMidiFileChangeTransform.Position.y = m_tMidiFilenameTransform.Position.y;

	m_MidiFileChangeButton.SetButtonTransform(tMidiFileChangeTransform);
	m_MidiFileChangeButton.SetButtonMainColour( Vector4(0.9f, 0.6f, 0.4f, 1.0f) );
	m_MidiFileChangeButton.SetButtonGradientClour( m_MidiFileChangeButton.GetButtonMainColour() * 0.5f );
	m_MidiFileChangeButton.SetButtonHoverColour( Vector4(0.0f, 0.0f, 1.0f, 1.0f) );
	m_MidiFileChangeButton.SetButtonText("Change");

	// Change Audio Button Setup
	TransformDetails2D tAudioFileChangeTransform;
	tAudioFileChangeTransform.Size = Vector2(Screen::ScaleToScreenWidth(150.0f), m_tAudioFilenameTransform.Size.y);
	tAudioFileChangeTransform.Position.x = (vEndOfAudioDescription - tAudioFileChangeTransform.Size).x;
	tAudioFileChangeTransform.Position.y = m_tAudioFilenameTransform.Position.y;

	m_AudioFileChangeButton.SetButtonTransform(tAudioFileChangeTransform);
	m_AudioFileChangeButton.SetButtonMainColour( Vector4(0.9f, 0.6f, 0.4f, 1.0f) );
	m_AudioFileChangeButton.SetButtonGradientClour( m_AudioFileChangeButton.GetButtonMainColour() );
	m_AudioFileChangeButton.SetButtonHoverColour( Vector4(0.0f, 0.0f, 1.0f, 1.0f) );
	m_AudioFileChangeButton.SetButtonText("Change");

	// Enter Next Scene Button Setup
	TransformDetails2D tNextSceneButtonTransform;
	tNextSceneButtonTransform.Size = Screen::ScaleToScreenSize(Vector2(145.0f, 145.0f));
	tNextSceneButtonTransform.Position = Vector2( (float)Screen::GetWidth() - (tNextSceneButtonTransform.Size.x * 1.2f), (float)Screen::GetHeight() - (tNextSceneButtonTransform.Size.y * 1.05f) );
	
	m_EnterNextSceneButton.SetButtonTransform(tNextSceneButtonTransform);
	m_EnterNextSceneButton.SetButtonMainColour( Vector4(1.0f, 1.0f, 1.0f, 1.0f) );
	m_EnterNextSceneButton.SetButtonGradientClour( Vector4(1.0f, 1.0f, 1.0f, 1.0f) );
	m_EnterNextSceneButton.SetButtonHoverColour( Vector4(1.0f, 0.0f, 1.0f, 1.0f) );
	m_EnterNextSceneButton.SetButtonTexture("Arrow.png");
	m_EnterNextSceneButton.SetButtonText("Next");
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set New Midi File Text
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_Select::_SetNewMidiFileText(std::string& Text)
{
	m_tMidiFilenameTransform.Size = m_tMidiDescriptionTransform.Size * 0.8f;

	if( m_tMidiFilenameTransform.Position.x + FontManager::GetInstance()->GetTotalDrawLength(Text, m_tMidiFilenameTransform.Size) > m_MidiFileChangeButton.GetButtonPosition().x )
	{
		m_tMidiFilenameTransform.Size *= (m_MidiFileChangeButton.GetButtonPosition().x / (m_tMidiFilenameTransform.Position.x + FontManager::GetInstance()->GetTotalDrawLength(Text, m_tMidiFilenameTransform.Size)));
		m_tMidiFilenameTransform.Size *= 0.8f;
	}

	m_sMidiFilenameDisplay = Text;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set New Audio File Text
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_Select::_SetNewAudioFileText(std::string& Text)
{
	m_tAudioFilenameTransform.Size = m_tAudioDescriptionTransform.Size * 0.8f;

	if( m_tAudioFilenameTransform.Position.x + FontManager::GetInstance()->GetTotalDrawLength(Text, m_tAudioFilenameTransform.Size) > m_AudioFileChangeButton.GetButtonPosition().x )
	{
		m_tAudioFilenameTransform.Size *= (m_AudioFileChangeButton.GetButtonPosition().x / (m_tAudioFilenameTransform.Position.x + FontManager::GetInstance()->GetTotalDrawLength(Text, m_tAudioFilenameTransform.Size)));
		m_tAudioFilenameTransform.Size *= 0.8f;
	}

	m_sAudioFilenameDisplay = Text;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Open File Dialog Box
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::string Scene_Select::_OpenDialogBox(FileExtensionFilter a_ExtensionFilter)
{
	std::string sInput = "";

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | 
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog *pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, 
                IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		
		pFileOpen->SetFileTypes(a_ExtensionFilter.first, a_ExtensionFilter.second);

        if (SUCCEEDED(hr))
        {
            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem *pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Display the file name to the user.
                    if (SUCCEEDED(hr))
                    {
						char str[256];
						wcstombs(str, pszFilePath, 256);
						sInput = str;
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
    return sInput;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Open Midi File Dialog Box
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_Select::_OpenMidiDialogBox()
{
	COMDLG_FILTERSPEC rgSpec[] = 
		{ 
			{ L"Midi", L"*.mid;*.midi" },
		};

	std::string sNewMidiFile = _OpenDialogBox( FileExtensionFilter(1, rgSpec) );
	if( sNewMidiFile != "" && sNewMidiFile != m_sMidiFilename )
	{
		m_bHasChangedMidiFiles = true;
		m_sMidiFilename = sNewMidiFile;
		m_sAudioFilename = m_sMidiFilename;

		unsigned int uiFound = m_sMidiFilename.find_last_of('\\');
		_SetNewMidiFileText(m_sMidiFilename.substr(uiFound+1));
		_SetNewAudioFileText(m_sMidiFilenameDisplay);

		unsigned int uiAudioID = AudioManager::GetInstance()->ImportAudioFromPath(m_sAudioFilename, AudioManager::BGM);
		AudioManager::GetInstance()->PlayAudio(uiAudioID);
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Open Audio FIle Dialog Box
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_Select::_OpenAudioDialogBox()
{
	COMDLG_FILTERSPEC rgSpec[] = 
		{ 
			{ L"Audio Files", L"*.mid;*.mp3;*.ogg;*.wav;*.wma" },
		};

	std::string sNewAudioFile = _OpenDialogBox( FileExtensionFilter(1, rgSpec) );
	if( sNewAudioFile != "" && sNewAudioFile != m_sAudioFilename )
	{
		m_sAudioFilename = sNewAudioFile;

		unsigned int uiFound = m_sAudioFilename.find_last_of('\\');
		_SetNewAudioFileText(m_sAudioFilename.substr(uiFound+1));

		unsigned int uiAudioID = AudioManager::GetInstance()->ImportAudioFromPath(m_sAudioFilename, AudioManager::BGM);
		AudioManager::GetInstance()->PlayAudio(uiAudioID);
	}
}