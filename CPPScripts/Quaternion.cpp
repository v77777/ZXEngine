#include "Quaternion.h"
#include "Math.h"

namespace ZXEngine
{
	Quaternion::Quaternion() 
	{
		x = 0;
		y = 0;
		z = 0;
		w = 1.0f;
	}

	Quaternion::Quaternion(float x, float y, float z, float w) 
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Quaternion Quaternion::Euler(float x, float y, float z)
	{
		// read here: http://www.euclideanspace.com/maths/geometry/rotations/conversions/eulerToQuaternion/index.htm
		float dx = x * Math::Deg2Rad(0.5f);
		float dy = y * Math::Deg2Rad(0.5f);
		float dz = z * Math::Deg2Rad(0.5f);
		// in order of YXZ, yaw, pitch, roll in local space
		float c1 = cos(dy);
		float s1 = sin(dy);
		float c2 = cos(dz);
		float s2 = sin(dz);
		float c3 = cos(dx);
		float s3 = sin(dx);
		float c1c2 = c1 * c2;
		float s1s2 = s1 * s2;

		Quaternion q;
		q.w = c1c2 * c3 - s1s2 * s3;
		q.x = c1c2 * s3 + s1s2 * c3;
		q.y = s1 * c2 * c3 + c1 * s2 * s3;
		q.z = c1 * s2 * c3 - s1 * c2 * s3;
		return q;
	}

	Quaternion Quaternion::operator * (const Quaternion& q)const 
	{
		float xx = w * q.x + q.w * x + y * q.z - z * q.y;
		float yy = w * q.y + q.w * y - x * q.z + z * q.x;
		float zz = w * q.z + q.w * z + x * q.y - y * q.x;
		float ww = w * q.w - (x * q.x + y * q.y + z * q.z);
		Quaternion result = Quaternion(xx, yy, zz, ww);
		result.Normalize();
		return result;
	}

	void Quaternion::operator *= (const Quaternion& q) 
	{
		// 把this解引用再乘
		*this = *this * q;
	}

	float Quaternion::Normal() const
	{
		return sqrtf(NormalSquare());
	}

	float Quaternion::NormalSquare() const
	{
		return x * x + y * y + z * z + w * w;
	}

	void Quaternion::Normalize()
	{
		float sqrNorm = NormalSquare();
		if (!Math::Approximately(sqrNorm, 0.f))
		{
			float normInverse = 1.0f / sqrtf(sqrNorm);
			x *= normInverse;
			y *= normInverse;
			z *= normInverse;
			w *= normInverse;
		}
	}

	vec3 Quaternion::GetEulerAngles() const 
	{
		// read here: http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/index.htm
		float test = x * y + z * w;
		float yaw, pitch, roll;
		if (test > 0.499f)
		{
			// singularity at north pole
			yaw = 2 * atan2(x, w);
			roll = Math::PI * 0.5f;
			pitch = 0;
		}
		else
		{
			if (test < -0.499f)
			{
				// singularity at south pole
				yaw = -2 * atan2(x, w);
				roll = -Math::PI * 0.5f;
				pitch = 0;
			}
			else
			{
				float sqx = x * x;
				float sqy = y * y;
				float sqz = z * z;

				yaw = atan2(2.f * y * w - 2 * x * z, 1 - 2 * sqy - 2 * sqz);
				roll = asin(2 * test);
				pitch = atan2(2 * x * w - 2 * y * z, 1 - 2 * sqx - 2 * sqz);
			}
		}

		vec3 result;
		result.x = Math::Rad2Deg(pitch);
		result.y = Math::Rad2Deg(yaw);
		result.z = Math::Rad2Deg(roll);
		return result;
	}

	void Quaternion::SetEulerAngles(float x, float y, float z) 
	{
		Quaternion q = Quaternion::Euler(x, y ,z);
		this->x = q.x;
		this->y = q.y;
		this->z = q.z;
		this->w = q.w;
	}

	mat4 Quaternion::ToMatrix()
	{
		float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

		x2 = this->x + this->x;
		y2 = this->y + this->y;
		z2 = this->z + this->z;

		xx = this->x * x2;
		xy = this->x * y2;
		xz = this->x * z2;

		yy = this->y * y2;
		yz = this->y * z2;

		zz = this->z * z2;

		wx = this->w * x2;
		wy = this->w * y2;
		wz = this->w * z2;

		// 第一行
		float m00 = 1.0f - (yy + zz);
		float m01 = xy - wz;
		float m02 = xz + wy;

		// 第二行
		float m10 = xy + wz;
		float m11 = 1.0f - (xx + zz);
		float m12 = yz - wx;

		// 第三行
		float m20 = xz - wy;
		float m21 = yz + wx;
		float m22 = 1.0f - (xx + yy);

		// 构建GLM矩阵的时候，行列是反的
		return mat4(
			m00, m10, m20, 0,
			m01, m11, m21, 0,
			m02, m12, m22, 0,
			0, 0, 0, 1);
	}
}