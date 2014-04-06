#include "ChaosGamePluginPCH.h"
#include "mPlayerCharacter.h"

V_IMPLEMENT_SERIAL ( mPlayerCharacter, mCharacter, 0, &g_myComponentModule );

START_VAR_TABLE( mPlayerCharacter, mCharacter, "Basic Entity of the game", 0, "")
END_VAR_TABLE

mPlayerCharacter::mPlayerCharacter(void)
{

}

void mPlayerCharacter::AddControllerComponent(VType* controllerType)
{
	mController* comp = static_cast<mController*>(controllerType->CreateInstance());
	AddComponent(comp);
}

mPlayerCharacter::~mPlayerCharacter(void)
{
}

void mPlayerCharacter::InitializeProperties()
{
	this->m_loading_script = "Scripts/CharLoad.lua";
	mCharacter::InitializeProperties();
}

void mPlayerCharacter::PostInitialize()
{
	mCharacter::PostInitialize();
	AddControllerComponent(V_RUNTIME_CLASS(mController));
}

void mPlayerCharacter::ThinkFunction()
{
	mCharacter::ThinkFunction();
}