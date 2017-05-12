#include "MeshUtils.h"



Face::Face(std::initializer_list<int> vert_ids, std::initializer_list<int> edge_ids)
{
	vertices.insert(vertices.begin(), vert_ids);
	edges.insert(edges.begin(), edge_ids);
}


std::vector<int> Mesh::getConnectedVertices(int vert_id) const
{
	if (vert_id < 0)
		return std::vector<int>();

	std::vector<int> ret;

	int i = 0;
	for (auto it = edges.begin(); it != edges.end(); ++it, ++i)
	{
		if (it->vertices[0] == vert_id)
			ret.push_back(it->vertices[1]);
		else if (it->vertices[1] == vert_id)
			ret.push_back(it->vertices[0]);
	}

	return ret;
}



std::vector<int> Mesh::getConnectedEdges(int vert_id) const
{
	if (vert_id < 0)
		return std::vector<int>();

	std::vector<int> ret;

	int i = 0;
	for (auto it = edges.begin(); it != edges.end(); ++it, ++i)
	{
		if (it->vertices[0] == vert_id || it->vertices[1] == vert_id)
			ret.push_back(i);
	}

	return ret;
}

std::vector<int> Mesh::getConnectedFaces(int vert_id) const
{
	if (vert_id < 0)
		return std::vector<int>();

	std::vector<int> ret;

	int i = 0;
	for (auto it = faces.begin(); it != faces.end(); ++it, ++i)
	{
		if (std::find(it->vertices.begin(), it->vertices.end(), vert_id) != it->vertices.end())
			ret.push_back(i);
	}

	return ret;
}

std::vector<int> Mesh::getConnectedFacesToEdge(int edge_id) const
{
	if (edge_id < 0)
		return std::vector<int>();

	std::vector<int> ret;

	int i = 0;
	for (auto it = faces.begin(); it != faces.end(); ++it, ++i)
	{
		if (std::find(it->edges.begin(), it->edges.end(), edge_id) != it->edges.end())
			ret.push_back(i);
	}

	return ret;
}

int Mesh::getEdgeId(const Edge & e)
{
	auto it = std::find(edges.begin(), edges.end(), e);
	if (it == edges.end())
		return -1;


	return static_cast<int>(std::distance(edges.begin(), it));
}


std::vector<uint16_t> Mesh::faceToIndices(int face_id, const Vertex &barycenter) const
{
	const Face &f = faces[face_id];

	std::vector<uint16_t> indices;
	std::deque<Edge> e;
	for (size_t i = 0; i < f.edges.size(); ++i)
		e.push_back(edges[f.edges[i]]);

	Edge current = e.front();
	e.pop_front();
	indices.push_back(current.vertices[0]);
	indices.push_back(current.vertices[1]);
	int search = current.vertices[1];
	while (!e.empty())
	{
		auto it = std::find_if(e.begin(), e.end(), [&search](const Edge &edge) { return edge.vertices[0] == search || edge.vertices[1] == search; });
		if (it == e.end())
			return std::vector<uint16_t>();

		current = *it;
		e.erase(it);
		if (current.vertices[0] != search)
			current.swap();
		indices.push_back(current.vertices[0]);
		indices.push_back(current.vertices[1]);
		search = current.vertices[1];
	}

	const Vertex &p1(vertices[0]), p2(vertices[1]), p3(vertices[2]);
	Vertex A(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z), B(p3.x - p2.x, p3.y - p2.y, p3.z - p2.z);
	Vertex ABary(barycenter.x - p1.x, barycenter.y - p1.y, barycenter.z - p1.z);
	Vertex N(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.y * B.z - A.z * B.y);

	float dot = N.x * ABary.x + N.y * ABary.y + N.z * ABary.z;
	if (dot > 0)
		std::reverse(indices.begin(), indices.end());

	return indices;
}

RenderableMesh Mesh::getRenderableMesh() const
{
	RenderableMesh ret;
	ret.vertices.reserve(vertices.size() * 3);
	for (auto it = vertices.begin(); it != vertices.end(); ++it)
	{
		ret.vertices.emplace_back(it->x);
		ret.vertices.emplace_back(it->y);
		ret.vertices.emplace_back(it->z);
	}

	for (size_t i = 0; i < faces.size(); ++i)
	{
		auto ind = faceToIndices(i);
		ret.indices.insert(ret.indices.end(), ind.begin(), ind.end());
	}

	return ret;
}


Vertex Mesh::getBaryCenter() const
{
	Vertex bary;
	for (auto it = vertices.begin(); it != vertices.end(); ++it)
	{
		bary.x += it->x;
		bary.y += it->y;
		bary.z += it->z;
	}

	bary.x /= vertices.size();
	bary.y /= vertices.size();
	bary.z /= vertices.size();

	return bary;
}




std::vector<glm::vec3> RenderableMesh::toVec3() const
{

	std::vector<glm::vec3> ret;
	ret.reserve(vertices.size() / 3);

	for (size_t j = 0; j < indices.size(); j++)
	{
		size_t i = indices[j] * 3;
		ret.emplace_back(glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]));
	}

	return ret;
}
