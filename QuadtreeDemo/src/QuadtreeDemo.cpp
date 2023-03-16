#include "QuadtreeDemo.h"

#include "actors/DotActor.h"
#include "components/DotRenderComponent.h"
#include "core/Random.h"

void QuadtreeDemo::LoadData()
{
	const SDL_Point WindowSize = GetWindowSize();
	m_DotsTree = Quadtree({ 0, (float)WindowSize.x, 0, (float)WindowSize.y });
	m_Dots = InitializeDots(m_DotsCount, GetWindowSize());
	m_QuadtreeDrawer = std::unique_ptr<QuadtreeDrawer>(new QuadtreeDrawer());
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

	RequestRedraw();
}

void QuadtreeDemo::DrawCustom(SDL_Renderer* Renderer)
{
	m_QuadtreeDrawer->DrawGrid(m_DotsTree, Renderer);

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
		Dots[i] = new DotActor(&m_DotsTree, this);
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
