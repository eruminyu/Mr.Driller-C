#include "DrillerLevel.h"
#include <Actor/Player.h>
#include <Actor/Block.h>
//#include "../Actor/Block.h"
#include <Game/GameSetting.h>
#include <Util/Util.h>

#include <Render/Renderer.h>
#include <Math/Vector2.h>
#include <Math/Color.h>
#include<string>
#include <Actor/UIActor.h>
#include <Input/Input.h>
#include <Engine/Engine.h>
#include <Game/DrillerGame.h>
#include <limits>
#include <queue>
#include <algorithm>

using namespace Craft;

void DrillerLevel::OnInitialized()
{

	Level::OnInitialized();
	Util::SetRandomSeed();

	// 재시작할 때마다 자동 완주 경로의 양 끝과 진행 방향을 새로 정한다.
	const int centerX = gridWidth / 2;
	routeLeftX = Util::RandomRange(0, centerX - 2);
	routeRightX = Util::RandomRange(centerX + 1, gridWidth - 1);
	routeRowsPerStep = Util::RandomRange(2, 4);
	routeInitialDirection = Util::RandomRange(0, 1) == 0 ? -1 : 1;

	player = SpawnActor<Player>();
	SpawnActor<UIActor>();
	GenerateRowsThrough(24);
}

void DrillerLevel::Tick(float deltaTime)
{
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		static_cast<DrillerGame&>(Engine::Get()).PauseGame();
		return;
	}

	if (!IsPlaying() && Input::Get().GetKeyDown('R'))
	{
		static_cast<DrillerGame&>(Engine::Get()).RestartGame();
		return;
	}

	if (IsPlaying() && Input::Get().GetKeyDown(VK_SPACE))
	{
		isAutoMode = !isAutoMode;
		autoActionTimer = 0.0f;
	}

	Level::Tick(deltaTime);
	UpdateCameraOffset();
	if (!IsPlaying())
	{
		return;
	}

	UpdateBlockGravity(deltaTime);
	GenerateRowsThrough(player->GetPosition().y + visibleGridRows + 10);
	UpdateAutoMode(deltaTime);
	CleanupBlocksAboveView();

	oxygenTimer += deltaTime;
	while (oxygenTimer >= 1.0f)
	{
		oxygenTimer -= 1.0f;
		oxygen = oxygen > 1.0f ? oxygen - 1.0f : 0.0f;
	}

	if (GetDepth() >= targetDepth)
	{
		gameResult = GameResult::GameClear;
	}
	else if (oxygen <= 0.0f)
	{
		gameResult = GameResult::GameOver;
	}
}

void DrillerLevel::GenerateRowsThrough(int endY)
{
	const int lastRow = endY < worldHeight ? endY : worldHeight - 1;
	for (int y = generatedThroughY + 1; y <= lastRow; ++y)
	{
		const int routeX = GetGuaranteedRouteX(y);
		const int previousRouteX = GetGuaranteedRouteX(y - 1);
		for (int x = 0; x < gridWidth; ++x)
		{
			const int chance = Util::RandomRange(0, 99);
			Block::SetColor blockColor;
			const bool isGuaranteedRoute = x == routeX || x == previousRouteX;
			const int routeDepth = y - depthOriginY;
			if (isGuaranteedRoute && routeDepth > 0 && routeDepth % 10 == 0)
			{
				// 자동 모드가 실제 산소 규칙으로 완주할 수 있는 보급 지점.
				blockColor = Block::SetColor::Oxygen;
			}
			else if (isGuaranteedRoute)
			{
				// 보장 경로에는 산소 -20인 먼지 블록을 배치하지 않는다.
				blockColor = static_cast<Block::SetColor>(routeDepth % 4);
			}
			else if (chance < 4)
			{
				blockColor = Block::SetColor::Oxygen;
			}
			else if (chance < 14)
			{
				blockColor = Block::SetColor::Dust;
			}
			else
			{
				blockColor = static_cast<Block::SetColor>(Util::RandomRange(0, 3));
			}
			SpawnActor<Block>(Vector2(x, y), blockColor);
		}
	}
	if (lastRow > generatedThroughY)
	{
		generatedThroughY = lastRow;
	}
}

void DrillerLevel::CleanupBlocksAboveView()
{
	if (!player)
	{
		return;
	}

	const int deleteBeforeY = cameraOffsetY - 10;
	if (deleteBeforeY <= 0)
	{
		return;
	}

	for (const std::shared_ptr<Actor>& actor : actorList)
	{
		if (actor->IsActive()
			&& actor->IsTypeOf<Block>()
			&& actor->GetPosition().y < deleteBeforeY)
		{
			actor->Destroy();
		}
	}
}

int DrillerLevel::GetGuaranteedRouteX(int worldY) const
{
	const int depth = worldY - depthOriginY;
	if (depth <= 0)
	{
		return gridWidth / 2;
	}

	// 시작 위치에서 매 게임 무작위로 정한 양 끝을 왕복한다.
	// 올림 나눗셈을 사용해 지표면 바로 아래부터 좌우 이동을 시작한다.
	const int stepCount = (depth + routeRowsPerStep - 1) / routeRowsPerStep;
	int routeX = gridWidth / 2;
	int direction = routeInitialDirection;
	for (int step = 0; step < stepCount; ++step)
	{
		if (routeX + direction < routeLeftX
			|| routeX + direction > routeRightX)
		{
			direction *= -1;
		}
		routeX += direction;
	}
	return routeX;
}

bool DrillerLevel::IsGuaranteedRouteCell(const Vector2& position) const
{
	return position.x == GetGuaranteedRouteX(position.y)
		|| position.x == GetGuaranteedRouteX(position.y - 1);
}

void DrillerLevel::ConsumeOxygen(float amount)
{
	oxygen = oxygen > amount ? oxygen - amount : 0.0f;
}

void DrillerLevel::UpdateAutoMode(float deltaTime)
{
	if (!isAutoMode || !player)
	{
		return;
	}

	autoActionTimer += deltaTime;
	if (autoActionTimer < 0.18f)
	{
		return;
	}
	autoActionTimer = 0.0f;

	GenerateRowsThrough(player->GetPosition().y + visibleGridRows + 10);
	const std::vector<Vector2> path = FindAutoPath();
	if (path.empty())
	{
		isAutoMode = false;
		return;
	}

	const Vector2 current = player->GetPosition();
	const Vector2 next = path.front();
	const Vector2 direction = next - current;
	player->SetLookDirection(direction);

	if (GetBlockAt(next))
	{
		DrillBlockAt(next);
	}
	else
	{
		player->SetPosition(next);
	}
}

void DrillerLevel::DrillBlockAt(const Vector2& position)
{
	const std::shared_ptr<Block> targetBlock = GetBlockAt(position);
	if (!targetBlock)
	{
		return;
	}

	std::vector<std::shared_ptr<Block>> connectedBlocks =
		FindConnectedBlocks(position);
	if (targetBlock->GetColor() == Block::SetColor::Dust)
	{
		ConsumeOxygen(20.0f);
	}
	else if (targetBlock->GetColor() == Block::SetColor::Oxygen)
	{
		RestoreOxygen();
	}
	else
	{
		ConsumeOxygen(3.0f);
	}
	AddScore(static_cast<int>(connectedBlocks.size()) * 10);
	for (const std::shared_ptr<Block>& block : connectedBlocks)
	{
		block->Destroy();
	}
}

std::vector<Vector2> DrillerLevel::FindAutoPath() const
{
	if (!player)
	{
		return {};
	}

	struct SearchState
	{
		Vector2 position = Vector2(-1, -1);
		int oxygen = -1;
	};

	const Vector2 start = player->GetPosition();
	const int finalDestinationY = depthOriginY + targetDepth;
	const int destinationY = generatedThroughY < finalDestinationY
		? generatedThroughY : finalDestinationY;
	const int minimumY = start.y > 2 ? start.y - 2 : 0;
	std::vector<std::vector<std::vector<bool>>> visited(
		worldHeight,
		std::vector<std::vector<bool>>(
			gridWidth, std::vector<bool>(101, false)));
	std::vector<std::vector<std::vector<SearchState>>> previous(
		worldHeight,
		std::vector<std::vector<SearchState>>(
			gridWidth, std::vector<SearchState>(101)));
	std::queue<SearchState> open;

	int startOxygen = static_cast<int>(oxygen);
	if (startOxygen < 1)
	{
		return {};
	}
	if (startOxygen > 100)
	{
		startOxygen = 100;
	}
	visited[start.y][start.x][startOxygen] = true;
	open.push({ start, startOxygen });
	SearchState destination;
	const Vector2 directions[4] =
	{
		Vector2(0, 1), Vector2(1, 0), Vector2(-1, 0), Vector2(0, -1)
	};

	while (!open.empty())
	{
		const SearchState current = open.front();
		open.pop();
		if (current.position.y >= destinationY)
		{
			destination = current;
			break;
		}

		for (const Vector2& direction : directions)
		{
			const Vector2 next = current.position + direction;
			if (next.x < 0 || next.x >= gridWidth
				|| next.y < minimumY || next.y >= worldHeight
				|| next.y > generatedThroughY)
			{
				continue;
			}
			if (next.y > start.y && !IsGuaranteedRouteCell(next))
			{
				continue;
			}

			// 이동 시간 동안의 자연 감소분까지 보수적으로 1 소모로 계산한다.
			int nextOxygen = current.oxygen - 1;
			const std::shared_ptr<Block> block = GetBlockAt(next);
			if (block)
			{
				if (block->GetColor() == Block::SetColor::Oxygen)
				{
					nextOxygen = 100;
				}
				else if (block->GetColor() == Block::SetColor::Dust)
				{
					nextOxygen -= 20;
				}
				else
				{
					nextOxygen -= 3;
				}
			}
			if (nextOxygen <= 0 || visited[next.y][next.x][nextOxygen])
			{
				continue;
			}
			visited[next.y][next.x][nextOxygen] = true;
			previous[next.y][next.x][nextOxygen] = current;
			open.push({ next, nextOxygen });
		}
	}

	if (destination.position.x < 0)
	{
		return {};
	}
	std::vector<Vector2> path;
	for (SearchState current = destination; current.position != start;
		current = previous[current.position.y][current.position.x][current.oxygen])
	{
		path.emplace_back(current.position);
	}
	std::reverse(path.begin(), path.end());
	return path;
}

void DrillerLevel::UpdateBlockGravity(float deltaTime)
{
	blockFallTimer += deltaTime;
	if (blockFallTimer < 0.2f)
	{
		return;
	}
	blockFallTimer = 0.0f;

	std::vector<std::shared_ptr<Block>> processedBlocks;
	std::vector<std::vector<std::shared_ptr<Block>>> movableGroups;
	std::shared_ptr<Block> autoThreatBlock;

	for (const std::shared_ptr<Actor>& actor : actorList)
	{
		if (!actor->IsActive() || !actor->IsTypeOf<Block>())
		{
			continue;
		}

		const std::shared_ptr<Block> block = Cast<Block>(actor);
		if (std::find(processedBlocks.begin(), processedBlocks.end(), block)
			!= processedBlocks.end())
		{
			continue;
		}

		std::vector<std::shared_ptr<Block>> group =
			FindConnectedBlocks(block->GetPosition());
		processedBlocks.insert(processedBlocks.end(), group.begin(), group.end());

		bool canFall = true;
		for (const std::shared_ptr<Block>& groupBlock : group)
		{
			const Vector2 below(
				groupBlock->GetPosition().x,
				groupBlock->GetPosition().y + 1);

			if (below.y >= worldHeight
				|| below.y > generatedThroughY)
			{
				canFall = false;
				break;
			}

			const std::shared_ptr<Block> blockBelow = GetBlockAt(below);
			const bool isInsideSameGroup =
				std::find(group.begin(), group.end(), blockBelow) != group.end();
			if (blockBelow && !isInsideSameGroup)
			{
				canFall = false;
				break;
			}
		}

		if (canFall)
		{
			// 오토 모드는 일반 규칙대로 위쪽을 드릴해서 낙하 압사를 피한다.
			// 수동 플레이에서는 기존처럼 떨어지는 블록에 맞으면 게임 오버다.
			if (isAutoMode)
			{
				for (const std::shared_ptr<Block>& groupBlock : group)
				{
					const Vector2 below(
						groupBlock->GetPosition().x,
						groupBlock->GetPosition().y + 1);
					if (IsPlayerAt(below))
					{
						autoThreatBlock = groupBlock;
						break;
					}
				}
			}

			if (!autoThreatBlock)
			{
				movableGroups.emplace_back(std::move(group));
			}
			else
			{
				// 위협 그룹은 이번 중력 틱에 멈춰 두고 아래에서 위로 제거한다.
				break;
			}
		}
	}

	if (autoThreatBlock)
	{
		player->SetLookDirection(Vector2(0, -1));
		DrillBlockAt(autoThreatBlock->GetPosition());
	}

	// 판정을 모두 끝낸 뒤 이동해서 순회 순서에 따른 연쇄 이동을 막는다.
	for (const std::vector<std::shared_ptr<Block>>& group : movableGroups)
	{
		for (const std::shared_ptr<Block>& block : group)
		{
			Vector2 newPosition = block->GetPosition();
			++newPosition.y;
			block->SetPosition(newPosition);
			if (IsPlayerAt(newPosition))
			{
				gameResult = GameResult::GameOver;
			}
		}
	}
}

void DrillerLevel::Draw()
{
	Level::Draw();



	// 가로선 문자열 정의
	const std::string horizontalLine = "+"
		+ std::string(playAreaWidth, '-')
		+ "+"
		+ std::string(uiAreaWidth, '-')
		+ "+";

	Renderer::Get().Submit(
		horizontalLine,
		Vector2(0, 0),
		Color::White
	);
	Renderer::Get().Submit(
		horizontalLine,
		Vector2(0, screenHeight - 1),
		Color::White
	);

	for (int y = 1; y < screenHeight - 1; ++y)
	{
		Renderer::Get().Submit("|", Vector2(0, y), Color::White);
		Renderer::Get().Submit("|", Vector2(dividerX, y), Color::White);
		Renderer::Get().Submit("|", Vector2(uiRightBorder, y), Color::White);
	}


	

}

void DrillerLevel::UpdateCameraOffset()
{
	if (!player)
	{
		return;
	}

	cameraOffsetY = std::clamp(
		player->GetPosition().y - visibleGridRows / 3,
		0,
		worldHeight - visibleGridRows);
}

Vector2 DrillerLevel::WorldToScreen(const Vector2& worldPosition) const
{
	return Vector2(
		fieldLeft + worldPosition.x * cellWidth,
		fieldTop + (worldPosition.y - cameraOffsetY) * cellHeight);
}

bool DrillerLevel::IsWorldPositionVisible(const Vector2& worldPosition) const
{
	return worldPosition.x >= 0 && worldPosition.x < gridWidth
		&& worldPosition.y >= cameraOffsetY
		&& worldPosition.y < cameraOffsetY + visibleGridRows;
}

int DrillerLevel::GetDepth() const
{
	if (!player)
	{
		return 0;
	}

	const int depth = player->GetPosition().y - depthOriginY;
	return depth > 0 ? depth : 0;
}

bool DrillerLevel::IsPlayerAt(const Craft::Vector2& position) const
{
	return player && player->IsActive() && player->GetPosition() == position;
}

std::shared_ptr<Block> DrillerLevel::GetBlockAt(const Craft::Vector2& position) const
{
	for (const auto& actor : actorList)
	{
		if (!actor->IsActive() || !actor->IsTypeOf<Block>())
		{
			continue;
		}

		if (actor->GetPosition() == position)
		{
			return Cast<Block>(actor);
		}
	}
	return nullptr;
}

std::vector<std::shared_ptr<Block>>
DrillerLevel::FindConnectedBlocks(const Craft::Vector2& startPosition) const
{
	std::vector<std::shared_ptr<Block>> connectedBlocks;

	std::shared_ptr<Block> startBlock = GetBlockAt(startPosition);

	if (!startBlock)
	{
		return connectedBlocks;
	}

	if (!startBlock->IsColorBlock())
	{
		connectedBlocks.push_back(startBlock);
		return connectedBlocks;
	}

	connectedBlocks.push_back(startBlock);

	for (size_t index = 0; index < connectedBlocks.size(); ++index)
	{
		std::shared_ptr<Block> currentBlock = connectedBlocks[index];

		Craft::Vector2 currentPosition = currentBlock->GetPosition();

		Craft::Vector2 neighborPositions[4] =
		{
			Craft::Vector2(currentPosition.x, currentPosition.y - 1),
			Craft::Vector2(currentPosition.x, currentPosition.y + 1),
			Craft::Vector2(currentPosition.x - 1, currentPosition.y),
			Craft::Vector2(currentPosition.x + 1, currentPosition.y),
		};

		for (const Craft::Vector2& neighborPosition : neighborPositions)
		{
			std::shared_ptr<Block> neighborBlock = GetBlockAt(neighborPosition);

			if (!neighborBlock)
			{
				continue;
			}

			if (neighborBlock->GetColor() != startBlock->GetColor())
			{
				continue;
			}

			bool alreadyFound =
				std::find(
					connectedBlocks.begin(),
					connectedBlocks.end(),
					neighborBlock) != connectedBlocks.end();

			if (alreadyFound)
			{
				continue;
			}

			connectedBlocks.push_back(neighborBlock);

		}

		
	}


	return connectedBlocks;
}






