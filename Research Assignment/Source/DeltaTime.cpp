#include "DeltaTime.h"

#include "glfw.h"
#include <Windows.h>
#include <iostream>
#include <assert.h>




DeltaTime* DeltaTime::m_Instance = NULL;

//===================================
//		Get Class Instance
//===================================
DeltaTime* DeltaTime::GetInstance()
{
	return m_Instance;
}


//===================================
//		Constructor
//===================================
DeltaTime::DeltaTime() 
	: CONST_F_EXPECTED_FRAMES_PER_SECOND ( 60.f )
	, CONST_F_FIXED_FRAME_TIME			 ((1.f / 60.f))
{
	if( m_Instance == NULL )
	{
		m_Instance = this;
		OnLoad();
	}
	else
	{	assert(0);	}
}

//===================================
//		Destructor
//===================================
DeltaTime::~DeltaTime()
{
	m_Instance = nullptr;
}





//===================================
//		OnLoad
//===================================
void DeltaTime::OnLoad()
{
	m_dCurr_Ticks = 0.0;
	m_dPrev_Ticks = 0.0;
	m_fDeltaTime  = 0.f;
	m_fTotalTime  = 0.f;
	m_fTrackTime  = 0.f;

	UpdateDeltaTime();
}


//===================================
//		OnUnload
//===================================
void DeltaTime::OnUnload()
{

}
//===================================
//		Reset Track Time
//===================================
void DeltaTime::ResetTrackTime()
{
	m_fTrackTime = 0.0f;
}







//===================================
//		Get Delta Time
//===================================
float DeltaTime::GetDeltaTime()
{
	return m_fDeltaTime;
}
//===================================
//	  Get Time From Last Update
//===================================
float DeltaTime::GetTimeFromLastDeltaUpdate()
{
	return (float)(glfwGetTime() - m_dCurr_Ticks);
}
//===================================
//		Get Total Time
//===================================
float DeltaTime::GetTotalTime()
{
	return m_fTotalTime;
}
//===================================
//		Get Track Time
//===================================
float DeltaTime::GetTrackTime()
{
	return m_fTrackTime;
}
//===================================
//	Get Delta Time Normalised Value
//===================================
float DeltaTime::GetDeltaTimeNormalisedValue( float fNormaliseThisValue )
{
	return( fNormaliseThisValue * CONST_F_EXPECTED_FRAMES_PER_SECOND );
}
//===================================
//	Has This Frame Exceeded Limit?
//===================================
bool DeltaTime::Get_HasThisFrameExceededFrameLimit()
{
	return GetTimeFromLastDeltaUpdate() > CONST_F_FIXED_FRAME_TIME;
}


//===================================
//		Update Delta Time
//===================================
void DeltaTime::UpdateDeltaTime()
{
	m_dCurr_Ticks	= glfwGetTime();
	m_fDeltaTime	= (float)(m_dCurr_Ticks - m_dPrev_Ticks);
	m_dPrev_Ticks	= m_dCurr_Ticks;
	m_fTotalTime   += m_fDeltaTime;
	m_fTrackTime   += m_fDeltaTime;
	
	//// Don't OverExert DeltaTime... this can be caused by simply not using the program, thus blocking DeltaTime updates and spiking the results
	//if( m_DeltaTime::GetInstance()->GetDeltaTime() > CONST_F_FIXED_FRAME_TIME )
	//{
	//	// It's simply better to have everything not update for one frame if this is the case
	//	m_DeltaTime::GetInstance()->GetDeltaTime() = CONST_F_FIXED_FRAME_TIME;
	//}
}






//===================================
//		Print Delta Time
//===================================
void DeltaTime::Print_DeltaTime()
{
	std::cout << "Current Ticks			= " << m_dCurr_Ticks << std::endl;
	std::cout << "Previous Ticks			= " << m_dPrev_Ticks << std::endl;
	std::cout << "Time Since last check		= ";
	std::cout << m_fDeltaTime << std::endl << std::endl;
}
