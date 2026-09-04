#include "PauseLevel.h"
#include <Engine/Engine.h>
#include <Game/DrillerGame.h>
#include <Input/Input.h>
#include <Render/Renderer.h>
#include <Windows.h>

using namespace Craft;

void PauseLevel::Tick(float deltaTime)
{
	Level::Tick(deltaTime);
	DrillerGame& game = static_cast<DrillerGame&>(Engine::Get());

	if (Input::Get().GetKeyDown(VK_ESCAPE)
		|| Input::Get().GetKeyDown(VK_RETURN))
	{
		game.ResumeGame();
	}
	else if (Input::Get().GetKeyDown('R'))
	{
		game.RestartGame();
	}
	else if (Input::Get().GetKeyDown('Q'))
	{
		game.QuitStartGame();
	}
}

void PauseLevel::Draw()
{
	Renderer::Get().Submit("========================================", Vector2(20, 6), Color::White);
	Renderer::Get().Submit("             PAUSED", Vector2(20, 8), Color::Yellow);
	Renderer::Get().Submit("========================================", Vector2(20, 10), Color::White);
	Renderer::Get().Submit("ESC / ENTER : RESUME", Vector2(27, 13), Color::BrightWhite);
	Renderer::Get().Submit("R           : RESTART", Vector2(27, 15), Color::Cyan);
	Renderer::Get().Submit("Q           : MAIN MENU", Vector2(27, 17), Color::White);
}
