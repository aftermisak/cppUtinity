#pragma  once
#include "../UnCopyable.h"
#include <cassert>

namespace utility
{
	namespace dataStructure
	{	
		/*
			ѭ������
		*/
		template <class T> class CycleArray : private UnCopyable
		{
		public:
			explicit CycleArray(unsigned size);
			~CycleArray();
			/*
			@param(out) buff ��ȡ���ݵ��ڴ���
			@param size  ���ȡ�ĳ���
			@return �ɹ����� true�����ݲ��㷵��false
			*/
			bool get(T * buff, const unsigned  size) const;
			/*
			@param(out) buff �������ݵ��ڴ���
			@param size ���ݳ���
			@return ����ɹ����룬����true�� ��ѭ����������������򷵻�false
			*/
			bool put(const  T * buff, const unsigned size);

			//ɾ��ǰ���size�����ݣ��ɹ�����true��size���㷵��false
			bool popFront(const unsigned size = 0);


			unsigned getDataSize()const{ return _dataSize; }
		private:

			unsigned _lastIndex; //��һ�����Բ������ݵ�index init 0
			T * _buffer; //װ���ݵ��ڴ�����
			unsigned _dataSize; //��ǰ���ݳ��� init 0
			unsigned _bufferSize; //������
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
		template <class T>
		bool CycleArray<T>::put(const  T * buff, const unsigned size)
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
				std::memcpy(_buffer + _lastIndex, buff, size * sizeof(T));
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
				std::memcpy(_buffer + _lastIndex, buff, firstAreaSize * sizeof(T));
				std::memcpy(_buffer, buff + firstAreaSize, secondAreaSize * sizeof(T));
				_lastIndex = secondAreaSize;
			}
			//����
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