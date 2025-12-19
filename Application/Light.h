#pragma once
#include "main.h"
#include "Entity.h"
#include "Shader.h"
#include "DefaultShaders.h"

enum LightType {
	Pointlight,
	Spotlight,
	Directional
};

class Light{
public:

	LightType lightType = LightType::Pointlight;

	int lightID;

	static void ResetIDs() {
		nextAvailablePointID = 0;
		nextAvailableSpotID = 0;
		nextAvailableDirID = 0;
	}

	static int nextAvailablePointID;
	static int nextAvailableSpotID;
	static int nextAvailableDirID;
	Shader* shader = &DefaultShaders::Lit;
	Transform uniqueTransform;

	float constant;
	float linear; 
	float quadratic;

	Colour ambient  = Colour(0.2f);
	Colour diffuse  = Colour(1);
	Colour specular = Colour(1);
};

class LightRenderer : public EntityComponent {
public:
	Light light;

	void Update() override {
		Draw(light.shader);
	}

	void SendPointLightData(Shader* s) {
		std::string index = "pointLights[" + (std::to_string(light.lightID)) + "]";

		s->SetVar<Vector3>(light.uniqueTransform.GlobalPosition() + entity->transform.GlobalPosition(), (index + (std::string)".position").c_str());

		s->SetVar<float>(light.constant,   (index + (std::string)".constant").c_str());
		s->SetVar<float>(light.linear,     (index + (std::string)".linear").c_str());
		s->SetVar<float>(light.quadratic,  (index + (std::string)".quadratic").c_str());

		s->SetVar<Vector4>(light.ambient,  (index + (std::string)".ambient").c_str());
		s->SetVar<Vector4>(light.diffuse,  (index + (std::string)".diffuse").c_str());
		s->SetVar<Vector4>(light.specular, (index + (std::string)".specular").c_str());
	}

	void RebindID() {
		switch (light.lightType) {
		case LightType::Pointlight:
			light.lightID = Light::nextAvailablePointID++;
			break;
		case LightType::Spotlight:
			light.lightID = Light::nextAvailableSpotID++;
			break;
		case LightType::Directional:
			light.lightID = Light::nextAvailableDirID++;
			break;
		}
	}

	void Draw(Shader* s) {

		RebindID();
		if (light.lightID >= KTYGlobal::MaxLights)
			return;

		switch (light.lightType) {
		case LightType::Pointlight:
			SendPointLightData(s);
			break; 
		case LightType::Spotlight:
			break; //Not implemented
		case LightType::Directional:
			break; //Not implemented
		}
		s->SetVar<int>(Light::nextAvailablePointID, "l_assignedPointLights");
		s->SetVar<int>(Light::nextAvailableSpotID,  "l_assignedSpotLights");
		s->SetVar<int>(Light::nextAvailableDirID,   "l_assignedDirectionalLights");

	}

};