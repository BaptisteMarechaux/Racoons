#include "CatMull.h"

CatMullData::CatMullData(const Mesh &mesh) :
	used_edge_points(mesh.edges.size(), -1),
	used_face_points(mesh.faces.size(), -1),
	used_vertex_points(mesh.vertices.size(), -1)
{
	edge_points.reserve(mesh.edges.size());
	mid_points.reserve(mesh.edges.size());
	face_points.reserve(mesh.faces.size());
	vertex_points.reserve(mesh.vertices.size());

	build(mesh);
}


void CatMullData::build(const Mesh &mesh)
{
	for (int i = 0; i < mesh.edges.size(); ++i)
	{
		edge_points.push_back(getEdgePoint(mesh, i));
		mid_points.push_back(getEdgeMidPoint(mesh, i));
	}

	for (int i = 0; i < mesh.faces.size(); ++i)
		face_points.push_back(getFaceCenter(mesh, i));

	for (int i = 0; i < mesh.vertices.size(); ++i)
		vertex_points.push_back(getVertexPoint(mesh, i));
}

Vertex CatMullData::getFaceCenter(const Mesh &mesh, int face_id)
{
	if (face_id < 0 || face_id >= mesh.faces.size())
		return Vertex();

	Vertex ret;

	for (auto it = mesh.faces[face_id].vertices.begin(); it != mesh.faces[face_id].vertices.end(); ++it)
	{
		const Vertex &ref(mesh.vertices[*it]);
		ret.x += ref.x;
		ret.y += ref.y;
		ret.z += ref.z;
	}

	size_t nb_points = mesh.faces[face_id].vertices.size();

	ret.x /= nb_points;
	ret.y /= nb_points;
	ret.z /= nb_points;

	return ret;
}

Vertex CatMullData::getEdgePoint(const Mesh &mesh, int edge_id)
{
	if (edge_id < 0 || edge_id >= mesh.edges.size())
		return Vertex();

	const Edge &edge = mesh.edges[edge_id];
	Vertex ret(mesh.vertices[edge.vertices[0]]);
	{
		const Vertex &ref(mesh.vertices[edge.vertices[1]]);
		ret.x += ref.x;
		ret.y += ref.y;
		ret.z += ref.z;
	}

	size_t nb_points = 2;
	auto edge_faces = mesh.getConnectedFacesToEdge(edge_id);

	for (auto it = edge_faces.begin(); it != edge_faces.end(); ++it)
	{
		Vertex p = getFaceCenter(mesh, *it);
		ret.x += p.x;
		ret.y += p.y;
		ret.z += p.z;
		++nb_points;
	}


	ret.x /= nb_points;
	ret.y /= nb_points;
	ret.z /= nb_points;

	return ret;
}

Vertex CatMullData::getEdgeMidPoint(const Mesh &mesh, int edge_id)
{
	Vertex ret;
	const Edge &ref(mesh.edges[edge_id]);
	const Vertex &v0(mesh.vertices[ref.vertices[0]]);
	const Vertex &v1(mesh.vertices[ref.vertices[1]]);

	ret.x = 0.5f * v0.x + 0.5f * v1.x;
	ret.y = 0.5f * v0.y + 0.5f * v1.y;
	ret.z = 0.5f * v0.z + 0.5f * v1.z;

	return ret;
}

Vertex CatMullData::getVertexPoint(const Mesh &mesh, int vert_id)
{
	Vertex ret;
	Vertex Q; // moyenne des points de faces
	Vertex R; // moyenne des points milieux des edges
	Vertex v; // vertex courant
	float n; // nombre d'edges incidents

				// Compute R
	{
		auto edge_ids = mesh.getConnectedEdges(vert_id);
		for (auto it = edge_ids.begin(); it != edge_ids.end(); ++it)
		{
			Vertex m = mid_points[*it];
			R.x += m.x;
			R.y += m.y;
			R.z += m.z;
		}

		n = static_cast<float>(edge_ids.size());

		R.x *= 2.0f / pow(n, 2.0f);
		R.y *= 2.0f / pow(n, 2.0f);
		R.z *= 2.0f / pow(n, 2.0f);
	}

	// Compute Q
	{
		auto face_ids = mesh.getConnectedFaces(vert_id);
		for (auto it = face_ids.begin(); it != face_ids.end(); ++it)
		{
			Vertex f = face_points[*it];
			Q.x += f.x;
			Q.y += f.y;
			Q.z += f.z;
		}

		float f_n = static_cast<float>(face_ids.size());

		Q.x /= n * f_n;
		Q.y /= n * f_n;
		Q.z /= n * f_n;
	}

	// Compute v
	{
		v = mesh.vertices[vert_id];
		v.x *= (n - 3.0f) / n;
		v.y *= (n - 3.0f) / n;
		v.z *= (n - 3.0f) / n;
	}


	ret.x = Q.x + R.x + v.x;
	ret.y = Q.y + R.y + v.y;
	ret.z = Q.z + R.z + v.z;

	return ret;
}




int catmull_internal::CatMull_get_vert_id(const Edge &edge0, const Edge &edge1)
{
	if (edge0.vertices[0] == edge1.vertices[1] || edge0.vertices[0] == edge1.vertices[0])
		return edge0.vertices[0];
	else if (edge0.vertices[1] == edge1.vertices[1] || edge0.vertices[1] == edge1.vertices[0])
		return edge0.vertices[1];

	return -1;
}


void catmull_internal::CatMull_add_vertices(CatMullData &data, int edge0_id, int edge1_id, int vert_id, Mesh &out)
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


void catmull_internal::CatMull_add_edge(CatMullData &data, int edgepoint0_id, int edgepoint1_id, int vert_id, Mesh &out)
{
	int facepoint_id = out.faces.back().vertices.front();
	
	Edge e(facepoint_id, edgepoint0_id);
	int e_id = out.getEdgeId(e);
	if (e_id < 0)
	{
		out.edges.push_back(e);
		out.faces.back().edges.push_back(static_cast<int>(out.edges.size() - 1));
	}
	else
		out.faces.back().edges.push_back(e_id);

	e = Edge(edgepoint0_id, vert_id);
	e_id = out.getEdgeId(e);
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

	e = Edge(edgepoint1_id, facepoint_id);
	e_id = out.getEdgeId(e);
	if (e_id < 0)
	{
		out.edges.push_back(e);
		out.faces.back().edges.push_back(static_cast<int>(out.edges.size() - 1));
	}
	else
		out.faces.back().edges.push_back(e_id);
}


void catmull_internal::CatMull_connect_edge(const Mesh &mesh, CatMullData &data, int edge0_id, int edge1_id, Mesh &out)
{
	CatMull_add_vertices(data, edge0_id, edge1_id, CatMull_get_vert_id(mesh.edges[edge0_id], mesh.edges[edge1_id]), out);
	const Face &face = out.faces.back();
	int current_index = static_cast<int>(face.vertices.size());
	CatMull_add_edge(data, face.vertices[current_index - 3], face.vertices[current_index - 1], face.vertices[current_index - 2], out);
}



void catmull_internal::CatMull_connect_face(const Mesh &mesh, CatMullData &data, int face_id, Mesh &out)
{
	out.faces.push_back(Face());
	if (data.used_face_points[face_id] == -1)
	{
		out.vertices.push_back(data.face_points[face_id]);
		data.used_face_points[face_id] = static_cast<int>(out.vertices.size() - 1);
	}

	out.faces.back().vertices.push_back(data.used_face_points[face_id]);
	
	int facepoint_id = data.used_face_points[face_id];


	const Face &face = mesh.faces[face_id];
	CatMull_connect_edge(mesh, data, face.edges.back(), face.edges.front(), out);
	for (int i = 0, imax = static_cast<int>(face.edges.size() - 1); i < imax; ++i)
	{
		out.faces.push_back(Face());
		out.faces.back().vertices.push_back(facepoint_id);
		CatMull_connect_edge(mesh, data, face.edges[i], face.edges[i + 1], out);
	}
}

Mesh CatMull(const Mesh &mesh)
{
	Mesh ret;
	CatMullData cm_data(mesh);

	for (int i = 0; i < mesh.faces.size(); ++i)
		catmull_internal::CatMull_connect_face(mesh, cm_data, i, ret);

	return ret;
}
