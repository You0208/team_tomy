// ReSharper disable All
#include "BehaviorTree.h"
#include "ActionBase.h"
#include "NodeBase.h"
#include "JudgmentBase.h"
#include "BehaviorData.h"


	// �f�X�g���N�^
	BehaviorTree::~BehaviorTree()
	{
		NodeAllClear(root);
	}

	void BehaviorTree::AddNode(std::string parentName, std::string entryName, int priority, SelectRule selectRule, JudgmentBase* judgment, ActionBase* action)
	{
		// �e�̖��O���ݒ肳��Ă����
		if (parentName != "")
		{
			// �ݒ肳�ꂽ�e���O������
			NodeBase* parentNode = root->SearchNode(parentName);

			if (parentNode != nullptr)
			{
				// �Z��m�[�h
				NodeBase* sibling = parentNode->GetLastChild();
				NodeBase* addNode = new NodeBase(entryName, parentNode, sibling, priority, selectRule, judgment, action, parentNode->GetHirerchyNo() + 1);

				parentNode->AddChild(addNode);
			}
		}
		//�e�����Ȃ���
		else {
			// �匳���Ȃ����
			if (root == nullptr)
			{
				// �匳�ɐݒ肷��
				root = new NodeBase(entryName, nullptr, nullptr, priority, selectRule, judgment, action, 1);
			}
		}
	}

	// ���_
	NodeBase* BehaviorTree::ActiveNodeInference(BehaviorData* data)
	{
		// �f�[�^�����Z�b�g���ĊJ�n
		data->Init();
		return root->Inference(data);
	}

	// �V�[�P���X�m�[�h����̐��_�J�n
	NodeBase* BehaviorTree::SequenceBack(NodeBase* sequenceNode, BehaviorData* data)
	{
		return sequenceNode->Inference(data);
	}

	// �m�[�h���s
	NodeBase* BehaviorTree::Run(NodeBase* actionNode, BehaviorData* data, float elapsedTime)
	{
		// �m�[�h���s
		ActionBase::State state = actionNode->Run(elapsedTime);

		// ����I��
		if (state == ActionBase::State::Complete)
		{
			// �V�[�P���X�̓r�����𔻒f

			//�V�[�P���X���Ă�e�m�[�h
			NodeBase* sequenceNode = data->PopSequenceNode();

			// �r������Ȃ��Ȃ�I��
			if (sequenceNode == nullptr)
			{
				return nullptr;
			}
			else {
				// �r���Ȃ炻������n�߂�
				return SequenceBack(sequenceNode, data);
			}
		}
		// ���s�͏I��
		else if (state == ActionBase::State::Failed) {
			return nullptr;
		}

		// ����ێ�
		return actionNode;
	}

	void BehaviorTree::Update()
	{
		// �����Ńr�w�C�r�A�c���[�ɂ��s���J��

	    // ���ݎ��s����Ă���m�[�h���������
		if (active_node == nullptr)
		{
			// ���Ɏ��s����m�[�h�𐄘_����B
			active_node = ActiveNodeInference(behaviorData);
		}
		// ���ݎ��s����m�[�h�������
		if (active_node != nullptr)
		{
			// �r�w�C�r�A�c���[����m�[�h�����s�B
			//active_node = Run(active_node, behaviorData, Lemur::Nero::Time::Timer::Instance().time_interval());
		}


	}

    // �o�^���ꂽ�m�[�h��S�č폜����
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


