#pragma once


#include <vector>
#include <initializer_list>


struct Vertex
{
	float x;
	float y;
	float z;
	/* à voir les trucs de textures et de normales */

	Vertex() : Vertex(0.0, 0.0, 0.0) {}
	Vertex(float x, float y, float z) : x(x), y(y), z(z) {}


	bool operator==(const Vertex &v) const
	{
		return x == v.x && y == v.y && z == v.z;
	}

	bool operator!=(const Vertex &v) const { return !(this->operator==(v)); }
};


struct Edge
{
	int vertices[2];

	Edge() : Edge(-1, -1) {}
	Edge(int v1, int v2) { vertices[0] = v1; vertices[1] = v2; }

	bool operator==(const Edge &e) const
	{
		return (vertices[0] == e.vertices[0] && vertices[1] == e.vertices[1])
			|| (vertices[0] == e.vertices[1] && vertices[1] == e.vertices[0]);
	}

	bool operator!=(const Edge &e) const { return !(this->operator==(e)); }
};


struct Face
{
	std::vector<int> vertices;
	std::vector<int> edges;

	Face() {}
	Face(std::initializer_list<int> vert_ids, std::initializer_list<int> edge_ids);

	bool operator==(const Face &f) const
	{
		for (auto it = vertices.begin(); it != vertices.end(); ++it)
		{
			if (std::count(f.vertices.begin(), f.vertices.end(), *it) != 1)
				return false;
		}

		for (auto it = edges.begin(); it != edges.end(); ++it)
		{
			if (std::count(f.edges.begin(), f.edges.end(), *it) != 1)
				return false;
		}

		return true;
	}

	bool operator!=(const Face &f) const { return !(this->operator==(f)); }
};


struct RenderableMesh
{
	std::vector<float> vertices;
	std::vector<uint16_t> indices;
};

struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<Edge> edges;
	std::vector<Face> faces;

	Mesh() {}

	std::vector<int> getConnectedEdges(int vert_id) const;

	std::vector<int> getConnectedFaces(int vert_id) const;

	std::vector<int> getConnectedFacesToEdge(int edge_id) const;

	int getEdgeId(const Edge &e);

	RenderableMesh getRenderableMesh()
	{
		RenderableMesh ret;
		ret.vertices.reserve(vertices.size() * 3);
		for (auto it = vertices.begin(); it != vertices.end(); ++it)
		{
			ret.vertices.emplace_back(it->x, it->y, it->z);
		}

		for (auto it = edges.begin(); it != edges.end(); ++it)
		{
			ret.indices.insert(ret.indices.end(), { static_cast<uint16_t>(it->vertices[0]), static_cast<uint16_t>(it->vertices[1]) });
		}
	}
};

