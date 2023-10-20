// ReSharper disable All
#pragma once
#include "BaseSkill.h"
#include "Lemur/Graphics/Graphics.h"

// 1.空白(何も起こらない)
class Blank :public BaseSkill
{
public:
    Blank(const wchar_t* ui_spr_filename) :BaseSkill("Blank", ui_spr_filename){}

    // 何もしない
};

