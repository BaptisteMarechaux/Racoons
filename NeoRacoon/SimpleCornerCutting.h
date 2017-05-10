#pragma once
#include "Surface3D.h"
#include "Edge3D.h"
#include <glm.hpp>

namespace SimpleCornerCutting
{
	Surface3D * sCutting ( Surface3D * s );
	Edge3D * eCutting ( Edge3D * e , float u = 0.25f , float v = 0.25f );

}