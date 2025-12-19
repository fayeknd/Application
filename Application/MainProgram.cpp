#ifndef MAINWINDOW
#define MAINWINDOW

#include "ProgramBase.h"
#include "Renderer.h"
#include "DefaultShaders.h"
#include "CameraController.h"
#include "Entity.h"
#include "Framebuffer.h"
#include "Portal.h"

using namespace Input;

class MainProgram : public Program {

	Time::Timer timer;

	Entity StuffWithComponentsOn;
	
	Entity cube;
	Entity bgCube;
	Entity bgCube2;
	Entity cube2;
	Entity cube3;

	void PreInit() override { 
		//stuff before window is built go here. things like setting window size, fullscreen mode, desired framerate. 
		KTYGlobal::renderSize = KTYGlobal::windowSize;
		SetVsyncState(true);
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
	}

	void Start() override {

		//glfwSetWindowAttrib(thisWindow, GLFW_DECORATED, false);

		StuffWithComponentsOn = Entity::CreateEntity("Stuff");
		StuffWithComponentsOn.AddComponent<PortalController>();
		StuffWithComponentsOn.AddComponent<CameraController>();
		
		Entity camera = *new Entity;
		camera = Entity::CreateEntity("MainCamera");
		camera.AddComponent<Camera>();
		camera.GetComponent<Camera>().clearColour = Colour(0, 0, 0, 0);
		camera.GetComponent<Camera>().SetMainCamera();
		//camera.transform.SetRotation(Vector3(0, 180, 0));

		cube = Entity::CreateEntity("Floor");
		cube.AddComponent<MeshRenderer>();
		cube.GetComponent<MeshRenderer>().mesh = MeshGenerator::Quad();
		cube.transform.rotation = Vector3(-90, 0, 0);
		cube.transform.position = Vector3(0, -3, 0);
		cube.transform.scale = Vector3(10, 10, 10);

		bgCube = Entity::CreateEntity();
		bgCube.AddComponent<MeshRenderer>();
		bgCube.GetComponent<MeshRenderer>().mesh = MeshGenerator::Quad(1, 1);
		bgCube.GetComponent<MeshRenderer>().autoDraw = false;
		bgCube.GetComponent<MeshRenderer>().material.diffuse = camera.GetComponent<Camera>().framebuffer->colourBuffer;

		cube2 = Entity::CreateEntity();
		cube2.AddComponent<MeshRenderer>();
		cube2.AddComponent<LightRenderer>();
		cube2.GetComponent<MeshRenderer>().material.shader = &DefaultShaders::Unlit;
		cube2.GetComponent<MeshRenderer>().mesh = MeshGenerator::Cube();
		cube2.transform.localPosition = Vector3(3, 0, 0);

		cube3 = Entity::CreateEntity();
		cube3.AddComponent<MeshRenderer>();
		cube3.AddComponent<LightRenderer>();
		cube3.GetComponent<MeshRenderer>().material.shader = &DefaultShaders::Unlit;
		cube3.GetComponent<MeshRenderer>().mesh = MeshGenerator::Cube();
		cube3.transform.scale = Vector3(0.2f);
		cube3.transform.position = Vector3(-1, 1, 0);

		targetFrameRate = Math::Inifinity();

		Texture tex;
		Texture tex2;
		tex.CreateTexture(Vector2(10, 10), GL_RGBA, true, 4);
		tex2.CreateTexture(Vector2(10, 10), GL_RGBA, true, 4);
		std::vector<Vector2> targetPixels = {
			Vector2(2, 2),
			Vector2(2, 5),
			Vector2(7, 2),
			Vector2(7, 5),
			Vector2(6, 6),
			Vector2(5, 6),
			Vector2(4, 6),
			Vector2(3, 6)
		};
		tex.SetPixels(targetPixels, Colour(0, 1, 1, 1));
		tex.RebindData();
		tex2.SetPixels(targetPixels, Colour(0, 0, 0, 1));
		tex2.RebindData();
		cube.GetComponent<MeshRenderer>().material.diffuse = tex;
		cube.GetComponent<MeshRenderer>().material.specular = tex2;

		Mouse::sensitivity = 0.065;
		cube2.SetParent(&cube3);
	}
	float xVal = 0, yVal = 0;
	int x, y;
	bool flag = false;

	float theta;

	void Update() override {

		Debug::CLog(FrameRate());

		cube3.transform.rotation.y += Time::deltaTime() * 30;
		cube3.transform.rotation.z += Time::deltaTime() * 30;
		cube2.transform.localPosition.z = (Math::sin(Time::SinceStartup()) * 2) - 1;

		cube2.GetComponent<LightRenderer>().light.diffuse = Colour(Math::abs(glm::sin(Time::SinceStartup())), 0, 1, 1);
		cube2.GetComponent<MeshRenderer>().material.diffuseColour = cube2.GetComponent<LightRenderer>().light.diffuse; 
	}
	void PostRender() override {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, (int)KTYGlobal::windowSize.x, (int)KTYGlobal::windowSize.y);
		bgCube.GetComponent<MeshRenderer>().Draw(&DefaultShaders::Screenspace);

		//bgCube.GetComponent<MeshRenderer>().SendMaterialTexture(&DefaultShaders::ScreenspaceDepth, &fb.depthBuffer, "depth");
		//bgCube.GetComponent<MeshRenderer>().Draw(&DefaultShaders::ScreenspaceDepth);
	}
};

#endif