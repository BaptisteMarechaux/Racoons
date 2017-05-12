#include "Kobbelt.h"

#ifndef M___PI
#define M___PI 3.14159265358979323846
#endif

KobbeltData::KobbeltData(const Mesh &mesh) :
	used_face_points(mesh.faces.size(), -1),
	used_vertex_points(mesh.vertices.size(), -1)
{
	face_points.reserve(mesh.faces.size());
	vertex_points.reserve(mesh.vertices.size());

	build(mesh);
}



void KobbeltData::build(const Mesh &mesh)
{
	for (int i = 0; i < mesh.faces.size(); ++i)
		face_points.push_back(getFaceCenter(mesh, i));

	for (int i = 0; i < static_cast<int>(mesh.vertices.size()); ++i)
		vertex_points.push_back(getVertexPoint(mesh, i));
}


Vertex KobbeltData::getFaceCenter(const Mesh &mesh, int face_id)
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




Vertex KobbeltData::getVertexPoint(const Mesh &mesh, int vert_id)
{
	const Vertex &v = mesh.vertices[vert_id];

	auto v_ids = mesh.getConnectedVertices(vert_id);
	float n = v_ids.size();
	float alpha = kobbelt_internal::Kobbelt_getAlpha(n);
	float n_alpha = 1 - alpha;

	Vertex ret(n_alpha * v.x, n_alpha * v.y, n_alpha * v.z);
	Vertex vs;
	for (size_t i = 0; i < n; ++i)
	{
		const Vertex &v_i = mesh.vertices[v_ids[i]];
		vs.x += v_i.x;
		vs.y += v_i.y;
		vs.z += v_i.z;
	}

	ret.x += (alpha / n) * vs.x;
	ret.y += (alpha / n) * vs.y;
	ret.z += (alpha / n) * vs.z;

	return ret;
}






float kobbelt_internal::Kobbelt_getAlpha(size_t n)
{
	return 1.0f / 9.0f * (4.0f - (2.0f * cos((2.0f * M___PI) / n)));
}




void kobbelt_internal::Kobbelt_connect_face(const Mesh &mesh, KobbeltData &data, int vert_id, Mesh &out)
{
	auto edge_ids = mesh.getConnectedEdges(vert_id);

	if (data.used_vertex_points[vert_id] == -1)
	{
		out.vertices.push_back(data.vertex_points[vert_id]);
		data.used_vertex_points[vert_id] = static_cast<int>(out.vertices.size() - 1);
	}
	int v = data.used_vertex_points[vert_id];

	for (size_t i = 0; i < edge_ids.size(); ++i)
	{
		auto face_ids = mesh.getConnectedFacesToEdge(vert_id);
		Face f;
		f.vertices.push_back(v);
		for (size_t j = 0; j < face_ids.size(); ++j)
		{
			if (face_ids[j] == 7)
				__debugbreak();


			if (data.used_face_points[face_ids[j]] == -1)
			{
				out.vertices.push_back(data.face_points[face_ids[j]]);
				data.used_face_points[face_ids[j]] = static_cast<int>(out.vertices.size() - 1);
			}
			f.vertices.push_back(data.used_face_points[face_ids[j]]);

			Edge e(f.vertices[f.vertices.size() - 2], f.vertices.back());
			int e_id = out.getEdgeId(e);
			if (e_id < 0)
			{
				out.edges.push_back(e);
				f.edges.push_back(static_cast<int>(out.edges.size() - 1));
			}
			else
				f.edges.push_back(e_id);
		}

		Edge e(f.vertices.front(), f.vertices.back());
		int e_id = out.getEdgeId(e);
		if (e_id < 0)
		{
			out.edges.push_back(e);
			f.edges.push_back(static_cast<int>(out.edges.size() - 1));
		}
		else
			f.edges.push_back(e_id);

		out.faces.push_back(f);
	}
}

Mesh Kobbelt(const Mesh &mesh)
{
	Mesh ret;
	KobbeltData cm_data(mesh);

	for (int i = 0; i < mesh.vertices.size(); ++i)
		kobbelt_internal::Kobbelt_connect_face(mesh, cm_data, i, ret);

	return ret;
}

