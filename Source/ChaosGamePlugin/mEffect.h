#pragma once

enum mEffect_Type
{
	ET_Buff = 0,
	ET_Debuff = 1
};
enum mEffect_Buff
{
	EB_HPRegen = 0
};
enum mEffect_Debuff
{
	ED_Poison = 0,
	ED_Freeze = 1,
	ED_Burn = 2,
	ED_Stun = 3
};

class mEffect
{
public:
	mEffect(void);
	~mEffect(void);

	void	SetTimeSpan(const float time = 0.f);
	float	GetTimeSpan() const;
	void	SetValue(const float value = 0.f);
	int		GetIntValue() const;
	float	GetFloatValue() const;

	void	SetType(const mEffect_Type type);
	void	SetBuff(const mEffect_Buff type);
	void	SetDebuff(const mEffect_Debuff type);
	mEffect_Type	GetType() const;
	mEffect_Buff	GetBuff() const;
	mEffect_Debuff	GetDebuff() const;

protected:
	mEffect_Type	m_effect_type;
	mEffect_Buff	m_effect_buff;
	mEffect_Debuff	m_effect_debuff;

	float m_effect_timespan;
	float m_effect_value;

	friend class mCharStats;
};

