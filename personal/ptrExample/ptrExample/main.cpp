#include <iostream>
#include <memory>
#include <windows.h>
using namespace std;

class classA{
private:
	int* arr;
	static const int k = 10000000;
public:
	classA() : arr(new int[k]) {
		LARGE_INTEGER start, end, f;
		QueryPerformanceFrequency(&f);
		QueryPerformanceCounter(&start);

		for (int i = 0; i < k; ++i)
		{
			arr[i] = i;
		}

		QueryPerformanceCounter(&end);
		double result = (end.QuadPart - start.QuadPart) / (double)f.QuadPart;

		cout << "classA constructor called, " << result << " passed" << endl;
	}
	~classA(){
		delete[] arr;
		cout << "classA destructor called" << endl;
	}

	//copy constructor
	classA(const classA& Other) : arr(new int[k]){
		LARGE_INTEGER start, end, f;
		QueryPerformanceFrequency(&f);
		QueryPerformanceCounter(&start);

		for (int i = 0; i < k; ++i)
		{
			arr[i] = Other.arr[i];
		}

		QueryPerformanceCounter(&end);
		double result = (end.QuadPart - start.QuadPart) / (double)f.QuadPart;
		cout << "classA copy constructor called, " << result << " passed" << endl;
	}

	//move constructor
	classA(classA&& Other) : arr(Other.arr){
		LARGE_INTEGER start, end, f;
		QueryPerformanceFrequency(&f);
		QueryPerformanceCounter(&start);

		//기존 것은 null로 만들어 줘라
		Other.arr = nullptr;

		QueryPerformanceCounter(&end);
		double result = (end.QuadPart - start.QuadPart) / (double)f.QuadPart;
		cout << "classA move constructor called, " << result << " passed" << endl;
	}
};

class classB{
private:
	int m_a;
public:
	classB(){
		m_a = 10;
		cout << "classB constructor called" << endl;
	}
	~classB(){
		cout << "classB destructor called" << endl;
	}
	void showMember(){
		cout << m_a << endl;
	}
};

int main(){
	std::shared_ptr<classA> A1 = make_shared<classA>();
	std::shared_ptr<classA> B1(A1);

	{
		classA Ains1;
		classA Ains2(Ains1);
		classA Ains3(std::move(Ains1));		//굉장히 빠른 시간에 수행됨
	}

	std::unique_ptr<classB> C1 = make_unique<classB>();
	//std::move()는 argument를 rvalue로 형변환시킨다. (class&&)
	std::unique_ptr<classB> D1(std::move(C1));
	return 0;
}