#pragma once

#include "ChaosGamePluginPCH.h"
#include <Vision/Runtime/EnginePlugins/VisionEnginePlugin/Scripting/VScriptIncludes.hpp>

enum mTeam
{
	Character = 0,
	Enemy = 1,
	Neutral = 2
};

class mCharacter : public mDamageableEntity
{
public:
	mCharacter(void);
	~mCharacter(void);


	void SetTeam(const mTeam cteam = Neutral);
	const mCharStats& GetCharacterStats() const; 
	mCharStats& GetCharacterStats();
	int TakeDamage(const int damageAmount, const mStats_AttackType attack) HKV_OVERRIDE;

	void GetDependencies(VResourceSnapshot &snapshot) HKV_OVERRIDE;
	void ThinkFunction() HKV_OVERRIDE;
	void ApplyEffect(const mEffect effect);
	virtual bool IsDead();
	
	virtual void AddControllerComponent(VType* controllerType);

protected:
	VString m_loading_script;

	mCharStats m_stats;

	mTeam m_char_team;

private:
	V_DECLARE_SERIAL_DLLEXP(mCharacter, SAMPLEPLUGIN_IMPEXP);
	V_DECLARE_VARTABLE(mCharacter, SAMPLEPLUGIN_IMPEXP);

};

namespace mUtil
{
	//Script loading partially copied from RPG_Game by Havok
	static const void LoadAndAttachScript(VisTypedEngineObject_cl* attachTo, const VString scriptName,VResourceSnapshot &snapshot)
	{
		if(scriptName.IsEmpty())
		{
			return;
		}

		VScriptResourceManager* scriptManager = static_cast<VScriptResourceManager*>(Vision::GetScriptManager());

		if(Vision::Editor.IsPlayingTheGame())
		{
			// only check for already loaded assets when playing. Need to force a full dependency walk otherwise in order to make the "Show Assets" button work in the export window.
			VManagedResource* resource = scriptManager->GetResourceByName(scriptName);

			if(resource && 
				resource->IsLoaded())
			{
				return;
			}
		}

		VScriptResource* scriptRes = scriptManager->LoadScriptFile(scriptName);
		VASSERT(scriptRes);

		if(scriptRes)
		{
			scriptRes->GetDependencies(snapshot);

			// create the script entity, but don't do a full initialization, as we don't need it here.
			IVScriptInstance* script = scriptRes->CreateScriptInstance();
			VASSERT(script);
		
			scriptManager->SetScriptInstance(attachTo, script);
		}
	}
}