#ifndef LOGGER_H
#define LOGGER_H
#include <ostream>

class logger
{
	
public:
	template<typename T>
	static void log(std::ostream& out, T obj, const std::string& name)
	{
		out << name;
		out << std::endl;
		out << obj;
		out << std::endl;
	}

};
#endif