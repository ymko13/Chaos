#include "ChaosGamePluginPCH.h"
#include "mCharStats.h"


mCharStats::mCharStats(void)
	: m_dead(false),
	m_attack_power(5),
	m_attack_speed(1.f),
	m_critical_chance(1.f),

	m_health(100),
	m_health_max(100),
	m_health_regen(1),
	m_health_regen_rate(0.5f),
	m_health_regen_acum(0.f),

	m_armor(2),

	m_movement_speed(0),
	m_movement_speed_normal(300),
	m_movement_speed_impair_tl(0.f),
	m_movement_speed_normal_pl(0.f),

	m_health_degen_tl(0.f),
	m_health_degen_rate(0.f),
	m_health_degen_acum(0.f),

	m_stunned(false),
	m_stunned_tl(0.f)
{
}
mCharStats::~mCharStats(void)
{
}

/*Char Attack Stats*/
void	mCharStats::SetAttackPower(int const attackpower)
{
	m_attack_power = attackpower;
}
int		mCharStats::GetAttackPower() const
{
	return m_attack_power;
}
void	mCharStats::SetAttackSpeed(float const attackspeed)
{
	m_attack_speed = attackspeed;
}
float	mCharStats::GetAttackSpeed() const
{
	return m_attack_speed;
}
void	mCharStats::SetCriticalChance(float const critchance)
{
	m_critical_chance = critchance;
}
float	mCharStats::GetCriticalChance() const
{
	return m_critical_chance;
}
	
/*Char Health Stats*/
void	mCharStats::SetHealth(int const health)
{
	m_health = health;
	m_health = hkvMath::Min(health, m_health_max);
	m_health = hkvMath::Min(health, m_health_max);
}
int		mCharStats::GetHealth() const
{
	return m_health;
}
int		mCharStats::GetHealthPercentage() const
{
	return (int)((float)(m_health) / (float)(m_health_max));
}
void	mCharStats::SetHealthMax(int const health)
{
	VASSERT(health > 0);
	m_health_max = health;
	m_health = hkvMath::Min(m_health, m_health_max);
}
int		mCharStats::GetHealthMax() const
{
	return m_health_max;
}
void mCharStats::SetHealthRegeneration(VBool const enabled)
{
	m_health_regen = enabled;
}
void	mCharStats::SetHealthRegenerationRate(float const healthRegenRate)
{
	m_health_regen_rate = healthRegenRate;
}

/*Char Armor Stats*/
void	mCharStats::SetArmor(int const armor)
{
	m_armor = hkvMath::Min(armor, 75);
}
int		mCharStats::GetArmor() const
{
	return m_armor;
}
void	mCharStats::SetArmorType(mStats_ArmorType const armortype)
{
	m_armor_type = armortype;
}
void	mCharStats::SetAttackType(mStats_AttackType const attacktype)
{
	m_attack_type = attacktype;
}
mStats_ArmorType	mCharStats::GetArmorType() const
{
	return m_armor_type;
}
mStats_AttackType	mCharStats::GetAttackType() const
{
	return m_attack_type;
}

/*Char Movement Speed Stats*/
void	mCharStats::SetMovementSpeed(int const movementspeed)
{
	m_movement_speed_normal = movementspeed;
}
int		mCharStats::GetMovementSpeed() const
{
	return m_movement_speed;
}

/*Char Apply Effects*/
void	mCharStats::SetHealthDegeneration(float const timeLapse, float const healthDegenRate)
{
	if(m_health_degen_tl > 0.f)
	{
		float old_total = m_health_degen_tl / m_health_degen_rate;
		float new_total = timeLapse / healthDegenRate;

		m_health_degen_tl = timeLapse;
		m_health_degen_rate = (old_total + new_total) / timeLapse;
	}
	else
	{		
		m_health_degen_tl = timeLapse;
		m_health_degen_rate = healthDegenRate;
	}
}
void	mCharStats::SetImpairMovementSpeed(float const timeLapse, float const impairPercentage)
{
	float ceiling = hkvMath::ceil(m_movement_speed_impair_tl_total);
	float r_percent = m_movement_speed_normal_pl * m_movement_speed_impair_tl / ceiling;
	if(r_percent < impairPercentage)
	{
		m_movement_speed_impair_tl = timeLapse;
		m_movement_speed_impair_tl_total = timeLapse;
		m_movement_speed_normal_pl = impairPercentage;
	}
}
void	mCharStats::SetStunned(float const timeLapse)
{
	m_stunned_tl = timeLapse;
}
bool	mCharStats::GetStunned()
{
	return m_stunned;
}

void	mCharStats::ApplyEffect(mEffect effect)
{
	m_health_degen_acum = 0;
	switch(effect.m_effect_type)
	{
	case ET_Buff:
		switch(effect.m_effect_buff)
		{
		case EB_HPRegen:
			SetHealthDegeneration(effect.m_effect_timespan, -effect.m_effect_value);
			break;
		}
		break;
	case ET_Debuff:
		switch(effect.m_effect_debuff)
		{
		case ED_Burn:
			SetHealthDegeneration(effect.m_effect_timespan, effect.m_effect_value);
			break;
		case ED_Poison:
			SetHealthDegeneration(effect.m_effect_timespan, effect.m_effect_value);
			SetImpairMovementSpeed(effect.m_effect_timespan, 0.3f);
			break;
		case ED_Freeze:
			SetImpairMovementSpeed(effect.m_effect_timespan, effect.m_effect_value);
			break;
		case ED_Stun:
			SetStunned(effect.m_effect_timespan);
			break;
		}
		break;
	}
}

/*Char Stats Updates*/
void	mCharStats::UpdateStats(float const delta)
{
	if(m_health == 0)
		m_dead = true;
	else
		m_dead = false;

	if(!m_dead)
	{
		UpdateHealthRegen(delta);
		UpdateHealthDegen(delta);
		UpdateMovementSpeed(delta);
		UpdateStunned(delta);
	}
}
void	mCharStats::UpdateHealthRegen(float const delta)
{
	if(m_health < m_health_max)
	{
		if(m_health_regen_acum < 0)
			m_health_regen_acum = 0;
		m_health_regen_acum += delta / m_health_regen_rate;
		if(m_health_regen_acum >= 1.f)
		{
			float total_regen = hkvMath::floor(m_health_regen_acum);
			int newHealth = hkvMath::Min(m_health + (int)total_regen, m_health_max);
			m_health_regen_acum -= total_regen;
			SetHealth(newHealth);
		}
	}
}
void	mCharStats::UpdateHealthDegen(float const delta)
{
	if(m_health > 0 && m_health_degen_tl > 0.f)
	{
		m_health_degen_acum += delta / m_health_degen_rate;
		m_health_degen_tl -= delta;
		if(m_health_degen_acum >= 1.f || m_health_degen_acum <= -1.f)
		{
			float total_degen = hkvMath::floor(m_health_degen_acum);
			int newHealth = hkvMath::Max(m_health - (int)total_degen, 0);
			m_health_degen_acum -= total_degen;
			SetHealth(newHealth);
		}
	}
	else
		m_health_degen_tl = 0.f;
}
void	mCharStats::UpdateMovementSpeed(float const delta)
{
	if(m_movement_speed_impair_tl > 0.f)
	{
		float ceiling = hkvMath::ceil(m_movement_speed_impair_tl_total);
		float r_percent = m_movement_speed_normal_pl * (m_movement_speed_impair_tl / ceiling);
		m_movement_speed = (int)(m_movement_speed_normal * (1 - r_percent));
		m_movement_speed_impair_tl -= delta;
	}
	else
	{
		m_movement_speed = m_movement_speed_normal;
		m_movement_speed_impair_tl = 0.f;
	}
}
void	mCharStats::UpdateStunned(float const delta)
{
	if(m_stunned_tl > 0.f)
	{
		m_stunned = true;
		m_movement_speed = 0;
		m_stunned_tl -= delta;
	}
	else
	{
		m_stunned = true;
		m_stunned_tl = 0.f;
	}
}

const int mCharStats::CalculateRealDamage(int const rawdamage, mStats_AttackType const attackerAttack, int const rawarmor, mStats_ArmorType const attackArmor)
{
	float damageMult = 1.f;

	// Normal > Heavy > Hero > Normal
	switch(attackerAttack)
	{
	case AtT_Normal:
		switch(attackArmor)
		{
		case ArT_Normal:
			damageMult = 1.0f;
			break;
		case ArT_Heavy:
			damageMult = 1.25f;
			break;
		case ArT_Hero:
			damageMult = 0.75f;
			break;
		}
		break;
	case AtT_Heavy:
		switch(attackArmor)
		{
		case ArT_Normal:
			damageMult = 0.8f;
			break;
		case ArT_Heavy:
			damageMult = 1.f;
			break;
		case ArT_Hero:
			damageMult = 1.25f;
			break;
		}
		break;
	case AtT_Hero:
		switch(attackArmor)
		{
		case ArT_Normal:
			damageMult = 1.25f;
			break;
		case ArT_Heavy:
			damageMult = 0.8f;
			break;
		case ArT_Hero:
			damageMult = 1.0f;
			break;
		}
		break;
	}

	//Armor and damage formula
	damageMult *= 1.f - ((float)rawarmor / 100.f);

	return (int)((float)rawdamage * damageMult);
}