// ReSharper disable All
#pragma once

#include <vector>
#include <stack>
#include <map>
#include <string>

class NodeBase;



// Behavior保存データ
class BehaviorData
{
public:
	// コンストラクタ
	BehaviorData() { Init(); }
	// シーケンスノードのプッシュ
	void PushSequenceNode(NodeBase* node) { sequenceStack.push(node); }
	// シーケンスノードのポップ
	NodeBase* PopSequenceNode();
	// シーケンスステップのゲッター
	int GetSequenceStep(std::string name);
	// シーケンスステップのセッター
	void SetSequenceStep(std::string name, int step);
	// 初期化
	void Init();

	// シーケンスする実行可能リスト取得
	const std::vector<NodeBase*>& GetSequenceList() { return sequence_list; }
private:

	/* シーケンスする実行可能リスト
	 *教材のやり方はシーケンスで次の行動に移行するときに毎回
	 *推論してしまうから、シーケンスとわかったなら一回目に
	 *実行可能リストに入ったアクションを保持してFailedにならない限り
	 *すべて実行する形にする。
	 *(いつかは教材のパターンと使い分けできるようになってもいいかも、
	 *使う機会あるかわからんけど)
	 */
	std::vector<NodeBase*> sequence_list{};

	// 挿入された側から要素を取り出す(本を積むイメージ)
	// シーケンス実行してる中間ノードをスタック(親本人)
	std::stack<NodeBase*> sequenceStack;

	// 実行中の中間ノードのステップを記録
	std::map<std::string, int> runSequenceStepMap;
};
