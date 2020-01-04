#ifndef __SHT21_HPP__
#define __SHT21_HPP__ 

class SHT21
{
public:
	SHT21() {}
	~SHT21() {}
	
	float temper();
	float humidity();
};

#endif