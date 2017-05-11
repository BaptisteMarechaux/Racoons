#include <iostream>


#include "BenTest.h"


#include "CatMull.h"
#include "Loops.h"




int testCatMull()
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



int testLoops()
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
		Edge(0, 1), //0
		Edge(1, 2),
		Edge(2, 0),
		Edge(2, 3),
		Edge(3, 0),

		Edge(4, 5), //5
		Edge(5, 6),
		Edge(6, 4),
		Edge(6, 7),
		Edge(7, 4),

		Edge(0, 4), //10
		Edge(1, 5),

		Edge(2, 6),
		Edge(3, 7),


		Edge(4, 1), //14
		Edge(3, 4),
		Edge(1, 6),
		Edge(2, 7),
	});

	cube.faces.insert(cube.faces.begin(), {
		Face({ 0, 1, 2 },{ 0, 1, 2}),
		Face({ 0, 2, 3 },{ 2, 3, 4 }),

		Face({ 4, 5, 6 },{ 5, 6, 7 }),
		Face({ 5, 6, 7 },{ 7, 8, 9 }),

		Face({ 0, 1, 4 },{ 10, 0, 14 }),
		Face({ 0, 4, 5 },{ 14, 11, 5 }),

		Face({ 2, 3, 6 },{ 13, 3, 17 }),
		Face({ 3, 6, 7 },{ 17, 12, 8 }),

		Face({ 0, 3, 4 },{ 10, 4, 15 }),
		Face({ 3, 4, 7 },{ 15, 9, 13 }),

		Face({ 1, 2, 6 },{ 1, 12, 16 }),
		Face({ 1, 5, 6 },{ 16, 6, 11 }),
	});



	Mesh loops_run = Loops(cube);

	return 0;
}