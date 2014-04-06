#include "ChaosGamePluginPCH.h"
#include "mAiCharacter.h"
#include "mAiController.h"

V_IMPLEMENT_SERIAL ( mAiCharacter, mCharacter, 0, &g_myComponentModule );

START_VAR_TABLE( mAiCharacter, mCharacter, "Basic AI Character", 0, "")

	DEFINE_VAR_VSTRING_AND_NAME(mAiCharacter, m_controller_script, "Controller Script", "Controller Script", "Scripts/basicAiController", 100, 0, 0);

	DEFINE_VAR_FLOAT_AND_NAME(mAiCharacter, m_destination.x, "Final Destination X", "", "0", 0, 0);
	DEFINE_VAR_FLOAT_AND_NAME(mAiCharacter, m_destination.y, "Final Destination Y", "", "0", 0, 0);
	DEFINE_VAR_FLOAT_AND_NAME(mAiCharacter, m_destination.z, "Final Destination Z", "", "0", 0, 0);
END_VAR_TABLE

mAiCharacter::mAiCharacter(void)
{
}


mAiCharacter::~mAiCharacter(void)
{
}

void mAiCharacter::InitializeProperties()
{
	this->m_loading_script = "Scripts/AICharLoad.lua";
	this->m_controller_script = "Scripts/AICharControllerScript.lua";
	mCharacter::InitializeProperties();
}

void mAiCharacter::AddControllerComponent(VType* controllerType)
{
	VTypedObject * ob = controllerType->CreateInstance();
	mAiController* comp = static_cast<mAiController*>(ob);
	AddComponent(comp);
}

void mAiCharacter::PostInitialize()
{
	mCharacter::PostInitialize();
	AddControllerComponent(V_RUNTIME_CLASS(mAiController));
}

//Load nescescerry scripts
void mAiCharacter::GetDependencies(VResourceSnapshot &snapshot)
{
	mUtil::LoadAndAttachScript(this, m_loading_script, snapshot);
	mUtil::LoadAndAttachScript(this, m_controller_script, snapshot);
	mCharacter::GetDependencies(snapshot);
}