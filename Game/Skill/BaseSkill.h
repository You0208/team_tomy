// ReSharper disable All
#pragma once
#include <memory>
#include <string>

#include "Lemur/Graphics/Graphics.h"
#include "Lemur/Graphics/sprite.h"

class Player;

// 取得スキルの基底クラス
class BaseSkill
{
public:
    // プレイヤーがシーンの全スキル配列からスキルを取得するタイミングで、オーナーをセットする。
    BaseSkill(std::string skill_name_, const wchar_t* ui_spr_filename,int priorty_ = 0) : skill_name(skill_name_),priorty(priorty_)
    {
        UI_spr = std::make_unique<sprite>(Lemur::Graphics::Graphics::Instance().GetDevice(), ui_spr_filename);
    }

    //BaseSkill(Player* player,std::string skill_name_) :owner(player),skill_name(skill_name_){}

    ~BaseSkill() = default;

    virtual void DrawImGui(){}

    // 戦闘開始時に一回だけ呼ばれる。
    virtual void Init(){}
    // 戦闘中呼び続ける。
    virtual void Update(){}
    // 戦闘終了時に一回だけ呼ばれる。
    virtual void Fin(){}

    // プレイヤーがスキルを取得するタイミングでオーナーをセット
    void SetOwner(Player* player) { owner = player; }

    Player* GetOwner()const { return owner; }

    // 名前取得
    std::string GetName()const { return skill_name; }

    // スキル抽選されてるフラグ
    bool is_select = false;

    //auto GetIT()
    //{
    //    std::iterator<int>
    //}

    // スキルの順番を入れ替えるために使う。
    //順番で性能などが左右されるスキルのみ設定する。
    int priorty = 0;

    // スキルのUIアセット
    std::unique_ptr<sprite> UI_spr;
protected:
    std::string skill_name;

    Player* owner;


};

// 10.連鎖(ダメージを受けずに敵を攻撃することでコンボが繋がりダメージが上がる)
// 17.策士(負けても賭けたステータスが減らない)
// 18.神力(バットスキルの効果を無効にする)
// 19.好調(バットスキル以外のスキルの効果が２倍になる)
// 20.豪運(戦いに勝利した場合1.5倍報酬が上がる)


// 3.不幸(勝っても1.1倍しか増えない(バットスキル))
// 4.盲目(選択する敵の倍率が見えない(バットスキル))
// 5.鈍足(移動スピードが遅くなる(バットスキル))
// 7.怠惰(ステータスが低い状態から始まる、時間経過でステータスが戻る)
// 8.嫉妬(HPが攻撃力より高い場合HPを攻撃力と同じステータスに下げる。
// 9.木剣(攻撃力が下がる)
// 10.不調(バットスキルの効果が２倍になる)

// 2.花畑(戦闘BGMが可愛い曲になる)
// 3.沈黙(音が全て無くなる)
// 4.発光(主人公が発光する)
// 5.牛声(斬撃の音が牛の声になる)
// 6.広告(死ぬと広告が流れる)
