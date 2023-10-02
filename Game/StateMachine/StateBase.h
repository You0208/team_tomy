// ReSharper disable All
#pragma once
#include <string>

class Player;
namespace Nero::Component::AI
{
    /* ステートマシンで使う動作を作るための基底クラス
     * こいつを継承して、ほしい動作、状態を自分で作る。
     * それをステートマシンを使って搭載する。
     * 作成時に owner と state_name を設定する。
     */
    class StateBase
    {
    public:
        // todo テンプレート化
        // 生成時に引数でownerをセット
        StateBase(Player* player, std::string state_name_)
            :owner(player), state_name(state_name_) {};

        ~StateBase() = default;

        virtual void Begin() = 0;// ステートが始まるとき
        virtual void Update() = 0;// ステート中、常に呼ばれる
        virtual void End() = 0;// ステートが終了するとき

        // ステート名
        std::string state_name;

    protected:
        // todo テンプレート化
        // ステートアクションする人
        Player* owner;

        // ステートの段階
        int step = 0;
    };

}
