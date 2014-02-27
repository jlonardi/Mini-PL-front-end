#include "Scanner/ReadBuffer.h"
#include <fstream>
#include <iostream>

/*
Reads the whole source file in memory. Memory should not be an
issue unlike keeping the file open a "long" time.
*/
ReadBuffer::ReadBuffer(std::string& path)
{
	std::ifstream is(path);     // open file
	if(is.fail())
	{
		std::cout << "Error when opening source file." << std::endl;
	}

	while (is.good())          // loop while extraction from file is possible
	{
		char c = is.get();       // get character from file
		if (is.good())
		{
			m_buffer.push_back(c);
		}
	}
}

ReadBuffer::ReadBuffer(const ReadBuffer& other)
{
	m_buffer = other.m_buffer;
}

char ReadBuffer::read()
{
	char c = m_buffer.front();
	m_buffer.pop_front();
	return c;
}

void ReadBuffer::push(char c)
{
	m_buffer.push_front(c);
}

bool ReadBuffer::empty()
{
	return m_buffer.empty();
}