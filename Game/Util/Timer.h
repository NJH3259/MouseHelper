#include <Windows.h>
#include <cstdint>

#pragma once

class Timer
{
public:
    Timer(float targetTime = 1.0f);

    void SetElapsedTimer(float elapsedTime = 0.0f);

    // 타이머 시간 업데이트 함수.
    void Tick(float deltaTime);

    // 경과시간 리셋 함수.
    void Reset();

    // 목표 시간 재설정 함수.
    void SetTargetTime(float targetTime);

    // 설정한 시간이 지났는지 확인하는 함수.
    inline bool IsTimeOut() const { return elapsedTime >= targetTime; }

    inline float GetElapsedTime() const { return elapsedTime; }

    // Debugging Time Fuctions

    inline void CheckStartStat()
    {
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);
        start = counter.QuadPart;
    }

    inline void CheckEndStat()
    {
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);
        end = counter.QuadPart;
    }

    inline float GetDebugTime() const
    { 
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        return static_cast<float>(end - start) / static_cast<float>(frequency.QuadPart);
    }

    inline void ResetDebugTime()
    {
        start = end = 0;
    }

private:
    // 경과 시간 계산용 변수.
    float elapsedTime = 0.0f;

    // 타이머 목표 시간.
    float targetTime = 0.0f;

    int64_t start;
    int64_t end;
};