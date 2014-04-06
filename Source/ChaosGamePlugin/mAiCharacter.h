#pragma once
#include "ChaosGamePluginPCH.h"

class mAiCharacter : public mCharacter
{
public:
	mAiCharacter(void);
	~mAiCharacter(void);

	hkvVec3 GetDestination() const { return m_destination; }

	void InitializeProperties() HKV_OVERRIDE;
	void PostInitialize() HKV_OVERRIDE;
	void GetDependencies(VResourceSnapshot &snapshot) HKV_OVERRIDE;

	void AddControllerComponent(VType* controllerType) HKV_OVERRIDE;

protected:
	hkvVec3 m_destination;

	VString m_controller_script;
private:
	V_DECLARE_SERIAL_DLLEXP(mAiCharacter, SAMPLEPLUGIN_IMPEXP);
	V_DECLARE_VARTABLE(mAiCharacter, SAMPLEPLUGIN_IMPEXP);
};

