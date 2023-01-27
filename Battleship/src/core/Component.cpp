#include "Component.h"
#include "Actor.h"

Component::Component(Actor* ComponentOwner, int UpdateOrder)
{
	mOwner = ComponentOwner;
	mUpdateOrder = UpdateOrder;

	mOwner->AddComponent(this);
}

Component::~Component()
{
	mOwner->RemoveComponent(this);
}

void Component::Update(float DeltaTime)
{
}
