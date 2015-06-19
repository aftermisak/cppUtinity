#pragma  once

#include "../UnCopyable.h"
namespace utility
{
	namespace dataStructure
	{
		/**
		�ֽ���ѭ������
		*/
		class CycleCharArray : private UnCopyable
		{
		public:
			explicit CycleCharArray(unsigned size);
			~CycleCharArray();
			/*
			@param(out) buff ��ȡ���ݵ��ڴ���
			@param size  ���ȡ�ĳ���
			@return �ɹ����� true�����ݲ��㷵��false
			*/
			bool get(char * buff, const unsigned  size) const;
			/*
			@param(out) buff �������ݵ��ڴ���
			@param size ���ݳ���
			@return ����ɹ����룬����true�� ��ѭ����������������򷵻�false
			*/
			bool put(const  char * buff, const unsigned size);

			//ɾ��ǰ���size�����ݣ��ɹ�����true��size���㷵��false
			bool popFront(const unsigned size = 0);


			unsigned getDataSize()const{ return _dataSize; }
		private:

			unsigned _lastIndex; //��һ�����Բ������ݵ�index init 0
			char * _buffer; //װ���ݵ��ڴ�����
			unsigned _dataSize; //��ǰ���ݳ��� init 0
			unsigned _bufferSize; //������
		};
	};// end of namespace dataStructure

};// end of namespace utility