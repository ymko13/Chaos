#pragma once
#include "ChaosGamePluginPCH.h"

class mProjectile : public mBasicEntity
{
public:
	mProjectile(void);
	~mProjectile(void);

	void SetProjectileMesh(const VString mesh_path);
	void SetProjectileSpeed(const int Speed);
	void SetProjectileTargetPosition(const hkvVec3 target);
	void SetAlive(const bool alive);
	void UpdateVals(const float delta);

	void ThinkFunction() HKV_OVERRIDE;
private:
	V_DECLARE_SERIAL_DLLEXP(mProjectile, SAMPLEPLUGIN_IMPEXP);
	V_DECLARE_VARTABLE(mProjectile, SAMPLEPLUGIN_IMPEXP);
	
	VString m_mesh;
	int m_speed;	
	bool m_alive;
	hkvVec3 m_target_to;
};

