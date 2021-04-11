#include <bitset>
#include <climits>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>

namespace DriverTesting {

	template <class T>
	void Print(const T& t) {
		std::cout << t << '\n';
	}

	inline void Print(const char* msg) {
		std::cout << msg << '\n';
	}

	inline void Print() {
		std::cout << '\n';
	}

	inline void PrintDivider() {
		std::cout << "========================================\n";
	}

	template <class T>
	void Print(const char* prefix, const T& obj) {
		std::cout << prefix << ": " << obj << '\n';
	}

	template <class T>
	void Print() {
		std::cout << typeid(T).name() << '\n';
	}

	template <class T, bool InsertSeparators = true, std::size_t Width = sizeof(T) * CHAR_BIT>
	std::string AsBinary(const T& t) {
		std::string ret = (std::ostringstream{} << std::bitset<Width>(t)).str();
		if constexpr (InsertSeparators) {
			const size_t upperBound = (ret.size() + 3) / 4;
			for (std::size_t i = 0; i < upperBound; i++) {
				ret.insert(ret.end() - i * 4 - i, ' ');
			}
		}
		return ret;
	}

} // namespace Testing