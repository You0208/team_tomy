// ReSharper disable All
#pragma once
#include "BaseSkill.h"

// 1.空白(何も起こらない)
class Blank :public BaseSkill
{
public:
    Blank() :BaseSkill("Blank") {}

    // 何もしない
};

