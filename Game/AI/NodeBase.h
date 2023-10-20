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
// ���������[�N�����p
#define debug_new new(_NORMAL_BLOCK,__FILE__,__LINE__)

// �m�[�h
class NodeBase
{
public:
	// �R���X�g���N�^
	NodeBase(std::string name, NodeBase* parent, NodeBase* sibling, int priority,
		BehaviorTree::SelectRule selectRule, JudgmentBase* judgment, ActionBase* action, int hierarchyNo) :
		name(name), parent(parent), sibling(sibling), priority(priority),
		selectRule(selectRule), judgment(judgment), action(action), hierarchyNo(hierarchyNo),
		children(NULL)
	{
	}
	// �f�X�g���N�^
	~NodeBase();

	// ���O�Q�b�^�[
	std::string GetName() { return name; }

	// �e�m�[�h�Q�b�^�[
	NodeBase* GetParent() { return parent; }

	// �q�m�[�h�Q�b�^�[
	NodeBase* GetChild(int index);
	// �q�m�[�h�Q�b�^�[(����)
	NodeBase* GetLastChild();
	// �q�m�[�h�Q�b�^�[(�擪)
	NodeBase* GetTopChild();

	// �Z��m�[�h�Q�b�^�[
	NodeBase* GetSibling() { return sibling; }

	// �K�w�ԍ��Q�b�^�[
	int GetHirerchyNo() { return hierarchyNo; }

	// �D�揇�ʃQ�b�^�[
	int GetPriority() { return priority; }

	// �e�m�[�h�Z�b�^�[
	void SetParent(NodeBase* parent) { this->parent = parent; }

	// �q�m�[�h�ǉ�
	void AddChild(NodeBase* child) { children.push_back(child); }

	// �Z��m�[�h�Z�b�^�[
	void SetSibling(NodeBase* sibling) { this->sibling = sibling; }

	// �s���f�[�^�������Ă��邩
	bool HasAction() { return action != nullptr ? true : false; }

	// ���s�۔���
	bool Judgment();

	// �D�揇�ʑI��
	NodeBase* SelectPriority(std::vector<NodeBase*>* list);
	// �����_���I��
	NodeBase* SelectRandom(std::vector<NodeBase*>* list);
	// �V�[�P���X�I��
	NodeBase* SelectSequence(std::vector<NodeBase*>& list, BehaviorData* data);


	// �m�[�h����(�����Ǝ����̑S�Ă̎q�̖��O���r���Č�������)
	NodeBase* SearchNode(std::string searchName);

	// �m�[�h���_(���̍s�������ɑI�ʂ���)
	NodeBase* Inference(BehaviorData* data);

	// ���s(�I�ʂ��ꂽ�s�������s�Aupdate�݂����Ȃ��)
	ActionBase::State Run(float elapsedTime);


	// �q�m�[�h�A�����̃m�[�h�ł͂Ȃ�����
	//�e�m�[�h�ɂ����̉��̊K�w�̃m�[�h�����B
	//(����̊K�w�����A�����܂ł͂Ȃ��B)
	std::vector<NodeBase*>		children;

protected:

	std::string					name;			// ���O
	BehaviorTree::SelectRule	selectRule;		// �I�����[��

	// ���肷��A����̓��e�B�p�����Ĕ���̓��e�����R�ɍ��B
	//nullptr�Ȃ画�肵�Ȃ��̂ő��I�����̌��ɂȂ�
	JudgmentBase* judgment;	    // ����N���X

	// ���ۂ̓����A����̓��e�B�p�����čs���̓��e�����R�ɍ��B
	//nullptr�Ȃ���ۂ̓����͂Ȃ��������ł͂Ȃ��͂�
	ActionBase* action;			// ���s�N���X

	unsigned int				priority;		// �D�揇��
	NodeBase* parent;			// �e�m�[�h
	NodeBase* sibling;		// �Z��m�[�h
	int							hierarchyNo;	// �K�w�ԍ�
};
