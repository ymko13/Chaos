#include "ChaosGamePluginPCH.h"
#include "mBaseComponent.h"

V_IMPLEMENT_SERIAL(mBaseComponent, IVObjectComponent, 0, &g_myComponentModule);

mBaseComponent_ComponentManager mBaseComponent_ComponentManager::g_GlobalManager;

mBaseComponent::mBaseComponent(void)
{
}

mBaseComponent::~mBaseComponent(void)
{
}

void mBaseComponent::Serialize(VArchive &ar)
{
	IVObjectComponent::Serialize(ar);
	if(ar.IsLoading())
	{
		mySerializer::ReadVariableList(this, ar);
	}
	else
	{
		mySerializer::WriteVariableList(this, ar);
	}
}

void mBaseComponent::SetOwner(VisTypedEngineObject_cl *pOwner)
{
  IVObjectComponent::SetOwner( pOwner );
  if (pOwner!=NULL)
  {
    mBaseComponent_ComponentManager::GlobalManager().Instances().AddUnique(this);
    Vision::Message.Add("myComponent - Component Instance created");
  }
  else
  {
    Vision::Message.Add("myComponent - Removed Component Instance from component Manager");
    mBaseComponent_ComponentManager::GlobalManager().Instances().SafeRemove(this);    
  }
}

void mBaseComponent::onFrameUpdate(float delta)
{
	if (!GetOwner())return;

	mCharacter* charC = (mCharacter *)GetOwner();
	
	hkvVec3 vPos = charC->GetPosition();
	VString s;
	int health = charC->GetCharacterStats().GetHealth(), movement = charC->GetCharacterStats().GetMovementSpeed();
	s.FormatEx("Health: %d  Movement Speed: %d", health, movement);
	Vision::Message.DrawMessage3D(s,vPos);

	for(int i = 0; i < (int)mMap::m_full_path.GetValidSize(); i++)
	{
		s.FormatEx("Path: %d", i);
		Vision::Message.DrawMessage3D(s, mMap::m_full_path[i]);
	}
}

START_VAR_TABLE(mBaseComponent, IVObjectComponent, "Base Component", VVARIABLELIST_FLAGS_NONE, "mBaseComponent")

END_VAR_TABLE


