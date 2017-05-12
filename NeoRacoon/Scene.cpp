#include "Scene.h"

#include "MeshUtils.h"
#include "CatMull.h"

#define POSITION_TEXTURE_UNIT           GL_TEXTURE1
#define POSITION_TEXTURE_UNIT_INDEX     1

Scene::Scene()
{
	model = glm::mat4(1.0);
	proj = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 1000.0f);
	view = glm::lookAt(
		glm::vec3(4, 3, 10), // Camera is at this position, in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	mvp = proj * view * model;

	input = Input();
	//g_vertex_buffer_data = std::vector<glm::vec3>();
	positions = std::vector<glm::vec3>();
	//indices = std::vector<GLuint>();
	normals = std::vector<glm::vec3>();
	vertices = std::vector<glm::vec3>();


	Initialize();
}


Scene::~Scene()
{
}

void Scene::Initialize()
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	program = LoadShaders("..\\shaders\\basic.vs", "..\\shaders\\basic.fs");
	position_location = glGetAttribLocation(program, "vertexPosition_modelspace");
	mvp_location = glGetUniformLocation(program, "MVP");
	color_location = glGetUniformLocation(program, "vertexColor");

	glGenVertexArrays(1, &voxelVertexArrayID);
	glBindVertexArray(voxelVertexArrayID);
	glGenBuffers(1, &vertexBufferPoints);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPoints);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(position_location);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &originShapeVertexArrayID);
	glBindVertexArray(originShapeVertexArrayID);
	glGenBuffers(1, &originShapeVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, originShapeVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * originShapeVertices.size(), originShapeVertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(position_location);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindVertexArray(0);
	

	glGenVertexArrays(1, &catMullVertexArrayID);
	glBindVertexArray(catMullVertexArrayID);
	glGenBuffers(1, &catmullVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, catmullVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * catmullVertices.size(), catmullVertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(position_location);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindVertexArray(0);

	lastTime = glfwGetTime();

}

void Scene::Render()
{
	//AutoRotateCamera(1);

	currentTime = glfwGetTime();
	deltaTime = float(currentTime - lastTime);

	GeometryPass(); //Used for renderning geometry

	lastTime = glfwGetTime();

}

void Scene::UpdateBuffers()
{

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPoints);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, originShapeVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, originShapeVertices.size() * sizeof(glm::vec3), originShapeVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, catmullVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, catmullVertices.size() * sizeof(glm::vec3), catmullVertices.data(), GL_STATIC_DRAW);

}


void Scene::TranslateCamera(glm::vec3 v)
{
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	camPosition += v * deltaTime * camSpeed;

	view = glm::lookAt(
		camPosition,
		direction,
		glm::vec3(0, 1, 0)
	);
	mvp = proj * view * model;
}

void Scene::TranslateCamera(CameraDirection dir)
{
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	if (dir == CameraDirection::forward)
	{
		camPosition += glm::normalize(direction - camPosition) * deltaTime * camSpeed;
	}
	else if (dir == CameraDirection::backward)
	{
		camPosition -= glm::normalize(direction - camPosition) * deltaTime * camSpeed;
	}
	else if (dir == CameraDirection::left)
	{

	}
	else if (dir == CameraDirection::right)
	{

	}

	view = glm::lookAt(
		camPosition,
		direction,
		glm::vec3(0, 1, 0)
	);
	mvp = proj * view * model;
}

int Scene::getVertexCount()
{
	return vertices.size();
}

void Scene::computeMatrixes(int winWidth, int winHeight, double xPos, double yPos)
{
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	horizontalAngle += mouseSpeed * deltaTime * float(winWidth / 2 - xPos);
	verticalAngle += mouseSpeed * deltaTime * float(winHeight / 2 - yPos);

	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	up = glm::cross(right, direction);

	proj = glm::perspective(initialFoV, 16.0f / 9.0f, 0.1f, 3000.0f);
	view = glm::lookAt(
		camPosition,
		direction,
		up
	);
	mvp = proj * view * model;
}

void Scene::zoomFoV(float val)
{
	FoV = glm::radians(initialFoV - glm::radians(5 * val));
	proj = glm::perspective(FoV, 16.0f / 9.0f, 0.1f, 3000.0f);
}

void Scene::AutoRotateCamera(float speed, float distance)
{
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	camPosition.x = distance * cos(currentTime * speed);
	camPosition.z = distance * sin(currentTime * speed);

	view = glm::lookAt(
		camPosition,
		direction,
		glm::vec3(0, 1, 0)
	);
	mvp = proj * view * model;
}

void Scene::AddLine()
{
}

void Scene::AddOriginCornerCutPoints(std::vector<glm::vec3> v)
{
	originShapeVertices.insert(originShapeVertices.end(), v.begin(), v.end());
	UpdateBuffers();
}

void Scene::AddCatMullShape()
{
	RenderableMesh mesh = testCatMull();
	catmullVertices = mesh.toVec3();

	UpdateBuffers();
}

void Scene::AddLoopShape()
{
	RenderableMesh mesh = testLoops();
	catmullVertices = mesh.toVec3();

	UpdateBuffers();
}

void Scene::AddKobbeltShape()
{
	RenderableMesh mesh = testLoops();
	catmullVertices = mesh.toVec3();

	UpdateBuffers();
}



void Scene::GeometryPass()
{
	glUseProgram(program);
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &mvp[0][0]);

	glProgramUniform4fv(program, color_location, 1, defaultFragmentColor);
	glBindVertexArray(voxelVertexArrayID);
	glPointSize(5);
	glDrawArrays(GL_POINTS, 0, vertices.size());
	glDrawArrays(GL_LINE_LOOP, 0, vertices.size());

	glProgramUniform4fv(program, color_location, 1, originShapeFragmentColor);
	glBindVertexArray(originShapeVertexArrayID);
	glPointSize(5);
	glDrawArrays(GL_POINTS, 0, originShapeVertices.size());
	glDrawArrays(GL_LINE_LOOP, 0, originShapeVertices.size());


	glProgramUniform4fv(program, color_location, 1, catmullFragmentColor);
	glBindVertexArray(catMullVertexArrayID);
	glPointSize(3);
	glDrawArrays(GL_POINTS, 0, catmullVertices.size());
	glDrawArrays(GL_LINES, 0, catmullVertices.size());

	glBindVertexArray(0);
}

float Scene::RandomFloat(float a, float b)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

void Scene::resetScene()
{
	normals.clear();
	positions.clear();
	vertices.clear();
	catmullVertices.clear();
	originShapeVertices.clear();

	UpdateBuffers();
}

void Scene::Destroy()
{
	glDeleteBuffers(1, &vertexBufferPoints);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &VertexArrayID);
}

void Scene::AddPointVertices(Surface3D surf, glm::vec3 position)
{
	for (int i = 0;i < surf.get_Edges().size();++i)
	{
		vertices.push_back(surf.get_Edges()[i]->get_A());
	}
	if (surf.get_Close())
	{
		vertices.push_back(surf.get_Edges()[surf.get_Edges().size() - 1]->get_B());
	}

	UpdateBuffers();
}
