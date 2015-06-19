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

	//�㿪ʼ�±�
	int beginIndex = _lastIndex - _dataSize;
	if (beginIndex < 0)
	{
		beginIndex = _bufferSize + beginIndex;
	}
	//ȡ����
	int indexForCpy = beginIndex;
	for (unsigned i = 0; i < size; i++)
	{
		buff[i] = _buffer[indexForCpy++];
		if (indexForCpy == _bufferSize)//Խ��
		{
			indexForCpy = 0;
		}
	}
	//����
	return true;

}
bool CycleCharArray::put(const  char * buff, const unsigned size)
{
	unsigned remainSize = _bufferSize - _dataSize;
	if (remainSize < size) //��������
	{
		return false;
	}

	//size��¼
	_dataSize += size;

	//������
	if (_bufferSize - _lastIndex >= size)  //_lastIndex�Ժ�������㹻��������
	{
		std::memcpy(_buffer + _lastIndex, buff, size);
		_lastIndex += size;
		assert(_lastIndex <= _bufferSize); //��������
		if (_lastIndex == _bufferSize)//���沿�ָպ�װ��
		{
			_lastIndex = 0;
		}
	}
	else//_lastIndex�Ժ�����򲻹�������
	{
		unsigned firstAreaSize = _bufferSize - _lastIndex;
		unsigned secondAreaSize = size - firstAreaSize;
		std::memcpy(_buffer + _lastIndex, buff, firstAreaSize);
		std::memcpy(_buffer, buff + firstAreaSize, secondAreaSize);
		_lastIndex = secondAreaSize;
	}
	//����
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