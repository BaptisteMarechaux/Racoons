#pragma once

#include "MeshUtils.h"

struct CatMullData
{
	std::vector<Vertex> edge_points;
	std::vector<Vertex> mid_points;
	std::vector<Vertex> face_points;
	std::vector<Vertex> vertex_points;
	
	std::vector<int> used_edge_points;
	std::vector<int> used_face_points;
	std::vector<int> used_vertex_points;


	CatMullData(const Mesh &mesh);


	private:
		void build(const Mesh &mesh);

		Vertex getFaceCenter(const Mesh &mesh, int face_id);

		Vertex getEdgePoint(const Mesh &mesh, int edge_id);

		Vertex getEdgeMidPoint(const Mesh &mesh, int edge_id);

		Vertex getVertexPoint(const Mesh &mesh, int vert_id);
};


namespace catmull_internal
{
	int CatMull_get_vert_id(const Edge &edge0, const Edge &edge1);

	void CatMull_add_vertices(CatMullData &data, int edge0_id, int edge1_id, int vert_id, Mesh &out);

	void CatMull_add_edge(CatMullData &data, int edgepoint0_id, int edgepoint1_id, int vert_id, Mesh &out);

	void CatMull_connect_edge(const Mesh &mesh, CatMullData &data, int edge0_id, int edge1_id, Mesh &out);

	void CatMull_connect_face(const Mesh &mesh, CatMullData &data, int face_id, Mesh &out);
}

Mesh CatMull(const Mesh &mesh);