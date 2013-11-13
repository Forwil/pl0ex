#include "function_block.h"
#include <cassert>
unsigned int function_block::instance_number = 0;

function_block::function_block()
{
	new_basic_block();
	new_block = false;
	following_label = false;
	is_end = false;
	former_block = NULL;
	number = instance_number++;
}

void function_block::new_basic_block()
{
	basic_block* new_block = new basic_block();
	blocks.push_back(new_block);
}

void function_block::flow(basic_block* entry, basic_block* exit)
{
	if(entry != NULL && exit != NULL)
	{
		entry->set_exit(exit);
		exit->set_entry(entry);
	}
}

void function_block::flow()
{
	for(std::map<basic_block*, symbol*>::iterator entry_block = block_exit.begin(); entry_block != block_exit.end(); entry_block++)
	{
		symbol* label = entry_block->second;
		std::map<symbol*, basic_block*>::iterator exit_block = block_entry.find(label);
		assert(exit_block != block_entry.end());
		flow(entry_block->first, exit_block->second);
	}
	for(unsigned int i = 0; i < return_blocks.size(); i++)
	{
		flow(return_blocks[i], blocks.back());
	}
}

void function_block::add(quaternion* q)
{
	assert(!is_end);
	index_map[q->get_number()] = sequential.size();
	sequential.push_back(q);
	if(new_block && q->get_operation() != quaternion::PARAM && q->get_operation() != quaternion::LABEL && q->get_operation() != quaternion::ENDF)
	{
		new_basic_block();
		new_block = false;
		flow(former_block, blocks.back());
	}
	switch(q->get_operation())
	{
	case quaternion::PARAM:
		new_block = true;
		following_label = false;
		former_block = this->blocks.back();
		break;
	case quaternion::ENDF:
		if(!following_label)
		{
			new_basic_block();
			flow(former_block, blocks.back());
		}
		is_end = true;
		break;
	case quaternion::LABEL:
		//LABEL���û�и���һ��LABEL֮����Ϊһ��������Ŀ�ʼ
		if(!following_label)
		{
			new_basic_block();
			//���������һ��������䣬�����֮ǰ��䲻��LABEL
			following_label = true;
			flow(former_block, blocks.back());
		}
		block_entry[q->get_arg(0)] = this->blocks.back();//һ���������п��ܳ��ֶ��label
		new_block = false;
		break;
	case quaternion::RET:	
		//��ת�ͷ��������һ��������Ľ�������һ���������һ��������Ŀ�ʼ
		new_block = true;
		following_label = false;
		former_block = NULL;
		return_blocks.push_back(this->blocks.back());
		break;
	case quaternion::JMP:
		block_exit[this->blocks.back()] = q->get_arg(0);
		new_block = true;
		following_label = false;
		former_block = NULL;
		break;
	case quaternion::JGT:
	case quaternion::JGE:
	case quaternion::JLT:
	case quaternion::JLE:
	case quaternion::JEQ:
	case quaternion::JNE:
	case quaternion::FJGT:
	case quaternion::FJGE:
	case quaternion::FJLT:
	case quaternion::FJLE:
	case quaternion::FJEQ:
	case quaternion::FJNE:
		block_exit[blocks.back()] = q->get_arg(2);
		new_block = true;
		following_label = false;
		former_block = this->blocks.back();
		break;
	default:
		following_label = false;
		former_block = this->blocks.back();
	}
	blocks.back()->add(q);
	if(is_end)flow();
}

std::vector<basic_block*> function_block::get_basic_blocks() const
{
	return this->blocks;
}

std::vector<quaternion*> function_block::get_sequential() const
{
	return this->sequential;
}

void function_block::flush()
{
	instance_number = 0;
}

quaternion* function_block::operator[](unsigned int index)
{
	return sequential[index_map[index]];
}

void operator<<(std::ostream& out, const function_block& block)
{
	out << "Function Block " << block.number << std::endl;
	for(unsigned int i = 0; i < block.blocks.size(); i++)
		out << *block.blocks[i];
	out << std::endl;
}
