#include "Player.h"
#include <Input/Input.h>
#include <Game/GameSetting.h>
#include <Level/DrillerLevel.h>
#include <Render/Renderer.h>


using namespace Craft;


Player::Player()
    : Actor("@@@", Vector2::Zero, Color::BrightWhite)
{
	SetPosition(Vector2(gridWidth / 2, 2));
	
}

void Player::Draw()
{
	std::shared_ptr<DrillerLevel> level = Cast<DrillerLevel>(GetOwner());
	if (!level || !level->IsWorldPositionVisible(GetPosition()))
	{
		return;
	}

	const Vector2 screenPosition = level->WorldToScreen(GetPosition());
	std::string sprite[cellHeight] =
	{
		" O ",
		"/|\\",
		"/ \\"
	};

	if (playerLook.x < 0)
	{
		sprite[1] = "<| ";
	}
	else if (playerLook.x > 0)
	{
		sprite[1] = " |>";
	}
	else if (playerLook.y > 0)
	{
		sprite[2] = " V ";
	}
	else if (playerLook.y < 0)
	{
		sprite[0] = " ^ ";
	}

	for (int row = 0; row < cellHeight; ++row)
	{
		Renderer::Get().Submit(
			sprite[row],
			Vector2(screenPosition.x, screenPosition.y + row),
			Color::Purple,
			20);
	}
}



void Player::Tick(float deltaTime)
{
    super::Tick(deltaTime);
	std::shared_ptr<DrillerLevel> level = Cast<DrillerLevel>(GetOwner());
	if (!level || !level->IsPlaying() || level->IsAutoMode())
	{
		return;
	}

	fallTimer.Tick(deltaTime);


	if (fallTimer.IsTimeOut())
	{
		Vector2 fallPosition = GetPosition();
		fallPosition.y += 1;
		

		bool canFall = fallPosition.y < worldHeight &&
			!level->GetBlockAt(fallPosition);

		if (canFall)
		{
			SetPosition(fallPosition);
		}

		fallTimer.Reset();

	}


	Vector2 newPosition = GetPosition();

	if (Input::Get().GetKeyDown('Z'))
	{
		Vector2 destroyPosition = GetPosition();
		destroyPosition.x += playerLook.x;
		destroyPosition.y += playerLook.y;
		level->DrillBlockAt(destroyPosition);
	}

	if (Input::Get().GetKeyDown(VK_RIGHT))
	{
		Vector2 inputDirection(1, 0);

		Vector2 movePosition = GetPosition();
		movePosition.x += 1;

		bool hasBlockInFront =
			level->GetBlockAt(movePosition) != nullptr;

		if (!hasBlockInFront)
		{
			playerLook = inputDirection;
			newPosition = movePosition;
		}
		else if (playerLook != inputDirection)
		{
			playerLook = inputDirection;
		}
		else
		{
			Vector2 blockUp = movePosition;
			blockUp.y -= 1;

			bool canBlockUp =
				blockUp.y > playAreaUpBorder &&
				!level->GetBlockAt(blockUp);

			if (canBlockUp)
			{
				newPosition = blockUp;
			}
		}
	}

	if (Input::Get().GetKeyDown(VK_LEFT))
	{
		Vector2 inputDirection(-1, 0);

		Vector2 movePosition = GetPosition();
		movePosition.x -= 1;

		bool hasBlockInFront =
			level->GetBlockAt(movePosition) != nullptr;

		if (!hasBlockInFront)
		{
			playerLook = inputDirection;
			newPosition = movePosition;
		}
		else if (playerLook != inputDirection)
		{
			playerLook = inputDirection;
		}
		else
		{
			Vector2 blockUp = movePosition;
			blockUp.y -= 1;

			bool canBlockUp =
				blockUp.y > playAreaUpBorder &&
				!level->GetBlockAt(blockUp);

			if (canBlockUp)
			{
				newPosition = blockUp;
			}
		}
	}

	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		playerLook = Vector2(0, 1);
	}
	if (Input::Get().GetKeyDown(VK_UP))
	{
		playerLook = Vector2(0, -1);
	}
	// 경계를 벗어났는지 체크
	bool isInside =
		newPosition.x >= 0 &&
		newPosition.x < gridWidth &&
		newPosition.y >= 0 &&
		newPosition.y < worldHeight;

	if (isInside)
	{
		SetPosition(newPosition);
	}


	



	



	
	
}


