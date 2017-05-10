#include "SimpleCornerCutting.h"
namespace SimpleCornerCutting
{
	Edge3D * eCutting ( Edge3D * e , float uRatio = 0.25f , float vRatio = 0.25f )
	{
		// gestion exception u et v si inf�rieur � 0 ou u + v sup�rieur � 1
		if ( uRatio < 0 )
			return nullptr;
		if ( vRatio < 0 )
			return nullptr;
		if ( uRatio + vRatio > 1 )
			return nullptr;
		// tada d�gueulasse


		glm::vec3 vecAB = e->get_B ( ) - e->get_A ( );
		glm::vec3 Abis = vecAB * uRatio + e->get_A ( );
		glm::vec3 Bbis = vecAB * ( 1 - vRatio ) + e->get_A ( );

		return new Edge3D ( Abis , Bbis );
	}

	Surface3D * sCutting ( Surface3D * s , int step = 1 , float uRatio = 0.25f , float vRatio = 0.25f )
	{

	}
}