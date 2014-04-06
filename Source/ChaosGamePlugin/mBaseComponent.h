#ifndef MBASECOMPONENT_H_INCLUDED
#define MBASECOMPONENT_H_INCLUDED

#include "ChaosGamePluginModule.h"

// Versions
#define MBASECOMPONENT_VERSION_0          0     // Initial version
#define MBASECOMPONENT_VERSION_CURRENT    1     // Current version

class mBaseComponent : public IVObjectComponent
{
public:
	mBaseComponent(void);
	~mBaseComponent(void);

	//This function is a copy of mBaseComponent function provided with the export of source in VForge
	VOVERRIDE BOOL CanAttachToObject( VisTypedEngineObject_cl *pObject, VString &sErrorMsgOut )
	{
		if ( !IVObjectComponent::CanAttachToObject( pObject, sErrorMsgOut )) return FALSE;  

		//  Define criteria here that allows the editor to attach this component to 
		//  the passed object.In this example, the object should be derived from 
		//  VisObject3D_cl eg it can be positionable. 
		if ( !pObject->IsOfType( V_RUNTIME_CLASS(mBasicEntity) ) )
		{
			sErrorMsgOut = "Component can only be added to instances of mBasicEntity or derived classes.";
			return FALSE;
		}
		return TRUE;
	}
	
	SAMPLEPLUGIN_IMPEXP VOVERRIDE void onFrameUpdate(float delta);

protected:
	void Serialize(VArchive& ar) HKV_OVERRIDE;
	void SetOwner(VisTypedEngineObject_cl *owner) HKV_OVERRIDE;

private:
	V_DECLARE_SERIAL_DLLEXP( mBaseComponent, SAMPLEPLUGIN_IMPEXP );
	V_DECLARE_VARTABLE( mBaseComponent, SAMPLEPLUGIN_IMPEXP );
};

//Everthing below is copied from my component
class mBaseComponent_Collection : public VRefCountedCollection<mBaseComponent> {};

//Copied from MyComponent.h
class mBaseComponent_ComponentManager : public IVisCallbackHandler_cl
{
public:

  //   Gets the singleton of the manager
  static mBaseComponent_ComponentManager &GlobalManager(){  return g_GlobalManager;  }

  //   Should be called at plugin initialization time.
  void OneTimeInit(){  Vision::Callbacks.OnUpdateSceneFinished += this;} // listen to this callback
  
  //   Should be called at plugin de-initialization time
  void OneTimeDeInit(){ Vision::Callbacks.OnUpdateSceneFinished -= this;} // de-register

  //   Callback method that takes care of updating the managed instances each frame
  VOVERRIDE void OnHandleCallback( IVisCallbackDataObject_cl *pData )
  {
    VASSERT( pData->m_pSender==&Vision::Callbacks.OnUpdateSceneFinished );
    // call update function on every component
    const int iCount = m_Components.Count();
    for (int i=0;i<iCount;i++)
	{ 
		m_Components.GetAt(i)->onFrameUpdate(Vision::GetTimer()->GetTimeDifference());
	}
  }

  //   Gets all VPlayableCharacterComponent instances this manager holds
  inline mBaseComponent_Collection &Instances() { return m_Components; }

protected:

  /// Holds the collection of all instances of MyComponent
  mBaseComponent_Collection m_Components;

  /// One global instance of our manager
  static mBaseComponent_ComponentManager g_GlobalManager;
};

#endif // MYCOMPONENT_H_INCLUDED