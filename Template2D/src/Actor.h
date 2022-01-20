#pragma once

#include "Math.h"
#include <vector>

enum ActorState 
{
	EActive = 0,
	EPaused,
	EDead
};

class Actor
{
public:
	Actor(class Game* GameInstance);
	virtual ~Actor();

	void Update(const float DeltaTime);
	void UpdateComponents(const float DeltaTime);
	virtual void UpdateActor(const float DeltaTime);

	void AddComponent(class Component* ComponentToAdd);
	void RemoveComponent(class Component* ComponentToRemove);

	ActorState GetState() const { return mState; }
	Game* GetGame() const { return GameInstance; }
	float GetScale() const { return mScale; }
	float GetRotation() const { return mRotation; }
	Vector2 GetPosition() const { return mPosition; }
	void SetPosition(const Vector2 Position) { mPosition = Position; }

private:
	ActorState mState = ActorState::EActive;
	Vector2 mPosition = Vector2::Zero;
	float mScale = 1.0f;
	float mRotation = 0.0f;

	std::vector<class Component*> mComponents;
	class Game* GameInstance;
};

