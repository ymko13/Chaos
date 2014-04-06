#include "ChaosGamePluginPCH.h"
#include "mAiPath.h"


mAiPath::mAiPath(void)
{
}


mAiPath::~mAiPath(void)
{
}

void mAiPath::SetPath(SerialDynArray_cl<hkvVec3> m_Pts)
{
	m_pts_on_path = m_Pts;
	m_current_point = m_pts_on_path.GetValidSize() - 1;
}

void mAiPath::GetClosestPoint(int &indexPoint, hkvVec3 fromPoint, hkvVec3 &closestPoint)
{
	indexPoint = m_current_point;
	closestPoint = m_pts_on_path[m_current_point];
	int distance = (int)closestPoint.getDistanceToSquared(fromPoint);

	for(int i = 0; i < (int)m_pts_on_path.GetValidSize(); i++)
	{
		int d = (int)m_pts_on_path[i].getDistanceToSquared(fromPoint);
		if(d < distance)
		{
			closestPoint = m_pts_on_path[i];
			indexPoint = i;
			distance = d;
		}
	}
}

hkvVec3 mAiPath::GetNextPoint()
{
	m_current_point--;
	m_current_point = hkvMath::Max(0, m_current_point);
	return m_pts_on_path[m_current_point];
}

hkvVec3 mAiPath::GetCurrentPoint()
{
	return m_pts_on_path[m_current_point];
}

hkvVec3 mAiPath::GetPreviousPoint()
{
	m_current_point++;
	m_current_point = hkvMath::Min(m_current_point, (int)m_pts_on_path.GetValidSize() - 1);
	return m_pts_on_path[m_current_point];
}

hkvVec3 mAiPath::GetAt(const int i)
{
	return m_pts_on_path[i];
}