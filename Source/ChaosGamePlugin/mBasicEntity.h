#pragma once
#include "ChaosGamePluginModule.h"
#include "mSerializer.h"

class mBasicEntity : public VisBaseEntity_cl
{
public:
	mBasicEntity(void);
	~mBasicEntity(void);

	virtual void InitializeProperties();
	virtual void PostInitialize();
	virtual void MoveTo(const hkvVec3 position);
	virtual void MoveBy(const hkvVec3 position);
protected:
	void Serialize(VArchive& ar) HKV_OVERRIDE;

	VBool WantsDeserializationCallback(const VSerializationContext& context) HKV_OVERRIDE;
	void OnDeserializationCallback(const VSerializationContext& context) HKV_OVERRIDE;
private:
	V_DECLARE_SERIAL_DLLEXP(mBasicEntity, SAMPLEPLUGIN_IMPEXP);
	V_DECLARE_VARTABLE(mBasicEntity, SAMPLEPLUGIN_IMPEXP);
};

