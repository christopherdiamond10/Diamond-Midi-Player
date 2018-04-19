//\====================================================================================
//\ Author: Christopher.Allport
//\ About : DeltaTime.h - 
//\ 
//\ Defines the DeltaTime Singleton Class. This class is used to give frame independent 
//\ movement to the game, it is more or less a hub for giving you a timer and a method 
//\ for slowing down speed using time.
//\====================================================================================
#ifndef _DELTA_TIME_H_
#define _DELTA_TIME_H_

class DeltaTime
{
//////////////////////////////////////////
public:
//////////////////////////////////////////
	DeltaTime();
	~DeltaTime();

	static DeltaTime* GetInstance();

	void OnLoad();
	void OnUnload();

	float GetDeltaTime();
	float GetTimeFromLastDeltaUpdate();
	float GetTotalTime();
	float GetTrackTime();
	float GetDeltaTimeNormalisedValue( float fNormaliseThisValue );
	bool  Get_HasThisFrameExceededFrameLimit();

	void Print_DeltaTime();
	void UpdateDeltaTime();
	void ResetTrackTime();


//////////////////////////////////////////
private:
//////////////////////////////////////////
	
	static DeltaTime* m_Instance;

	const float CONST_F_FIXED_FRAME_TIME;
	const float CONST_F_EXPECTED_FRAMES_PER_SECOND;
	double m_dCurr_Ticks;
	double m_dPrev_Ticks;
	float m_fDeltaTime;
	float m_fTotalTime;
	float m_fTrackTime;
};


#endif