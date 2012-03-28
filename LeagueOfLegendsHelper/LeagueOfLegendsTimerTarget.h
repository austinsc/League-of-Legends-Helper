#pragma once

#include <ctime>

#define DEFAULT_COUNTDOWN_TEXT _T("00:00.00")
#define KEYSTROKE_DELAY 25

namespace std { using ::clock_t; using ::clock; }

class timer
{
public:
	timer() 
		{ _start_time = std::clock(); }
		
	void   restart() 
		{ _start_time = std::clock(); }
		
	float elapsed() const
		{ return  float(std::clock() - _start_time) / CLOCKS_PER_SEC; }

	float elapsed_max() const
		{ return (float((std::numeric_limits<std::clock_t>::max)()) - float(_start_time)) / float(CLOCKS_PER_SEC); }

	float elapsed_min() const
		{ return float(1)/float(CLOCKS_PER_SEC); }

private:
	std::clock_t _start_time;
};

class LeagueOfLegendsTimerTarget
{
private:
	timer targetTimer;
	float spawnTime;
	float initialSpawnTime;
	float standardSpawnRate;
	bool notifyTargetSpawning;
	bool notifyTargetReady;

public:
	LPCTSTR notifyTargetSpawningMessage;
	LPCTSTR notifyTargetReadyMessage;

	LeagueOfLegendsTimerTarget(float startSpawnTime, float spawnRate, LPCTSTR spawningMessage, LPCTSTR readyMessage)
		: targetTimer(),
		  spawnTime(0.0f),
		  initialSpawnTime(startSpawnTime),
		  standardSpawnRate(spawnRate),
		  notifyTargetSpawning(false),
		  notifyTargetReady(false),
		  notifyTargetSpawningMessage(spawningMessage),
		  notifyTargetReadyMessage(readyMessage)
	{

	}

	void Start()
	{
		targetTimer.restart();
		spawnTime = initialSpawnTime;
	}

	void Stop()
	{
		spawnTime = 0.0f;
	}

	float PercentComplete()
	{
		if(spawnTime > 0.0f)
			return (targetTimer.elapsed() / spawnTime) * 100.0f;
		else
			return 0.0f;
	}

	void Trigger()
	{
		targetTimer.restart();
		spawnTime = standardSpawnRate;
		notifyTargetSpawning = false;
		notifyTargetReady = false;
	}

	bool DisplayWarning()
	{
		if(PercentComplete() >= 90.0f && !notifyTargetSpawning)
			return notifyTargetSpawning = true;
		else
			return false;
	}

	bool DisplayReady()
	{
		if(PercentComplete() >= 100.0f && !notifyTargetReady)
			return notifyTargetReady = true;
		else
			return false;
	}

	void UpdateCountdownText(CEzLcd& lcd, HANDLE txtControl)
	{
		if(PercentComplete() >= 100.0f)
			lcd.SetText(txtControl, DEFAULT_COUNTDOWN_TEXT);
		else
		{
			float countdown = abs(spawnTime - targetTimer.elapsed());
			TCHAR buffer[24];
			swprintf_s(buffer, _T("%02.0f:%05.2f"), floor(fmod(countdown, 3600.0f) / 60.0f), fmod(countdown, 60.0f));
			lcd.SetText(txtControl, static_cast<LPCTSTR>(buffer));
		}
	}

	static void GenerateKey(WORD vk, BOOL bExtended) 
	{
		KEYBDINPUT  kb = { NULL };
		INPUT       Input = { NULL };

		// Generate a "key down"
		if (bExtended) {
			kb.dwFlags  = KEYEVENTF_EXTENDEDKEY;
		}

		kb.wVk  = vk;
		Input.type  = INPUT_KEYBOARD;
		Input.ki  = kb;
		SendInput(1, &Input, sizeof(Input));

		Sleep(KEYSTROKE_DELAY);

		//Generate a "key up"
		ZeroMemory(&kb, sizeof(KEYBDINPUT));
		ZeroMemory(&Input, sizeof(INPUT));
		kb.dwFlags  =  KEYEVENTF_KEYUP;
		if (bExtended) {
			kb.dwFlags |= KEYEVENTF_EXTENDEDKEY;
		}

		kb.wVk = vk;
		Input.type = INPUT_KEYBOARD;
		Input.ki = kb;
		SendInput(1, &Input, sizeof(Input));

		Sleep(KEYSTROKE_DELAY);
	}

	static void SendString(LPCTSTR str) 
	{
		GenerateKey(VK_RETURN, TRUE);

		INPUT inp[2];
		memset(inp,0,sizeof(INPUT));
		inp[0].type = INPUT_KEYBOARD;
		inp[0].ki.dwFlags = KEYEVENTF_UNICODE; // to avoid shift, and so on
		inp[1] = inp[0];
		inp[1].ki.dwFlags |= KEYEVENTF_KEYUP;

		for (LPCTSTR p=str; *p; p++) 
		{
			inp[0].ki.wScan = inp[1].ki.wScan = *p;
			SendInput(2, inp, sizeof(INPUT));
			Sleep(KEYSTROKE_DELAY);
		}

		GenerateKey(VK_RETURN, TRUE);
	}

	void CheckMessages()
	{
		if(DisplayWarning())
			SendString(notifyTargetSpawningMessage);
		else if(DisplayReady())
			SendString(notifyTargetReadyMessage);
	}
};