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

	template<class C>
	C* GetComponent();

	void AddComponent(class Component* ComponentToAdd);
	void RemoveComponent(class Component* ComponentToRemove);

	ActorState GetState() const { return mState; }
	Game* GetGame() const { return GameInstance; }
	float GetScale() const { return mScale; }
	void SetScale(const float Scale) { mScale = Scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(const float Rotation) { mRotation = Rotation; }
	Vector2 GetPosition() const { return mPosition; }
	void SetPosition(const Vector2 Position) { mPosition = Position; }
	Vector2 GetForwardVector() const;

	Vector2 GetActorVelocity() const;

private:
	ActorState mState = ActorState::EActive;
	Vector2 mPosition = Vector2::Zero;
	float mScale = 1.0f;
	float mRotation = 0.0f;

	std::vector<class Component*> mComponents;
	class Game* GameInstance;
};
