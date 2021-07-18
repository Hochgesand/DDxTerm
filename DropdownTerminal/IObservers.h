#pragma once
class IObserver
{
public:
	IObserver(IObservable);
	virtual void Update();
};

class IObservable
{
	virtual void add(IObserver);
	virtual void remove(IObserver);
};