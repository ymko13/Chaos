#pragma once
#include <Vision/Runtime/Engine/System/Vision.hpp>

//This class is directly copied from RPG Game Plugin provided with Havok SDK, which is stated in .cpp file
namespace mySerializer
{
  void WriteVariableList(VTypedObject const *typedObj, VArchive& ar);

  void ReadVariableList(VTypedObject *typedObj, VArchive /*const*/& ar);
};


