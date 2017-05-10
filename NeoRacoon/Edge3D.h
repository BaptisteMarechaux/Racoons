#pragma once
#include "glm.hpp"
class Edge3D
{
	private:
	glm::vec3 _A;
	glm::vec3 _B;
	public:
	Edge3D ( glm::vec3 A , glm::vec3 B ) : _A ( A ) , _B ( B )
	{
	};
	Edge3D ( glm::vec3 B ) : _A ( ) , _B ( B )
	{
	};
	~Edge3D ( );

	glm::vec3 get_A ( ) const;
	glm::vec3 get_B ( ) const;

};

