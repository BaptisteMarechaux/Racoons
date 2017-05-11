#include <iostream>


#include "CatMull.h"




int testCurves()
{
	Mesh cube;
	cube.vertices.insert(cube.vertices.begin(), {
		Vertex(-0.5f, -0.5f, -0.5f),
		Vertex(-0.5f, -0.5f, 0.5f),
		Vertex(-0.5f, 0.5f, 0.5f),
		Vertex(-0.5f, 0.5f, -0.5f),

		Vertex(0.5f, -0.5f, -0.5f),
		Vertex(0.5f, -0.5f, 0.5f),
		Vertex(0.5f, 0.5f, 0.5f),
		Vertex(0.5f, 0.5f, -0.5f)
	});


	cube.edges.insert(cube.edges.begin(), {
		Edge(0, 1),
		Edge(1, 2),
		Edge(2, 3),
		Edge(3, 0),

		Edge(4, 5),
		Edge(5, 6),
		Edge(6, 7),
		Edge(7, 4),

		Edge(0, 4),
		Edge(1, 5),
		Edge(2, 6),
		Edge(3, 7)
	});

	cube.faces.insert(cube.faces.begin(), {
		Face({ 0, 1, 2, 3 }, { 0, 1, 2, 3 }),
		Face({ 4, 5, 6, 7 }, { 4, 5, 6, 7 }),

		Face({ 0, 1, 4, 5 }, { 8, 0, 9, 4 }),
		Face({ 2, 3, 6, 7 }, { 10, 2, 11, 6 }),

		Face({ 0, 3, 4, 7 }, { 3, 11, 7, 8 }),
		Face({ 1, 2, 5, 6 }, { 1, 10, 5, 9 }),
	});



	Mesh catmull_run = CatMull(cube);

	return 0;
}