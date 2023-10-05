// ReSharper disable All
#include "JudgmentBase.h"
#include "NodeBase.h"
#include "BehaviorData.h"
#include "BehaviorTree.h"
#include "ActionBase.h"

// デストラクタ
	NodeBase::~NodeBase()
	{
		delete judgment;
		delete action;
	}
	// 子ノードゲッター
	NodeBase* NodeBase::GetChild(int index)
	{
		if (children.size() <= index)
		{
			return nullptr;
		}
		return children.at(index);
	}

	// 子ノードゲッター(末尾)
	NodeBase* NodeBase::GetLastChild()
	{
		if (children.size() == 0)
		{
			return nullptr;
		}

		return children.at(children.size() - 1);
	}

	// 子ノードゲッター(先頭)
	NodeBase* NodeBase::GetTopChild()
	{
		if (children.size() == 0)
		{
			return nullptr;
		}

		return children.at(0);
	}


	// ノード検索
	NodeBase* NodeBase::SearchNode(std::string searchName)
	{
		// 名前が一致
		if (name == searchName)
		{
			return this;
		}
		else {
			// 子ノードで検索
			for (auto itr = children.begin(); itr != children.end(); itr++)
			{
				NodeBase* ret = (*itr)->SearchNode(searchName);

				if (ret != nullptr)
				{
					return ret;
				}
			}
		}

		return nullptr;
	}

	// ノード推論
	NodeBase* NodeBase::Inference(BehaviorData* data)
	{
		// 次の行動のリスト、ここにできる選択肢の行動が入る
		std::vector<NodeBase*> list;
		// 決定されたノードが入る
		NodeBase* result = nullptr;

		// シーケンスする実行可能リストが空なら(シーケンスしてない、またはまだ登録されてないってこと)
		if (data->GetSequenceList().empty())
		{
			// 次の行動の選択肢を絞ってる

			// childrenの数だけループを行う。
			for (int i = 0; i < children.size(); i++)
			{
				// children.at(i)->judgmentがnullptrでなければ
				if (children.at(i)->judgment != nullptr)
				{
					// children.at(i)->judgment->Judgment()関数を実行し、tureであれば
					// listにchildren.at(i)を追加していく
					if (children.at(i)->judgment->Judgment())
					{
						list.emplace_back(children.at(i));
					}
				}
				else {
					// 判定クラスがなければ無条件に追加
					list.emplace_back(children.at(i));
				}
			}
		}

		// 選択ルールでノード決め
		switch (selectRule)
		{
			// 優先順位
		case BehaviorTree::SelectRule::Priority:
			result = SelectPriority(&list);
			break;
			// ランダム
		case BehaviorTree::SelectRule::Random:
			result = SelectRandom(&list);
			break;
			// シーケンス
		case BehaviorTree::SelectRule::Sequence:
		case BehaviorTree::SelectRule::SequentialLooping:
			result = SelectSequence(list, data);
			break;
		}

		// ここでアクションがあれば推論が終了
		// 違う場合はさらに一個下のノードから選択肢を探す
		// アクションが見つかるまでやる。アクションがなかったらバグるので注意。
		if (result != nullptr)
		{
			// 行動があれば終了
			if (result->HasAction() == true)
			{
				return result;
			}
			else {
				// 決まったノードでまた推論開始
				result = result->Inference(data);
			}
		}

		_ASSERT_EXPR(result, L"ノードが決まらなかった。うんち");

		return result;
	}

	// 優先順位でノード選択
	NodeBase* NodeBase::SelectPriority(std::vector<NodeBase*>* list)
	{
		NodeBase* selectNode = nullptr;
		int priority = INT_MAX;

		// 一番優先順位が高いノードを探してselectNodeに格納
		//全ての選択肢の数だけ繰り返す
		for (NodeBase* node : *list)
		{
			//一番優先順位が高いものを探す
			if (node->GetPriority() < priority)
			{
				priority = node->GetPriority();

				// 最終的に一番優先順位の高いものを選択する。
				selectNode = node;
			}
		}

		return selectNode;
	}


	// ランダムでノード選択
	NodeBase* NodeBase::SelectRandom(std::vector<NodeBase*>* list)
	{
		int selectNo = 0;
		// listのサイズで乱数を取得してselectNoに格納
		selectNo = rand() % list->size();

		// listのselectNo番目の実態をリターン
		return (*list).at(selectNo);
	}

	// シーケンス・シーケンシャルルーピングでノード選択
	NodeBase* NodeBase::SelectSequence(std::vector<NodeBase*>& list, BehaviorData* data)
	{
		// 子ノードのどこまで処理したか
		int step = 0;

		// 指定されている中間ノードのシーケンスがどこまで実行されたか取得する
		step = data->GetSequenceStep(name);

		// 中間ノードに登録されているノード数以上の場合(最後までシーケンスで処理した場合)、
		if (step >= children.size())
		{
			// ルールによって処理を切り替える
			// ルールがBehaviorTree::SelectRule::SequentialLoopingのときは最初から実行するため、stepに0を代入
			if (selectRule == BehaviorTree::SelectRule::SequentialLooping)
			{
				step = 0;
			}

			// ルールがBehaviorTree::SelectRule::Sequenceのときは次に実行できるノードがないため、nullptrをリターン
			if (selectRule == BehaviorTree::SelectRule::Sequence)
			{
				return nullptr;
			}
		}

		// シーケンスする実行可能リストが保存されていたら
		if (!data->GetSequenceList().empty())
			//その実行可能リストを使う。
			list = data->GetSequenceList();

		// 実行可能リストに登録されているデータの数だけループを行う
		for (; step < children.size(); step++) {
			for (auto itr = list.begin(); itr != list.end(); itr++)
			{
				// 子ノードが実行可能リストに含まれているか
				if (children.at(step)->GetName() == (*itr)->GetName())
				{
					// 現在の実行ノードの保存、次に実行するステップの保存を行った後、
					// 現在のステップ番号のノードをリターンする
					// (次のフレームでも何のノードを実行してるか判別するため)

					// 実行中の中間ノードを保存。シーケンスしてる親本人ノード。
					data->PushSequenceNode(this);

					// ②また、次に実行する中間ノードとステップ数を保存する
					// 　保存にはdata->SetSequenceStep関数を使用。
					// 　保存データは中間ノードの名前と次のステップ数です(step + 1)
					data->SetSequenceStep(this->name, step + 1);

					// ③ステップ番号目の子ノードを実行ノードとしてリターンする
					return children.at(step);
				}
			}
		}

		// 指定された中間ノードに実行可能ノードがないのでnullptrをリターンする
		return nullptr;
	}

	// 判定
	bool NodeBase::Judgment()
	{
		// judgmentがあるか判断。あればメンバ関数Judgment()実行した結果をリターン。
		if (judgment != nullptr)
		{
			return judgment->Judgment();
		}
		return true;
	}

	// ノード実行
	ActionBase::State NodeBase::Run(float elapsedTime)
	{
		// actionがあるか判断。あればメンバ関数Run()実行した結果をリターン。
		if (action != nullptr)
		{
			return action->Run(elapsedTime);
		}

		return ActionBase::State::Failed;
	}
