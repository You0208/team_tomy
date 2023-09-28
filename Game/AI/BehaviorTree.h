// ReSharper disable All
#pragma once
#include <string>

#include "BehaviorData.h"

	class NodeBase;
	class BehaviorData;
	class ActionBase;
	class JudgmentBase;

class Enemy;

// �r�w�C�r�A�c���[
//���ꂪ�r�w�C�r�A�x�[�X�̑S�Ă̓���
class BehaviorTree
{
public:
	/*
	 * ���̃r�w�C�r�A�c���[�̑g�ݕ���ƁA�V�[�P���X�͒��ԃm�[�h���S�Ď��s�\�m�[�h��
	 * �o�^����ĂȂ��Ƃ��܂����_����Ȃ�(SelectSequence�֐���for��������)
	 * �m�[�h���V�[�P���X�ɂ���Ƃ��͑S�Ă̎q�m�[�h����{�I�ɐ�Βʂ�悤�ɂ���(Failed�A���s���s�͑��v)
	 * ������������Priority���g�����B���܂���낤�B
	 */

	 // �I�����[��
	enum class SelectRule
	{
		Non,				// ����(���[�m�[�h�p)
		Priority,			// �D�揇��
		Sequence,			// �V�[�P���X
		SequentialLooping,	// �V�[�P���V�������[�s���O
		Random,				// �����_��
	};

public:
	BehaviorTree() : root(nullptr), owner(nullptr) {}

	BehaviorTree(Enemy* enemy) : root(nullptr), owner(enemy) {}

	~BehaviorTree();

	// ���s�m�[�h�𐄘_����
	NodeBase* ActiveNodeInference(BehaviorData* data);

	// �V�[�P���X�m�[�h���琄�_�J�n
	NodeBase* SequenceBack(NodeBase* sequenceNode, BehaviorData* data);

	// �m�[�h�ǉ�
	// �����̏���
	// �e���O�A���[�g�m�[�h�̏ꍇ�͂ǂ�������q�����Ă��Ă��Ȃ��̂ŋ󕶎�������B
	// �����̖��O�A�d��������o�O��B
	// �D�揇�ʁA�l���Ⴂ���̂��D�悳���B
	// �I���A���S���Y���A�e���[����enum class SelectRule �Œ�`����Ă�B
	// ���肷�邩�A�o�^����ۂ� new �ŗ~��������N���X�𐶐�����B
	// �s�������邩�B���[�g�m�[�h�⒆�ԃm�[�h�̏ꍇ�͍s���������̂�nullptr ���w�肷��B���蓯�l new �Ő����B
	void AddNode(std::string parentName, std::string entryName, int priority, SelectRule selectRule, JudgmentBase* judgment, ActionBase* action);


	// ���s
	NodeBase* Run(NodeBase* actionNode, BehaviorData* data, float elapsedTime);
	void Update();
private:
	// �m�[�h�S�폜
	void NodeAllClear(NodeBase* delNode);
private:
	// ���[�g�m�[�h(�c���[�̑匳)
	NodeBase* root;

	// ��ɃV�[�N�G���X�̍ۂɎg�p����B���ԃm�[�h�Ƃ��̒��ԃm�[�h�̃X�e�b�v���Ȃǂ�ێ�����B
	BehaviorData* behaviorData = new BehaviorData;

	// �I�����ꂽ���[�m�[�h(���ۂ̓���)������B
	//��ɉ��������ĂȂ��ƑS�����삵�Ȃ����ƂɂȂ�̂Ńo�O��B
	NodeBase* active_node;

	// todo �e���v���[�g��
	// �r�w�C�r�A�c���[�̎g�p��(�^��Character�ɂ��ׂ��H)
	Enemy* owner;
};
