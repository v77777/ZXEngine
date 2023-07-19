#include "FGSpring.h"
#include "../RigidBody.h"

namespace ZXEngine
{
	namespace PhysZ
	{
		FGSpring::FGSpring(const Vector3& connectionPoint, const Vector3& otherConnectionPoint, RigidBody* other, float springConstant, float restLength) :
			mConnectionPoint(connectionPoint), 
			mOtherConnectionPoint(otherConnectionPoint), 
			mOther(other), 
			mSpringConstant(springConstant), 
			mRestLength(restLength)
		{}

        void FGSpring::UpdateForce(RigidBody* rigidBody, float duration)
        {
            // �������ӵ���������������ϵ�µ�λ��
            Vector3 lws = rigidBody->GetTransform() * mConnectionPoint;
            Vector3 ows = mOther->GetTransform() * mOtherConnectionPoint;
            // ���
            Vector3 dis = lws - ows;
            // ���ɵ�ǰ����
            float length = dis.GetMagnitude();
            // ������������
            float delta = abs(length - mRestLength);
            // ������С
            float forceScalar = delta * mSpringConstant;
            // ��������
            Vector3 force = -forceScalar * dis.GetNormalized();
            // ʩ�ӵ���ǰ������
            rigidBody->AddForceAtPoint(force, lws);
        }
	}
}