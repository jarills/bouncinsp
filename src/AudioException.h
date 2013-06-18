/*

    Bouncin' SP copyright jarills 2013

    This file is part of Bouncin' SP.

    Bouncin' SP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Bouncin' SP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Bouncin' SP.  If not, see <http://www.gnu.org/licenses/>.

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
