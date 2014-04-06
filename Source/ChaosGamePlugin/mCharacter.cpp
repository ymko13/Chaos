#include "ChaosGamePluginPCH.h"
#include "mCharacter.h"

V_IMPLEMENT_SERIAL ( mCharacter, mDamageableEntity, 0, &g_myComponentModule );

START_VAR_TABLE( mCharacter, mDamageableEntity, "This is a basic character", 0, "")

	DEFINE_VAR_VSTRING_AND_NAME(mCharacter, m_loading_script, "Loading Script", "The script from which to load the character", "Scripts/CharLoad.lua", 100, 0,0);

	/*Character Base Stats*/
	DEFINE_VAR_INT_AND_NAME(mCharacter, m_stats.m_attack_power, "Attack Power", "The base power of the attack", "10", 0, 0);
	DEFINE_VAR_FLOAT_AND_NAME(mCharacter, m_stats.m_attack_speed, "Attack Speed", "Rate of attacks(1 attack per val)", "1.2", 0, 0);
	DEFINE_VAR_FLOAT_AND_NAME(mCharacter, m_stats.m_critical_chance, "Crit Chance", "Chance to strike for 150% damage", "0.1", 0, 0);

	/*Character Health Stats*/
	DEFINE_VAR_INT_AND_NAME(mCharacter, m_stats.m_health, "Current Health", "Current Character Health", "1", 0, 0);
	DEFINE_VAR_INT_AND_NAME(mCharacter, m_stats.m_health_max, "Maximum Health", "Attacks per second", "100", 0, 0);
	DEFINE_VAR_BOOL_AND_NAME(mCharacter, m_stats.m_health_regen, "Health Regen Enabled", "Health regeneration enabled", "FALSE", 0, 0);
	DEFINE_VAR_FLOAT_AND_NAME(mCharacter, m_stats.m_health_regen_rate, "Health Regen Rate", "Rate of Health Regeneration(1 hp per val)", "0.1", 0, 0);

	/*Character Armor Stats*/
	DEFINE_VAR_INT_AND_NAME(mCharacter, m_stats.m_armor, "Armor", "Current Armor", "5", 0, 0);
	DEFINE_VAR_ENUM_AND_NAME(mCharacter, m_stats.m_attack_type, "Attack Type", "The type of Attack", "AtT_Normal", "AtT_Normal/AtT_Heavy/AtT_Hero", 0, 0);
	DEFINE_VAR_ENUM_AND_NAME(mCharacter, m_stats.m_armor_type, "Armor Type", "The type of Armor", "ArT_Normal", "ArT_Normal/ArT_Heavy/ArT_Hero", 0, 0);

	/*Character Movement Speed Stats*/
	DEFINE_VAR_INT_AND_NAME(mCharacter, m_stats.m_movement_speed, "Movement Speed Current", "Speed of the character", "150", 0, 0);
	DEFINE_VAR_INT_AND_NAME(mCharacter, m_stats.m_movement_speed_normal, "Movement Speed Normal", "Speed of the character", "150", 0, 0);
	
END_VAR_TABLE

mCharacter::mCharacter(void)
{
}

mCharacter::~mCharacter(void)
{
}

void mCharacter::SetTeam(const mTeam cteam)
{
	m_char_team = cteam;
}
int mCharacter::TakeDamage(const int damageAmount, const mStats_AttackType attack)
{
	int realDamage = mCharStats::CalculateRealDamage(damageAmount, attack, m_stats.GetArmor(), m_stats.GetArmorType());
	int health = m_stats.GetHealth();

	m_stats.SetHealth(health - realDamage);
	return realDamage;
}
void mCharacter::ApplyEffect(const mEffect effect)
{
	m_stats.ApplyEffect(effect);
}

const mCharStats& mCharacter::GetCharacterStats() const
{
	return m_stats;
} 
mCharStats& mCharacter::GetCharacterStats()
{
	return m_stats;
} 

//Load nescescerry scripts
void mCharacter::GetDependencies(VResourceSnapshot &snapshot)
{
	mUtil::LoadAndAttachScript(this, m_loading_script, snapshot);
	mDamageableEntity::GetDependencies(snapshot);
}

void mCharacter::AddControllerComponent(VType* controllerType)
{
}

bool mCharacter::IsDead()
{
	return m_stats.m_dead;
}

/*Think Function*/
void mCharacter::ThinkFunction()
{
	const float delta = Vision::GetTimer()->GetTimeDifference();

	if(IsDead())
	{
		return;
	}

	m_stats.UpdateStats(delta);
}