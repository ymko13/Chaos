#include "ChaosGamePluginPCH.h"
#include "mController.h"
#include <Vision/Runtime/EnginePlugins/Havok/HavokPhysicsEnginePlugin/vHavokConversionUtils.hpp>
#include <Ai/Pathfinding/Character/Behavior/hkaiPathFollowingBehavior.h>

V_IMPLEMENT_SERIAL(mController, mBaseComponent, 0, &g_myComponentModule);

START_VAR_TABLE(mController, mBaseComponent, "Basic Controller", VVARIABLELIST_FLAGS_NONE, "mController")
	DEFINE_VAR_ENUM_AND_NAME(mController, m_new_state, "Current Task", "The current task", "sNotControlled",
		"sIdling/sMoving/sAttackingMelee/sAttackingRanged/sUsingSkill/sProtecting/sWandering/sFleeing/sAskingForHelp/sPursuit", 0, 0);
END_VAR_TABLE

mController::mController(void)
	: mBaseComponent(),
	m_current_state(mControllerState::sNotControlled),
	m_new_state(mControllerState::sNotControlled),
	m_changing_state(false),
	m_sense_range(100.f),
	m_help_range(250.f),
	m_direction(0.f),
	m_target(NULL)
{
}

void mController::SetOwner(VisTypedEngineObject_cl *pOwner)
{
	mBaseComponent::SetOwner(pOwner);
	if(pOwner!=NULL)
	{ 
		if(pOwner)
		{
			// Create states
			m_states[mControllerState::sNotControlled] = new mControllerState::NotControlled();
		}
	}
	else
	{

	}
}

void mController::SetState(mControllerState::Enum newState)
{
	VASSERT(!m_changing_state);
	if(!m_changing_state && m_current_state != newState)
	{
		m_changing_state = true;

		mControllerStateBase *currentState = m_states[m_current_state];
		if(currentState)
		{
			currentState->OnExit(this);
		}

		m_current_state = newState;

		currentState = m_states[m_current_state];

		if(currentState)
		{
			currentState->OnEnter(this);
		}

		Vision::GetConsoleManager()->OutputTextLine(currentState->GetName());

		m_changing_state = false;
	}
}

mControllerState::Enum mController::GetState()
{
	return m_current_state;
}

void mController::RequestPath(hkvVec3 const& dest)
{
	if (!GetOwner()) return;
	mCharacter* charC = (mCharacter *)GetOwner();

	m_full_path.SetPath(mMap::m_full_path); 
	
	hkvVec3 curPos = charC->GetPosition();
	
	int spIndex, epIndex;
	hkvVec3 spPoint, epPoint;
	m_full_path.GetClosestPoint(spIndex, curPos, spPoint);
	m_full_path.GetClosestPoint(epIndex, dest, epPoint);

	SerialDynArray_cl<hkvVec3> new_path = SerialDynArray_cl<hkvVec3>();

	if(spIndex > epIndex)
	{
		for(int i = epIndex; i < spIndex; i++)
		{
			new_path[new_path.GetFreePos()] = m_full_path.GetAt(i);
		}
	}
	else
	{
		for(int i = epIndex; i > spIndex; i--)
		{
			new_path[new_path.GetFreePos()] = m_full_path.GetAt(i);
		}
	}
	
	mAiPath path = mAiPath();
	path.SetPath(new_path);

	m_current_path = path;
}

void mController::SetDirection(hkvVec3 dir)
{
	m_direction = dir;
}

hkvVec3 mController::GetNextPointInPath()
{	
	if (!GetOwner()) return hkvVec3(0);
	mCharacter* charC = (mCharacter *)GetOwner();
	
	hkvVec3 curPos = charC->GetPosition();
	hkvVec3 curPoint = m_current_path.GetCurrentPoint();
	float dist = curPos.getDistanceTo(curPoint);
	VString s;
	s.FormatEx("Distance: %d to X: %d Y: %d", (int)dist, (int)curPoint.x, (int)curPoint.y);

	Vision::GetConsoleManager()->OutputTextLine(s);
	hkvVec3 next(0);
	if(dist < GetDistanceToGoalTolerance())
	{
		hkvVec3 next = m_current_path.GetNextPoint();
		if(next.isIdentical(curPoint))
		{
			this->SetState(mControllerState::sNotControlled);
		}
	}
	else
	{
		next = curPoint;
	}

	return next;
}

float mController::GetDistanceToGoalTolerance() const
{
	return 15.f;
}

void mController::SetHelpRange(float helpRange)
{
	m_help_range = helpRange;
}

void mController::SetSenseEnemyRange(float senseRange)
{
	m_sense_range = senseRange;
}

float mController::GetWantedSpeed()
{
	if (!GetOwner()) return 0.f;
	mCharacter* charC = (mCharacter *)GetOwner();
	return (float)charC->GetCharacterStats().GetMovementSpeed();
}

void mController::onFrameUpdate(float delta)
{
	mBaseComponent::onFrameUpdate(delta);

	if(m_new_state != m_current_state)
	{
		this->SetState(m_new_state);
	}

	if(m_states[m_current_state])
	{
		m_states[m_current_state]->OnTick(this, delta);
	}
}

bool mController::TryMeleeAttack()
{
	return true;
}
bool mController::TryRangedAttack()
{
	return false;
}

void mUtil::CalcDirection(hkvVec3& resultDir, hkvVec3 const& currentDir, hkvVec3 const& desiredDir, float t)
{
  if(currentDir.dot(desiredDir) < 0.99f)
  {
    float dot = currentDir.dot(desiredDir);
    float theta = hkvMath::acosRad(dot) * hkvMath::clamp(t, 0.f, 1.f);
    hkvVec3 vec = (desiredDir - currentDir * dot).getNormalized();
    resultDir = currentDir * hkvMath::cosRad(theta) + vec * hkvMath::sinRad(theta);
  }
  else
  {
    resultDir = desiredDir;
  }
}