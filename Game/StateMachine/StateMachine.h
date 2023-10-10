// ReSharper disable All
#pragma once
#include <vector>
#include <memory>
#include "Game/Player/Player.h"
#include "Game/StateMachine/StateBase.h"


namespace Nero::Component::AI
{
    /* 状態の変化を管理するコンポーネント。
     * 条件が満たされると状態から別の状態に遷移する
     * 
     *  使い方:
     * 各動作、ステート(状態)は StateDerived.h で自分で作る。
     * そこで状態から状態の切り替えなども行う。
     * 使用する人のInit関数で SetUpState関数 を使って
     * 作ったステートをセットできる。
     * その時に必ずfirst_stateを設定すること。
     */
    class StateMachine
    {
    public:
        StateMachine(){}
        ~StateMachine() = default;

        void Update();
        void ImGui();

        //次のステートをセット
        void SetNextState(int next_state_);

        // ステートの生成とセット
        template<class State>
        void SetUpState(Player* player, bool is_first_state = false)// todo このI配列のndex決め方きれいじゃない感じして納得いかん、PlayerとかにEnum持たせるんやなくて
            // そういうクラスとかネームスペース作るべき？マップ配列はおもなるかもしれへん
        {

            // todo これメモリリーク起きる？
            State* state = new State(player);
            states.emplace_back(state);

            // todo 自動で最初のステートを設定できるようにしたい。今はデフォルト引数で妥協
            // 初めのステートならセットする
            if (is_first_state)
            {
                current_state = state;
                current_state->Begin();
            }

        }

        StateBase* GetCurrentState()const { return current_state; }

        float GetMotionValue();
    private:
        // ステートを切り替える関数
        //次のステートがセットされたら勝手に実行される
        void ChangeState();

    private:

        std::vector<std::unique_ptr<StateBase>>states;

        // 現在のステート
        StateBase* current_state = nullptr;

        // 次のステート(セットされたら即入れ替わる)
        StateBase* next_state = nullptr;
    };

}
