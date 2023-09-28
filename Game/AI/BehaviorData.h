// ReSharper disable All
#pragma once

#include <vector>
#include <stack>
#include <map>
#include <string>

class NodeBase;



// Behavior�ۑ��f�[�^
class BehaviorData
{
public:
	// �R���X�g���N�^
	BehaviorData() { Init(); }
	// �V�[�P���X�m�[�h�̃v�b�V��
	void PushSequenceNode(NodeBase* node) { sequenceStack.push(node); }
	// �V�[�P���X�m�[�h�̃|�b�v
	NodeBase* PopSequenceNode();
	// �V�[�P���X�X�e�b�v�̃Q�b�^�[
	int GetSequenceStep(std::string name);
	// �V�[�P���X�X�e�b�v�̃Z�b�^�[
	void SetSequenceStep(std::string name, int step);
	// ������
	void Init();

	// �V�[�P���X������s�\���X�g�擾
	const std::vector<NodeBase*>& GetSequenceList() { return sequence_list; }
private:

	/* �V�[�P���X������s�\���X�g
	 *���ނ̂����̓V�[�P���X�Ŏ��̍s���Ɉڍs����Ƃ��ɖ���
	 *���_���Ă��܂�����A�V�[�P���X�Ƃ킩�����Ȃ���ڂ�
	 *���s�\���X�g�ɓ������A�N�V������ێ�����Failed�ɂȂ�Ȃ�����
	 *���ׂĎ��s����`�ɂ���B
	 *(�����͋��ނ̃p�^�[���Ǝg�������ł���悤�ɂȂ��Ă����������A
	 *�g���@��邩�킩��񂯂�)
	 */
	std::vector<NodeBase*> sequence_list{};

	// �}�����ꂽ������v�f�����o��(�{��ςރC���[�W)
	// �V�[�P���X���s���Ă钆�ԃm�[�h���X�^�b�N(�e�{�l)
	std::stack<NodeBase*> sequenceStack;

	// ���s���̒��ԃm�[�h�̃X�e�b�v���L�^
	std::map<std::string, int> runSequenceStepMap;
};
