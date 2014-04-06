#include "ChaosGamePluginPCH.h"
#include "myComponent.h"

V_IMPLEMENT_SERIAL ( mCell, mBasicEntity, 0, &g_myComponentModule );

START_VAR_TABLE( mCell, mBasicEntity, "The Cell", 0, "")
END_VAR_TABLE

void mCell::InitializeProperties()
{
	mBasicEntity::InitializeProperties();
}

//Place the object, create the objects etc
void mCell::PostInitialize()
{
}

mCell::mCell(void)
{	
}

mCell::~mCell(void)
{
}

void mCell::EditorThinkFunction()
{
	VString strin;
	strin.Format("< %d, %d >", this->x, this->y);
	Vision::Message.DrawMessage3D(strin, hkvVec3(x * 100.7f + 50.5f, y * 100.7f + 50.5f, 0));
}

void mCell::Dispose()
{
	mBasicEntity::DisposeObject();
	VString key;
	for(int i = 0; i < 6; i++)
	{
		key.FormatEx("C_%d_%d_Tree_%d", x, y, i);
		VSmartPtr<mStatObject> eCell = (mStatObject *) Vision::Game.SearchEntity(key);
		if(eCell.m_pPtr != nullptr)
		{
			eCell->DisposeObject();
		}
	}
	for(int i = 0; i < 10; i++)
	{
		key.FormatEx("C_%d_%d_Obj_%d", x, y, i);
		VSmartPtr<mStatObject> eCell = (mStatObject *) Vision::Game.SearchEntity(key);
		if(eCell.m_pPtr != nullptr)
		{
			eCell->DisposeObject();
		}
	}
}