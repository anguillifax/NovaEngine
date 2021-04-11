#include <functional>
#include <iostream>

#include "Sequencer.hpp"

namespace DriverTesting {

	template <typename T>
	void Print(const T& obj) {
		std::cout << obj << '\n';
	}

	template <typename T>
	void Print(const char* msg, const T& obj) {
		std::cout << msg << ": " << obj << '\n';
	}

	void Print(const char* msg) {
		std::cout << msg << '\n';
	}

	class Tester {
	public:
		void MyFunc() {
			std::cout << "MyFunc()\n";
		}
	};

} // namespace DriverTesting

namespace DriverTesting {
	void Main() {

		Nova::OneTimeCallable<int> c{};

		c.Bind([](int i) {
			std::cout << "Lambda with " << i << "\n";
			Tester{}.MyFunc();
		});

		c.Call(5);
		c.Call(3);
		Print("Reset");
		c.Reset();
		c.Call(8);
	}
} // namespace DriverTesting

int main(int argc, char** argv) {
	DriverTesting::Main();
}