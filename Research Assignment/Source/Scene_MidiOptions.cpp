#include "Scene_midiOptions.h"

#include "MidiSequencer.h"
#include "Screen.h"
#include <Windows.h>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ** Constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Scene_MidiOptions::Scene_MidiOptions(SceneManager* pSceneMan)
	: Scene_Base()
	, m_ReturnSceneButton()
	, m_PlayGameButton()
{
	m_pSceneManager = pSceneMan;
	m_paChannelButtons = new MidiChannelButton[16];
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// *~ Deconstructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Scene_MidiOptions::~Scene_MidiOptions()
{
	delete[] m_paChannelButtons;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// * Redefined Method: On Load
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_MidiOptions::OnLoad()
{

}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// * Redefined Method: On Unload
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_MidiOptions::OnUnload()
{

}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// * Redefined Method: Scene Start
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_MidiOptions::SceneStart()
{
	_SetupButtonPositions();
	_SetupButtonChannelInfo();
	_SetupButtonColours();
	_SetupSceneChangingButtons();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// * Redefined Method: Scene Terminate
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_MidiOptions::SceneTerminate()
{

}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// * Redefined Method: Update
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_MidiOptions::Update()
{
	// Update Track Buttons
	for( int i = 0; i < 16; ++i )
	{
		if( m_paChannelButtons[i].eChannelActivity != MidiSequencer::NOT_AVAILABLE )
		{
			m_paChannelButtons[i].Button.Update();
			if( m_paChannelButtons[i].Button.WasClicked() )
			{
				if( m_paChannelButtons[i].eChannelActivity == MidiSequencer::ACTIVE )
				{
					m_paChannelButtons[i].eChannelActivity = MidiSequencer::INACTIVE;
					MidiSequencer::GetInstance()->SetChannelActivity(i, m_paChannelButtons[i].eChannelActivity);

					Vector4 vButtonColour = m_paChannelButtons[i].Button.GetButtonMainColour();
					vButtonColour.w = 0.5f;
					m_paChannelButtons[i].Button.SetButtonMainColour(vButtonColour);
				}
				else
				{
					m_paChannelButtons[i].eChannelActivity = MidiSequencer::ACTIVE;
					MidiSequencer::GetInstance()->SetChannelActivity(i, m_paChannelButtons[i].eChannelActivity);

					Vector4 vButtonColour = m_paChannelButtons[i].Button.GetButtonMainColour();
					vButtonColour.w = 1.0f;
					m_paChannelButtons[i].Button.SetButtonMainColour(vButtonColour);
				}

				break;
			}
		}
	}


	// Update Scene Buttons
	m_PlayGameButton.Update();
	m_ReturnSceneButton.Update();

	if( m_PlayGameButton.WasClicked() )
	{
		bool bCanContinue = false;
		for(int i = 0; i < 16; ++i)
		{
			if( m_paChannelButtons[i].eChannelActivity == MidiSequencer::ACTIVE )
			{
				bCanContinue = true;
				break;
			}
		}

		if( bCanContinue )
		{
			m_pSceneManager->ChangeScene(SceneManager::PLAYGAME_SCENE);
		}
		else
		{
			MessageBox(NULL, L"You Must Have at least one channel active before you can continue!", L"ERROR", 64);
		}
	}
	else if(m_ReturnSceneButton.WasClicked())
	{
		m_pSceneManager->ChangeScene(SceneManager::SELECT_SCENE);
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// * Redefined Method: Draw
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_MidiOptions::Draw()
{
	for( int i = 0; i < 16; ++i )
	{
		m_paChannelButtons[i].Button.Draw();
	}

	m_PlayGameButton.Draw();
	m_ReturnSceneButton.DrawReversed();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// * New Method: Setup Button Positions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_MidiOptions::_SetupButtonPositions()
{
	float fStartX = Screen::ScaleToScreenWidth(80.0f);
	Vector2 vDrawPosition( Vector2(fStartX, Screen::ScaleToScreenHeight(50.0f)) );
	Vector2 vDrawSize( Screen::ScaleToScreenSize(Vector2(250.0f, 120.0f)) );
	for( int i = 0; i < 16; ++i )
	{
		if( i > 0 && (i % 4) == 0 )
		{
			vDrawPosition.x  = fStartX;
			vDrawPosition.y += (vDrawSize.y * 1.1f);
		}

		m_paChannelButtons[i].Button.SetButtonPosition(vDrawPosition);
		m_paChannelButtons[i].Button.SetButtonSize(vDrawSize);

		vDrawPosition.x += (vDrawSize.x * 1.1f);
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// * New Method: Setup Button Channel Info
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_MidiOptions::_SetupButtonChannelInfo()
{
	for( int i = 0; i < 16; ++i )
	{
		m_paChannelButtons[i].eChannelActivity = MidiSequencer::GetInstance()->GetChannelActivity(i);
		m_paChannelButtons[i].Button.ResetButtonTextSize();
		
		if( m_paChannelButtons[i].eChannelActivity == MidiSequencer::NOT_AVAILABLE )
			m_paChannelButtons[i].Button.SetButtonText("Unavailable");
		else
			m_paChannelButtons[i].Button.SetButtonText( (MidiSequencer::GetInstance()->GetChannelName(i) != "") ? MidiSequencer::GetInstance()->GetChannelName(i) : _PlaceHolderTrackName(i) );
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// * New Method: Setup Button Colours
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_MidiOptions::_SetupButtonColours()
{
	for( int i = 0; i < 16; ++i )
	{
		m_paChannelButtons[i].Button.SetButtonGradientClour( MidiSequencer::GetInstance()->GetChannelColour(i) * 0.5f );
		m_paChannelButtons[i].Button.SetButtonHoverColour( Vector4(0.0f, 0.0f, 1.0f, 1.0f) );

		if( m_paChannelButtons[i].eChannelActivity == MidiSequencer::NOT_AVAILABLE )
		{
			m_paChannelButtons[i].Button.SetButtonMainColour( Vector4( 0.2f, 0.2f, 0.2f, 0.6f ) );
		}
		else
		{
			if(m_paChannelButtons[i].eChannelActivity == MidiSequencer::ACTIVE)
			{
				m_paChannelButtons[i].Button.SetButtonMainColour( MidiSequencer::GetInstance()->GetChannelColour(i) );
			}
			else
			{
				Vector4 vColour = MidiSequencer::GetInstance()->GetChannelColour(i);
				vColour.w = 0.5f;
				m_paChannelButtons[i].Button.SetButtonMainColour( vColour );
			}
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// * New Method: Setup Scene Changing Buttons
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene_MidiOptions::_SetupSceneChangingButtons()
{
	TransformDetails2D tPlaySceneButtonTransform;
	tPlaySceneButtonTransform.Size = Screen::ScaleToScreenSize(Vector2(145.0f, 145.0f));
	tPlaySceneButtonTransform.Position = Vector2( (float)Screen::GetWidth() - (tPlaySceneButtonTransform.Size.x * 1.2f), (float)Screen::GetHeight() - (tPlaySceneButtonTransform.Size.y * 1.05f) );

	m_PlayGameButton.SetButtonTransform(tPlaySceneButtonTransform);
	m_PlayGameButton.SetButtonMainColour( Vector4(1.0f, 1.0f, 1.0f, 1.0f) );
	m_PlayGameButton.SetButtonGradientClour( Vector4(1.0f, 1.0f, 1.0f, 1.0f) );
	m_PlayGameButton.SetButtonHoverColour( Vector4(1.0f, 0.0f, 1.0f, 1.0f) );
	m_PlayGameButton.SetButtonTexture("Arrow.png");
	m_PlayGameButton.SetButtonText("Play");


	TransformDetails2D tReturnSceneButtonTransform;
	tReturnSceneButtonTransform.Size = Screen::ScaleToScreenSize(Vector2(145.0f, 145.0f));
	tReturnSceneButtonTransform.Position = Vector2( (tReturnSceneButtonTransform.Size.x * 0.2f), (float)Screen::GetHeight() - (tReturnSceneButtonTransform.Size.y * 1.05f) );

	m_ReturnSceneButton.SetButtonTransform(tReturnSceneButtonTransform);
	m_ReturnSceneButton.SetButtonMainColour( Vector4(1.0f, 1.0f, 1.0f, 1.0f) );
	m_ReturnSceneButton.SetButtonGradientClour( Vector4(1.0f, 1.0f, 1.0f, 1.0f) );
	m_ReturnSceneButton.SetButtonHoverColour( Vector4(1.0f, 0.0f, 1.0f, 1.0f) );
	m_ReturnSceneButton.SetButtonTexture("Arrow.png");
	m_ReturnSceneButton.SetButtonText("Return");
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// * New Method: Get PlaceHolder Track Name
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::string Scene_MidiOptions::_PlaceHolderTrackName(int i)
{
	switch(i)
	{
	case 0:  return "Track1";
	case 1:  return "Track2";
	case 2:  return "Track3";
	case 3:  return "Track4";
	case 4:  return "Track5";
	case 5:  return "Track6";
	case 6:  return "Track7";
	case 7:  return "Track8";
	case 8:  return "Track9";
	case 9:  return "Track10";
	case 10: return "Track11";
	case 11: return "Track12";
	case 12: return "Track13";
	case 13: return "Track14";
	case 14: return "Track15";
	default: return "Track16";
	}
}