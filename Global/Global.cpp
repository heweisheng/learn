#include <iostream>
#include "Global.h"
Global::Global()
{
	SetData();
}


Global::~Global()
{
}
/*
 *使用local static代替class static 是因为.h文件中的定义初始化可能会导致引用的编译单元都编译了
 *static的初始化这样编译器可能会报错（如果你没有初始化就使用也会报错）,因为编译过程无法确定哪个
 *单元变量优先实例化了，特别是在一个编译单元的全局域中对一个class进行初始化操作，这样更容易产生bug
 */
void Global::SetData(Data *User) {
	static Data bak;
	if (NULL != User) {
		/*优先实例化而不使用指针赋值主要是因为作用域结束，栈空间值可能会无效，指针指向局部对象会发生奇妙的事情*/
		bak = *User;
	}
	/*由于local static的生命周期是整个项目，所以这里可以返回地址*/
	this->point = &bak;
}
Data Global::GetDate() {
	/*上面已经解释了*/
	return *this->point;
}
void function() {
	Global dGet;
	std::cout << dGet.GetDate().x << std::endl;
	Data nData;
	nData.x = 200;
	dGet.SetData(&nData);
}
int main()
{
	Global dSet;
	Data local;
	local.x = 100;
	dSet.SetData(&local);
	function();
	std::cout << dSet.GetDate().x << std::endl;
   // std::cout << "Hello World!\n"; 
}
