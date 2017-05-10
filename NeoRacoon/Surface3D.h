#pragma once
#include <vector>
#include <glm.hpp>
#include "Edge3D.h"

class Surface3D
{
	private:
	std::vector<Edge3D*> _Edges;
	bool _Close;
	public:
	Surface3D ( std::vector<Edge3D*> edges , bool close = false ) : _Edges ( edges ) , _Close ( close )
	{
	};

	std::vector<Edge3D*> get_Edges ( ) const;
	bool get_Close ( ) const;
	~Surface3D ( );
};

