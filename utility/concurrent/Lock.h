#pragma  once
#include <memory>
using namespace std;
#include "Condition.h"
namespace utility{
	namespace concurrenct{
		class Lock
		{
		public:
			virtual ~Lock(){}
			/*
				������ֱ����ȡ����
			*/
			virtual void lock() = 0;
			/*
				������ֱ����ȡ�����߳�ʱ
				@param ms �ȴ�ʱ�䣬����Ϊ��λ
				@return  �ɹ���ȡ������ true, ��ʱ����false
			*/
			virtual bool lock( unsigned ms ) = 0;
			/*
				����,���ڽ��Look::look��ȡ������
			*/
			virtual void unlock() = 0;

			/*
				����һ���͵�ǰ����ص���������
			*/
			virtual shared_ptr<Condition> newSharedPtrCondition() = 0;

			/*
				����һ���͵�ǰ����ص�����������
			��������ڴ档
			*/
			virtual Condition* newCondition() = 0;
		};

		/*
			��д��,
			ͬһ���ö����д�����ų������д��
			ͬһ���ö���Ķ������ų�д�������ǲ����ų����
			ps: ���ڶ�����д��������Ϊ��д�����ڲ����ݣ��ڶ��߳�����£�����ָ��Ŀ����ǲ���ȫ�ģ�����
		���ﷵ�ص���ԭʼָ��,����get������Ҫ��֤ʵ�ֵ���ȷ��
		*/
		class ReadWriteLock
		{
		public:
			virtual Lock* getReadLock() = 0;
			virtual Lock* getWriteLock() = 0;
			virtual ~ReadWriteLock(){}
		};
};
};
