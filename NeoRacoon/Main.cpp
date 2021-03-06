// VoxelWorldApp.cpp�: d�finit le point d'entr�e pour l'application console.
//

#include "stdafx.h"
#include "imgui\imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include <cstdio>
#include <gl3w\GL\gl3w.h>
#include <glfw\include\GLFW\glfw3.h>

#include "ShaderManager.h"
#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"

#include "Scene.h"

#include "Edge3D.h"
#include "SimpleCornerCutting.h"

#include "BenTest.h"

GLFWwindow* window;
GLuint vertexBufferPoints , vaoPoints , colorbuffer;

int width , height;
int nbFrames = 0;
double lastTime = 0.0f;
GLfloat fragmentColor [ 4 ] = { 1, 0, 0, 1 };
float newVoxelPosition [ 3 ];
int newChunkSize = 3;

Scene *mainScene;
double mouseX , mouseY;

int iters = 2;

static void error_callback ( int error , const char* description )
{
	fprintf ( stderr , "Error %d: %s\n" , error , description );
}

void callbackMousePos ( GLFWwindow* window , int button , int action , int mods )
{

}

void callbackMouseMove ( GLFWwindow* window , double x , double y )
{
	mouseX = x;
	mouseY = y;
	if ( ImGui::IsMouseDown ( 0 ) )
	{
		mainScene->computeMatrixes ( width , height , mouseX , mouseY );
	}
}

void Render ( )
{

	mainScene->zoomFoV ( ImGui::GetScrollY ( ) );
	if ( ImGui::IsKeyDown ( 262 ) ) //right
		mainScene->TranslateCamera ( glm::vec3 ( 1 , 0 , 0 ) );
	if ( ImGui::IsKeyDown ( 263 ) ) //left
		mainScene->TranslateCamera ( glm::vec3 ( -1 , 0 , 0 ) );
	if ( ImGui::IsKeyDown ( 264 ) ) //down
		mainScene->TranslateCamera ( CameraDirection::backward );
	if ( ImGui::IsKeyDown ( 265 ) ) //up
		mainScene->TranslateCamera ( CameraDirection::forward );

	mainScene->Render ( );
}

void Initialize ( )
{
	//Setup de la fen�tre
	width = 1280;
	height = 720;

	glfwSetErrorCallback ( error_callback );

	//Initialisation de GLFW
	if ( !glfwInit ( ) )
		exit ( EXIT_FAILURE );

	glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR , 3 );
	glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR , 3 );
	glfwWindowHint ( GLFW_OPENGL_PROFILE , GLFW_OPENGL_CORE_PROFILE );

	window = glfwCreateWindow ( width , height , "Voxel World" , NULL , NULL );
	glfwMakeContextCurrent ( window );
	gl3wInit ( );

	// Setup ImGui binding
	ImGui_ImplGlfwGL3_Init ( window , true );
	//glfwSetCursorPosCallback(window, callbackMouseMove);
	mainScene = new Scene ( );

}

float frand_a_b ( float a , float b )
{
	return ( rand ( ) / ( float ) RAND_MAX ) * ( b - a ) + a;
}

int main ( int , char** )
{
	//testLoops ( );

	bool show_test_window = true;
	bool reset = false;
	bool show_another_window = false;
	bool addCornerCutLine = false;
	bool addCatmull = false;
	bool addLoop = false;
	bool addKobbelt = false;
	ImVec4 clear_color = ImColor ( 12 , 14 , 17 );

	Initialize ( );

	//Points
	glm::vec3 A = glm::vec3 ( 3 , -0 , 0 );
	glm::vec3 B = glm::vec3 ( 0.5f , -3 , 0 );
	glm::vec3 C = glm::vec3 ( 0 , 0 , 0 );
	glm::vec3 D = glm::vec3 ( -5 , 4 , 0 );
	glm::vec3 E = glm::vec3 ( 1 , 3 , 0 );
	//Edges
	Edge3D * AB = new Edge3D ( A , B );
	Edge3D * BC = new Edge3D ( B , C );
	Edge3D * CD = new Edge3D ( C , D );
	Edge3D * DE = new Edge3D ( D , E );
	//Edge3D * EA = new Edge3D ( E , A );
	std::vector<Edge3D*> edges;
	edges.push_back ( AB );
	edges.push_back ( BC );
	edges.push_back ( CD );
	edges.push_back ( DE );
	//edges.push_back ( EA );

	//Surface
	Surface3D * s = new Surface3D ( edges , false );

	// Main loop
	while ( !glfwWindowShouldClose ( window ) )
	{
		glfwPollEvents ( );
		ImGui_ImplGlfwGL3_NewFrame ( );

		ImGui::Begin ( "Menu" );
		ImGui::Text ( "This window is here to use the application!" );
		ImGui::Columns ( 2 , "mixed" );
		ImGui::Separator ( );

		ImGui::Text ( "Column" );


		ImGui::Columns ( 1 );
		ImGui::Separator ( );
		ImGui::InputInt ( "Iterations" , &iters );
		ImGui::Separator ( );
		ImGui::Text ( "Add Things" );
		ImGui::DragFloat3 ( "Position" , ( float* ) &newVoxelPosition );
		if ( ImGui::Button ( "Add  Corner Cut Line" ) ) addCornerCutLine ^= 1;
		if ( ImGui::Button ( "Add  Catmull Shape" ) ) addCatmull ^= 1;
		if ( ImGui::Button ( "Add  Loop Shape" ) ) addLoop ^= 1;
		if (ImGui::Button("Add  Kobbelt Shape")) addKobbelt ^= 1;
		ImGui::Separator ( );
		ImGui::ColorEdit3 ( "Default color" , ( float* ) &mainScene->defaultFragmentColor );
		ImGui::ColorEdit3 ( "Simple Line color" , ( float* ) &mainScene->originShapeFragmentColor );
		ImGui::ColorEdit3 ( "Catmull/Loops color" , ( float* ) &mainScene->catmullFragmentColor );
		ImGui::Separator ( );
		if ( ImGui::Button ( "Reset" ) ) reset ^= 1;
		ImGui::ColorEdit3 ( "Clear color" , ( float* ) &clear_color );
		if ( ImGui::Button ( "Test Window" ) ) show_test_window ^= 1;
		ImGui::End ( );

		static bool p_open = true;
		ImGui::SetNextWindowPos ( ImVec2 ( 10 , 10 ) );
		if ( !ImGui::Begin ( "Example: Fixed Overlay" , &p_open , ImVec2 ( 0 , 0 ) , 0.3f , ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings ) )
		{
			ImGui::End ( );
			return 0;
		}
		ImGui::Text ( "FPS Counter" );
		ImGui::Separator ( );
		ImGui::Text ( "%.3f ms/frame (%.1f FPS)" , 1000.0f / ImGui::GetIO ( ).Framerate , ImGui::GetIO ( ).Framerate );
		ImGui::Separator ( );
		ImGui::Text ( "Vertex Count : %d" , mainScene->getVertexCount ( ) );
		ImGui::End ( );

		if ( show_test_window )
		{
			ImGui::SetNextWindowPos ( ImVec2 ( 650 , 20 ) , ImGuiSetCond_FirstUseEver );
			ImGui::ShowTestWindow ( &show_test_window );
		}

		if ( addCornerCutLine )
		{
			Surface3D * res = SimpleCornerCutting::sCutting ( s , iters );

			glm::vec3 pos = glm::vec3 ( newVoxelPosition [ 0 ] , newVoxelPosition [ 1 ] , newVoxelPosition [ 2 ] );
			mainScene->AddPointVertices ( *res , pos );
			mainScene->AddPointOriginShapeVertices ( *s , pos );


			//mainScene->AddOriginCornerCutPoints ( originPoints );
			addCornerCutLine = false;
		}

		if ( addCatmull )
		{
			mainScene->AddCatMullShape ( iters);
			addCatmull = false;
		}

		if ( addLoop )
		{
			mainScene->AddLoopShape ( iters );
			addLoop = false;
		}

		if (addKobbelt)
		{
			mainScene->AddKobbeltShape(iters);
			addKobbelt = false;
		}

		if ( reset )
		{
			mainScene->resetScene ( );
			reset = false;
		}

		// Rendering

		int display_w , display_h;
		glfwGetFramebufferSize ( window , &display_w , &display_h );
		glViewport ( 0 , 0 , display_w , display_h );
		glClearColor ( clear_color.x , clear_color.y , clear_color.z , clear_color.w );
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		Render ( );
		ImGui::Render ( );
		glfwSwapBuffers ( window );
	}

	// Cleanup
	mainScene->Destroy ( );
	ImGui_ImplGlfwGL3_Shutdown ( );
	glfwTerminate ( );
	return 0;
}

