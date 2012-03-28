
#pragma once

#include "timer.h"

#define DEFAULT_COUNTDOWN_TEXT _T("00:00.00")

class LeagueOfLegendsTimerTarget
{
private:
	boost::timer targetTimer;
	double spawnTime;
	double initialSpawnTime;
	double standardSpawnRate;
	bool notifyTargetSpawning;
	bool notifyTargetReady;

public:
	LPCTSTR notifyTargetSpawningMessage;
	LPCTSTR notifyTargetReadyMessage;

	LeagueOfLegendsTimerTarget(double startSpawnTime, double spawnRate, LPCTSTR spawningMessage, LPCTSTR readyMessage)
		: targetTimer(),
		spawnTime(0.0),
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
		spawnTime = 0.0;
	}

	double PercentComplete()
	{
		if(spawnTime > 0.0)
			return (targetTimer.elapsed() / spawnTime) * 100.0;
		else
			return 0.0;
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
		if(PercentComplete() >= 90.0 && !notifyTargetSpawning)
			return notifyTargetSpawning = true;
		else
			return false;
	}

	bool DisplayReady()
	{
		if(PercentComplete() >= 100.0 && !notifyTargetReady)
			return notifyTargetReady = true;
		else
			return false;
	}

	void UpdateCountdownText(CEzLcd& lcd, HANDLE txtControl)
	{
		if(PercentComplete() >= 100)
			lcd.SetText(txtControl, DEFAULT_COUNTDOWN_TEXT);
		else
		{
			double countdown = abs(spawnTime - targetTimer.elapsed());
			TCHAR buffer[24];
			swprintf_s(buffer, _T("%02.0f:%05.2f"), floor(fmod(countdown,3600.0)/60.0), fmod(countdown,60.0));
			lcd.SetText(txtControl, static_cast<LPCTSTR>(buffer));
		}
	}

	static void GenerateKey(int vk, BOOL bExtended) 
	{
		KEYBDINPUT  kb = { NULL };
		INPUT       Input = { NULL };

		/* Generate a "key down" */
		if (bExtended) {
			kb.dwFlags  = KEYEVENTF_EXTENDEDKEY;
		}

		kb.wVk  = vk;
		Input.type  = INPUT_KEYBOARD;
		Input.ki  = kb;
		SendInput(1, &Input, sizeof(Input));

		Sleep(25);

		/* Generate a "key up" */
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

		Sleep(25);
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

		for (LPCTSTR p=str; *p; p++) {
			inp[0].ki.wScan = inp[1].ki.wScan = *p;
			SendInput(2, inp, sizeof(INPUT));
			Sleep(25);
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