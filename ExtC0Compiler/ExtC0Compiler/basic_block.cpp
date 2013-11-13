#include "basic_block.h"

unsigned int basic_block::instance_number = 0;

basic_block::basic_block()
{
	number = instance_number++;
}

void basic_block::add(quaternion* q)
{
	index_map[q->get_number()] = ins.size();
	ins.push_back(q);
}

std::vector<quaternion*> basic_block::get_sequential() const
{
	return ins;
}

void basic_block::set_entry(basic_block* block)
{
	entry.push_back(block);
}

void basic_block::set_exit(basic_block* block)
{
	exit.push_back(block);
}

void basic_block::flush()
{
	instance_number = 0;
}

quaternion* basic_block::operator[](unsigned int index)
{
	return ins[index_map[index]];
}

void operator<<(std::ostream& out, const basic_block& block)
{
	out << "Basic Block " << block.number << std::endl;
	out << "Entry:";
	for(unsigned int i = 0; i < block.entry.size(); i++)
		out << " " << block.entry[i]->number;
	out << std::endl;
	out << "Exit:";
	for(unsigned int i = 0; i < block.exit.size(); i++)
		out << " " << block.exit[i]->number;
	out << std::endl;
	for(unsigned int i = 0; i < block.ins.size(); i++)
		out << *block.ins[i];
	out << std::endl;
}

