#pragma once
#include "Object3d.h"
#include "CollisionInfo.h"

//�Փ˔���p�֘A�̌^��`
enum CollisionShapeType
{
	SHAPE_UNKNOWN = -1, // ���ݒ�

	COLLISIONSHAPE_SPHERE, // ��
	COLLISIONSHAPE_MESH, // ���b�V��
};

class BaseCollider
{
public:
	BaseCollider() = default;
	virtual ~BaseCollider() = default;

	inline void SetObject(Object3d *object)
	{
		this->object3d = object;
	}
	
	inline Object3d* GetObject3d()
	{
		return object3d;
	}

	//�X�V
	virtual void Update() = 0;
	//�`��^�C�v�擾
	inline CollisionShapeType GetShapeType() { return shapeType; }

	//�Փˎ��R�[���o�b�N�֐�
	inline void OnCollision(const CollisionInfo& info)
	{
		//object3d->OnCollision(info);
	}

protected:
	Object3d *object3d = nullptr;
	//�`��^�C�v
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
};