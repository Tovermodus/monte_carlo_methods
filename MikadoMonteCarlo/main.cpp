#include <iostream>
#include <vector>
#include <functional>
#include <ctime>
class A{
    public:
	double x;
	void change()
	{
		x = 99;
	}
	A(double xx): x(xx){};

};

std::ostream &operator<<(std::ostream &os, A const &m) {
	return os << m.x;
}
int main ()
{
	std::cout << "Hello, World!" << std::endl;
	A a1(1);
	A a2(3);
	A a3(5);
	std::vector<A> nums{a1,a2,a3};
	std::vector<std::reference_wrapper<A>> nums2(nums.begin(),nums.end());
	//nums2[0] = A(7);
	A a4(7);
	nums2[0].get().change();
	nums2[0] = std::ref(a4);
	for (auto const& c : nums)
		std::cout << c << ' ';
	std::cout <<"\n";
	for (auto const& c : nums2)
		std::cout << c << ' ';
	std::cout <<"\n";

	nums = std::vector<A>(nums2.begin(),nums2.end());
	nums2[0].get().change();

	for (auto const& c : nums)
		std::cout << c << ' ';
	std::cout <<"\n";
	for (auto const& c : nums2)
		std::cout << c << ' ';
	std::cout <<"\n";
	return 0;
}
