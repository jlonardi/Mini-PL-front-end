#pragma once

#ifndef READ_BUFFER_H
#define READ_BUFFER_H

#include "Scanner/Buffer.h"
#include <list>

class ReadBuffer : public Buffer
{
	public:
		ReadBuffer(std::string& path);
		ReadBuffer(const ReadBuffer& other);
		char read();
		void push(char c);
		bool empty();
	private:
		std::list<char> m_buffer;
};

#endif