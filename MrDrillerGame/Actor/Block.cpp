#include "Block.h"
#include <Level/DrillerLevel.h>
#include <Game/GameSetting.h>
#include <Render/Renderer.h>

using namespace Craft;

Block::Block(const Craft::Vector2& position, SetColor color)
	: Actor("#", position, ChangeColor(color)), blockColor(color)
{
	//Craft::Vector2 position = (30, 10);
}

void Block::Draw()
{
	std::shared_ptr<DrillerLevel> level = Cast<DrillerLevel>(GetOwner());
	if (!level || !level->IsWorldPositionVisible(GetPosition()))
	{
		return;
	}

	const Vector2 screenPosition = level->WorldToScreen(GetPosition());
	std::string blockImage = "###";
	if (blockColor == SetColor::Dust)
	{
		blockImage = "XXX";
	}
	else if (blockColor == SetColor::Oxygen)
	{
		blockImage = "OOO";
	}

	for (int row = 0; row < cellHeight; ++row)
	{
		Renderer::Get().Submit(
			blockImage,
			Vector2(screenPosition.x, screenPosition.y + row),
			Actor::color,
			10);
	}
}

Craft::Color Block::ChangeColor(SetColor color)
{
	switch (color)
	{
	case SetColor::Red:
		return Craft::Color::Red;

	case SetColor::Blue:
		return Craft::Color::Blue;

	case SetColor::Green:
		return Craft::Color::Green;

	case SetColor::Yellow:
		return Craft::Color::Yellow;

	case SetColor::Dust:
		return Craft::Color::Brown;

	case SetColor::Oxygen:
		return Craft::Color::Cyan;

	default:
		return Craft::Color::White;
	}
}
