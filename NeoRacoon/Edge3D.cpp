#include "Edge3D.h"

Edge3D::~Edge3D ( )
{
}

glm::vec3 Edge3D::get_A ( ) const
{
	return _A;
}

glm::vec3 Edge3D::get_B ( ) const
{
	return _B;
}
