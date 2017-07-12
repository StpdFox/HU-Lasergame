#ifndef TEST_HPP
#define TEST_HPP

#include "rtos.hpp"

class test:public rtos::task<>{
public:
	void main(){};
	test();
	int i =1;
};
#endif