#include "bit_set.h"
#include <iostream>
#include <cassert>

#define _NO_WASTE_SPACE(arg) ((arg).members.size() <= 1 || (arg).members.back() != 0)
const unsigned int bit_set::bits_per_byte = 8;
const unsigned int bit_set::bits_per_int = bit_set::bits_per_byte * sizeof(unsigned int);

bit_set bit_set::operator+(const unsigned int& element)
{
	bit_set result(*this);
	result += element;
	return result;
}

bit_set bit_set::operator+(const bit_set& oper)
{
	bit_set result(*this);
	result += oper;
	return result;
}

bit_set& bit_set::operator+=(const unsigned int& element)
{
	unsigned int index = element / bits_per_int;
	unsigned int bit = element % bits_per_int;

	if(index >= members.size())
	{
		members.resize(index + 1);
	}
	members[index] |= (1 << bit);
	assert(_NO_WASTE_SPACE((*this)));
	return *this;
}

bit_set& bit_set::operator+=(const bit_set& oper)
{
	unsigned int lower_bound = members.size() < oper.members.size() ? members.size() : oper.members.size();
	unsigned int upper_bound = members.size() > oper.members.size() ? members.size() : oper.members.size();

	members.resize(upper_bound);
	for(unsigned int i = 0; i < lower_bound; i++)
	{
		members[i] |= oper.members[i];
	}
	for(unsigned int i = lower_bound; i < oper.members.size(); i++)
	{
		members[i] = oper.members[i];
	}
	assert(_NO_WASTE_SPACE(*this));
	return *this;
}

bit_set bit_set::operator-(const unsigned int& element)
{
	bit_set result(*this);
	result -= element;
	return result;
}

bit_set& bit_set::operator-=(const unsigned int& element)
{
	unsigned int index = element / bits_per_int;
	unsigned int bit = element % bits_per_int;
	if(index < members.size())
	{
		members[index] &= (~(1 << bit));
	}
	for(unsigned int i = members.size(); i > 0; i--)
	{
		if(members[i - 1] || i == 1)
		{
			members.resize(i);
			break;
		}
	}
	assert(_NO_WASTE_SPACE(*this));
	return *this;
}

bit_set bit_set::operator-(const bit_set& oper)
{
	bit_set result(*this);
	result -= oper;
	return result;
}

bit_set& bit_set::operator-=(const bit_set& oper)
{
	unsigned int lower_bound = members.size() < oper.members.size() ? members.size() : oper.members.size();

	for(unsigned int i = 0; i < lower_bound; i++)
	{
		members[i] &= (~oper.members[i]);
	}
	for(unsigned int i = members.size(); i > 0; i--)
	{
		if(members[i - 1] || i == 1)
		{
			members.resize(i);
			break;
		}
	}
	assert(_NO_WASTE_SPACE(*this));
	return *this;
}

bit_set bit_set::operator&(const bit_set& oper)
{
	bit_set result(*this);
	result &= oper;
	return result;
}

bit_set& bit_set::operator&=(const bit_set& oper)
{
	unsigned int lower_bound = members.size() < oper.members.size() ? members.size() : oper.members.size();

	members.resize(lower_bound);
	for(unsigned int i = 0; i < lower_bound; i++)
	{
		members[i] &= oper.members[i];
	}
	for(unsigned int i = members.size(); i > 0; i--)
	{
		if(members[i - 1] || i == 1)
		{
			members.resize(i);
			break;
		}
	}
	assert(_NO_WASTE_SPACE(*this));
	return *this;
}

bool bit_set::empty() const
{
	assert(_NO_WASTE_SPACE(*this));
	if(members.size() == 0 || (members.size() == 1 && members[0] == 0))
		return true;
	else
		return false;
}

bool bit_set::find(unsigned int element) const
{
	assert(_NO_WASTE_SPACE(*this));
	unsigned int index = element / bits_per_int;
	unsigned int bit = element % bits_per_int;
	if(index >= members.size())
	{
		return false;
	}
	else 
	{
		if(members[index] & (1 << bit))
			return true;
		else
			return false;
	}
}

std::vector<unsigned int> bit_set::get_numbers() const
{
	unsigned int number = 0;
	std::vector<unsigned int> numbers;
	for(unsigned int i = 0; i < members.size(); i++)
	{
		for(unsigned int j = 0; j < bits_per_int; j++)
		{
			if(members[i] & (1 << j))
			{
				numbers.push_back(number + j);
			}
		}
		number += bits_per_int;
	}
	return numbers;
}

bool operator==(const bit_set& oper1, const bit_set& oper2)
{
	if(oper1.members.size() != oper2.members.size())return false;
	assert(_NO_WASTE_SPACE(oper1));
	assert(_NO_WASTE_SPACE(oper2));
	for(unsigned int i = 0; i < oper1.members.size(); i++)
	{
		if(oper1.members[i] != oper2.members[i])
			return false;
	}
	return true;
}

bool operator!=(const bit_set& oper1, const bit_set& oper2)
{
	return !(oper1 == oper2);
}

void operator<<(std::ostream& out, const bit_set& set)
{
	unsigned int number = 0;
	bool first_element = true;
	out << "{";
	for(unsigned int i = 0; i < set.members.size(); i++)
	{
		for(unsigned int j = 0; j < bit_set::bits_per_int; j++)
		{
			if(set.members[i] & (1 << j))
			{
				if(!first_element)
					out << ",";
				else
					first_element = false;
				out << (number + j);
			}
		}
		number += bit_set::bits_per_int;
	}
	out << "}" << std::endl;
}
