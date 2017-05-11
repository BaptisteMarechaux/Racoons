#include "Loops.h"

#ifndef M___PI
	#define M___PI 3.14159265358979323846
#endif


LoopsData::LoopsData(const Mesh &mesh) :
	used_edge_points(mesh.edges.size(), -1),
	used_vertex_points(mesh.vertices.size(), -1)
{
	edge_points.reserve(mesh.edges.size());
	vertex_points.reserve(mesh.vertices.size());

	build(mesh);
}



void LoopsData::build(const Mesh &mesh)
{
	for (int i = 0; i < static_cast<int>(mesh.vertices.size()); ++i)
		vertex_points.push_back(getVertexPoint(mesh, i));

	for (int i = 0; i < mesh.edges.size(); ++i)
		edge_points.push_back(getEdgePoint(mesh, i));
}

Vertex LoopsData::getEdgePoint(const Mesh &mesh, int edge_id)
{
	if (edge_id < 0 || edge_id >= mesh.edges.size())
		return Vertex();

	Vertex ret;
	int v1_id = mesh.edges[edge_id].vertices[0], v2_id = mesh.edges[edge_id].vertices[1];
	const Vertex &v1(vertex_points[v1_id]), &v2(vertex_points[v2_id]);
	auto face_ids = mesh.getConnectedFacesToEdge(edge_id);
	std::vector<int> v_ids;
	for (size_t i = 0; i < face_ids.size(); ++i)
	{
		const Face &face = mesh.faces[face_ids[i]];
		auto it = std::find_if(face.vertices.begin(), face.vertices.end(), [v1_id, v2_id](int v_id) { return v_id != v1_id && v_id != v2_id; });
		if (it == face.vertices.end())
			continue;

		v_ids.push_back(*it);
	}

	Vertex v1v2(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
	Vertex v_other;
	for (auto it = v_ids.begin(); it != v_ids.end(); ++it)
	{
		const Vertex &v = vertex_points[*it];
		v_other.x += v.x;
		v_other.y += v.y;
		v_other.z += v.z;
	}

	v_other.x /= 8.0f;
	v_other.y /= 8.0f;
	v_other.z /= 8.0f;

	v1v2.x *= 3.0f / 8.0f;
	v1v2.y *= 3.0f / 8.0f;
	v1v2.z *= 3.0f / 8.0f;

	ret.x = v_other.x + v1v2.x;
	ret.y = v_other.y + v1v2.y;
	ret.z = v_other.z + v1v2.z;

	return ret;
}

Vertex LoopsData::getVertexPoint(const Mesh &mesh, int vert_id)
{
	const Vertex &v = mesh.vertices[vert_id];

	auto v_ids = mesh.getConnectedVertices(vert_id);
	float alpha = loops_internal::getAlpha(v_ids.size());
	float n_alpha = 1 - (v_ids.size() * alpha);

	Vertex ret(n_alpha * v.x, n_alpha * v.y, n_alpha * v.z);
	for (size_t i = 0; i < v_ids.size(); ++i)
	{
		const Vertex &v_i = mesh.vertices[v_ids[i]];
		ret.x += alpha * v_i.x;
		ret.y += alpha * v_i.y;
		ret.z += alpha * v_i.z;
	}

	return ret;
}






float loops_internal::getAlpha(size_t n)
{
	if (n == 3)
		return 3.0f / 16.0f;

	return 1.0f / n * (5.0f / 8.0f - pow(3.0f / 8.0f + (1.0f / 4.0f * cos((2.0f * M___PI) / n)), 2.0f));
}




int loops_internal::Loops_get_vert_id(const Edge &edge0, const Edge &edge1)
{
	if (edge0.vertices[0] == edge1.vertices[1] || edge0.vertices[0] == edge1.vertices[0])
		return edge0.vertices[0];
	else if (edge0.vertices[1] == edge1.vertices[1] || edge0.vertices[1] == edge1.vertices[0])
		return edge0.vertices[1];

	return -1;
}


void loops_internal::Loops_add_vertices(LoopsData &data, int edge0_id, int edge1_id, int vert_id, Mesh &out)
{
	if (data.used_edge_points[edge0_id] == -1)
	{
		out.vertices.push_back(data.edge_points[edge0_id]);
		data.used_edge_points[edge0_id] = static_cast<int>(out.vertices.size() - 1);
	}
	out.faces.back().vertices.push_back(data.used_edge_points[edge0_id]);

	if (data.used_vertex_points[vert_id] == -1)
	{
		out.vertices.push_back(data.vertex_points[vert_id]);
		data.used_vertex_points[vert_id] = static_cast<int>(out.vertices.size() - 1);
	}
	out.faces.back().vertices.push_back(data.used_vertex_points[vert_id]);


	if (data.used_edge_points[edge1_id] == -1)
	{
		out.vertices.push_back(data.edge_points[edge1_id]);
		data.used_edge_points[edge1_id] = static_cast<int>(out.vertices.size() - 1);
	}
	out.faces.back().vertices.push_back(data.used_edge_points[edge1_id]);
}


void loops_internal::Loops_add_edge(LoopsData &data, int edgepoint0_id, int edgepoint1_id, int vert_id, Mesh &out)
{
	int facepoint_id = out.faces.back().vertices.front();

	Edge e(edgepoint0_id, vert_id);
	int e_id = out.getEdgeId(e);
	if (e_id < 0)
	{
		out.edges.push_back(e);
		out.faces.back().edges.push_back(static_cast<int>(out.edges.size() - 1));
	}
	else
		out.faces.back().edges.push_back(e_id);

	e = Edge(vert_id, edgepoint1_id);
	e_id = out.getEdgeId(e);
	if (e_id < 0)
	{
		out.edges.push_back(e);
		out.faces.back().edges.push_back(static_cast<int>(out.edges.size() - 1));
	}
	else
		out.faces.back().edges.push_back(e_id);

	e = Edge(edgepoint1_id, edgepoint0_id);
	e_id = out.getEdgeId(e);
	if (e_id < 0)
	{
		out.edges.push_back(e);
		out.faces.back().edges.push_back(static_cast<int>(out.edges.size() - 1));
	}
	else
		out.faces.back().edges.push_back(e_id);
}


void loops_internal::Loops_connect_edge(const Mesh &mesh, LoopsData &data, int edge0_id, int edge1_id, Mesh &out)
{
	Loops_add_vertices(data, edge0_id, edge1_id, Loops_get_vert_id(mesh.edges[edge0_id], mesh.edges[edge1_id]), out);
	const Face &face = out.faces.back();
	int current_index = static_cast<int>(face.vertices.size());
	Loops_add_edge(data, face.vertices[current_index - 3], face.vertices[current_index - 1], face.vertices[current_index - 2], out);
}



void loops_internal::Loops_connect_face(const Mesh &mesh, LoopsData &data, int face_id, Mesh &out)
{
	out.faces.push_back(Face());

	const Face &face = mesh.faces[face_id];
	Loops_connect_edge(mesh, data, face.edges.back(), face.edges.front(), out);
	for (int i = 0, imax = static_cast<int>(face.edges.size() - 1); i < imax; ++i)
	{
		out.faces.push_back(Face());
		Loops_connect_edge(mesh, data, face.edges[i], face.edges[i + 1], out);
	}

	Face edge_face;
	Edge e(data.used_edge_points[face.edges.back()], data.used_edge_points[face.edges.front()]);
	int e_id = out.getEdgeId(e);
	if (e_id < 0)
	{
		out.edges.push_back(e);
		edge_face.edges.push_back(static_cast<int>(out.edges.size() - 1));
	}
	else
		edge_face.edges.push_back(e_id);

	for (int i = 0, imax = static_cast<int>(face.edges.size() - 1); i < imax; ++i)
	{
		e = Edge(data.used_edge_points[face.edges[i]], data.used_edge_points[face.edges[i + 1]]);
		e_id = out.getEdgeId(e);
		if (e_id < 0)
		{
			out.edges.push_back(e);
			edge_face.edges.push_back(static_cast<int>(out.edges.size() - 1));
		}
		else
			edge_face.edges.push_back(e_id);
	}

	out.faces.push_back(edge_face);
}

Mesh Loops(const Mesh &mesh)
{
	Mesh ret;
	LoopsData cm_data(mesh);

	for (int i = 0; i < mesh.faces.size(); ++i)
		loops_internal::Loops_connect_face(mesh, cm_data, i, ret);

	return ret;
}