#include "ChaosGamePluginPCH.h"
#include "mProjectile.h"

V_IMPLEMENT_SERIAL ( mProjectile, mBasicEntity, 0, &g_myComponentModule );

START_VAR_TABLE( mProjectile, mBasicEntity, "The Projectile", 0, "")
END_VAR_TABLE

mProjectile::mProjectile(void)
{
}


mProjectile::~mProjectile(void)
{
}

void mProjectile::SetProjectileMesh(const VString mesh_path)
{
	m_mesh = mesh_path;
}
void mProjectile::SetProjectileSpeed(const int Speed)
{
	this->m_speed = Speed;
}
void mProjectile::SetProjectileTargetPosition(const hkvVec3 target)
{
	this->m_target_to = target;
}
void mProjectile::SetAlive(const bool alive)
{
	this->SetMesh(m_mesh);
	this->ReComputeVisibility();
	m_alive = true;
}

void mProjectile::UpdateVals(float delta)
{	
	if(m_alive)
	{
		hkvVec3 pos = GetPosition();
		if(m_target_to.getDistanceTo(pos) < (float)(m_speed / 30))
		{
			m_alive = false;
			DisposeObject();
			return;
		}
		pos = m_target_to - pos;
		pos.normalizeIfNotZero();
		pos *= ((float)m_speed * delta);
		this->MoveBy(pos);
	}
}

void mProjectile::ThinkFunction()
{
	float delta = Vision::GetTimer()->GetTimeDifference();	
	UpdateVals(delta);
}