#ifndef _COMPILER_BIT_SET
#define _COMPILER_BIT_SET

#include <vector>

class bit_set
{
public:
	bit_set operator+(const unsigned int&);
	bit_set operator+(const bit_set&);//并集
	bit_set& operator+=(const unsigned int&);
	bit_set& operator+=(const bit_set&);
	bit_set operator-(const unsigned int&);
	bit_set operator-(const bit_set&);//差集
	bit_set& operator-=(const unsigned int&);
	bit_set& operator-=(const bit_set&);
	bit_set operator&(const bit_set&);//交集
	bit_set& operator&=(const bit_set&);

	bool empty() const;
	bool find(unsigned int) const;
	std::vector<unsigned int> get_numbers() const;
	friend bool operator==(const bit_set&, const bit_set&);
	friend bool operator!=(const bit_set&, const bit_set&);
	friend void operator<<(std::ostream&, const bit_set&);
private:
	std::vector<unsigned int> members;
	const static unsigned int bits_per_byte;
	const static unsigned int bits_per_int;
};

#endif