#include "ChaosGamePluginPCH.h"
#include "mStatObject.h"

V_IMPLEMENT_SERIAL ( mStatObject, mBasicEntity, 0, &g_myComponentModule );

START_VAR_TABLE( mStatObject, mBasicEntity, "The Object", 0, "")
END_VAR_TABLE

void mStatObject::Set(int x, int y, int treeId)
{
	this->x = x;
	this->y = y;
	this->objId = treeId;

	hkvMat3 rotation;
	rotation = this->GetRotationMatrix();
	rotation.setRotationMatrixZ((float)mMap::getRandom(0,180) / 1.f);
	float Scaling = 1.f;
	if(this->objId == -1)
	{
		this->SetMesh("Meshes/Stones/Grass.vmesh");
		Scaling = (float)mMap::getRandom(85, 150) / 100.f;
	}	
	if(this->objId == 0)
	{
		this->SetMesh("Meshes/Trees/Tree00.vmesh");
		Scaling = (float)mMap::getRandom(85, 125) / 100.f;
	}
	if(this->objId == 1)
	{
		this->SetMesh("Meshes/Trees/Tree01.vmesh");
		Scaling = (float)mMap::getRandom(85, 125) / 100.f;
	}
	if(this->objId == 2) //1
	{
		this->SetMesh("Meshes/Stones/Stone00.vmesh");
		Scaling = (float)mMap::getRandom(85, 110) / 100.f;
	}
	if(this->objId == 3) //2
	{
		this->SetMesh("Meshes/Stones/Stone01.vmesh");
		Scaling = (float)mMap::getRandom(85, 110) / 100.f;
	}
	if(this->objId == 4) //3
	{
		this->SetMesh("Meshes/Stones/Stone02.vmesh");
		Scaling = (float)mMap::getRandom(85, 110) / 100.f;
	}
	
	this->SetRotationMatrix(rotation);
	this->SetScaling(Scaling);
	this->SetPosition((float)x,(float)y,0);

	this->ReComputeVisibility();
}

void mStatObject::InitializeProperties()
{
	mBasicEntity::InitializeProperties();
}

//Place the object, create the objects etc
void mStatObject::PostInitialize()
{
}

mStatObject::mStatObject(void)
{
}

mStatObject::~mStatObject(void)
{
}
