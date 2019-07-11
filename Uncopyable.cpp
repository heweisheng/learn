#include <iostream>
#include<string>
class Uncopyable {
protected:
	Uncopyable() {};
	~Uncopyable() {};
private:
	Uncopyable(const Uncopyable&);
	Uncopyable& operator=(const Uncopyable&);
};
class A:private Uncopyable {
private:
	int x;
public:
	A(int &value) :x(value) {};
	int get()
	{
		return x;
	}
};
int main()
{
	int x = 5;
	A test(x);
	A test2(x);
	//test = test2;
}
