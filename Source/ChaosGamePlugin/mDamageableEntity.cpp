#include "ChaosGamePluginPCH.h"
#include "mDamageableEntity.h"

V_IMPLEMENT_SERIAL ( mDamageableEntity, mBasicEntity, 0, &g_myComponentModule );

START_VAR_TABLE( mDamageableEntity, mBasicEntity, "This is a damageable entity", 0, "")
  DEFINE_VAR_FLOAT_AND_NAME(mDamageableEntity, m_col_radius, "Collision Radius", "Collision shape radius", "35", 0, 0);
  DEFINE_VAR_FLOAT_AND_NAME(mDamageableEntity, m_col_height, "Collision Height", "Collision shape height", "70", 0, 0);
END_VAR_TABLE

mDamageableEntity::mDamageableEntity()
	: mBasicEntity()
	, m_col_radius(100.f)
	, m_col_height(100.f)
{

}

mDamageableEntity::~mDamageableEntity(void)
{
}

int mDamageableEntity::TakeDamage(const int damageAmount, const mStats_AttackType attack)
{
  return 0;
}

float mDamageableEntity::GetColRadius() const
{
  return m_col_radius;
}

float mDamageableEntity::GetColHeight() const
{
  return m_col_height;
}