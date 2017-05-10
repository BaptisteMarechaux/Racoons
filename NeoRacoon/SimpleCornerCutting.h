#pragma once
#include "Surface3D.h"
#include "Edge3D.h"
#include <vector>
#include <glm.hpp>

namespace SimpleCornerCutting
{
	Edge3D * eCutting ( Edge3D * e , float uRatio = 0.25f , float vRatio = 0.25f );
	Surface3D * sCutting ( Surface3D * s , int step = 1 , float uRatio = 0.25f , float vRatio = 0.25f );
}