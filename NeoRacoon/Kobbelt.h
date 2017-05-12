#pragma once


#pragma once

#include "MeshUtils.h"

struct KobbeltData
{
	std::vector<Vertex> face_points;
	std::vector<Vertex> vertex_points;

	std::vector<int> used_face_points;
	std::vector<int> used_vertex_points;


	KobbeltData(const Mesh &mesh);


private:
	void build(const Mesh &mesh);

	Vertex getFaceCenter(const Mesh &mesh, int edge_id);

	Vertex getVertexPoint(const Mesh &mesh, int vert_id);
};



namespace kobbelt_internal
{
	float Kobbelt_getAlpha(size_t n);

	int Kobbelt_get_vert_id(const Edge &edge0, const Edge &edge1);

	void Kobbelt_add_vertices(KobbeltData &data, int edge0_id, int edge1_id, int vert_id, Mesh &out);

	void Kobbelt_add_edge(KobbeltData &data, int edgepoint0_id, int edgepoint1_id, int vert_id, Mesh &out);

	void Kobbelt_connect_edge(const Mesh &mesh, KobbeltData &data, int edge0_id, int edge1_id, Mesh &out);

	void Kobbelt_connect_face(const Mesh &mesh, KobbeltData &data, int face_id, Mesh &out);
}

Mesh Kobbelt(const Mesh &mesh);

