// ReSharper disable All
#pragma once
#include <string>

#include "BehaviorData.h"

	class NodeBase;
	class BehaviorData;
	class ActionBase;
	class JudgmentBase;

class Enemy;

// ビヘイビアツリー
//これがビヘイビアベースの全ての動き
class BehaviorTree
{
public:
	/*
	 * このビヘイビアツリーの組み方やと、シーケンスは中間ノードが全て実行可能ノードに
	 * 登録されてないとうまく推論されない(SelectSequence関数のfor文が原因)
	 * ノードをシーケンスにするときは全ての子ノードを基本的に絶対通るようにする(Failed、実行失敗は大丈夫)
	 * そういう時はPriorityを使おう。うまくやろう。
	 */

	 // 選択ルール
	enum class SelectRule
	{
		Non,				// 無い(末端ノード用)
		Priority,			// 優先順位
		Sequence,			// シーケンス
		SequentialLooping,	// シーケンシャルルーピング
		Random,				// ランダム
	};

public:
	BehaviorTree() : root(nullptr), owner(nullptr) {}

	BehaviorTree(Enemy* enemy) : root(nullptr), owner(enemy) {}

	~BehaviorTree();

	// 実行ノードを推論する
	NodeBase* ActiveNodeInference(BehaviorData* data);

	// シーケンスノードから推論開始
	NodeBase* SequenceBack(NodeBase* sequenceNode, BehaviorData* data);

	// ノード追加
	// 引数の順に
	// 親名前、ルートノードの場合はどこからも繋がってきていないので空文字を入れる。
	// 自分の名前、重複したらバグる。
	// 優先順位、値が低いものが優先される。
	// 選択アルゴリズム、各ルールはenum class SelectRule で定義されてる。
	// 判定するか、登録する際に new で欲しい判定クラスを生成する。
	// 行動があるか。ルートノードや中間ノードの場合は行動が無いのでnullptr を指定する。判定同様 new で生成。
	void AddNode(std::string parentName, std::string entryName, int priority, SelectRule selectRule, JudgmentBase* judgment, ActionBase* action);


	// 実行
	NodeBase* Run(NodeBase* actionNode, BehaviorData* data, float elapsedTime);
	void Update();
private:
	// ノード全削除
	void NodeAllClear(NodeBase* delNode);
private:
	// ルートノード(ツリーの大元)
	NodeBase* root;

	// 主にシークエンスの際に使用する。中間ノードとその中間ノードのステップ数などを保持する。
	BehaviorData* behaviorData = new BehaviorData;

	// 選択された末端ノード(実際の動作)が入る。
	//常に何か入ってないと全く動作しないことになるのでバグる。
	NodeBase* active_node;

	// ビヘイビアツリーの使用者(型はCharacterにすべき？)
	Enemy* owner;
};
