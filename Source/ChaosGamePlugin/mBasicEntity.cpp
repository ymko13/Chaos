#pragma once
#include "mBasicEntity.h"
#include "ChaosGamePluginPCH.h"

V_IMPLEMENT_SERIAL ( mBasicEntity, VisBaseEntity_cl, 0, &g_myComponentModule );

START_VAR_TABLE( mBasicEntity, VisBaseEntity_cl, "Basic Entity of the game", 0, "")
END_VAR_TABLE

//Code for serialize is more or less copied from the
//RPG Game Plugin(BaseEntity) as an example of how Serialize should be done
void mBasicEntity::Serialize(VArchive &ar)
{
	VisBaseEntity_cl::Serialize(ar);
	if(ar.IsLoading())
	{
		mySerializer::ReadVariableList(this, ar);
	}
	else
	{
		mySerializer::WriteVariableList(this, ar);
	}
}

void mBasicEntity::InitializeProperties()
{
	TriggerScriptEvent("OnInitProperties");
}

void mBasicEntity::PostInitialize()
{
}

void mBasicEntity::MoveTo(const hkvVec3 pos)
{
	this->SetPosition(pos);
}

void mBasicEntity::MoveBy(const hkvVec3 pos)
{
	hkvVec3 p = this->GetPosition();
	p += pos;
	this->SetPosition(p);
}

void mBasicEntity::OnDeserializationCallback(const VSerializationContext& cnt)
{
	VisBaseEntity_cl::OnDeserializationCallback(cnt);

	InitializeProperties();

	PostInitialize();
}

VBool mBasicEntity::WantsDeserializationCallback(const VSerializationContext& cnt)
{
	return TRUE;
}

mBasicEntity::mBasicEntity(void)
{
}

mBasicEntity::~mBasicEntity(void)
{
}
