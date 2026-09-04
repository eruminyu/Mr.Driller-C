#include "StartLevel.h"
#include <Engine/Engine.h>
#include <Game/DrillerGame.h>
#include <Input/Input.h>
#include <Render/Renderer.h>
#include <Windows.h>

using namespace Craft;

void StartLevel::Tick(float deltaTime)
{
	Level::Tick(deltaTime);
	DrillerGame& game = static_cast<DrillerGame&>(Engine::Get());

	if (Input::Get().GetKeyDown(VK_UP))
	{
		selectedMenuIndex = 0;
	}
	else if (Input::Get().GetKeyDown(VK_DOWN))
	{
		selectedMenuIndex = 1;
	}

	if (Input::Get().GetKeyDown('Z'))
	{
		if (selectedMenuIndex == 0)
		{
			game.StartGame();
		}
		else
		{
			game.ExitGame();
		}
	}
}

void StartLevel::Draw()
{
	Renderer::Get().Submit("========================================", Vector2(20, 6), Color::Cyan);
	Renderer::Get().Submit("          MR. DRILLER-C", Vector2(20, 8), Color::Yellow);
	Renderer::Get().Submit("========================================", Vector2(20, 10), Color::Cyan);
	Renderer::Get().Submit(
		selectedMenuIndex == 0 ? "> GAME START" : "  GAME START",
		Vector2(32, 14),
		selectedMenuIndex == 0 ? Color::Yellow : Color::White);
	Renderer::Get().Submit(
		selectedMenuIndex == 1 ? "> EXIT" : "  EXIT",
		Vector2(32, 16),
		selectedMenuIndex == 1 ? Color::Yellow : Color::White);
	Renderer::Get().Submit("UP / DOWN : SELECT    Z : CONFIRM", Vector2(24, 19), Color::Cyan);
	Renderer::Get().Submit("Reach the target depth before oxygen runs out!", Vector2(18, 21), Color::Green);
}
