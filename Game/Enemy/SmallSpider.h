#pragma once
#include "Enemy.h"
#include "Enemy.h"
class SmallSpider :
    public Enemy
{
public:
    SmallSpider() {}
    SmallSpider(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_) :Enemy(input_, physics_, graphics_)
    {
        enemy_type = "SmallSpider";
    }


};

