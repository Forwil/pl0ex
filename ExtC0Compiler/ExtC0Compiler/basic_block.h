#ifndef _COMPILER_BASIC_BLOCK
#define _COMPILER_BASIC_BLOCK

#include "quaternion.h"
#include <vector>
#include <iostream>

/*
	������
	һ�����������ҽ���һ����ں�һ�����ڣ����е�ָ��˳��ִ��
	���Ҫô��LABEL��Ҫô��FUN��Ҫô�Ƿ���������ָ���ת��ָ�RETָ��֮���һ��ָ��
	����Ҫô��ת��ָ�RETָ�Ҫô��ENDF

	������ͼʱ��ͨ��ת��ָ���LABEL֮���ӳ�䣬�����𲿷������
	�����������ͼһ���������ڵ������������֮��˳����ͨ��

	���֮ǰ�Ļ�������JMP��RET��ֹ������һ�����ڵĻ������Ƿ����ǰ������ȷ��
*/
class basic_block
{
public:
	friend class dag;
	friend class data_stream;
	friend class nets;
	friend class live_variable;
	friend class reach_definition;
	friend class conflict_graph;

	basic_block();
	/*
		������������һ����Ԫʽ
	*/
	void add(quaternion* q);

	std::vector<quaternion*> get_sequential() const;
	/*
		���û���������
	*/
	void set_entry(basic_block* block);
	/*
		���û�����ĳ���
	*/
	void set_exit(basic_block* block);
	/*
		��ʼ�������飬�������ȵ�
	*/
	static void flush();

	quaternion* operator[](unsigned int);
	friend void operator<<(std::ostream&, const basic_block&);
private:
	std::vector<quaternion*> ins;
	std::vector<basic_block*> entry;
	std::vector<basic_block*> exit;
	std::map<unsigned int, unsigned int> index_map;
	unsigned int number;

	static unsigned int instance_number;

};

#endif