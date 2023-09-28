// ReSharper disable All
#pragma once

#include <vector>
#include <string>
#include "BehaviorTree.h"
#include "ActionBase.h"
#include "NodeBase.h"
#include "BehaviorData.h"

class JudgmentBase;

//namespace Nero::Component::AI
//{
//	class BehaviorData;
//
//	class BehaviorTree;
//}
//
// メモリリーク調査用
#define debug_new new(_NORMAL_BLOCK,__FILE__,__LINE__)

// ノード
class NodeBase
{
public:
	// コンストラクタ
	NodeBase(std::string name, NodeBase* parent, NodeBase* sibling, int priority,
		BehaviorTree::SelectRule selectRule, JudgmentBase* judgment, ActionBase* action, int hierarchyNo) :
		name(name), parent(parent), sibling(sibling), priority(priority),
		selectRule(selectRule), judgment(judgment), action(action), hierarchyNo(hierarchyNo),
		children(NULL)
	{
	}
	// デストラクタ
	~NodeBase();

	// 名前ゲッター
	std::string GetName() { return name; }

	// 親ノードゲッター
	NodeBase* GetParent() { return parent; }

	// 子ノードゲッター
	NodeBase* GetChild(int index);
	// 子ノードゲッター(末尾)
	NodeBase* GetLastChild();
	// 子ノードゲッター(先頭)
	NodeBase* GetTopChild();

	// 兄弟ノードゲッター
	NodeBase* GetSibling() { return sibling; }

	// 階層番号ゲッター
	int GetHirerchyNo() { return hierarchyNo; }

	// 優先順位ゲッター
	int GetPriority() { return priority; }

	// 親ノードセッター
	void SetParent(NodeBase* parent) { this->parent = parent; }

	// 子ノード追加
	void AddChild(NodeBase* child) { children.push_back(child); }

	// 兄弟ノードセッター
	void SetSibling(NodeBase* sibling) { this->sibling = sibling; }

	// 行動データを持っているか
	bool HasAction() { return action != nullptr ? true : false; }

	// 実行可否判定
	bool Judgment();

	// 優先順位選択
	NodeBase* SelectPriority(std::vector<NodeBase*>* list);
	// ランダム選択
	NodeBase* SelectRandom(std::vector<NodeBase*>* list);
	// シーケンス選択
	NodeBase* SelectSequence(std::vector<NodeBase*>& list, BehaviorData* data);


	// ノード検索(自分と自分の全ての子の名前を比較して検索する)
	NodeBase* SearchNode(std::string searchName);

	// ノード推論(次の行動を順に選別する)
	NodeBase* Inference(BehaviorData* data);

	// 実行(選別された行動を実行、updateみたいなやつ)
	ActionBase::State Run(float elapsedTime);


	// 子ノード、末尾のノードではない限り
	//各ノードにもその下の階層のノードを持つ。
	//(一個下の階層だけ、末尾まではない。)
	// todo vectorにしてるってことは同じノードを入れれるから、無理やり確立をいじれるのでは？
	std::vector<NodeBase*>		children;

protected:

	std::string					name;			// 名前
	BehaviorTree::SelectRule	selectRule;		// 選択ルール

	// 判定する、判定の内容。継承して判定の内容を自由に作る。
	//nullptrなら判定しないので即選択肢の候補になる
	JudgmentBase* judgment;	    // 判定クラス

	// 実際の動き、動作の内容。継承して行動の内容を自由に作る。
	//nullptrなら実際の動きはない＝末尾ではないはず
	ActionBase* action;			// 実行クラス

	unsigned int				priority;		// 優先順位
	NodeBase* parent;			// 親ノード
	NodeBase* sibling;		// 兄弟ノード
	int							hierarchyNo;	// 階層番号
};
