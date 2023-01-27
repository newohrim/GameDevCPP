#pragma once
class Component
{
public:
	Component(class Actor* ComponentOwner, int UpdateOrder = 100);
	virtual ~Component();

	virtual void Update(float DeltaTime);
	int GetUpdateOrder() const { return mUpdateOrder; }

protected:
	class Actor* mOwner;
	int mUpdateOrder;
};

