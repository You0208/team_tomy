// ReSharper disable All
#include "BehaviorTree.h"
#include "ActionBase.h"
#include "NodeBase.h"
#include "JudgmentBase.h"
#include "BehaviorData.h"


	// デストラクタ
	BehaviorTree::~BehaviorTree()
	{
		NodeAllClear(root);
	}

	void BehaviorTree::AddNode(std::string parentName, std::string entryName, int priority, SelectRule selectRule, JudgmentBase* judgment, ActionBase* action)
	{
		// 親の名前が設定されていれば
		if (parentName != "")
		{
			// 設定された親名前を検索
			NodeBase* parentNode = root->SearchNode(parentName);

			if (parentNode != nullptr)
			{
				// 兄弟ノード
				NodeBase* sibling = parentNode->GetLastChild();
				NodeBase* addNode = new NodeBase(entryName, parentNode, sibling, priority, selectRule, judgment, action, parentNode->GetHirerchyNo() + 1);

				parentNode->AddChild(addNode);
			}
		}
		//親がいなくて
		else {
			// 大元もなければ
			if (root == nullptr)
			{
				// 大元に設定する
				root = new NodeBase(entryName, nullptr, nullptr, priority, selectRule, judgment, action, 1);
			}
		}
	}

	// 推論
	NodeBase* BehaviorTree::ActiveNodeInference(BehaviorData* data)
	{
		// データをリセットして開始
		data->Init();
		return root->Inference(data);
	}

	// シーケンスノードからの推論開始
	NodeBase* BehaviorTree::SequenceBack(NodeBase* sequenceNode, BehaviorData* data)
	{
		return sequenceNode->Inference(data);
	}

	// ノード実行
	NodeBase* BehaviorTree::Run(NodeBase* actionNode, BehaviorData* data, float elapsedTime)
	{
		// ノード実行
		ActionBase::State state = actionNode->Run(elapsedTime);

		// 正常終了
		if (state == ActionBase::State::Complete)
		{
			// シーケンスの途中かを判断

			//シーケンスしてる親ノード
			NodeBase* sequenceNode = data->PopSequenceNode();

			// 途中じゃないなら終了
			if (sequenceNode == nullptr)
			{
				// todo ここでnullptrを返すのが割と都合悪い
				return nullptr;
			}
			else {
				// 途中ならそこから始める
				return SequenceBack(sequenceNode, data);
			}
		}
		// 失敗は終了
		else if (state == ActionBase::State::Failed) {
			return nullptr;
		}

		// 現状維持
		return actionNode;
	}

	void BehaviorTree::Update()
	{
		// ここでビヘイビアツリーによる行動遷移

	    // 現在実行されているノードが無ければ
		if (active_node == nullptr)
		{
			// 次に実行するノードを推論する。
			active_node = ActiveNodeInference(behaviorData);
		}
		// 現在実行するノードがあれば
		if (active_node != nullptr)
		{
			// ビヘイビアツリーからノードを実行。
			//active_node = Run(active_node, behaviorData, Lemur::Nero::Time::Timer::Instance().time_interval());
		}


	}

    // 登録されたノードを全て削除する
	void BehaviorTree::NodeAllClear(NodeBase* delNode)
	{
		size_t count = delNode->children.size();
		if (count > 0)
		{
			for (NodeBase* node : delNode->children)
			{
				NodeAllClear(node);
			}
			delete delNode;
		}
		else
		{
			delete delNode;
		}
	}


