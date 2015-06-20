#pragma once
namespace utility
{
	/**
		将类标记为不可拷贝构造与复制操作
		使用方式，私有继承该类，example class Factory : private utility::UnCopyable

	*/
	class UnCopyable
	{
	protected:
		UnCopyable() = default;
		~UnCopyable() = default;
	public:
		UnCopyable(const UnCopyable&) = delete;
		UnCopyable& operator=(const UnCopyable&) = delete;
		
	//protected:
	//	UnCopyable() {}
	//	~UnCopyable(){}
	//private:
	//	UnCopyable( const UnCopyable& );
	//	UnCopyable& operator=( const UnCopyable& );
		
	};
};

