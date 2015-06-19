#pragma once
namespace utility
{
	/**
		������Ϊ���ɿ��������븴�Ʋ���
		ʹ�÷�ʽ��˽�м̳и��࣬example class Factory : private utility::UnCopyable

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

