#pragma once

#include "Entity.h"

struct Transform{
private:
	
	glm::mat4 transformMatrix = glm::mat4(1);
	glm::mat4 rotationMatrix  = glm::mat4(1);
	glm::mat4 scaleMatrix     = glm::mat4(1);
	glm::mat4 _calcMatrix     = glm::mat4(1);

	/// <summary>
	/// Calculates matrix given various factors. Use calculatedMatrix() to return the transform matrix in it's current state.
	/// </summary>
	/// <param name="parentMatrix"></param>
	/// <param name="pos"></param>
	/// <param name="scale"></param>
	/// <param name="rot"></param>
	/// <returns></returns>
	glm::mat4 localOrWorldMatrix(Vector3 pos, Vector3 scale, Vector3 rot, glm::mat4 parentT = glm::mat4(1), glm::mat4 parentS = glm::mat4(1), glm::mat4 parentR = glm::mat4(1)) {

		transformMatrix = glm::translate(glm::mat4(1), pos.glm());
		
		rotationMatrix = glm::rotate(glm::mat4(1), (float)Math::deg2rad(rot.y), Vector3::YAxis().glm());
		rotationMatrix = glm::rotate(rotationMatrix, (float)Math::deg2rad(rot.x), Vector3::XAxis().glm());
		rotationMatrix = glm::rotate(rotationMatrix, (float)Math::deg2rad(rot.z), Vector3::ZAxis().glm());

		scaleMatrix = glm::scale(glm::mat4(1), scale.glm());

		_calcMatrix = (parentT * parentR * parentS) * (transformMatrix) * (rotationMatrix) * (scaleMatrix);

		globalPosition = Vector3(_calcMatrix[3][0], _calcMatrix[3][1], _calcMatrix[3][2]);

		glm::extractEulerAngleYXZ(_calcMatrix, y, p, r);
		globalRotation = Math::rad2deg(Vector3(p, y, r));

		return _calcMatrix;
	};

	float y, p, r = 0;

	Vector3 globalPosition = Vector3::zero();
	Vector3 globalRotation = Vector3::zero();

public:

	glm::mat4 calculatedMatrix() {
		return _calcMatrix;
	}

	glm::mat4 calculatedTransformMatrix() {
		return transformMatrix;
	}

	glm::mat4 calculatedScaleMatrix() {
		return scaleMatrix;
	}

	glm::mat4 calculatedRotationMatrix() {
		return rotationMatrix;
	}

	Vector3 GlobalPosition() { return globalPosition; }
	Vector3 GlobalRotation() { return globalRotation; }

	/// <summary>
    /// The position of the transform. It is suggested to use localPosition.
    /// </summary>
	Vector3 position = Vector3::zero();
	/// <summary>
	/// The rotation of the transform (in radians). It is suggested to use localRotation.
	/// </summary>
	Vector3 rotation;
	/// <summary>
	/// The scale of the transform.  It is suggested to use localScale.
	/// </summary>
	Vector3 scale = Vector3::one();

	/// <summary>
	/// The local position of the transform.
	/// </summary>
	Vector3 localPosition = Vector3::zero();
	/// <summary>
	/// The local rotation of the transform (in radians).
	/// </summary>
	Vector3 localRotation;
	/// <summary>
	/// The local scale of the transform.
	/// </summary>
	Vector3 localScale = Vector3::one();
	
	glm::mat4 WorldMatrix(Transform* parent = nullptr) {
		if (parent != nullptr) {
			return localOrWorldMatrix(localPosition, localScale, localRotation, parent->calculatedTransformMatrix(), parent->calculatedScaleMatrix(), parent->calculatedRotationMatrix());
		}
		return localOrWorldMatrix(position + localPosition, scale * localScale, rotation + localRotation);
	}
	Vector3 forward() {
		Vector3 euler = Math::deg2rad(globalRotation);
		return Vector3(
			glm::cos (euler.x) * glm::sin(euler.y),
			-glm::sin(euler.x),
			glm::cos (euler.x) * glm::cos(euler.y)
		);
	}

	Vector3 left() {
		Vector3 euler = Math::deg2rad(globalRotation);
		return Vector3(
			 glm::cos(euler.y),
			0,
			-glm::sin(euler.y)
		);
	}

	Vector3 up() {
		Vector3 euler = Math::deg2rad(globalRotation);
		return Vector3(
			glm::sin(euler.x) * glm::sin(euler.y),
			glm::cos(euler.x),
			glm::sin(euler.x) * glm::cos(euler.y)
		);
	}

	Vector3 back() { return -forward(); }
	Vector3 right() { return -left(); }
	Vector3 down() { return -up(); }

	Transform() {};
	Transform(
		Vector3 pos,
		Vector3 rot,
		Vector3 sc,

		Vector3 lPos = Vector3(0),
		Vector3 lRot = Vector3(0),
		Vector3 lSc  = Vector3(0)
	)
	{
		position	= pos;
		rotation    = rot;
		scale		= sc;

		localPosition    = lPos;
		localRotation    = lRot;
		localScale       = lSc;
	}
	Transform& operator+=(const Transform& t) {
		position         + t.position;
		rotation         + t.rotation;
		scale			 + t.scale;

		localPosition    + t.localPosition;
		localRotation    + t.localRotation;
		localScale       + t.localScale;
		return *this;
	}
	Transform& operator-=(const Transform& t) {
		position         - t.position;
		rotation         - t.rotation;
		scale			 - t.scale;

		localPosition    - t.localPosition;
		localRotation    - t.localRotation;
		localScale       - t.localScale;
		return *this;
	}
};
inline Transform operator+(const Transform& t1, const Transform& t2) {
	return Transform(
		t1.position         + t2.position,
		t1.rotation         + t2.rotation,
		t1.scale			+ t2.scale,

		t1.localPosition    + t2.localPosition,
		t1.localRotation    + t2.localRotation,
		t1.localScale       + t2.localScale
	);
}
inline Transform operator-(const Transform& t1, const Transform& t2) {
	return Transform{
		t1.position         - t2.position,
		t1.rotation         - t2.rotation,
		t1.scale			- t2.scale,

		t1.localPosition    - t2.localPosition,
		t1.localRotation    - t2.localRotation,
		t1.localScale       - t2.localScale
	};
}
