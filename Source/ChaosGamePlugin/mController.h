#pragma once
#include "mBaseComponent.h"
#include "ChaosGamePluginPCH.h"
#include "mAiPath.h"
#include <Ai/Internal/hkaiInternal.h>
#include <Ai/Pathfinding/Character/hkaiPath.h>
#include <Ai/Pathfinding/Character/hkaiCharacter.h>
#include <Ai/Pathfinding/Character/Behavior/hkaiBehavior.h>

class hkaiCharacter;
class hkaiBehavior;

class mCharacter;
class mDamageableEntity;

class mControllerStateBase;

namespace mControllerState
{
	enum Enum
	{
		//Normal
		sNotControlled,
		sIdling,
		sMoving,
		sAttackingMelee,
		sAttackingRanged,
		sUsingSkill,

		//AI
		sProtecting,
		sWandering,
		sFleeing,
		sAskingForHelp,
		sPursuit,

		sNumStates
	};
}

class mController : public mBaseComponent
{
public:
	mController(void);

	void SetState(mControllerState::Enum newState);
	mControllerState::Enum GetState();
	mCharacter* GetCharacter() const { return vstatic_cast<mCharacter*>(GetOwner()); }

	//AI
	void RequestPath(hkvVec3 const& dest);

	VOVERRIDE hkvVec3 GetNextPointInPath();

	float GetDistanceToGoalTolerance() const;

	void SetDirection(hkvVec3 dir);
	hkvVec3 GetDirection() const { return m_speed > 0.f ? m_direction : GetCharacter()->GetDirection(); }

	float GetSpeed() const { return m_speed; }

	void SetHelpRange(float helpRange);
	void SetSenseEnemyRange(float senseRange);

	float GetWantedSpeed();

	void SetTarget(mDamageableEntity* target) { m_target = (target ? target->GetWeakReference() : NULL); }
	void SetTargetPoint(hkvVec3 const& targetPoint) { m_target_point = targetPoint; }

	VOVERRIDE bool TryMeleeAttack();
	VOVERRIDE bool TryRangedAttack();

	void onFrameUpdate(float delta) HKV_OVERRIDE;

protected:
	void SetOwner(VisTypedEngineObject_cl *owner) HKV_OVERRIDE;
	VSmartPtr<mControllerStateBase> m_states[mControllerState::sNumStates];

	bool m_changing_state;
	mControllerState::Enum m_current_state;
	mControllerState::Enum m_new_state;

	mAiPath m_full_path;
	mAiPath m_current_path;

	float m_speed; 
	hkvVec3 m_direction;
	
	float m_help_range; 
	float m_sense_range; 

	VWeakPtr<VisBaseEntity_cl> m_target;
	hkvVec3 m_target_point;

private:
	V_DECLARE_SERIAL_DLLEXP( mController, SAMPLEPLUGIN_IMPEXP );
	V_DECLARE_VARTABLE( mController, SAMPLEPLUGIN_IMPEXP );
};

class mControllerStateBase : public VRefCounter
{
public:
	VOVERRIDE void OnEnter(mController *controller) {}
	VOVERRIDE void OnExit(mController *controller) {}
	VOVERRIDE void OnTick(mController *controller, float delta) {}

	VOVERRIDE char const *GetName() const = 0;
};

namespace mControllerState
{
	class NotControlled : public mControllerStateBase
	{
		char const *GetName() const HKV_OVERRIDE { return "Controller::NotControlled"; }
	};
}

//this part of the util is copied from RPG_Game by Havok
namespace mUtil
{
	void CalcDirection(hkvVec3& resultDir, hkvVec3 const& currentDir, hkvVec3 const& desiredDir, float t);

	//float GetMinimumDistanceToAttack(mCharacter const *character, mDamageableEntity *const target);
}