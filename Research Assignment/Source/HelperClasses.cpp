#include "HelperClasses.h"

#include "DeltaTime.h"



//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Class: TimeTracker
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HelperClasses::TimeTracker::TimeTracker(float a_fTotalWaitTime)
	: m_fCurrentTime(0.0f)
	, m_fWaitTimer(a_fTotalWaitTime)
	, m_bTimeUp(false)
{
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	*~ Deconstructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HelperClasses::TimeTracker::~TimeTracker()
{
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Reset Timer
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void HelperClasses::TimeTracker::Reset()
{
	m_fCurrentTime = 0.0f;
	m_bTimeUp = false;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Methods: Update Timer
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void HelperClasses::TimeTracker::Update()
{
	if( !TimeUp() )
	{
		m_fCurrentTime += DeltaTime::GetInstance()->GetDeltaTime();

		if( CheckIfTimeIsUp() )
		{
			m_fCurrentTime = m_fWaitTimer;
			m_bTimeUp = true;
		}
	}
}

void HelperClasses::TimeTracker::Update(float TimeMultiplier)
{
	if( !TimeUp() )
	{
		m_fCurrentTime += DeltaTime::GetInstance()->GetDeltaTime() * TimeMultiplier;

		if( CheckIfTimeIsUp() )
		{
			m_fCurrentTime = m_fWaitTimer;
			m_bTimeUp = true;
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Check if Time Is Up
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool HelperClasses::TimeTracker::CheckIfTimeIsUp()
{
	return (m_fCurrentTime > m_fWaitTimer);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Is Time Up?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool HelperClasses::TimeTracker::TimeUp()
{
	return m_bTimeUp;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Completion Percentage
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
float HelperClasses::TimeTracker::GetCompletionPercentage()
{
	return (m_fCurrentTime / m_fWaitTimer);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Current Time
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void HelperClasses::TimeTracker::SetCurrentTime(float a_fTime)
{
	m_fCurrentTime = a_fTime;
	m_bTimeUp = false;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Set Wait Time
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void HelperClasses::TimeTracker::SetWaitTime(float a_fTime)
{
	m_fWaitTimer = a_fTime;
	m_bTimeUp = false;
}


















//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//				Class: MovementBasedOnTime
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	** Constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HelperClasses::MovementBasedOnTime::MovementBasedOnTime(const Vector2& a_rvStartPosition, const Vector2& a_rvEndPosition, float a_fSeconds)
		: m_TTMovementTime(a_fSeconds)
{
	SetupMovement(a_rvStartPosition, a_rvEndPosition, a_fSeconds);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	*~ Deconstructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HelperClasses::MovementBasedOnTime::~MovementBasedOnTime()
{
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Setup Movement
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void HelperClasses::MovementBasedOnTime::SetupMovement(const Vector2& a_rvStartPosition, const Vector2& a_rvEndPosition, float a_fSeconds)
{
	m_vStartPosition	= a_rvStartPosition;
	m_vTargetPosition	= a_rvEndPosition;
	m_TTMovementTime.SetWaitTime(a_fSeconds);

	Reset();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Reset
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void HelperClasses::MovementBasedOnTime::Reset()
{
	GetTimeInstance().Reset();
	m_vCurrentPosition = m_vStartPosition;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Update
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void HelperClasses::MovementBasedOnTime::Update()
{
	GetTimeInstance().Update();
	m_vCurrentPosition = m_vStartPosition + ((m_vTargetPosition - m_vStartPosition) * GetTimeInstance().GetCompletionPercentage());
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Has Reached Destination?
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool HelperClasses::MovementBasedOnTime::HasReachedDestination()
{
	return GetTimeInstance().TimeUp();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Current Position
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const Vector2& HelperClasses::MovementBasedOnTime::GetCurrentPosition()
{
	return m_vCurrentPosition;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	* New Method: Get Time Tracker Instance
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HelperClasses::TimeTracker& HelperClasses::MovementBasedOnTime::GetTimeInstance()
{
	return m_TTMovementTime;
}