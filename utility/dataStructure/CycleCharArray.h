#pragma  once

#include "../UnCopyable.h"
namespace utility
{
	namespace dataStructure
	{
		/**
		字节型循环数组
		*/
		class CycleCharArray : private UnCopyable
		{
		public:
			explicit CycleCharArray(unsigned size);
			~CycleCharArray();
			/*
			@param(out) buff 获取数据的内存区
			@param size  想获取的长度
			@return 成功返回 true，数据不足返回false
			*/
			bool get(char * buff, const unsigned  size) const;
			/*
			@param(out) buff 放入数据的内存区
			@param size 数据长度
			@return 如果成功加入，返回true， 若循环数组的容量不足则返回false
			*/
			bool put(const  char * buff, const unsigned size);

			//删除前面的size个数据，成功返回true，size不足返回false
			bool popFront(const unsigned size = 0);


			unsigned getDataSize()const{ return _dataSize; }
		private:

			unsigned _lastIndex; //下一个可以插入数据的index init 0
			char * _buffer; //装数据的内存区域
			unsigned _dataSize; //当前数据长度 init 0
			unsigned _bufferSize; //总容量
		};
	};// end of namespace dataStructure

};// end of namespace utility