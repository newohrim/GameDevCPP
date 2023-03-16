#include "QuadtreeDemo.h"

#include "actors/DotActor.h"
#include "components/DotRenderComponent.h"
#include "core/Random.h"

void QuadtreeDemo::LoadData()
{
	m_Dots = InitializeDots(m_DotsCount, GetWindowSize());
}

void QuadtreeDemo::ProcessInput()
{
	Game::ProcessInput();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				ExitGame();
				break;
		}
	}
}

void QuadtreeDemo::DrawCustom(SDL_Renderer* Renderer)
{
	for (DotActor* Dot : m_Dots) 
	{
		Dot->GetDotRenderComponent()->DrawDot(Renderer);
	}
}

std::vector<DotActor*> QuadtreeDemo::InitializeDots(size_t Count, SDL_Point Bounds)
{
	std::vector<DotActor*> Dots(Count);
	for (int i = 0; i < Count; ++i) 
	{
		Dots[i] = new DotActor(this);
		Dots[i]->SetPosition(GetRandomPosition(Bounds));
		m_DotsTree.AddEntity(Dots[i], Dots[i]->GetPosition());
	}

	return Dots;
}

Vector2 QuadtreeDemo::GetRandomPosition(SDL_Point Bounds) const
{
	const float X = Random::GetFloat() * Bounds.x;
	const float Y = Random::GetFloat() * Bounds.y;

	return Vector2 { X, Y };
}
