#ifndef ALF_ReadRasterException_H
#define ALF_ReadRasterException_H

#include <iostream>
#include <string>
#include <stdexcept>

class MyException : public std::exception
{
	public:
		MyException(std::string m="exception!") : msg(m) { std::cout << m << std::endl; }
		MyException(std::string m, const std::string fn) { std::cout << m << std::endl << fn << std::endl; }
		MyException(const MyException&) throw();
		MyException(const char& m, std::string fn) : msg(&m) { std::cout << m << std::endl << fn << std::endl; }
		MyException& operator= (const MyException&) throw();
	
		~MyException() throw() { std::cout << msg << std::endl; }
		MyException rethrow() { return MyException(msg); }
		const char* what() const throw() { std::cout << msg << std::endl; return msg.c_str(); }

	private:
		std::string msg;
};

#endif
