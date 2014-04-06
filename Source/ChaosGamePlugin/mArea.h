#pragma once
#include "ChaosGamePluginModule.h"
#include "mBasicEntity.h"
#include <vector>

class mArea : public mBasicEntity
{
public:
	mArea(void);
	~mArea(void);



	bool Intersects(std::shared_ptr<mArea> b, int lengthApart);
};

