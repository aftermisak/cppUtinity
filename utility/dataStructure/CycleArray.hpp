#pragma  once
#include "../UnCopyable.h"
#include <cassert>

namespace utility
{
	namespace dataStructure
	{	
		/*
			循环数组
		*/
		template <class T> class CycleArray : private UnCopyable
		{
		public:
			explicit CycleArray(unsigned size);
			~CycleArray();
			/*
			@param(out) buff 获取数据的内存区
			@param size  想获取的长度
			@return 成功返回 true，数据不足返回false
			*/
			bool get(T * buff, const unsigned  size) const;
			/*
			@param(out) buff 放入数据的内存区
			@param size 数据长度
			@return 如果成功加入，返回true， 若循环数组的容量不足则返回false
			*/
			bool put(const  T * buff, const unsigned size);

			//删除前面的size个数据，成功返回true，size不足返回false
			bool popFront(const unsigned size = 0);


			unsigned getDataSize()const{ return _dataSize; }
		private:

			unsigned _lastIndex; //下一个可以插入数据的index init 0
			T * _buffer; //装数据的内存区域
			unsigned _dataSize; //当前数据长度 init 0
			unsigned _bufferSize; //总容量
		};
		template <class T>
		CycleArray<T>::CycleArray(unsigned size) :
			_bufferSize(size),
			_dataSize(0),
			_lastIndex(0)
		{
			this->_buffer = new T[_bufferSize];
		}
		template <class T>
		CycleArray<T>::~CycleArray()
		{
			delete[] this->_buffer;
		}
		template <class T>
		bool CycleArray<T>::get(T * buff, const unsigned  size) const
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
		template <class T>
		bool CycleArray<T>::put(const  T * buff, const unsigned size)
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
				std::memcpy(_buffer + _lastIndex, buff, size * sizeof(T));
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
				std::memcpy(_buffer + _lastIndex, buff, firstAreaSize * sizeof(T));
				std::memcpy(_buffer, buff + firstAreaSize, secondAreaSize * sizeof(T));
				_lastIndex = secondAreaSize;
			}
			//返回
			return true;
		}
		template <class T>
		bool CycleArray<T>::popFront(const unsigned size)
		{
			if (size > _dataSize)
			{
				return false;
			}
			_dataSize -= size;
			return true;
		}

		
	};// end of namespace dataStructure

};// end of namespace utility