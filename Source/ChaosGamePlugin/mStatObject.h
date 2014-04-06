#pragma once
#include "ChaosGamePluginModule.h"
#include "mBasicEntity.h"
#include <Vision/Runtime/Base/String/VString.hpp> 

class mStatObject : public mBasicEntity
{
public:
	mStatObject(void);
	~mStatObject(void);

	int x;
	int y;
	int objId;

	void InitializeProperties() HKV_OVERRIDE;
	void PostInitialize() HKV_OVERRIDE;
	void Set(int x, int y, int objId);
private:	
	V_DECLARE_SERIAL_DLLEXP ( mStatObject, SAMPLEPLUGIN_IMPEXP );
	V_DECLARE_VARTABLE ( mStatObject, SAMPLEPLUGIN_IMPEXP );
};

