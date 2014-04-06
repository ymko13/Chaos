#pragma once
#include "ChaosGamePluginModule.h"
#include "mBasicEntity.h"

class mDamageableEntity : public mBasicEntity
{
public:
	mDamageableEntity();
	~mDamageableEntity(void);
	
	virtual int TakeDamage(const int damageAmount, const mStats_AttackType attack);

	virtual float GetColRadius() const;
	virtual float GetColHeight() const;
	float m_col_radius;
	float m_col_height;

private:
	V_DECLARE_SERIAL_DLLEXP(mDamageableEntity, SAMPLEPLUGIN_IMPEXP);
	V_DECLARE_VARTABLE(mDamageableEntity, SAMPLEPLUGIN_IMPEXP);
};

