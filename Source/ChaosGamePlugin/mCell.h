#pragma once
#include "ChaosGamePluginModule.h"
#include "mBasicEntity.h"
#include <Vision/Runtime/Base/String/VString.hpp> 

class mCell : public mBasicEntity
{
public:	
	mCell(void);
	~mCell(void);  

	int x;
	int y;

	void InitializeProperties() HKV_OVERRIDE;
	void PostInitialize() HKV_OVERRIDE;
	
	void EditorThinkFunction() HKV_OVERRIDE;
	void Dispose();
private:	
	V_DECLARE_SERIAL_DLLEXP ( mCell, SAMPLEPLUGIN_IMPEXP );
	V_DECLARE_VARTABLE ( mCell, SAMPLEPLUGIN_IMPEXP );
};

