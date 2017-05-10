#include "Surface3D.h"

std::vector<Edge3D*> Surface3D::get_Edges ( ) const
{
	return this->_Edges;
}

bool Surface3D::get_Close ( ) const
{
	return this->_Close;
}

Surface3D::~Surface3D ( )
{
}
