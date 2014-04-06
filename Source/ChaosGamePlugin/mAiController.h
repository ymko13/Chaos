#pragma once
#include "mcontroller.h"
class mAiController : public mController
{
public:
	mAiController(void);
	~mAiController(void);

	bool TryMeleeAttack() HKV_OVERRIDE;
	bool TryRangedAttack() HKV_OVERRIDE;

	SerialDynArray_cl<SerialDynArray_cl<VString>> m_function_calls;

	void onFrameUpdate(float delta) HKV_OVERRIDE;

protected:
	void SetOwner(VisTypedEngineObject_cl *owner) HKV_OVERRIDE;

private:
	V_DECLARE_SERIAL_DLLEXP(mAiController, SAMPLEPLUGIN_IMPEXP);
	V_DECLARE_VARTABLE(mAiController, SAMPLEPLUGIN_IMPEXP);
};

namespace mControllerState
{
	class aiIdling : public mControllerStateBase
	{
		void OnEnter(mController *controller) HKV_OVERRIDE {
			mAiController* aiCont = (mAiController *)controller;
			aiCont->TriggerScriptEvent(aiCont->m_function_calls[aiCont->GetState()][0]); 
		}
		void OnExit(mController *controller) HKV_OVERRIDE {
			mAiController* aiCont = (mAiController *)controller;
			aiCont->TriggerScriptEvent(aiCont->m_function_calls[aiCont->GetState()][1]); 
		}
		void OnTick(mController *controller, float delta) HKV_OVERRIDE {
			mAiController* aiCont = (mAiController *)controller;
			aiCont->TriggerScriptEvent(aiCont->m_function_calls[aiCont->GetState()][2]); 
		}
		char const *GetName() const HKV_OVERRIDE { return "aiController::Idling"; }
	};
	class aiMoving : public mControllerStateBase
	{
		void OnEnter(mController *controller) HKV_OVERRIDE {
			mAiCharacter* chr = (mAiCharacter*)controller->GetOwner();
			controller->RequestPath(chr->GetDestination());
		}
		void OnExit(mController *controller) HKV_OVERRIDE {
			mAiController* aiCont = (mAiController *)controller;
			aiCont->TriggerScriptEvent(aiCont->m_function_calls[aiCont->GetState()][1]); 
		}
		void OnTick(mController *controller, float delta) HKV_OVERRIDE {
			if (!controller->GetOwner()) return;
			mCharacter* owner = (mCharacter *)controller->GetOwner();
			hkvVec3 currentPos = owner->GetPosition();

			hkvVec3 resultDir, currentDir, desiredDir;
		
			desiredDir = controller->GetNextPointInPath() - currentPos;
			desiredDir.normalizeIfNotZero();

			currentDir = controller->GetDirection();

			mUtil::CalcDirection(resultDir, currentDir, desiredDir, .95f);

			resultDir.z = 0.f;

			hkvVec3 Velocity = resultDir * controller->GetWantedSpeed() * delta;

			controller->SetDirection(resultDir);

			hkvVec3 newPos = currentPos + Velocity;

			owner->SetPosition(newPos);
		}
		char const *GetName() const HKV_OVERRIDE { return "aiController::Moving"; }
	};
	class aiAttackingMelee : public mControllerStateBase
	{
		void OnEnter(mController *controller) HKV_OVERRIDE {
			mAiController* aiCont = (mAiController *)controller;
			aiCont->TriggerScriptEvent(aiCont->m_function_calls[aiCont->GetState()][0]); 
		}
		void OnExit(mController *controller) HKV_OVERRIDE {
			mAiController* aiCont = (mAiController *)controller;
			aiCont->TriggerScriptEvent(aiCont->m_function_calls[aiCont->GetState()][1]); 
		}
		void OnTick(mController *controller, float delta) HKV_OVERRIDE {
			mAiController* aiCont = (mAiController *)controller;
			aiCont->TriggerScriptEvent(aiCont->m_function_calls[aiCont->GetState()][2]); 
		}
		char const *GetName() const HKV_OVERRIDE { return "aiController::AttackingMelee"; }
	};
	class aiAttackingRanged : public mControllerStateBase
	{
		void OnEnter(mController *controller) HKV_OVERRIDE {
			mAiController* aiCont = (mAiController *)controller;
			aiCont->TriggerScriptEvent(aiCont->m_function_calls[aiCont->GetState()][0]); 
		}
		void OnExit(mController *controller) HKV_OVERRIDE {
			mAiController* aiCont = (mAiController *)controller;
			aiCont->TriggerScriptEvent(aiCont->m_function_calls[aiCont->GetState()][1]); 
		}
		void OnTick(mController *controller, float delta) HKV_OVERRIDE {
			mAiController* aiCont = (mAiController *)controller;
			aiCont->TriggerScriptEvent(aiCont->m_function_calls[aiCont->GetState()][2]); 
		}
		char const *GetName() const HKV_OVERRIDE { return "aiController::AttackingRanged"; }
	};

	//AI
	class aiProtecting : public mControllerStateBase
	{
		void OnEnter(mController *controller) HKV_OVERRIDE {
			mAiController* aiCont = (mAiController *)controller;
			aiCont->TriggerScriptEvent(aiCont->m_function_calls[aiCont->GetState()][0]); 
		}
		void OnExit(mController *controller) HKV_OVERRIDE {
			mAiController* aiCont = (mAiController *)controller;
			aiCont->TriggerScriptEvent(aiCont->m_function_calls[aiCont->GetState()][1]); 
		}
		void OnTick(mController *controller, float delta) HKV_OVERRIDE {
			mAiController* aiCont = (mAiController *)controller;
			aiCont->TriggerScriptEvent(aiCont->m_function_calls[aiCont->GetState()][2]); 
		}
		char const *GetName() const HKV_OVERRIDE { return "aiController::Protecting"; }
	};
	class aiWandering : public mControllerStateBase
	{
		void OnEnter(mController *controller) HKV_OVERRIDE {
			mAiController* aiCont = (mAiController *)controller;
			aiCont->TriggerScriptEvent(aiCont->m_function_calls[aiCont->GetState()][0]); 
		}
		void OnExit(mController *controller) HKV_OVERRIDE {
			mAiController* aiCont = (mAiController *)controller;
			aiCont->TriggerScriptEvent(aiCont->m_function_calls[aiCont->GetState()][1]); 
		}
		void OnTick(mController *controller, float delta) HKV_OVERRIDE {
			mAiController* aiCont = (mAiController *)controller;
			aiCont->TriggerScriptEvent(aiCont->m_function_calls[aiCont->GetState()][2]); 
		}
		char const *GetName() const HKV_OVERRIDE { return "aiController::Wandering"; }
	};
	class aiFleeing : public mControllerStateBase
	{
		void OnEnter(mController *controller) HKV_OVERRIDE {
			mAiController* aiCont = (mAiController *)controller;
			aiCont->TriggerScriptEvent(aiCont->m_function_calls[aiCont->GetState()][0]); 
		}
		void OnExit(mController *controller) HKV_OVERRIDE {
			mAiController* aiCont = (mAiController *)controller;
			aiCont->TriggerScriptEvent(aiCont->m_function_calls[aiCont->GetState()][1]); 
		}
		void OnTick(mController *controller, float delta) HKV_OVERRIDE {
			mAiController* aiCont = (mAiController *)controller;
			aiCont->TriggerScriptEvent(aiCont->m_function_calls[aiCont->GetState()][2]); 
		}
		char const *GetName() const HKV_OVERRIDE { return "aiController::Fleeing"; }
	};
	class aiAskingForHelp : public mControllerStateBase
	{
		void OnEnter(mController *controller) HKV_OVERRIDE {
			mAiController* aiCont = (mAiController *)controller;
			aiCont->TriggerScriptEvent(aiCont->m_function_calls[aiCont->GetState()][0]); 
		}
		void OnExit(mController *controller) HKV_OVERRIDE {
			mAiController* aiCont = (mAiController *)controller;
			aiCont->TriggerScriptEvent(aiCont->m_function_calls[aiCont->GetState()][1]); 
		}
		void OnTick(mController *controller, float delta) HKV_OVERRIDE {
			mAiController* aiCont = (mAiController *)controller;
			aiCont->TriggerScriptEvent(aiCont->m_function_calls[aiCont->GetState()][2]); 
		}
		char const *GetName() const HKV_OVERRIDE { return "aiController::AskingForHelp"; }
	};
}