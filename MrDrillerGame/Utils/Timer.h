#pragma once
class Timer
{
public:
	Timer(float targetTime = 1.0f);

	void Tick(float deltaTime);

	void Reset();

	bool IsTimeOut() const { return elapsedTime >= targetTime; }


private:
	// 경과 시간 계산용 변수
	float elapsedTime = 0.0f;

	// 타이머 목표 시간
	float targetTime = 0.0f;

};

