#include "SimpleCornerCutting.h"
namespace SimpleCornerCutting
{
	Edge3D * eCutting ( Edge3D * e , float uRatio , float vRatio )
	{
		if ( uRatio < 0 )
			throw std::invalid_argument ( "uRatio is negative." );
		if ( vRatio < 0 )
			throw std::invalid_argument ( "vRatio is negative." );
		if ( uRatio + vRatio > 1 )
			throw std::invalid_argument ( "uRatio + vRatio is higher than 1." );

		glm::vec3 vecAB = e->get_B ( ) - e->get_A ( );
		glm::vec3 Abis = vecAB * uRatio + e->get_A ( );
		glm::vec3 Bbis = vecAB * ( 1 - vRatio ) + e->get_A ( );

		return new Edge3D ( Abis , Bbis );
	}

	Surface3D * sCutting ( Surface3D * s , int steps , float uRatio , float vRatio )
	{
		if ( uRatio < 0 )
			throw std::invalid_argument ( "uRatio is negative." );
		if ( vRatio < 0 )
			throw std::invalid_argument ( "vRatio is negative." );
		if ( uRatio + vRatio > 1 )
			throw std::invalid_argument ( "uRatio + vRatio is higher than 1." );

		std::vector<Edge3D*> edges = s->get_Edges ( );
		std::vector<Edge3D*> intermediaireEdges;
		Edge3D * e;

		for ( int step = 0; step < steps; ++step )
		{
			for ( int edge = 0; edge < edges.size ( ); ++edge )
			{
				e = eCutting ( edges [ edge ] );
				if ( edge > 0 )
					intermediaireEdges.push_back ( new Edge3D ( intermediaireEdges [ intermediaireEdges.size ( ) - 1 ]->get_B ( ) , e->get_A ( ) ) );
				intermediaireEdges.push_back ( e );
				if ( s->get_Close ( ) && edge == edges.size ( ) - 1 )
					intermediaireEdges.push_back ( new Edge3D ( intermediaireEdges [ intermediaireEdges.size ( ) - 1 ]->get_B ( ) , intermediaireEdges [ 0 ]->get_A ( ) ) );
			}
			edges = intermediaireEdges;
			intermediaireEdges.clear ( );
		}

		return new Surface3D ( edges );
	}
}