#pragma once

#ifndef BUFFER_H
#define BUFFER_H

class Buffer
{
public:
	virtual ~Buffer() {};
	virtual char read() = 0;
	virtual void push(char c) = 0;
	virtual bool empty() = 0;
};

#endif