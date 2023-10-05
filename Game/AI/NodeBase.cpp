// ReSharper disable All
#include "JudgmentBase.h"
#include "NodeBase.h"
#include "BehaviorData.h"
#include "BehaviorTree.h"
#include "ActionBase.h"

// �f�X�g���N�^
	NodeBase::~NodeBase()
	{
		delete judgment;
		delete action;
	}
	// �q�m�[�h�Q�b�^�[
	NodeBase* NodeBase::GetChild(int index)
	{
		if (children.size() <= index)
		{
			return nullptr;
		}
		return children.at(index);
	}

	// �q�m�[�h�Q�b�^�[(����)
	NodeBase* NodeBase::GetLastChild()
	{
		if (children.size() == 0)
		{
			return nullptr;
		}

		return children.at(children.size() - 1);
	}

	// �q�m�[�h�Q�b�^�[(�擪)
	NodeBase* NodeBase::GetTopChild()
	{
		if (children.size() == 0)
		{
			return nullptr;
		}

		return children.at(0);
	}


	// �m�[�h����
	NodeBase* NodeBase::SearchNode(std::string searchName)
	{
		// ���O����v
		if (name == searchName)
		{
			return this;
		}
		else {
			// �q�m�[�h�Ō���
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

	// �m�[�h���_
	NodeBase* NodeBase::Inference(BehaviorData* data)
	{
		// ���̍s���̃��X�g�A�����ɂł���I�����̍s��������
		std::vector<NodeBase*> list;
		// ���肳�ꂽ�m�[�h������
		NodeBase* result = nullptr;

		// �V�[�P���X������s�\���X�g����Ȃ�(�V�[�P���X���ĂȂ��A�܂��͂܂��o�^����ĂȂ����Ă���)
		if (data->GetSequenceList().empty())
		{
			// ���̍s���̑I�������i���Ă�

			// children�̐��������[�v���s���B
			for (int i = 0; i < children.size(); i++)
			{
				// children.at(i)->judgment��nullptr�łȂ����
				if (children.at(i)->judgment != nullptr)
				{
					// children.at(i)->judgment->Judgment()�֐������s���Ature�ł����
					// list��children.at(i)��ǉ����Ă���
					if (children.at(i)->judgment->Judgment())
					{
						list.emplace_back(children.at(i));
					}
				}
				else {
					// ����N���X���Ȃ���Ζ������ɒǉ�
					list.emplace_back(children.at(i));
				}
			}
		}

		// �I�����[���Ńm�[�h����
		switch (selectRule)
		{
			// �D�揇��
		case BehaviorTree::SelectRule::Priority:
			result = SelectPriority(&list);
			break;
			// �����_��
		case BehaviorTree::SelectRule::Random:
			result = SelectRandom(&list);
			break;
			// �V�[�P���X
		case BehaviorTree::SelectRule::Sequence:
		case BehaviorTree::SelectRule::SequentialLooping:
			result = SelectSequence(list, data);
			break;
		}

		// �����ŃA�N�V����������ΐ��_���I��
		// �Ⴄ�ꍇ�͂���Ɉ���̃m�[�h����I������T��
		// �A�N�V������������܂ł��B�A�N�V�������Ȃ�������o�O��̂Œ��ӁB
		if (result != nullptr)
		{
			// �s��������ΏI��
			if (result->HasAction() == true)
			{
				return result;
			}
			else {
				// ���܂����m�[�h�ł܂����_�J�n
				result = result->Inference(data);
			}
		}

		_ASSERT_EXPR(result, L"�m�[�h�����܂�Ȃ������B����");

		return result;
	}

	// �D�揇�ʂŃm�[�h�I��
	NodeBase* NodeBase::SelectPriority(std::vector<NodeBase*>* list)
	{
		NodeBase* selectNode = nullptr;
		int priority = INT_MAX;

		// ��ԗD�揇�ʂ������m�[�h��T����selectNode�Ɋi�[
		//�S�Ă̑I�����̐������J��Ԃ�
		for (NodeBase* node : *list)
		{
			//��ԗD�揇�ʂ��������̂�T��
			if (node->GetPriority() < priority)
			{
				priority = node->GetPriority();

				// �ŏI�I�Ɉ�ԗD�揇�ʂ̍������̂�I������B
				selectNode = node;
			}
		}

		return selectNode;
	}


	// �����_���Ńm�[�h�I��
	NodeBase* NodeBase::SelectRandom(std::vector<NodeBase*>* list)
	{
		int selectNo = 0;
		// list�̃T�C�Y�ŗ������擾����selectNo�Ɋi�[
		selectNo = rand() % list->size();

		// list��selectNo�Ԗڂ̎��Ԃ����^�[��
		return (*list).at(selectNo);
	}

	// �V�[�P���X�E�V�[�P���V�������[�s���O�Ńm�[�h�I��
	NodeBase* NodeBase::SelectSequence(std::vector<NodeBase*>& list, BehaviorData* data)
	{
		// �q�m�[�h�̂ǂ��܂ŏ���������
		int step = 0;

		// �w�肳��Ă��钆�ԃm�[�h�̃V�[�P���X���ǂ��܂Ŏ��s���ꂽ���擾����
		step = data->GetSequenceStep(name);

		// ���ԃm�[�h�ɓo�^����Ă���m�[�h���ȏ�̏ꍇ(�Ō�܂ŃV�[�P���X�ŏ��������ꍇ)�A
		if (step >= children.size())
		{
			// ���[���ɂ���ď�����؂�ւ���
			// ���[����BehaviorTree::SelectRule::SequentialLooping�̂Ƃ��͍ŏ�������s���邽�߁Astep��0����
			if (selectRule == BehaviorTree::SelectRule::SequentialLooping)
			{
				step = 0;
			}

			// ���[����BehaviorTree::SelectRule::Sequence�̂Ƃ��͎��Ɏ��s�ł���m�[�h���Ȃ����߁Anullptr�����^�[��
			if (selectRule == BehaviorTree::SelectRule::Sequence)
			{
				return nullptr;
			}
		}

		// �V�[�P���X������s�\���X�g���ۑ�����Ă�����
		if (!data->GetSequenceList().empty())
			//���̎��s�\���X�g���g���B
			list = data->GetSequenceList();

		// ���s�\���X�g�ɓo�^����Ă���f�[�^�̐��������[�v���s��
		for (; step < children.size(); step++) {
			for (auto itr = list.begin(); itr != list.end(); itr++)
			{
				// �q�m�[�h�����s�\���X�g�Ɋ܂܂�Ă��邩
				if (children.at(step)->GetName() == (*itr)->GetName())
				{
					// ���݂̎��s�m�[�h�̕ۑ��A���Ɏ��s����X�e�b�v�̕ۑ����s������A
					// ���݂̃X�e�b�v�ԍ��̃m�[�h�����^�[������
					// (���̃t���[���ł����̃m�[�h�����s���Ă邩���ʂ��邽��)

					// ���s���̒��ԃm�[�h��ۑ��B�V�[�P���X���Ă�e�{�l�m�[�h�B
					data->PushSequenceNode(this);

					// �A�܂��A���Ɏ��s���钆�ԃm�[�h�ƃX�e�b�v����ۑ�����
					// �@�ۑ��ɂ�data->SetSequenceStep�֐����g�p�B
					// �@�ۑ��f�[�^�͒��ԃm�[�h�̖��O�Ǝ��̃X�e�b�v���ł�(step + 1)
					data->SetSequenceStep(this->name, step + 1);

					// �B�X�e�b�v�ԍ��ڂ̎q�m�[�h�����s�m�[�h�Ƃ��ă��^�[������
					return children.at(step);
				}
			}
		}

		// �w�肳�ꂽ���ԃm�[�h�Ɏ��s�\�m�[�h���Ȃ��̂�nullptr�����^�[������
		return nullptr;
	}

	// ����
	bool NodeBase::Judgment()
	{
		// judgment�����邩���f�B����΃����o�֐�Judgment()���s�������ʂ����^�[���B
		if (judgment != nullptr)
		{
			return judgment->Judgment();
		}
		return true;
	}

	// �m�[�h���s
	ActionBase::State NodeBase::Run(float elapsedTime)
	{
		// action�����邩���f�B����΃����o�֐�Run()���s�������ʂ����^�[���B
		if (action != nullptr)
		{
			return action->Run(elapsedTime);
		}

		return ActionBase::State::Failed;
	}
