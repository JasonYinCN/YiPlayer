#pragma once
public ref class OperationBase
{
public:
	OperationBase() {};
	~OperationBase() {};

public:
	virtual int Stop() = 0;
	double GetProgress() { return m_progress; };
protected:
	double m_progress = 0;
};

