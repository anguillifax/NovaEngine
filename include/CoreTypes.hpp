#pragma once

#include <exception>

namespace Nova {

	class NotImplementedException : public std::exception {
	public:
		explicit NotImplementedException();
	};

}