#pragma  once
namespace utility{
	namespace concurrenct{

	class Condition
	{
	public:
		virtual ~Condition(){}
		/*
			�����ȴ���ֱ��������
		*/
		virtual void wait() = 0;
		/*
			�����ȴ���ֱ�������ѻ��߳�ʱ
			@param ms �ȴ�ʱ�䣬����Ϊ��λ
			@return ��������ѣ�����true���ȴ���ʱ������false
		*/
		virtual bool wait(unsigned ms) = 0;
		
		/*
			�������һ���������ĵȴ�
		*/
		virtual void notify() = 0;
		/*
			�������и������ĵȴ�
		*/
		virtual void notifyAll() = 0 ;
	};
};
};
