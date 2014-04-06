#include "ChaosGamePluginPCH.h"
#include "mMap.h"
#include <map>

V_IMPLEMENT_SERIAL ( mMap, mBasicEntity, 0, &g_myComponentModule );

START_VAR_TABLE( mMap, mBasicEntity, "Thy Map", 0, "")
	DEFINE_VAR_VSTRING_AND_NAME(mMap, currentTask, "Current Task", "State's current task of Generate method", "Doing nothing", 50, 0, 0);

	DEFINE_VAR_INT_AND_NAME(mMap, seed, "Generation seed", "Seed used to generate the object(Level Number)", "0", 0, 0);
	
	DEFINE_VAR_FLOAT_AND_NAME(mMap, m_enemySpawn.x, "Enemy Spawn Point X", "X Spawn Point", "0", 0, 0);
	DEFINE_VAR_FLOAT_AND_NAME(mMap, m_enemySpawn.y, "Enemy Spawn Point Y", "Y Spawn Point", "0", 0, 0);
	DEFINE_VAR_FLOAT_AND_NAME(mMap, m_enemySpawn.z, "Enemy Spawn Point Z", "Z Spawn Point", "0", 0, 0);

	DEFINE_VAR_FLOAT_AND_NAME(mMap, m_friendlySpawn.x, "Friendly Spawn Point X", "X Spawn Point", "0", 0, 0);
	DEFINE_VAR_FLOAT_AND_NAME(mMap, m_friendlySpawn.y, "Friendly Spawn Point Y", "Y Spawn Point", "0", 0, 0);
	DEFINE_VAR_FLOAT_AND_NAME(mMap, m_friendlySpawn.z, "Friendly Spawn Point Z", "Z Spawn Point", "0", 0, 0);

	DEFINE_VAR_INT_AND_NAME(mMap, total_rooms, "Total Rooms", "The amount of total rooms", "2", 0, 0);
	DEFINE_VAR_INT_AND_NAME(mMap, room_min, "Minimum room size", "The smallest possible room", "4", 0, 0);
	DEFINE_VAR_INT_AND_NAME(mMap, room_max, "Maximum room size", "The biggest possible room", "15", 0, 0);
	DEFINE_VAR_INT_AND_NAME(mMap, outer_ring, "Size of outer ring of the map", "Size of map coat", "1", 0, 0);
	
	DEFINE_VAR_FLOAT_AND_NAME(mMap, vMeshSize, "vMesh Size", "The size of the vMesh in float", "100.1", 0, 0);
	DEFINE_VAR_INT_AND_NAME(mMap, xoff, "X Space Between Rooms", "X SBR", "9", 0, 0);
	DEFINE_VAR_INT_AND_NAME(mMap, yoff, "Y Space Between Rooms", "Y SBR", "9", 0, 0);
	DEFINE_VAR_INT_AND_NAME(mMap, normal_textures, "Total Normal Textures", ".", "1", 0, 0);
	DEFINE_VAR_INT_AND_NAME(mMap, road_textures, "Total Road Textures", ".", "1", 0, 0);
	DEFINE_VAR_INT_AND_NAME(mMap, object_meshes, "Total Object Meshes", ".", "0", 0, 0);

	DEFINE_VAR_INT_AND_NAME(mMap, xC1, "xC1", ".", "9", 0, 0);
	DEFINE_VAR_INT_AND_NAME(mMap, yC1, "yC1", ".", "1", 0, 0);
	DEFINE_VAR_INT_AND_NAME(mMap, xC2, "xC2", ".", "1", 0, 0);
	DEFINE_VAR_INT_AND_NAME(mMap, yC2, "yC2", ".", "0", 0, 0);
	DEFINE_VAR_BOOL_AND_NAME(mMap, buildRoom, "Build Room", ".", "FALSE", 0,0);
	DEFINE_VAR_BOOL_AND_NAME(mMap, buildPath, "Build Path", ".", "FALSE", 0,0);
END_VAR_TABLE

int mMap::minX = 0, mMap::minY = 0, mMap::maxX = 0, mMap::maxY = 0;
SerialDynArray_cl<hkvVec3> mMap::m_full_path = SerialDynArray_cl<hkvVec3>(0);

void mMap::InitializeProperties()
{
	mBasicEntity::InitializeProperties();
}

//Place the object, create the objects etc
void mMap::PostInitialize()
{
}

void mMap::InitMap()
{
	this->Generate(seed);
	if(outer_ring > 0)
	{
		for(int x = mMap::minX; x <= mMap::maxX; x++)
		{
			for(int y = mMap::minY; y <= mMap::maxY; y++)
			{
				VString key;
				key.FormatEx("C_%d_%d", x, y);
				VSmartPtr<mCell> eCell = (mCell *) Vision::Game.SearchEntity(key);
				if(eCell.m_pPtr != nullptr)
				{
					CheckCloseCells(x, y, outer_ring);
				}
			}
		}
	}
	WasBuilt = true;
}

void mMap::ClearMap()
{
	if(WasBuilt)
	{
		for(int x = mMap::minX; x <= mMap::maxX; x++)
		{
			for(int y = mMap::minY; y <= mMap::maxY; y++)
			{
				VString key;
				key.FormatEx("C_%d_%d", x, y);
				VSmartPtr<mCell> eCell = (mCell *) Vision::Game.SearchEntity(key);
				if(eCell.m_pPtr != nullptr)
				{
					eCell->Dispose();
				}
				else
				{
					key.FormatEx("C_%d_%d_SUPL", x, y);
					VSmartPtr<mCell> eCell1 = (mCell *) Vision::Game.SearchEntity(key);
					if(eCell.m_pPtr != nullptr)
					{
						eCell1->Dispose();
					}
				}
			}
		}
		WasBuilt = false;
	}
}

//Send x-1 and y-1
void mMap::CheckCloseCells(int x, int y, int size)
{
	int sX = x - size, sY = y - size;
	int limX = x + size, limY = y + size;
	for(x = sX; x <= limX; x++)
	{
		for(y = sY; y <= limY; y++)
		{
			VString key;
			key.FormatEx("C_%d_%d", x, y);
			VSmartPtr<mCell> eCell = (mCell *) Vision::Game.SearchEntity(key);
			key.FormatEx("C_%d_%d_SUPL", x, y);
			VSmartPtr<mCell> eCell1 = (mCell *) Vision::Game.SearchEntity(key);
			if(eCell.m_pPtr == nullptr && eCell1 == nullptr)
			{
				int oPos = 5;
				mMap::addCell(0, 0, -132, x, y, oPos, vMeshSize);
			}
		}
	}
}

mMap::mMap(void)
{
	WasBuilt = false;
}

mMap::mMap(int rooms, int room_size_min, int room_size_max)
{
	this->total_rooms = ((rooms > 0) ? rooms : 1);
	this->room_min = ((room_size_min > 4) ? room_size_min : 4);
	this->room_max = ((room_size_max < 15) ? room_size_max : 15);
}

mMap::~mMap(void)
{
}

void mMap::SetSettings(int normal, int road, int objects)
{
	this->normal_textures = normal;
	this->road_textures = road;
	this->object_meshes = objects;
}

void mMap::Generate(int seed)
{
	bool hadProblems = false;
	int rndMax = this->total_rooms * 4;
	srand(seed);
	DynObjArray_cl<VSmartPtr<mRoom>> rRooms = DynObjArray_cl<VSmartPtr<mRoom>>(rndMax);
	VSmartPtr<mRoom> EnemyRoom = GenerateRoom();
	VSmartPtr<mRoom> FriendlyRoom = GenerateRoom();
	hkvVec2 largest;
	//Step 1: Generate some random rooms
	currentTask = "Generating Random Rooms";
	for(int i = 0; i < rndMax; i++)
	{
		rRooms[i] = GenerateRoom();
	}
	//Step 2: Pick out the biggest rooms and make them main bases
	currentTask = "Picking out enemy and friendly room";
	EnemyRoom = rRooms[0];
	FriendlyRoom = rRooms[1];
	float eRArea = EnemyRoom->endPoint.x * EnemyRoom->endPoint.y;
	float fRArea = FriendlyRoom->endPoint.x * FriendlyRoom->endPoint.y;
	for(int i = 2; i < rndMax; i++)
	{
		float cRArea = rRooms[i]->endPoint.x * rRooms[i]->endPoint.y;
		if(cRArea > eRArea)
		{
			if(eRArea > fRArea)
			{
				FriendlyRoom = EnemyRoom;
				fRArea = eRArea;
			}
			EnemyRoom = rRooms[i];
			eRArea = cRArea;
		}
	}


	//Step 3: Add Room Connection point to each room
	FriendlyRoom->addConnectors(mMap::getRandom(0,4), -1);
	EnemyRoom->addConnectors(mMap::getRandom(0,4), -1);

	//Step 4:Select a random room, add a path to it then select another room
	currentTask = "Picking out other rooms and connecting them";
	VSmartPtr<mRoom> fromRoom = FriendlyRoom;
	fromRoom->Build(normal_textures,object_meshes, vMeshSize);
	int OffSetX = 0, OffSetY = 0;
	int fC = 1, tC = 1;
	for(int i = 0; i < total_rooms - 2; i++)
	{
		VSmartPtr<mRoom> room;
		int rnd = 0;
		while(room.m_pPtr == nullptr)
		{
			rnd = mMap::getRandom(0, rndMax);
			room = rRooms[rnd];
		}

		int Con1 = mMap::getRandom(0,4);
		int Con2 = mMap::getRandom(0,4);
		Con2 = (Con1 == Con2) ? (Con2 + 1) % 4 : Con2;

		room->addConnectors(Con1, Con2);

		OffSetX = (int)fromRoom->endPoint.x + xoff;
		OffSetY = (int)fromRoom->endPoint.y + yoff;
		room->OffSet(OffSetX, OffSetY);

		tC = (Con2 > Con1) ? 1 : 2;

		CreatePath(fromRoom, room, fC, tC).Build(vMeshSize);
		room->Build(normal_textures,object_meshes, vMeshSize);

		fromRoom = room;
		fC = (tC == 1) ? 2 : 1;
		rRooms.Remove(rnd);
	}
	
	OffSetX = (int)fromRoom->endPoint.x + xoff;
	OffSetY = (int)fromRoom->endPoint.y + yoff;
	EnemyRoom->OffSet(OffSetX, OffSetY);
	CreatePath(fromRoom, EnemyRoom, fC, 1).Build(vMeshSize);
	EnemyRoom->Build(normal_textures,object_meshes, vMeshSize);
	m_enemySpawn = EnemyRoom->Center();
	m_friendlySpawn = FriendlyRoom->Center();
	currentTask = "Finished Generating";
}

VSmartPtr<mRoom> mMap::GenerateRoom()
{
	hkvVec3 from(0);
	int dif = this->room_max - this->room_min;
	hkvVec3 to((float)(this->room_min + mMap::getRandom(1,dif)),(float)(this->room_min + mMap::getRandom(1,dif)),0);
	VSmartPtr<mRoom> m = (mRoom *) Vision::Game.CreateEntity("mRoom", from);
	m->startPoint = from;
	m->endPoint = to;
	return m;
}

mPath mMap::CreatePath(VSmartPtr<mRoom> from, VSmartPtr<mRoom> to, int fromCN, int toCN)
{
	mPath path = mPath();
	
	hkvVec3 fromCon;
	hkvVec3 toCon;

	if(toCN == 1)
		toCon = to->RC1;
	else
		toCon = to->RC2;

	if(fromCN == 1)
		fromCon = from->RC1;
	else
		fromCon = from->RC2;

	int fromZ = (int)fromCon.z;
	int toZ = (int)toCon.z;

	if(fromZ > toZ)
	{
		hkvVec3 tmp = toCon;
		toCon = fromCon;
		fromCon = tmp;
	}
		
	int xZ = (int)fromCon.z, yZ = (int)toCon.z;

	path.addPoint(hkvVec2(fromCon.x, fromCon.y), 3);
	
	int min = 0, max = 0;
	hkvVec2 currentPoint(0);

	int m = mMap::getRandom(0,2);

	if(xZ == 0 && yZ == 0) // Checked
	{
		currentPoint = hkvVec2(fromCon.x, fromCon.y - (float)mMap::getRandom(6, xoff / 2 + 2));
		path.addPoint(currentPoint, 3);
		if(m == 0)
		{
			currentPoint = hkvVec2(from->endPoint.x + (float)mMap::getRandom(6, xoff / 2 + 2), currentPoint.y);
			path.addPoint(currentPoint, 3);
			currentPoint = hkvVec2(currentPoint.x, toCon.y - (float)mMap::getRandom(6, xoff / 2 + 2));
			path.addPoint(currentPoint, 3);
		}
		currentPoint = hkvVec2(toCon.x, currentPoint.y);
		path.addPoint(currentPoint, 3);
	}
	else if(xZ == 0 && yZ == 1)  // Checked
	{
		currentPoint = hkvVec2(fromCon.x, fromCon.y - (float)mMap::getRandom(6, xoff / 2 + 2));
		path.addPoint(currentPoint, 3);
		currentPoint = hkvVec2(toCon.x - (float)mMap::getRandom(6, xoff / 2 + 2), currentPoint.y);
		path.addPoint(currentPoint, 3);
		currentPoint = hkvVec2(currentPoint.x, toCon.y);
		path.addPoint(currentPoint, 3);
	}
	else if(xZ == 0 && yZ == 2) // Checked
	{
		currentPoint = hkvVec2(fromCon.x, fromCon.y - (float)mMap::getRandom(8, xoff / 2 + 3));
		path.addPoint(currentPoint, 3);		
		float f = (from->endPoint.x - to->startPoint.x);
		currentPoint = hkvVec2(from->endPoint.x - f / 2, currentPoint.y);
		path.addPoint(currentPoint, 3);
		currentPoint = hkvVec2(currentPoint.x, toCon.y + (float)mMap::getRandom(8, xoff / 2 + 3));
		path.addPoint(currentPoint, 3);
		currentPoint = hkvVec2(toCon.x, currentPoint.y);
		path.addPoint(currentPoint, 3);
	}
	else if(xZ == 0 && yZ == 3) // Checked
	{
		currentPoint = hkvVec2(fromCon.x, fromCon.y - (float)mMap::getRandom(6, xoff / 2 + 5));
		path.addPoint(currentPoint, 3);	
		currentPoint = hkvVec2(toCon.x + (float)mMap::getRandom(6, xoff / 2 + 5), currentPoint.y);
		path.addPoint(currentPoint, 3);
		currentPoint = hkvVec2(currentPoint.x, toCon.y);
		path.addPoint(currentPoint, 3);
	}
	else if(xZ == 1 && yZ == 1) // Checked
	{
		currentPoint = hkvVec2(fromCon.x - (float)mMap::getRandom(6, xoff / 2 + 3), fromCon.y);
		path.addPoint(currentPoint, 3);	
		currentPoint = hkvVec2(currentPoint.x, from->startPoint.y + (float)mMap::getRandom(6, xoff / 2 + 3));
		path.addPoint(currentPoint, 3);
		currentPoint = hkvVec2(toCon.x - (float)mMap::getRandom(6, xoff / 2 + 3), currentPoint.y);
		path.addPoint(currentPoint, 3);
		currentPoint = hkvVec2(currentPoint.x, toCon.y);
		path.addPoint(currentPoint, 3);
	}
	else if(xZ == 1 && yZ == 2) // Checked
	{
		currentPoint = hkvVec2(fromCon.x - (float)mMap::getRandom(6, xoff / 2 + 3), fromCon.y);
		path.addPoint(currentPoint, 3);	
		currentPoint = hkvVec2(currentPoint.x, toCon.y + (float)mMap::getRandom(6, xoff / 2 + 3));
		path.addPoint(currentPoint, 3);
		currentPoint = hkvVec2(toCon.x, currentPoint.y);
		path.addPoint(currentPoint, 3);
	}
	else if(xZ == 1 && yZ == 3) // Checked
	{
		currentPoint = hkvVec2(fromCon.x - (float)mMap::getRandom(8, xoff / 2 + 3), fromCon.y);
		path.addPoint(currentPoint, 3);
		float f = (from->endPoint.y - to->startPoint.y);
		currentPoint = hkvVec2(currentPoint.x, from->endPoint.y - f / 2);
		path.addPoint(currentPoint, 3);	
		currentPoint = hkvVec2(toCon.x + (float)mMap::getRandom(8, xoff / 2 + 3), currentPoint.y);
		path.addPoint(currentPoint, 3);	
		currentPoint = hkvVec2(currentPoint.x, toCon.y);
		path.addPoint(currentPoint, 3);

	}
	else if(xZ == 2 && yZ == 2)
	{
		currentPoint = hkvVec2(fromCon.x, fromCon.y + (float)mMap::getRandom(6, xoff / 2 + 3));
		path.addPoint(currentPoint, 3);	
		currentPoint = hkvVec2(to->startPoint.x - (float)mMap::getRandom(4, 8), currentPoint.y);
		path.addPoint(currentPoint, 3);	
		currentPoint = hkvVec2(currentPoint.x, toCon.y + (float)mMap::getRandom(6, xoff / 2 + 3));
		path.addPoint(currentPoint, 3);	
		currentPoint = hkvVec2(toCon.x, currentPoint.y);
		path.addPoint(currentPoint, 3);	
	}
	else if(xZ == 2 && yZ == 3)
	{
		currentPoint = hkvVec2(fromCon.x, fromCon.y + (float)mMap::getRandom(6, xoff / 2 + 3));
		path.addPoint(currentPoint, 3);	
		currentPoint = hkvVec2(toCon.x + (float)mMap::getRandom(4, 8), currentPoint.y);
		path.addPoint(currentPoint, 3);	
		currentPoint = hkvVec2(currentPoint.x, toCon.y);
		path.addPoint(currentPoint, 3);
	}
	else if(xZ == 3 && yZ == 3)
	{
		currentPoint = hkvVec2(fromCon.x + (float)mMap::getRandom(6, xoff / 2 + 3), fromCon.y);
		path.addPoint(currentPoint, 3);	
		currentPoint = hkvVec2(currentPoint.x, from->endPoint.y + (float)mMap::getRandom(6, xoff / 2 + 3));
		path.addPoint(currentPoint, 3);
		currentPoint = hkvVec2(toCon.x + (float)mMap::getRandom(6, xoff / 2 + 3), currentPoint.y);
		path.addPoint(currentPoint, 3);
		currentPoint = hkvVec2(currentPoint.x, toCon.y);
		path.addPoint(currentPoint, 3);
	}
	
	if(xZ != -1 && yZ != -1)
	{
		currentPoint = hkvVec2(toCon.x, toCon.y);
		path.addPoint(currentPoint, 3);
	}

	return path;
}

const VSmartPtr<mCell> mMap::addCell(int tType, int tId, int objId, int x, int y, int oPos, float meshSize)
{
	hkvVec3 pos((float)x * meshSize,(float)y * meshSize, 0);
	VSmartPtr<mCell> s = (mCell *) Vision::Game.CreateEntity("mCell", pos);

	if(oPos != 5)
	{
		if(mMap::minX > x)
			mMap::minX = x;
		if(mMap::minY > y)
			mMap::minY = y;
		if(mMap::maxX < x)
			mMap::maxX = x;
		if(mMap::maxY < y)
			mMap::maxY = y;
	}

	s->x = x;
	s->y = y;

	VString key;

	switch(tType)
	{
	case 0:
		switch(tId)
		{
		case 0:
			s->SetMesh("Meshes/Cells/Cell00.vmesh");
			break;
		case 1:
			s->SetMesh("Meshes/Cells/Cell01.vmesh");
			break;
		case 2:
			s->SetMesh("Meshes/Cells/Cell02.vmesh");
			break;
		case 3:
			s->SetMesh("Meshes/Cells/Cell03.vmesh");
			break;
		default:
			s->SetMesh("Meshes/Cells/Cell00.vmesh");
			break;
		}
		break;
	case 1:
		switch(tId)
		{
		case 0:
			s->SetMesh("Meshes/Cells/Cell10.vmesh");
			break;
		case 1:
			s->SetMesh("Meshes/Cells/Cell11.vmesh");
			break;
		case 2:
			s->SetMesh("Meshes/Cells/Cell12.vmesh");
			break;
		case 3:
			s->SetMesh("Meshes/Cells/Cell13.vmesh");
			break;
		case 4:
			s->SetMesh("Meshes/Cells/Cell14.vmesh");
			break;
		case 5:
			s->SetMesh("Meshes/Cells/Cell15.vmesh");
			break;
		case 6:
			s->SetMesh("Meshes/Cells/Cell16.vmesh");
			break;
		case 7:
			s->SetMesh("Meshes/Cells/Cell17.vmesh");
			break;
		case 8:
			s->SetMesh("Meshes/Cells/Cell18.vmesh");
			break;
		case 9:
			s->SetMesh("Meshes/Cells/Cell19.vmesh");
			break;
		case 10:
			s->SetMesh("Meshes/Cells/Cell110.vmesh");
			break;
		case 11:
			s->SetMesh("Meshes/Cells/Cell111.vmesh");
			break;
		case 12:
			s->SetMesh("Meshes/Cells/Cell112.vmesh");
			break;
		}
		break;
	}

	SerialDynArray_cl<hkvVec2> treePos = SerialDynArray_cl<hkvVec2>(6);
	SerialDynArray_cl<hkvVec3> objPos = SerialDynArray_cl<hkvVec3>(10);
	
	for(int i = 0; i < (int)treePos.GetSize(); i ++)
	{
		if(i < (int)treePos.GetSize())
			treePos[i] = hkvVec2(-1000.f);
		objPos[i] = hkvVec3(-1000.f);
	}
	
	float fThird = meshSize / 18 * 5;
	float sCell = 12.f;
	float eCell = 27.f;
	
	int makeGrassUpTo = 4;

	switch(oPos)
	{
	case -3://-3 For no grass
		makeGrassUpTo = 0;
		break;
	case -2://-3 For no grass
		makeGrassUpTo = mMap::getRandom(1,4);
		break;
	case -1://-3 For no grass
		makeGrassUpTo = mMap::getRandom(3,7);
		break;
	case 0: //On Grass
		makeGrassUpTo = mMap::getRandom(1,9);
		break;
	case 1:
		treePos[0] = hkvVec2(x * meshSize + fThird, y * meshSize + fThird * (float)mMap::getRandom(1,4) - sCell);
		treePos[1] = hkvVec2(x * meshSize + fThird * 2.f, y * meshSize + fThird * (float)mMap::getRandom(1,4) - sCell);
		treePos[2] = hkvVec2(x * meshSize + fThird * 3.f, y * meshSize + fThird * (float)mMap::getRandom(1,4) - sCell);
		break;
	case 2:
		treePos[0] = hkvVec2(x * meshSize + fThird * (float)mMap::getRandom(0,3) + eCell, y * meshSize + fThird);
		treePos[1] = hkvVec2(x * meshSize + fThird * (float)mMap::getRandom(0,3) + eCell, y * meshSize + fThird * 2.f);
		treePos[2] = hkvVec2(x * meshSize + fThird * (float)mMap::getRandom(0,3) + eCell, y * meshSize + fThird * 3.f);
		break;
	case 3:
		treePos[0] = hkvVec2(x * meshSize + fThird, y * meshSize + fThird * (float)mMap::getRandom(0,3) + eCell);
		treePos[1] = hkvVec2(x * meshSize + fThird * 2.f, y * meshSize + fThird * (float)mMap::getRandom(0,3) + eCell);
		treePos[2] = hkvVec2(x * meshSize + fThird * 3.f, y * meshSize + fThird * (float)mMap::getRandom(0,3) + eCell);
		break;
	case 4:
		treePos[0] = hkvVec2(x * meshSize + fThird * (float)mMap::getRandom(1,4) - sCell, y * meshSize + fThird);
		treePos[1] = hkvVec2(x * meshSize + fThird * (float)mMap::getRandom(1,4) - sCell, y * meshSize + fThird * 2.f);
		treePos[2] = hkvVec2(x * meshSize + fThird * (float)mMap::getRandom(1,4) - sCell, y * meshSize + fThird * 3.f);
		break;
	case 5:
		treePos[0] = hkvVec2(x * meshSize + fThird * (float)mMap::getRandom(1,4) - sCell, y * meshSize + fThird);
		treePos[1] = hkvVec2(x * meshSize + fThird * (float)mMap::getRandom(1,4) - sCell, y * meshSize + fThird * 2.f);
		treePos[2] = hkvVec2(x * meshSize + fThird * (float)mMap::getRandom(1,4) - sCell, y * meshSize + fThird * 3.f);
		treePos[3] = hkvVec2(x * meshSize + fThird * (float)mMap::getRandom(0,3) + eCell, y * meshSize + fThird);
		treePos[4] = hkvVec2(x * meshSize + fThird * (float)mMap::getRandom(0,3) + eCell, y * meshSize + fThird * 2.f);
		treePos[5] = hkvVec2(x * meshSize + fThird * (float)mMap::getRandom(0,3) + eCell, y * meshSize + fThird * 3.f);
		break;
	}

	for(int i = 0; i < makeGrassUpTo; i ++)
	{
		float VarX = (float)mMap::getRandom(10, (int)meshSize - 10);
		float VarY = (float)mMap::getRandom(10, (int)meshSize - 10);
		objPos[i] = hkvVec3((float)(x * meshSize + VarX), (float)(y * meshSize + VarY), -1.f);
	}

	for(int i = 0; i < (int)treePos.GetSize(); i++)
	{
		hkvVec3 tPos = treePos[i].getAsVec3(0);

		if(tPos.x <= -999.f)
			continue;

		VSmartPtr<mStatObject> tree = (mStatObject *) Vision::Game.CreateEntity("mStatObject", tPos);
		tree->Set((int)tPos.x, (int)tPos.y, mMap::getRandom(0,2));
		key.FormatEx("C_%d_%d_Tree_%d", x, y, i);
		tree->SetEntityKey(key);
	}

	for(int i = 0; i < (int)objPos.GetSize(); i++)
	{
		hkvVec3 tPos = objPos[i];

		if(tPos.x <= -999.f)
			continue;

		VSmartPtr<mStatObject> object = (mStatObject *) Vision::Game.CreateEntity("mStatObject", tPos);
		object->Set((int)tPos.x, (int)tPos.y, (int)tPos.z);
		key.FormatEx("C_%d_%d_Obj_%d", x, y, i);
		object->SetEntityKey(key);
	}
	
	if(objId == -132)
		key.FormatEx("C_%d_%d_SUPL", x, y);
	else
		key.FormatEx("C_%d_%d", x, y);

	s->SetEntityKey(key);
	s->SetPosition(pos);
	s->ReComputeVisibility();
	return s;
}

const int mMap::getRandom(int min, int max)
{
	int x = max - min;
	int rnd;
	if(x < 2)
	{
		rnd = 0;
	}
	else
	{
		rnd = (rand() % x);
	}
	return rnd + min;
}

const int mMap::PrintToConsole(VString classFrom, VString text)
{
	text.InsertAt(0, classFrom);
	Vision::GetConsoleManager()->OutputTextLine(text);
	return 0;
}