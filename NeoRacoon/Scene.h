#pragma once
#include "imgui\imgui.h"
#include <stdio.h>
#include <vector>
#include <list>
#include <gl3w\GL\gl3w.h>
#include <glfw\include\GLFW\glfw3.h>
#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"

#include "Input.h"
#include "Voxel.h"
#include "Chunk.h"
#include "Surface3D.h"
#include "BenTest.h"
#include "Kobbelt.h"

enum CameraDirection {
	forward,
	backward,
	left,
	right
};

class Scene
{
private:
	//Matrix
	glm::mat4 model, proj, mvp, view;
	//General Elements
	

	//Buffers
	GLuint vertexBufferPoints;
	GLuint normalbuffer;
	//Other Buffers
	GLuint catmullVertexBuffer;
	GLuint originShapeVertexBuffer;

	std::vector<glm::vec3> normals, positions, vertices, originShapeVertices, catmullVertices;
	std::vector<GLuint> indices;

	//Shader References
	GLuint program, ssaoProgram;
	GLuint position_location, color_location, mvp_location, light_location;
	GLuint MatrixID, VertexArrayID, LightID, ModelMatrixID, ViewMatrixID, deltaTimeID;

	

	//VAO
	GLuint voxelVertexArrayID;
	GLuint originShapeVertexArrayID;
	GLuint catMullVertexArrayID;

	float lastTime;
	float currentTime;
	float deltaTime;

	float lightPos[3] = { 3, 3, 0 };

	Input input;

	std::vector<glm::vec3> computedVertices, computedNormals;
	std::vector<GLuint> computedIndices;


	//Camera management
	glm::vec3 camPosition = glm::vec3(4, 3, 20);
	float horizontalAngle = 3.14f;
	float verticalAngle = 0.0f;
	float initialFoV = glm::radians(45.0f);
	float FoV;
	glm::vec3 direction, up, right;

	float camSpeed = 0.2f;
	float mouseSpeed = 0.001f;
	glm::vec3 cameraPosition;

	//SSAO
	int kernelSize = 10;
	std::vector<glm::vec3> kernel;

public:
	Scene();
	~Scene();

	GLfloat defaultFragmentColor[4] = { 0.6f, 0, 0.4f, 1 };
	GLfloat originShapeFragmentColor[4] = { 1, 1, 1, 1 };

	GLfloat catmullFragmentColor[4] = { 0.2f, 0.4f, 0.8f };

	void Initialize();
	void Render();
	void UpdateBuffers();
	
	glm::vec3 getCameraPosition();
	void TranslateCamera(glm::vec3 v);
	void TranslateCamera(CameraDirection direction);
	int getVertexCount();
	void computeMatrixes(int winWidth, int winHeight, double xPos, double yPos);
	void zoomFoV(float);

	void AutoRotateCamera(float speed, float distance=500);

	void AddLine();

	void AddOriginCornerCutPoints(std::vector<glm::vec3>);

	void AddCatMullShape(int iter);
	void AddLoopShape(int iter);
	void AddKobbeltShape(int iter);
	//Render Passes
	void GeometryPass(); 

	float RandomFloat(float a, float b);
	void resetScene();
	void Destroy();


	void AddPointVertices ( Surface3D surf , glm::vec3 position ); 
	void Scene::AddPointOriginShapeVertices ( Surface3D surf , glm::vec3 position );
};

