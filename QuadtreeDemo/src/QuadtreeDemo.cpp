#include "QuadtreeDemo.h"

#include "actors/DotActor.h"
#include "components/DotRenderComponent.h"
#include "core/Random.h"

#include "ui/UIContainerActor.h"
#include "components/TextUIComponent.h"

void QuadtreeDemo::LoadData()
{
	const SDL_Point WindowSize = GetWindowSize();
	m_DotsTree = Quadtree({ 0, (float)WindowSize.x, 0, (float)WindowSize.y });
	m_Dots = InitializeDots(m_DotsCount, GetWindowSize());
	m_QuadtreeDrawer = std::unique_ptr<QuadtreeDrawer>(new QuadtreeDrawer());

	m_MainTextFont = TTF_OpenFont("Assets/arialmt.ttf", 24);
	if (!m_MainTextFont)
	{
		SDL_Log(TTF_GetError());
	}
	m_StatusLabelContainer = new UIContainerActor(this);
	m_MsLabel = new TextUIComponent(m_MainTextFont, m_StatusLabelContainer);
	m_StatusLabelContainer->AddUIComponent(m_MsLabel);
	m_CountLabel = new TextUIComponent(m_MainTextFont, m_StatusLabelContainer);
	m_CountLabel->SetRectPosition(0, 32);
	m_StatusLabelContainer->AddUIComponent(m_CountLabel);
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

void QuadtreeDemo::UpdateGame()
{
	Game::UpdateGame();

	const Vector2_Int MousePos = GetMousePos();
	const Vector2 FMousePos = Vector2{ (float)MousePos.x, (float)MousePos.y };
	const std::vector<Actor*> DotsInRadius = 
		m_DotsTree.GetEntitiesInRadius(FMousePos, 25.0f);
	for (Actor* Dot : DotsInRadius) 
	{
		DotActor* const DotA = static_cast<DotActor*>(Dot);
		DotA->GetDotRenderComponent()->SetDotColor({ 255, 255, 0, 255 });
		DotA->SetMoveDirection(Vector2::Normalize(DotA->GetPosition() - FMousePos));

		if (DotA->GetReproduceCooldown() <= 0.0f)
		{
			DotA->SetReproduceCooldown(REPRODUCE_COOLDOWN);
			DotActor* const NewDot = new DotActor(&m_DotsTree, this);
			NewDot->SetPosition(DotA->GetPosition() + Vector2 { 1.0f, 1.0f });
			NewDot->GetDotRenderComponent()->SetDotColor({ 255, 255, 0, 255 });
			NewDot->SetReproduceCooldown(REPRODUCE_COOLDOWN);
			m_Dots.push_back(NewDot);
			m_DotsTree.AddEntity(NewDot, NewDot->GetPosition());
		}
	}

	m_MsLabel->SetText(std::to_string(GetDeltaTime() * 1000.0f));
	m_CountLabel->SetText(std::to_string(m_Dots.size()));

	RequestRedraw();
}

void QuadtreeDemo::DrawCustom(SDL_Renderer* Renderer)
{
	m_QuadtreeDrawer->DrawGrid(m_DotsTree, Renderer);

	for (DotActor* Dot : m_Dots) 
	{
		Dot->GetDotRenderComponent()->DrawDot(Renderer);
		Dot->GetDotRenderComponent()->SetDotColor({255, 255, 255, 255});
	}

	const Vector2_Int MousePos = GetMousePos();
}

std::vector<DotActor*> QuadtreeDemo::InitializeDots(size_t Count, SDL_Point Bounds)
{
	std::vector<DotActor*> Dots(Count);
	for (int i = 0; i < Count; ++i) 
	{
		Dots[i] = new DotActor(&m_DotsTree, this);
		Dots[i]->SetPosition(GetRandomPosition(Bounds) * 0.5 + Vector2 { 100, 100 });
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
