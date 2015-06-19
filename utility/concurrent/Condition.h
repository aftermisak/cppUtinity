#pragma  once
namespace utility{
	namespace concurrenct{

	class Condition
	{
	public:
		virtual ~Condition(){}
		/*
			阻塞等待，直到被唤醒
		*/
		virtual void wait() = 0;
		/*
			阻塞等待，直到被唤醒或者超时
			@param ms 等待时间，毫秒为单位
			@return 如果被唤醒，返回true，等待超时，返回false
		*/
		virtual bool wait(unsigned ms) = 0;
		
		/*
			随机唤醒一个该条件的等待
		*/
		virtual void notify() = 0;
		/*
			唤醒所有该条件的等待
		*/
		virtual void notifyAll() = 0 ;
	};
};
};
