#pragma once
#include <Level/Level.h>
//#include <Actor/Block.h>
#include <memory>
#include <vector>

class Block;
class Player;



class DrillerLevel : public Craft::Level
{
	private:
		enum class GameResult
		{
			Playing,
			GameOver,
			GameClear
		};

public:
	virtual void OnInitialized() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	Craft::Vector2 WorldToScreen(const Craft::Vector2& worldPosition) const;
	bool IsWorldPositionVisible(const Craft::Vector2& worldPosition) const;
	bool IsPlaying() const { return gameResult == GameResult::Playing; }
	bool IsGameClear() const { return gameResult == GameResult::GameClear; }
	bool IsAutoMode() const { return isAutoMode; }
	int GetDepth() const;
	int GetScore() const { return score; }
	float GetOxygen() const { return oxygen; }
	void AddScore(int amount) { score += amount; }
	void ConsumeOxygen(float amount);
	void RestoreOxygen() { oxygen = 100.0f; }
	void DrillBlockAt(const Craft::Vector2& position);

	bool IsPlayerAt(const Craft::Vector2& position) const;

	std::shared_ptr<Block> GetBlockAt(const Craft::Vector2& position) const;
	std::vector<std::shared_ptr<Block>> FindConnectedBlocks(
		const Craft::Vector2& startPosition) const;

private:
	void GenerateRowsThrough(int endY);
	void CleanupBlocksAboveView();
	void UpdateBlockGravity(float deltaTime);
	void UpdateAutoMode(float deltaTime);
	void UpdateCameraOffset();
	std::vector<Craft::Vector2> FindAutoPath() const;
	int GetGuaranteedRouteX(int worldY) const;
	bool IsGuaranteedRouteCell(const Craft::Vector2& position) const;

	std::shared_ptr<Player> player;
	int cameraOffsetY = 0;
	int score = 0;
	float oxygen = 100.0f;
	float oxygenTimer = 0.0f;
	float blockFallTimer = 0.0f;
	float autoActionTimer = 0.0f;
	GameResult gameResult = GameResult::Playing;
	bool isAutoMode = false;
	int generatedThroughY = 6;
	int routeLeftX = 1;
	int routeRightX = 8;
	int routeRowsPerStep = 3;
	int routeInitialDirection = 1;
};


