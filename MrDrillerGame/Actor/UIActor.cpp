#include "UIActor.h"
#include <Game/GameSetting.h>
#include <Level/DrillerLevel.h>
#include <Render/Renderer.h>

using namespace Craft;

UIActor::UIActor()
	: Actor()
{
	sortingOrder = 30;
}

void UIActor::Draw()
{
	std::shared_ptr<DrillerLevel> level = Cast<DrillerLevel>(GetOwner());
	if (!level)
	{
		return;
	}

	const int filledCount = static_cast<int>(level->GetOxygen() / 10.0f);
	const int uiX = dividerX + 2;
	DrawText("[ MR. DRILLER-C ]", uiX + 1, 1, Color::Yellow);

	DrawSection("STATUS", uiX, 3);
	DrawText(
		"DEPTH " + std::to_string(level->GetDepth())
			+ " / " + std::to_string(targetDepth) + "m",
		uiX, 4, Color::Green);
	DrawText("SCORE " + std::to_string(level->GetScore()), uiX, 5, Color::White);
	DrawText(
		level->IsAutoMode() ? "MODE  AUTO" : "MODE  MANUAL",
		uiX, 6,
		level->IsAutoMode() ? Color::Cyan : Color::White);

	DrawSection("OXYGEN", uiX, 8);
	const Color gaugeColor = level->GetOxygen() > 25.0f ? Color::Cyan : Color::Red;
	for (int row = 0; row < 10; ++row)
	{
		const bool isFilled = filledCount > 9 - row;
		DrawText(isFilled ? "[#]" : "[.]", uiX, 9 + row, gaugeColor);
	}
	DrawText(std::to_string(static_cast<int>(level->GetOxygen())) + "%", uiX + 5, 9, gaugeColor);

	DrawSection("CONTROLS", uiX, 20);
	DrawText("L/R   Move", uiX, 21, Color::White);
	DrawText("U/D   Face", uiX, 22, Color::White);
	DrawText("Z     Drill", uiX, 23, Color::White);
	DrawText("SPACE Auto", uiX, 24, level->IsAutoMode() ? Color::Cyan : Color::White);
	DrawText("ESC   Pause", uiX, 25, Color::White);

	if (level->IsGameClear())
	{
		DrawText("*** CLEAR! ***", uiX, 27, Color::Yellow);
	}
	else if (!level->IsPlaying())
	{
		DrawText("*** GAME OVER ***", uiX, 27, Color::Red);
		DrawText("R : RESTART", uiX, 28, Color::BrightWhite);
	}
}

void UIActor::DrawText(const std::string& text, int x, int y, Color textColor) const
{
	Renderer::Get().Submit(text, Vector2(x, y), textColor, sortingOrder);
}

void UIActor::DrawSection(const std::string& title, int x, int y) const
{
	std::string section = "-- " + title + " ";
	if (section.size() < 18)
	{
		section += std::string(18 - section.size(), '-');
	}
	DrawText(section, x, y, Color::Yellow);
}
