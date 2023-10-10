#pragma once
#include <vector>

#include "Lemur/Component/GameObject.h"

class ColliderManager
{
public:

	static ColliderManager& Instance()
	{
		static ColliderManager instance;
		return instance;
	}

	void Update();

	//�����蔻��Z�b�g
	void SetCollider(GameObject* game_object) { colliders.emplace_back(game_object); }

private:
    std::vector<GameObject*> colliders;
};

