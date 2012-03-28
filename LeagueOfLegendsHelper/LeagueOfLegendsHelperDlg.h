#pragma once

const double DEFAULT_JUNGLE_SPAWN_TIME = 115.0;
const double DEFAULT_JUNGLE_SPAWN_RATE = 300.0;
const double DEFAULT_DRAGON_SPAWN_TIME = 150.0;
const double DEFAULT_DRAGON_SPAWN_RATE = 360.0;
const double DEFAULT_BARON_SPAWN_TIME = 900.0;
const double DEFAULT_BARON_SPAWN_RATE = 420.0;

#include "EZ_LCD.h"
#include "Bitmap.h"
#include "LeagueOfLegendsTimerTarget.h"

class LeagueOfLegendsHelperDlg : public CDialog
{
private:
	LeagueOfLegendsTimerTarget redFriendly;
	LeagueOfLegendsTimerTarget redEnemy;
	LeagueOfLegendsTimerTarget blueFriendly;
	LeagueOfLegendsTimerTarget blueEnemy;
	LeagueOfLegendsTimerTarget dragon;
	LeagueOfLegendsTimerTarget baron;

	bool colors;
	bool started;
	bool notify;

public:
	// Construction
    LeagueOfLegendsHelperDlg(CWnd* pParent = NULL);	// standard constructor

    // Dialog Data
    enum { IDD = IDD_COLORANDMONO_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


    // Implementation
protected:
    HICON m_hIcon;

    CEzLcd m_lcd;

    INT m_currentHighlightPosition;

    // LCD object handles
	HANDLE txtRed1;
	HANDLE txtBlue1;
	HANDLE txtDragon1;
	HANDLE txtBaron1;
	HANDLE txtRed2;
	HANDLE txtBlue2;
	HANDLE txtDragon2;
	HANDLE txtBaron2;
	
	HANDLE pbRedFriendly;
    HANDLE pbRedEnemy;
	HANDLE pbBlueFriendly;
	HANDLE pbBlueEnemy;
	HANDLE pbDragon;
	HANDLE pbBaron;

	HANDLE txtRedFriendlyTimer;
	HANDLE txtRedEnemyTimer;
	HANDLE txtBlueFriendlyTimer;
	HANDLE txtBlueEnemyTimer;
	HANDLE txtDragonTimer;
	HANDLE txtBaronTimer;

    VOID InitLCDObjects();
    VOID CheckButtonPresses();
    VOID UpdateTimers();
	VOID StartMatch();
	VOID EndMatch();
	VOID FlashColors();

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnDestroy( );
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
    DECLARE_MESSAGE_MAP()
};