#pragma once

#include "Entity.h"
#include "main.h"
#include "global.h"
#include "Framebuffer.h"

#define KTY_PROJECTION
#define KTY_ORTHOGRAPHIC

enum CameraPerspective {
	Projection,
	Orthographic
};
enum CameraViewportMode {
	FixedToWindow,
	Independent
};
enum DepthTestMode {
	Less = GL_LESS,
	Always = GL_ALWAYS,
	Never = GL_NEVER,
	Equal = GL_EQUAL,
	LessOrEqual = GL_LEQUAL,
	GreaterOrEqual = GL_GEQUAL,
	Greater = GL_GREATER,
	NotEqual = GL_NOTEQUAL
};

class Camera : public EntityComponent {
private:
	void CreatePerspectiveMatrix() {
		switch (perspective) {
		case CameraPerspective::Orthographic:
			perspectiveMatrix = glm::ortho(0.0f, (float)(KTYGlobal::windowSize.x / KTYGlobal::windowSize.y), mainCamera->nearClipPlane, mainCamera->farClipPlane);
			break;
		case CameraPerspective::Projection:
			perspectiveMatrix = glm::perspective(glm::radians(mainCamera->fieldOfView), (float)(KTYGlobal::windowSize.x / KTYGlobal::windowSize.y), mainCamera->nearClipPlane, mainCamera->farClipPlane);
			break;
		}
	}
	void CreateViewMatrix() {
		viewMatrix = glm::lookAt(
			entity->transform.GlobalPosition().glm(),
			(entity->transform.GlobalPosition() + entity->transform.forward()).glm(),
			entity->transform.up().glm()
		);
	}

	static DebugLogger CamLog;
public:

	Colour clearColour = Colour(0, 0, 0, 1);

	Framebuffer* framebuffer = Framebuffer::Default;

	bool DontDrawParent = false;

	unsigned int DepthMode = DepthTestMode::Less;
	std::vector<unsigned int> RenderFunctions{
		GL_DEPTH_TEST,
		GL_MULTISAMPLE,
		GL_CULL_FACE
	};
	unsigned int ClearBuffers = (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glm::mat4 viewMatrix = glm::mat4(1);
	glm::mat4 perspectiveMatrix = glm::mat4(1);

	float nearClipPlane = 0.01f;
	float farClipPlane = 1000.0f;

	Vector2 renderResolution;
	Viewport viewPort = (0, 0, KTYGlobal::windowSize.x, KTYGlobal::windowSize.y);

	static Camera* mainCamera;
	static Camera* activeCamera;
	bool enabled = true;

	CameraPerspective perspective = CameraPerspective::Projection;
	CameraViewportMode viewportMode = CameraViewportMode::FixedToWindow;

	//Camera is rotated by vec2 angle. autoClamp, if left true, will automatically clamp the pitch between -89 and 89, giving a first-person camera feel. Note: Camera localRotation is dependent exclusively on the localRotation of the object it is assigned to.
	void Look(Vector2 angle, bool autoClamp = true, double min_pitchclamp = -89, double max_pitchclamp = 89) {
		if (KTYGlobal::WindowFocused()) {
			entity->transform.localRotation.x += angle.y;
			entity->transform.localRotation.y += -angle.x;
			//entity->transform.RotateAngleAxis(Math::deg2rad(angle.x), entity->transform.up());
		}
		if (autoClamp) {
			entity->transform.localRotation.x = Math::clamp(entity->transform.localRotation.x, min_pitchclamp, max_pitchclamp);
		}
	}

	Camera* SetMainCamera() {
		glDepthFunc(DepthMode);
		(mainCamera != this) ? mainCamera = this : NULL;
		return this;
	}
	static Camera* SetMainCamera(Camera* camera) {
		(mainCamera != camera) ? mainCamera = camera : NULL;
		return camera;
	}

	void Start() override {

		CamLog.LoggerName = "CAMERA";
		if (renderResolution.x == 0)
			renderResolution = KTYGlobal::windowSize;
		if (mainCamera == nullptr) {
			SetMainCamera();
			CamLog.CLog("Camera::mainCamera ptr was null, Camera on entity " + entity->name + " assumed main.");
		}
		CamLog.CLog("Camera on entity " + entity->name + " set to render @ " + std::to_string((int)renderResolution.x) + "x" + std::to_string((int)renderResolution.y));
	}

	void CameraDraw(bool force = false) {
		if (mainCamera == this && !force)
			return;
		framebuffer->Use();
		activeCamera = this;

		if (viewportMode == CameraViewportMode::FixedToWindow) {
			viewPort = Viewport(0, 0, (int)KTYGlobal::renderSize.x, (int)KTYGlobal::renderSize.y);
		}
		for (int i = 0; i < RenderFunctions.size(); i++) {
			glEnable(RenderFunctions[i]);
		}
		glClearColorV4(clearColour);

		glClear(ClearBuffers);
		glViewport(viewPort.x, viewPort.y, viewPort.z, viewPort.w);

		CreatePerspectiveMatrix();
		CreateViewMatrix();

		if (entity->Parent() != nullptr)
			entity->transform.WorldMatrix(&entity->Parent()->transform);
		else
			entity->transform.WorldMatrix();
		Object::RenderFunc(); //Draw objects

	}

	void Update() override {
		CameraDraw();
	}
	float fieldOfView = (float)75;
	// TODO : Create a rendertexture 
};