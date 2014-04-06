#include "ChaosGamePluginPCH.h"
#include "mEffect.h"

mEffect::mEffect(void)
	: m_effect_type(ET_Buff),
	m_effect_buff(),
	m_effect_debuff(),
	m_effect_timespan(1.f),
	m_effect_value(0.f)
{
}

mEffect::~mEffect(void)
{
}

void mEffect::SetTimeSpan(const float time)
{
	m_effect_timespan = time;
}
float mEffect::GetTimeSpan() const
{
	return m_effect_timespan;
}

void mEffect::SetValue(const float value)
{
	m_effect_value = value;
}
int mEffect::GetIntValue() const
{
	return (int)(m_effect_value * 100);
}
float mEffect::GetFloatValue() const
{
	return m_effect_value;
}

void mEffect::SetType(const mEffect_Type type)
{
	m_effect_type = type;
}
void mEffect::SetBuff(const mEffect_Buff type)
{
	m_effect_buff = type;
}
void mEffect::SetDebuff(const mEffect_Debuff type)
{
	m_effect_debuff = type;
}

mEffect_Type mEffect::GetType() const
{
	return m_effect_type;
}
mEffect_Buff mEffect::GetBuff() const
{
	return m_effect_buff;
}
mEffect_Debuff mEffect::GetDebuff() const
{
	return m_effect_debuff;
}


