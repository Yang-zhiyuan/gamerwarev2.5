#pragma once
#include "..\SDK\VMatrix.h"

#define PI 3.14159265358979323846f

class Math {
public:

	class Quaternion				// same data-layout as engine's vec4_t,
	{								//		which is a vec_t[4]
	public:
		inline Quaternion(void) {

			// Initialize to NAN to catch errors
#ifdef _DEBUG
#ifdef VECTOR_PARANOIA
			x = y = z = w = VEC_T_NAN;
#endif
#endif
		}
		inline Quaternion(vec_t ix, vec_t iy, vec_t iz, vec_t iw) : x(ix), y(iy), z(iz), w(iw) { }
		//inline Quaternion(RadianEuler const& angle);	// evil auto type promotion!!!

		inline void Init(vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f, vec_t iw = 0.0f) { x = ix; y = iy; z = iz; w = iw; }

		bool IsValid() const;
		void Invalidate();

		bool operator==(const Quaternion& src) const;
		bool operator!=(const Quaternion& src) const;

		vec_t* Base() { return (vec_t*)this; }
		const vec_t* Base() const { return (vec_t*)this; }

		// array access...
		vec_t operator[](int i) const;
		vec_t& operator[](int i);

		vec_t x, y, z, w;
	};

	class RadianEuler
	{
	public:
		inline RadianEuler(void) { }
		inline RadianEuler(vec_t X, vec_t Y, vec_t Z) { x = X; y = Y; z = Z; }
		inline RadianEuler(Quaternion const& q);	// evil auto type promotion!!!
		inline RadianEuler(QAngle const& angles);	// evil auto type promotion!!!

		// Initialization
		inline void Init(vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f) { x = ix; y = iy; z = iz; }

		//	conversion to qangle
		QAngle ToQAngle(void) const;
		bool IsValid() const;
		void Invalidate();

		// array access...
		vec_t operator[](int i) const;
		vec_t& operator[](int i);

		vec_t x, y, z;
	};

	void RotateMovement(const QAngle& oldAngles, const Vector& oldMove, const QAngle& newAngles, Vector& newMove);

	void VectorAngles(const Vector &vecForward, Vector &vecAngles);

	float fDotProduct(const float* v1, const float* v2);

	void AngleMatrix(const QAngle angles, matrix3x4_t& matrix);

	void MatrixSetColumn(const Vector& in, int column, matrix3x4_t& out);

	void AngleMatrix(const QAngle& angles, const Vector& position, matrix3x4_t& matrix_out);

	void MatrixCopy(const matrix3x4_t& source, matrix3x4_t& target);

	void MatrixMultiply(matrix3x4_t& in1, const matrix3x4_t& in2);

	void ConcatTransforms(const matrix3x4_t& in1, const matrix3x4_t& in2, matrix3x4_t& out);

	void MatrixMultiply(const matrix3x4_t& in1, const matrix3x4_t& in2, matrix3x4_t& out);

	void VectorRotate(const float* in1, const matrix3x4_t& in2, float* out);

	void VectorRotate(const Vector& in1, const matrix3x4_t& in2, Vector& out);

	void VectorRotate(const Vector& in1, const QAngle& in2, Vector& out);

	void VectorAngles(const Vector& forward, Vector& up, Vector& angles);

	void VectorTransform(const Vector in1, const matrix3x4_t in2, Vector &out);

	Vector CalcAngle(const Vector& vecSource, const Vector& vecDestination);

	Vector calculate_angle(const Vector& source, const Vector& destination, const Vector& viewAngles);

	vec_t VectorNormalize(Vector& v);

	void AngleVectors(const Vector& angles, Vector* forward);

	void sin_cos(float radian, float* sin, float* cos);

	void AngleVectors(const QAngle& angles, Vector* forward, Vector* right, Vector* up);

	void NormalizeAngles(Vector& angles);

	float NormalizeYaw(float yaw);

	auto get_rotated_position(Vector start, const float rotation, const float distance)->Vector;

	void AngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up);

	void RandomSeed(int seed);

	float RandomFloat(float min, float max);

	bool Clamp(Vector &angles);

	void ClampAngles(Vector &angles);

	float GRD_TO_BOG(float GRD);

	float Distance(Vector2D point1, Vector2D point2);



}; 
extern Math g_Math;