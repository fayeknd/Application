#pragma once

#include "main.h"
#include "GLMmath.h"
#include "Logger.h"

class Matrix4x4 {
public:
	double data[4][4];
	
	Matrix4x4() : data { 1, 0, 0, 0, 
						 0, 1, 0, 0,
						 0, 0, 1, 0,
						 0, 0, 0, 1 
	} { }
	Matrix4x4(double x[4][4]) : data {
		x[0][0], x[0][1], x[0][2], x[0][3],
		x[1][0], x[1][1], x[1][2], x[1][3],
		x[2][0], x[2][1], x[2][2], x[2][3],
		x[3][0], x[3][1], x[3][2], x[3][3]
	} {
	}
	Matrix4x4(
		double m1,  double m2,  double m3,  double m4,
		double m5,  double m6,  double m7,  double m8, 
		double m9,  double m10, double m11, double m12,
		double m13, double m14, double m15, double m16
	) : data{
		m1,         m2,         m3,         m4,
		m5,         m6,         m7,         m8,
		m9,         m10,        m11,        m12, 
		m13,        m14,        m15,        m16
	} { }

	Matrix4x4(double e) : data { 
						 e, 0, 0, 0,
						 0, e, 0, 0,
						 0, 0, e, 0,
						 0, 0, 0, e } { }

	Matrix4x4(glm::mat4 mat) : data{
		mat[0][0], mat[0][1], mat[0][2], mat[0][3],
		mat[1][0], mat[1][1], mat[1][2], mat[1][3],
		mat[2][0], mat[2][1], mat[2][2], mat[2][3],
		mat[3][0], mat[3][1], mat[3][2], mat[3][3]
	} { }

	glm::mat4 glm() {
		return glm::mat4(
			data[0][0], data[0][1], data[0][2], data[0][3],
			data[1][0], data[1][1], data[1][2], data[1][3],
			data[2][0], data[2][1], data[2][2], data[2][3],
			data[3][0], data[3][1], data[3][2], data[3][3]
		);
	}
#define _mult(mat) Multiply(*this, mat)
	/// <summary>
	/// Translates a matrix by a Vector3. 
	/// Usage: "mat.Translate(v)"
	/// </summary>
	/// <param name="vec"></param>
	/// <returns></returns>
	Matrix4x4 Translate(Vector3 vec) {
		Matrix4x4 translateMatrix(
			(double)1, (double)0, (double)0, (double)0,
			(double)0, (double)1, (double)0, (double)0,
			(double)0, (double)0, (double)1, (double)0,
			vec.x,     vec.y,     vec.z,     (double)1
		);
		*this = Multiply(*this, translateMatrix);
		return *this;
	}
	/// <summary>
	/// Rotates matrix given theta (radians) and Vector3 axis.
	/// Usage: "mat.Rotate(Math::rad2deg(90), Vector3(0, 1, 0));" (A 90 degree rotation in the Y axis)
	/// </summary>
	/// <param name="theta"></param>
	/// <param name="axis"></param>
	/// <returns></returns>
	Matrix4x4 Rotate(double theta, Vector3 axis) {
		Matrix4x4 constructed;
		/* I have no idea why this is broken
		Matrix4x4 rotate;

		double c = glm::cos(theta);
		double s = glm::sin(theta);
		Vector3 t(1 - c * axis);


		rotate.data[0][0] = c + t.x * axis.x;
		rotate.data[0][1] =		t.x * axis.y + s * axis.z;
		rotate.data[0][2] =		t.x * axis.z - s * axis.y;

		rotate.data[1][0] =		t.y * axis.x - s * axis.z;
		rotate.data[1][1] = c + t.y * axis.y;
		rotate.data[1][2] =		t.y * axis.z + s * axis.x;

		rotate.data[2][0] =		t.z * axis.x + s * axis.y;
		rotate.data[2][1] =		t.z * axis.y - s * axis.x;
		rotate.data[2][2] = c + t.z * axis.z;

		constructed.data[0][0] = data[0][0] * rotate.data[0][0] + data[1][0] * rotate.data[0][1] + data[2][0] * rotate.data[0][2];
		constructed.data[0][1] = data[0][1] * rotate.data[0][0] + data[1][1] * rotate.data[0][1] + data[2][1] * rotate.data[0][2];
		constructed.data[0][2] = data[0][2] * rotate.data[0][0] + data[1][2] * rotate.data[0][1] + data[2][2] * rotate.data[0][2];
		constructed.data[0][3] = data[0][3] * rotate.data[0][0] + data[1][3] * rotate.data[0][1] + data[2][3] * rotate.data[0][2];

		constructed.data[1][0] = data[0][0] * rotate.data[1][0] + data[1][0] * rotate.data[1][1] + data[2][0] * rotate.data[1][2];
		constructed.data[1][1] = data[0][1] * rotate.data[1][0] + data[1][1] * rotate.data[1][1] + data[2][1] * rotate.data[1][2];
		constructed.data[1][2] = data[0][2] * rotate.data[1][0] + data[1][2] * rotate.data[1][1] + data[2][2] * rotate.data[1][2];
		constructed.data[1][3] = data[0][3] * rotate.data[1][0] + data[1][3] * rotate.data[1][1] + data[2][3] * rotate.data[1][2];

		constructed.data[2][0] = data[0][0] * rotate.data[2][0] + data[1][0] * rotate.data[2][1] + data[2][0] * rotate.data[2][2];
		constructed.data[2][1] = data[0][1] * rotate.data[2][0] + data[1][1] * rotate.data[2][1] + data[2][1] * rotate.data[2][2];
		constructed.data[2][2] = data[0][2] * rotate.data[2][0] + data[1][2] * rotate.data[2][1] + data[2][2] * rotate.data[2][2];
		constructed.data[2][3] = data[0][3] * rotate.data[2][0] + data[1][3] * rotate.data[2][1] + data[2][3] * rotate.data[2][2];

		constructed.data[3][0] = data[3][0];
		constructed.data[3][1] = data[3][1];
		constructed.data[3][2] = data[3][2];
		constructed.data[3][3] = data[3][3];*/

		
		//matrix *= constructed;
		*this = glm::rotate(glm(), (float)theta, axis.glm());
		return *this;
	}
	/// <summary>
	/// Scales a matrix by a Vector3. 
	/// Usage: "mat.Scale(Vector3(2, 2, 2));"
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	Matrix4x4 Scale(Vector3 scale) {
		double x = scale.x;
		double y = scale.y;
		double z = scale.z;

		Matrix4x4 scaleMatrix;

		scaleMatrix.data[0][0] = data[0][0] * x;
		scaleMatrix.data[0][1] = data[0][1] * x;
		scaleMatrix.data[0][2] = data[0][2] * x;
		scaleMatrix.data[0][3] = data[0][3] * x;

		scaleMatrix.data[1][0] = data[1][0] * y;
		scaleMatrix.data[1][1] = data[1][1] * y;
		scaleMatrix.data[1][2] = data[1][2] * y;
		scaleMatrix.data[1][3] = data[1][3] * y;
		
		scaleMatrix.data[2][0] = data[2][0] * z;
		scaleMatrix.data[2][1] = data[2][1] * z;
		scaleMatrix.data[2][2] = data[2][2] * z;
		scaleMatrix.data[2][3] = data[2][3] * z;

		scaleMatrix.data[3][0] = data[3][0];
		scaleMatrix.data[3][1] = data[3][1];
		scaleMatrix.data[3][2] = data[3][2];
		scaleMatrix.data[3][3] = data[3][3];

		*this = scaleMatrix;
		return *this;

	}

	static Matrix4x4 Multiply(Matrix4x4 mat4_1, Matrix4x4 mat4_2) {
		double dest[4][4];

		dest[0][0] = mat4_1.data[0][0] * mat4_2.data[0][0] + mat4_1.data[0][1] * mat4_2.data[1][0] + mat4_1.data[0][2] * mat4_2.data[2][0] + mat4_1.data[0][3] * mat4_2.data[3][0];
		dest[0][1] = mat4_1.data[0][0] * mat4_2.data[0][1] + mat4_1.data[0][1] * mat4_2.data[1][1] + mat4_1.data[0][2] * mat4_2.data[2][1] + mat4_1.data[0][3] * mat4_2.data[3][1];
		dest[0][2] = mat4_1.data[0][0] * mat4_2.data[0][2] + mat4_1.data[0][1] * mat4_2.data[1][2] + mat4_1.data[0][2] * mat4_2.data[2][2] + mat4_1.data[0][3] * mat4_2.data[3][2];
		dest[0][3] = mat4_1.data[0][0] * mat4_2.data[0][3] + mat4_1.data[0][1] * mat4_2.data[1][3] + mat4_1.data[0][2] * mat4_2.data[2][3] + mat4_1.data[0][3] * mat4_2.data[3][3];

		dest[1][0] = mat4_1.data[1][0] * mat4_2.data[0][0] + mat4_1.data[1][1] * mat4_2.data[1][0] + mat4_1.data[1][2] * mat4_2.data[2][0] + mat4_1.data[1][3] * mat4_2.data[3][0];
		dest[1][1] = mat4_1.data[1][0] * mat4_2.data[0][1] + mat4_1.data[1][1] * mat4_2.data[1][1] + mat4_1.data[1][2] * mat4_2.data[2][1] + mat4_1.data[1][3] * mat4_2.data[3][1];
		dest[1][2] = mat4_1.data[1][0] * mat4_2.data[0][2] + mat4_1.data[1][1] * mat4_2.data[1][2] + mat4_1.data[1][2] * mat4_2.data[2][2] + mat4_1.data[1][3] * mat4_2.data[3][2];
		dest[1][3] = mat4_1.data[1][0] * mat4_2.data[0][3] + mat4_1.data[1][1] * mat4_2.data[1][3] + mat4_1.data[1][2] * mat4_2.data[2][3] + mat4_1.data[1][3] * mat4_2.data[3][3];

		dest[2][0] = mat4_1.data[2][0] * mat4_2.data[0][0] + mat4_1.data[2][1] * mat4_2.data[1][0] + mat4_1.data[2][2] * mat4_2.data[2][0] + mat4_1.data[2][3] * mat4_2.data[3][0];
		dest[2][1] = mat4_1.data[2][0] * mat4_2.data[0][1] + mat4_1.data[2][1] * mat4_2.data[1][1] + mat4_1.data[2][2] * mat4_2.data[2][1] + mat4_1.data[2][3] * mat4_2.data[3][1];
		dest[2][2] = mat4_1.data[2][0] * mat4_2.data[0][2] + mat4_1.data[2][1] * mat4_2.data[1][2] + mat4_1.data[2][2] * mat4_2.data[2][2] + mat4_1.data[2][3] * mat4_2.data[3][2];
		dest[2][3] = mat4_1.data[2][0] * mat4_2.data[0][3] + mat4_1.data[2][1] * mat4_2.data[1][3] + mat4_1.data[2][2] * mat4_2.data[2][3] + mat4_1.data[2][3] * mat4_2.data[3][3];

		dest[3][0] = mat4_1.data[3][0] * mat4_2.data[0][0] + mat4_1.data[3][1] * mat4_2.data[1][0] + mat4_1.data[3][2] * mat4_2.data[2][0] + mat4_1.data[3][3] * mat4_2.data[3][0];
		dest[3][1] = mat4_1.data[3][0] * mat4_2.data[0][1] + mat4_1.data[3][1] * mat4_2.data[1][1] + mat4_1.data[3][2] * mat4_2.data[2][1] + mat4_1.data[3][3] * mat4_2.data[3][1];
		dest[3][2] = mat4_1.data[3][0] * mat4_2.data[0][2] + mat4_1.data[3][1] * mat4_2.data[1][2] + mat4_1.data[3][2] * mat4_2.data[2][2] + mat4_1.data[3][3] * mat4_2.data[3][2];
		dest[3][3] = mat4_1.data[3][0] * mat4_2.data[0][3] + mat4_1.data[3][1] * mat4_2.data[1][3] + mat4_1.data[3][2] * mat4_2.data[2][3] + mat4_1.data[3][3] * mat4_2.data[3][3];
		Matrix4x4 n(dest);
		//std::cout << n.str() << std::endl;
		return n;
	}

	Matrix4x4 operator*=(const Matrix4x4& mat4) {
		return Multiply(*this, mat4);
	}

	double* at(int row, int column) {
		return &data[row][column];
	}

	const double size = 16;

#define _s(x, y) std::to_string(data[x][y]) + ", " +
	std::string str() {
		
		return 
		"\n(" + _s(0, 0) _s(0, 1) _s(0, 2) _s(0, 3) "\n" + 
			    _s(1, 0) _s(1, 1) _s(1, 2) _s(1, 3) "\n" + 
				_s(2, 0) _s(2, 1) _s(2, 2) _s(2, 3) "\n" +
		    	_s(3, 0) _s(3, 1) _s(3, 2) _s(3, 3) ")";
	}
	Matrix4x4 operator-() const {
		return (
			-data[0][0], -data[0][1], -data[0][2], -data[0][3],
			-data[1][0], -data[1][1], -data[1][2], -data[1][3],
			-data[2][0], -data[2][1], -data[2][2], -data[2][3],
			-data[3][0], -data[3][1], -data[3][2], -data[3][3]
		);
	}
	Matrix4x4& operator=(const Matrix4x4& mat) {
		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				data[x][y] = mat.data[x][y];
			}
		}
		return *this;
	}
};
inline Matrix4x4 operator+(const Matrix4x4& mat4_1, const Matrix4x4& mat4_2) {
	Matrix4x4 newMat;
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			newMat.data[x][y] = mat4_1.data[x][y] + mat4_2.data[x][y];
		}
	}
	return newMat;
}
inline Matrix4x4 operator-(const Matrix4x4& mat4_1, const Matrix4x4& mat4_2) {
	Matrix4x4 newMat;
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			newMat.data[x][y] = mat4_1.data[x][y] - mat4_2.data[x][y];
		}
	}
	return newMat;
}
inline std::ostream& operator<<(std::ostream& out, const Matrix4x4& mat4) {
#define _out(x, y) std::to_string(mat4.data[x][y]) << ", " <<
	return out << "\n(" << 
		   _out(0, 0) _out(0, 1) _out(0, 2) _out(0, 3) "\n" <<
		   _out(1, 0) _out(1, 1) _out(1, 2) _out(1, 3) "\n" <<
		   _out(2, 0) _out(2, 1) _out(2, 2) _out(2, 3) "\n" <<
		   _out(3, 0) _out(3, 1) _out(3, 2) _out(3, 3) ")\n";
}
inline Matrix4x4 operator*(const Matrix4x4& mat4_1, const Matrix4x4& mat4_2) {
	return Matrix4x4::Multiply(mat4_1, mat4_2);
}
inline Matrix4x4 operator/(const Matrix4x4& mat4_1, const Matrix4x4& mat4_2) {
	return mat4_1 * -mat4_2;
}
inline Matrix4x4 operator*(double& i, const Matrix4x4& mat4_2) {
	Matrix4x4 newMat;
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			newMat.data[x][y] = i * mat4_2.data[x][y];
		}
	}
	return newMat;
}
inline Matrix4x4 operator/(double& i, const Matrix4x4& mat4_2) {
	Matrix4x4 newMat;
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			newMat.data[x][y] = i / mat4_2.data[x][y];
		}
	}
	return newMat;
}