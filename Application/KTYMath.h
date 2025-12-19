#ifndef MATH_H 
#define MATH_H
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include <limits>
#include <cmath>

class Math {
public:

	/// <summary>
	/// Returns pi to 15 digits. (3.14...)
	/// </summary>
	inline static const double pi = 3.141592653589793;
	/// <summary>
	/// Returns tau to 15 digits. (6.28...)
	/// </summary>
	inline static const double tau = 6.283185307179586;
	/// <summary>
	/// Returns the golden ratio. (?5 - 1 / 2)
	/// </summary>
	inline static const double goldenRatio = (glm::sqrt(5) - 1) / 2;

	/// <summary>
	/// Effectively returns infinity.
	/// </summary>
	/// <returns></returns>
	static double Inifinity() { return std::numeric_limits<double>::infinity(); }

	/// <summary>
	/// Wrapper for generic cos function. (std::cos)
	/// </summary>
	/// <param name="theta"></param>
	/// <returns></returns>
	static double cos(double theta) {
		return std::cos(theta);
	}
	/// <summary>
	/// Wrapper for generic sin function. (std::sin)
	/// </summary>
	/// <param name="theta"></param>
	/// <returns></returns>
	static double sin(double theta) {
		return std::sin(theta);
	}
	/// <summary>
	/// Wrapper for generic acos function. (std::acos)
	/// </summary>
	/// <param name="theta"></param>
	/// <returns></returns>
	static double arccos(double theta) {
		return std::acos(theta);
	}
	/// <summary>
	/// Wrapper for generic asin function. (std::asin)
	/// </summary>
	/// <param name="theta"></param>
	/// <returns></returns>
	static double arcsin(double theta) {
		return std::asin(theta);
	}
	/// <summary>
	/// Clamps value between minimum value and maximum value. Ex. Math::clamp((v)7, (min)10, (max)100) returns 10.
	/// </summary>
	/// <param name="value"></param>
	/// <param name="min"></param>
	/// <param name="max"></param>
	/// <returns></returns>
	template <typename T>
	static T clamp(T v, T min, T max) {
		v = (v > min) ? v : min;
		v = (v < max) ? v : max;
		return v;
	}
	/// <summary>
	/// Returns absolute of value. Ex. abs(-1) = 1.
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	static double abs(double x) {
		if (x < 0)
			return -1 * x;
		return x;
	}
	/// <summary>
	/// Returns absolute of value. Ex. abs(-1) = 1.
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	static Vector2 abs(Vector2 x) {
		return Vector2(abs(x.x), abs(x.y));
	}
	/// <summary>
	/// Returns absolute of value. Ex. abs(-1) = 1.
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	static Vector3 abs(Vector3 x) {
		return Vector3(abs(x.x), abs(x.y), abs(x.z));
	}
	/// <summary>
	/// Returns absolute of value. Ex. abs(-1) = 1.
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	static Vector4 abs(Vector4 x) {
		return Vector4(abs(x.x), abs(x.y), abs(x.z), abs(x.w));
	}

	/// <summary>
	/// Returns the value of "degrees" converted to radians.
	/// </summary>
	/// <param name="degrees"></param>
	/// <returns></returns>
	static double deg2rad(double degrees) {
		return degrees * (pi / (double)180);
	}
	/// <summary>
	/// Returns the value of "degrees" converted to radians.
	/// </summary>
	/// <param name="degrees"></param>
	/// <returns></returns>
	static Vector2 deg2rad(Vector2 degrees) {
		return Vector2(
			degrees.x * (pi / (double)180),
			degrees.y * (pi / (double)180)
		);
	}
	/// <summary>
	/// Returns the value of "degrees" converted to radians.
	/// </summary>
	/// <param name="degrees"></param>
	/// <returns></returns>
	static Vector3 deg2rad(Vector3 degrees) {
		return Vector3(
			degrees.x * (pi / (double)180),
			degrees.y * (pi / (double)180),
			degrees.z * (pi / (double)180)
		);
	}
	/// <summary>
	/// Returns the value of "degrees" converted to radians.
	/// </summary>
	/// <param name="degrees"></param>
	/// <returns></returns>
	static Vector4 deg2rad(Vector4 degrees) {
		return Vector4(
			degrees.x * (pi / (double)180),
			degrees.y * (pi / (double)180),
			degrees.z * (pi / (double)180),
			degrees.w * (pi / (double)180)
		);
	}

	/// <summary>
	/// Returns the value of "radians" converted to degrees.
	/// </summary>
	/// <param name="radians"></param>
	/// <returns></returns>
	static double rad2deg(double radians) {
		return radians * ((double)180 / pi);
	}
	/// <summary>
	/// Returns the value of "radians" converted to degrees.
	/// </summary>
	/// <param name="radians"></param>
	/// <returns></returns>
	static Vector2 rad2deg(Vector2 radians) {
		return Vector2(
			radians.x * ((double)180 / pi),
			radians.y * ((double)180 / pi)
		);
	}
	/// <summary>
	/// Returns the value of "radians" converted to degrees.
	/// </summary>
	/// <param name="radians"></param>
	/// <returns></returns>
	static Vector3 rad2deg(Vector3 radians) {
		return Vector3(
			radians.x * ((double)180 / pi),
			radians.y * ((double)180 / pi),
			radians.z * ((double)180 / pi)
		);
	}
	/// <summary>
	/// Returns the value of "radians" converted to degrees.
	/// </summary>
	/// <param name="radians"></param>
	/// <returns></returns>
	static Vector4 rad2deg(Vector4 radians) {
		return Vector4(
			radians.x * ((double)180 / pi),
			radians.y * ((double)180 / pi),
			radians.z * ((double)180 / pi),
			radians.w * ((double)180 / pi)
		);
	}

	/// <summary>
	/// Returns either true or false depending on if "val" is in between both "low" and "high".
	/// </summary>
	/// <param name="val"></param>
	/// <param name="low"></param>
	/// <param name="high"></param>
	/// <returns></returns>
	static bool inrange(double val, double low, double high) {
		return (val >= low && val <= high);
	}

};

#endif