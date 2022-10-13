#pragma once
#include "Object3d.h"
#include "CollisionInfo.h"

//衝突判定用関連の型定義
enum CollisionShapeType
{
	SHAPE_UNKNOWN = -1, // 未設定

	COLLISIONSHAPE_SPHERE, // 球
	COLLISIONSHAPE_MESH, // メッシュ
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

	//更新
	virtual void Update() = 0;
	//形状タイプ取得
	inline CollisionShapeType GetShapeType() { return shapeType; }

	//衝突時コールバック関数
	inline void OnCollision(const CollisionInfo& info)
	{
		//object3d->OnCollision(info);
	}

protected:
	Object3d *object3d = nullptr;
	//形状タイプ
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
};