#include "Surface3D.h"

std::vector<Edge3D*> Surface3D::get_Edges ( ) const
{
	return this->_Edges;
}

bool Surface3D::get_Close ( ) const
{
	return this->_Close;
}

int Surface3D::get_NbrEdges ( ) const
{
	return ( this->_Close ) ? this->_Edges.size ( ) : this->_Edges.size ( ) + 1;
}

Surface3D::~Surface3D ( )
{
}
