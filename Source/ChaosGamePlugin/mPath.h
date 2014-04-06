#pragma once
#include "mcell.h"

class mPath
{
public:
	mPath(void);
	~mPath(void);

	SerialDynArray_cl<hkvVec3> paths;

	void addPoint(hkvVec2 path, int size);
	
	void Build(float meshSize);
};

