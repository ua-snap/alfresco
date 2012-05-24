#ifndef ALF_ReadRasterException_H
#define ALF_ReadRasterException_H

#include <iostream>
#include <string>
#include <stdexcept>

class ReadRasterException : public std::exception
{
	public:
		ReadRasterException(std::string m="exception!") : msg(m) { std::cout << m << std::endl; }
		ReadRasterException(std::string m, const std::string fn) { std::cout << m << std::endl << fn << std::endl; }
		ReadRasterException(const ReadRasterException&) throw();
		ReadRasterException(const char& m, std::string fn) : msg(&m) { std::cout << m << std::endl << fn << std::endl; }
		ReadRasterException& operator= (const ReadRasterException&) throw();
	
		~ReadRasterException() throw() { std::cout << msg << std::endl; }
		ReadRasterException rethrow() { return ReadRasterException(msg); }
		const char* what() const throw() { std::cout << msg << std::endl; return msg.c_str(); }

	private:
		std::string msg;
};

#endif
