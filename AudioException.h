/*
 *  AudioException.h
 */

#pragma once

#include <exception>
#include <string>

class AudioException : public std::exception
{
public:
	
	AudioException( const char *pWhat ) throw()
	:
	what_( pWhat )
	{
	}
	
	~AudioException() throw() {}
	
	const char* what() const throw()
	{
		return what_.c_str();
	}
	
private:
	
	std::string what_;
	
};
