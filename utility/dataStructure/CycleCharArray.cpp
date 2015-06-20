#include "CycleCharArray.h"
#include <cstring>
#include <cassert>
using namespace utility::dataStructure;
CycleCharArray::CycleCharArray(unsigned size) :
_bufferSize(size),
_dataSize(0),
_lastIndex(0)
{
	this->_buffer = new char[_bufferSize];
}
CycleCharArray::~CycleCharArray()
{
	delete[] this->_buffer;
}

bool CycleCharArray::get(char * buff, const unsigned  size) const
{
	if (size > this->_dataSize)
	{
		return false;
	}

	//算开始下标
	int beginIndex = _lastIndex - _dataSize;
	if (beginIndex < 0)
	{
		beginIndex = _bufferSize + beginIndex;
	}
	//取数据
	int indexForCpy = beginIndex;
	for (unsigned i = 0; i < size; i++)
	{
		buff[i] = _buffer[indexForCpy++];
		if (indexForCpy == _bufferSize)//越界
		{
			indexForCpy = 0;
		}
	}
	//返回
	return true;

}
bool CycleCharArray::put(const  char * buff, const unsigned size)
{
	unsigned remainSize = _bufferSize - _dataSize;
	if (remainSize < size) //容量不足
	{
		return false;
	}

	//size记录
	_dataSize += size;

	//存数据
	if (_bufferSize - _lastIndex >= size)  //_lastIndex以后的区域足够放入数据
	{
		std::memcpy(_buffer + _lastIndex, buff, size);
		_lastIndex += size;
		assert(_lastIndex <= _bufferSize); //必须满足
		if (_lastIndex == _bufferSize)//后面部分刚好装满
		{
			_lastIndex = 0;
		}
	}
	else//_lastIndex以后的区域不够放数据
	{
		unsigned firstAreaSize = _bufferSize - _lastIndex;
		unsigned secondAreaSize = size - firstAreaSize;
		std::memcpy(_buffer + _lastIndex, buff, firstAreaSize);
		std::memcpy(_buffer, buff + firstAreaSize, secondAreaSize);
		_lastIndex = secondAreaSize;
	}
	//返回
	return true;
}

bool CycleCharArray::popFront(const unsigned size)
{
	if (size > _dataSize)
	{
		return false;
	}
	_dataSize -= size;
	return true;
}