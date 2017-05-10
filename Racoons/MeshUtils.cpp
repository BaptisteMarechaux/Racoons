#include "MeshUtils.h"



Face::Face(std::initializer_list<int> vert_ids, std::initializer_list<int> edge_ids)
{
	vertices.insert(vertices.begin(), vert_ids);
	edges.insert(edges.begin(), edge_ids);
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
