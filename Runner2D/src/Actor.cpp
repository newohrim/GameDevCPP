#include "Actor.h"
#include "Component.h"
#include "Game.h"
#include "IMoveComponent.h"

Actor::Actor(Game* GameInstance)
{
	this->GameInstance = GameInstance;
	GameInstance->AddActor(this);
}

Actor::~Actor()
{
	GameInstance->RemoveActor(this);
	while (!mComponents.empty()) 
	{
		delete mComponents.back();
	}
}

void Actor::Update(const float DeltaTime)
{
	if (mState == ActorState::EActive) 
	{
		UpdateComponents(DeltaTime);
		UpdateActor(DeltaTime);
	}
}

void Actor::UpdateComponents(const float DeltaTime)
{
	for (Component* Comp : mComponents) 
	{
		Comp->Update(DeltaTime);
	}
}

void Actor::UpdateActor(const float DeltaTime)
{
	// OVERRIDES IN CHILDREN
}

template<class C>
C* Actor::GetComponent()
{
	for (Component* Comp : mComponents) 
	{
		if (C* CastedComp = dynamic_cast<C*>(Comp))
		{
			return CastedComp;
		}
	}

	return nullptr;
}

void Actor::AddComponent(Component* ComponentToAdd)
{
	int UpdOrder = ComponentToAdd->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (;
		iter != mComponents.end();
		++iter)
	{
		if (UpdOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}

	mComponents.insert(iter, ComponentToAdd);
}

void Actor::RemoveComponent(Component* ComponentToRemove)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), ComponentToRemove);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}

Vector2 Actor::GetForwardVector() const
{
	return Vector2(Math::Cos(mRotation), -Math::Sin(mRotation));
}

Vector2 Actor::GetActorVelocity() const
{
	for (Component* Comp : mComponents) 
	{
		if (IMoveComponent* MoveComponent = 
			dynamic_cast<IMoveComponent*>(Comp)) 
		{
			return MoveComponent->GetVelocity();
		}
	}

	return Vector2::Zero;
}
