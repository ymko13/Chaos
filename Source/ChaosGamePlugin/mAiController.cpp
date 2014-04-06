#include "ChaosGamePluginPCH.h"
#include "mAiController.h"

V_IMPLEMENT_SERIAL(mAiController, mController, 0, &g_myComponentModule);

START_VAR_TABLE(mAiController, mController, "Basic AI Controller", VVARIABLELIST_FLAGS_NONE, "mAiController")
	DEFINE_VAR_VSTRING_AND_NAME(mAiController, m_function_calls[mControllerState::sIdling][0], "Idling Enter", "", "AiIdling_Enter", 100, 0,0);
	DEFINE_VAR_VSTRING_AND_NAME(mAiController, m_function_calls[mControllerState::sIdling][1], "Idling Exit", "", "AiIdling_Exit", 100, 0,0);
	DEFINE_VAR_VSTRING_AND_NAME(mAiController, m_function_calls[mControllerState::sIdling][2], "Idling Update", "", "AiIdling_Update", 100, 0,0);
	
	DEFINE_VAR_VSTRING_AND_NAME(mAiController, m_function_calls[mControllerState::sMoving][0], "Moving Enter", "", "AiMoving_Enter", 100, 0,0);
	DEFINE_VAR_VSTRING_AND_NAME(mAiController, m_function_calls[mControllerState::sMoving][1], "Moving Exit", "", "AiMoving_Exit", 100, 0,0);
	DEFINE_VAR_VSTRING_AND_NAME(mAiController, m_function_calls[mControllerState::sMoving][2], "Moving Update", "", "AiMoving_Update", 100, 0,0);

	
	DEFINE_VAR_VSTRING_AND_NAME(mAiController, m_function_calls[mControllerState::sAttackingMelee][0], "Attacking Melee Enter", "", "AiAttackM_Enter", 100, 0,0);
	DEFINE_VAR_VSTRING_AND_NAME(mAiController, m_function_calls[mControllerState::sAttackingMelee][1], "Attacking Melee Exit", "", "AiAttackM_Exit", 100, 0,0);
	DEFINE_VAR_VSTRING_AND_NAME(mAiController, m_function_calls[mControllerState::sAttackingMelee][2], "Attacking Melee Update", "", "AiAttackM_Update", 100, 0,0);

	
	DEFINE_VAR_VSTRING_AND_NAME(mAiController, m_function_calls[mControllerState::sAttackingRanged][0], "Attacking Ranged Enter", "", "AiAttackR_Enter", 100, 0,0);
	DEFINE_VAR_VSTRING_AND_NAME(mAiController, m_function_calls[mControllerState::sAttackingRanged][1], "Attacking Ranged Exit", "", "AiAttackR_Exit", 100, 0,0);
	DEFINE_VAR_VSTRING_AND_NAME(mAiController, m_function_calls[mControllerState::sAttackingRanged][2], "Attacking Ranged Update", "", "AiAttackR_Update", 100, 0,0);

	
	DEFINE_VAR_VSTRING_AND_NAME(mAiController, m_function_calls[mControllerState::sProtecting][0], "Protecting Enter", "", "AiProtect_Enter", 100, 0,0);
	DEFINE_VAR_VSTRING_AND_NAME(mAiController, m_function_calls[mControllerState::sProtecting][1], "Protecting Exit", "", "AiProtect_Exit", 100, 0,0);
	DEFINE_VAR_VSTRING_AND_NAME(mAiController, m_function_calls[mControllerState::sProtecting][2], "Protecting Update", "", "AiProtect_Update", 100, 0,0);
	
	DEFINE_VAR_VSTRING_AND_NAME(mAiController, m_function_calls[mControllerState::sWandering][0], "Wandering Enter", "", "AiWander_Enter", 100, 0,0);
	DEFINE_VAR_VSTRING_AND_NAME(mAiController, m_function_calls[mControllerState::sWandering][1], "Wandering Exit", "", "AiWander_Exit", 100, 0,0);
	DEFINE_VAR_VSTRING_AND_NAME(mAiController, m_function_calls[mControllerState::sWandering][2], "Wandering Update", "", "AiWander_Update", 100, 0,0);
	
	DEFINE_VAR_VSTRING_AND_NAME(mAiController, m_function_calls[mControllerState::sFleeing][0], "Fleeing Enter", "", "AiFleeing_Enter", 100, 0,0);
	DEFINE_VAR_VSTRING_AND_NAME(mAiController, m_function_calls[mControllerState::sFleeing][1], "Fleeing Exit", "", "AiFleeing_Exit", 100, 0,0);
	DEFINE_VAR_VSTRING_AND_NAME(mAiController, m_function_calls[mControllerState::sFleeing][2], "Fleeing Update", "", "AiFleeing_Update", 100, 0,0);
	
	DEFINE_VAR_VSTRING_AND_NAME(mAiController, m_function_calls[mControllerState::sAskingForHelp][0], "Help Enter", "", "AiHelp_Enter", 100, 0,0);
	DEFINE_VAR_VSTRING_AND_NAME(mAiController, m_function_calls[mControllerState::sAskingForHelp][1], "Help Exit", "", "AiHelp_Exit", 100, 0,0);
	DEFINE_VAR_VSTRING_AND_NAME(mAiController, m_function_calls[mControllerState::sAskingForHelp][2], "Help Update", "", "AiHelp_Update", 100, 0,0);

	DEFINE_VAR_ENUM_AND_NAME(mAiController, m_current_state, "Current Task", "The current task", "sNotControlled",
		"sIdling/sMoving/sAttackingMelee/sAttackingRanged/sUsingSkill/sProtecting/sWandering/sFleeing/sAskingForHelp/sPursuit", 0, 0);
END_VAR_TABLE

mAiController::mAiController(void)
: mController(),
 m_function_calls(mControllerState::sNumStates)
{
	for(int i = 0; i < mControllerState::sNumStates; i++)
	{
		m_function_calls[i][m_function_calls[i].GetFreePos()] = "";
		m_function_calls[i][m_function_calls[i].GetFreePos()] = "";
		m_function_calls[i][m_function_calls[i].GetFreePos()] = "";
	}
}

mAiController::~mAiController(void)
{
}

void mAiController::SetOwner(VisTypedEngineObject_cl *pOwner)
{
	mController::SetOwner(pOwner);
	if(pOwner!=NULL)
	{ 
		if(pOwner)
		{
			m_states[mControllerState::sIdling] = new mControllerState::aiIdling();
			m_states[mControllerState::sMoving] = new mControllerState::aiMoving();
			m_states[mControllerState::sAttackingMelee] = new mControllerState::aiAttackingMelee();
			m_states[mControllerState::sAttackingRanged] = new mControllerState::aiAttackingRanged();
			
			m_states[mControllerState::sProtecting] = new mControllerState::aiProtecting();
			m_states[mControllerState::sWandering] = new mControllerState::aiWandering();
			m_states[mControllerState::sFleeing] = new mControllerState::aiFleeing();
			m_states[mControllerState::sAskingForHelp] = new mControllerState::aiAskingForHelp();
		}
	}
	else
	{

	}
}

bool mAiController::TryMeleeAttack()
{
	return true;
}

bool mAiController::TryRangedAttack()
{
	return true;
}

void mAiController::onFrameUpdate(float delta)
{
	mController::onFrameUpdate(delta);
}