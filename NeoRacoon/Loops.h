#pragma once

#include "MeshUtils.h"

struct LoopsData
{
	std::vector<Vertex> edge_points;
	std::vector<Vertex> vertex_points;

	std::vector<int> used_edge_points;
	std::vector<int> used_vertex_points;


	LoopsData(const Mesh &mesh);


private:
	void build(const Mesh &mesh);

	Vertex getEdgePoint(const Mesh &mesh, int edge_id);

	Vertex getVertexPoint(const Mesh &mesh, int vert_id);
};



namespace loops_internal
{
	float Loops_getAlpha(size_t n);

	int Loops_get_vert_id(const Edge &edge0, const Edge &edge1);

	void Loops_add_vertices(LoopsData &data, int edge0_id, int edge1_id, int vert_id, Mesh &out);

	void Loops_add_edge(LoopsData &data, int edgepoint0_id, int edgepoint1_id, int vert_id, Mesh &out);

	void Loops_connect_edge(const Mesh &mesh, LoopsData &data, int edge0_id, int edge1_id, Mesh &out);

	void Loops_connect_face(const Mesh &mesh, LoopsData &data, int face_id, Mesh &out);
}

Mesh Loops(const Mesh &mesh);
