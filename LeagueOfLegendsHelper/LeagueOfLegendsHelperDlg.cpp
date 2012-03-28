//************************************************************************
//  The Logitech LCD SDK, including all acompanying documentation,
//  is protected by intellectual property laws.  All use of the Logitech
//  LCD SDK is subject to the License Agreement found in the
//  "Logitech LCD SDK License Agreement" file and in the Reference Manual.  
//  All rights not expressly granted by Logitech are reserved.
//************************************************************************

// ColorAndMonoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LeagueOfLegendsHelper.h"
#include "LeagueOfLegendsHelperDlg.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Defines for testing EZ LCD Wrapper functions that aren't being used in sample
//#define BUTTON_TESTING
//#define SCREEN_PRIORITY_TESTING
//#define FOREGROUND_TESTING
//#define VISIBLE_TESTING
//#define PAGE_TESTING

LeagueOfLegendsHelperDlg::LeagueOfLegendsHelperDlg(CWnd* pParent /*=NULL*/)
 : CDialog(LeagueOfLegendsHelperDlg::IDD, pParent),
   started(false),
   colors(false),
   notify(true),
   redFriendly(DEFAULT_JUNGLE_SPAWN_TIME, DEFAULT_JUNGLE_SPAWN_TIME, _T("our red buff is spawning soon"), _T("our red buff is up")),
   redEnemy(DEFAULT_JUNGLE_SPAWN_TIME, DEFAULT_JUNGLE_SPAWN_RATE, _T("their red buff is spawning soon"), _T("their red buff is up")),
   blueFriendly(DEFAULT_JUNGLE_SPAWN_TIME, DEFAULT_JUNGLE_SPAWN_RATE, _T("our blue buff is spawning soon"), _T("our blue buff is up")),
   blueEnemy(DEFAULT_JUNGLE_SPAWN_TIME, DEFAULT_JUNGLE_SPAWN_RATE, _T("their blue buff is spawning soon"), _T("their blue buff is up")),
   dragon(DEFAULT_DRAGON_SPAWN_TIME, DEFAULT_DRAGON_SPAWN_RATE, _T("dragon is spawning soon"), _T("dragon is up")),
   baron(DEFAULT_BARON_SPAWN_TIME, DEFAULT_BARON_SPAWN_RATE, _T("baron is spawning soon"), _T("baron is up"))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_currentHighlightPosition = 1;
}

void LeagueOfLegendsHelperDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(LeagueOfLegendsHelperDlg, CDialog)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_DESTROY()
    ON_WM_TIMER()
    ON_WM_WINDOWPOSCHANGING()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL LeagueOfLegendsHelperDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    ShowWindow(SW_HIDE);
    SetWindowPos(NULL, 0, 0, 0, 0, NULL);

    HRESULT hRes = m_lcd.Initialize(_T("League of Legends Helper"), LG_MONOCHROME_MODE_ONLY, FALSE, TRUE);

    if (hRes != S_OK)
    {
        // Something went wrong, when connecting to the LCD Manager software. Deal with it...
        PostQuitMessage(0);
        return FALSE;
    }

    InitLCDObjects();

    SetTimer(0xabab, 30, NULL); // for scrolling to work smoothly, timer should be pretty fast

    return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void LeagueOfLegendsHelperDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR LeagueOfLegendsHelperDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void LeagueOfLegendsHelperDlg::OnDestroy( )
{
    
}

void LeagueOfLegendsHelperDlg::OnTimer(UINT_PTR nIDEvent)
{
    UNREFERENCED_PARAMETER(nIDEvent);
    CheckButtonPresses();
    UpdateTimers();
    m_lcd.Update();
}

void LeagueOfLegendsHelperDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
    ASSERT(NULL != lpwndpos);

    if(NULL != lpwndpos)
    {
        lpwndpos->flags &= ~SWP_SHOWWINDOW;
    }

    CDialog::OnWindowPosChanging(lpwndpos);
}

VOID LeagueOfLegendsHelperDlg::InitLCDObjects()
{
    m_lcd.ModifyDisplay(LG_MONOCHROME);

    /***************/
    /* FIRST PAGE */
    /***************/

    txtRed1 = m_lcd.AddText(LG_STATIC_TEXT, LG_TINY, DT_RIGHT, 36);
    m_lcd.SetOrigin(txtRed1, 0, 0);
    m_lcd.SetText(txtRed1, _T("Red Buff"));

    txtBlue1 = m_lcd.AddText(LG_STATIC_TEXT, LG_TINY, DT_RIGHT, 36);
    m_lcd.SetOrigin(txtBlue1, 0, 9);
    m_lcd.SetText(txtBlue1, _T("Blue Buff"));

	txtDragon1 = m_lcd.AddText(LG_STATIC_TEXT, LG_MEDIUM, DT_RIGHT, 36);
	m_lcd.SetOrigin(txtDragon1, 0, 18);
	m_lcd.SetText(txtDragon1, _T("Dragon"));

	txtBaron1 = m_lcd.AddText(LG_STATIC_TEXT, LG_MEDIUM, DT_RIGHT, 36);
	m_lcd.SetOrigin(txtBaron1, 0, 30);
	m_lcd.SetText(txtBaron1, _T("Baron"));

	pbRedFriendly = m_lcd.AddProgressBar(LG_FILLED);
	m_lcd.SetProgressBarSize(pbRedFriendly, 58, 8);
	m_lcd.SetOrigin(pbRedFriendly, 38, 0);

	pbRedEnemy = m_lcd.AddProgressBar(LG_FILLED);
	m_lcd.SetProgressBarSize(pbRedEnemy, 58, 8);
	m_lcd.SetOrigin(pbRedEnemy, 98, 0);

	pbBlueFriendly = m_lcd.AddProgressBar(LG_FILLED);
	m_lcd.SetProgressBarSize(pbBlueFriendly, 58, 8);
	m_lcd.SetOrigin(pbBlueFriendly, 38, 9);

	pbBlueEnemy = m_lcd.AddProgressBar(LG_FILLED);
	m_lcd.SetProgressBarSize(pbBlueEnemy, 58, 8);
	m_lcd.SetOrigin(pbBlueEnemy, 98, 9);

	pbDragon = m_lcd.AddProgressBar(LG_FILLED);
	m_lcd.SetProgressBarSize(pbDragon, 118, 11);
	m_lcd.SetOrigin(pbDragon, 38, 18);

	pbBaron = m_lcd.AddProgressBar(LG_FILLED);
	m_lcd.SetProgressBarSize(pbBaron, 118, 11);
	m_lcd.SetOrigin(pbBaron, 38, 30);

	/***************/
    /* SECOND PAGE */
    /***************/
    m_lcd.AddNewPage();
    m_lcd.ModifyControlsOnPage(1);

	txtRed2 = m_lcd.AddText(LG_STATIC_TEXT, LG_TINY, DT_RIGHT, 36);
	m_lcd.SetOrigin(txtRed2, 0, 0);
	m_lcd.SetText(txtRed2, _T("Red Buff"));

	txtBlue2 = m_lcd.AddText(LG_STATIC_TEXT, LG_TINY, DT_RIGHT, 36);
	m_lcd.SetOrigin(txtBlue2, 0, 9);
	m_lcd.SetText(txtBlue2, _T("Blue Buff"));

	txtDragon2 = m_lcd.AddText(LG_STATIC_TEXT, LG_MEDIUM, DT_RIGHT, 36);
	m_lcd.SetOrigin(txtDragon2, 0, 18);
	m_lcd.SetText(txtDragon2, _T("Dragon"));

	txtBaron2 = m_lcd.AddText(LG_STATIC_TEXT, LG_MEDIUM, DT_RIGHT, 36);
	m_lcd.SetOrigin(txtBaron2, 0, 30);
	m_lcd.SetText(txtBaron2, _T("Baron"));

	txtRedFriendlyTimer = m_lcd.AddText(LG_STATIC_TEXT, LG_TINY, DT_CENTER, 58);
	m_lcd.SetOrigin(txtRedFriendlyTimer, 38, 0);
	m_lcd.SetText(txtRedFriendlyTimer, DEFAULT_COUNTDOWN_TEXT);

	txtRedEnemyTimer = m_lcd.AddText(LG_STATIC_TEXT, LG_TINY, DT_CENTER, 58);
	m_lcd.SetOrigin(txtRedEnemyTimer, 98, 0);
	m_lcd.SetText(txtRedEnemyTimer, DEFAULT_COUNTDOWN_TEXT);

	txtBlueFriendlyTimer = m_lcd.AddText(LG_STATIC_TEXT, LG_TINY, DT_CENTER, 58);
	m_lcd.SetOrigin(txtBlueFriendlyTimer, 38, 9);
	m_lcd.SetText(txtBlueFriendlyTimer, DEFAULT_COUNTDOWN_TEXT);

	txtBlueEnemyTimer = m_lcd.AddText(LG_STATIC_TEXT, LG_TINY, DT_CENTER, 58);
	m_lcd.SetOrigin(txtBlueEnemyTimer, 98, 9);
	m_lcd.SetText(txtBlueEnemyTimer, DEFAULT_COUNTDOWN_TEXT);

	txtDragonTimer = m_lcd.AddText(LG_STATIC_TEXT, LG_MEDIUM, DT_CENTER, 118);
	m_lcd.SetOrigin(txtDragonTimer, 38, 18);
	m_lcd.SetText(txtDragonTimer, DEFAULT_COUNTDOWN_TEXT);

	txtBaronTimer = m_lcd.AddText(LG_STATIC_TEXT, LG_MEDIUM, DT_CENTER, 118);
	m_lcd.SetOrigin(txtBaronTimer, 38, 30);
	m_lcd.SetText(txtBaronTimer, DEFAULT_COUNTDOWN_TEXT);


    //HBITMAP logoBitmap_ = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_LOGO));
    //m_logo = m_lcd.AddBitmap(55, 43);
    //m_lcd.SetBitmap(m_logo, logoBitmap_);
    //m_lcd.SetOrigin(m_logo, 0, 0);

    //m_right2 = m_lcd.AddIcon(rightIcon_, 16, 16);
    //m_lcd.SetOrigin(m_right2, 142, 35);
}

VOID LeagueOfLegendsHelperDlg::CheckButtonPresses()
{
    m_lcd.ModifyDisplay(LG_MONOCHROME);

    if (m_lcd.ButtonTriggered(LG_BUTTON_1))
        StartMatch();
	else if (m_lcd.ButtonTriggered(LG_BUTTON_2))
		EndMatch();
	else if (m_lcd.ButtonTriggered(LG_BUTTON_3))
		notify = !notify;
	else if (m_lcd.ButtonTriggered(LG_BUTTON_4))
		m_lcd.ShowPage((m_lcd.GetCurrentPageNumber() + 1) % m_lcd.GetPageCount());

	if(GetKeyState(VK_CONTROL) & 0x80 && GetKeyState(VK_SHIFT) & 0x80 && GetKeyState(VK_MENU) & 0x80)
	{
		if(GetKeyState(VK_F1) & 0x80)
			redFriendly.Trigger();
		else if(GetKeyState(VK_F2) & 0x80)
			redEnemy.Trigger();
		else if(GetKeyState(VK_F3) & 0x80)
			dragon.Trigger();
		else if(GetKeyState(VK_F4) & 0x80)
			blueFriendly.Trigger();
		else if(GetKeyState(VK_F5) & 0x80)
			blueEnemy.Trigger();
		else if(GetKeyState(VK_F6) & 0x80)
			baron.Trigger();
	}
}

VOID LeagueOfLegendsHelperDlg::UpdateTimers()
{
    m_lcd.ModifyDisplay(LG_MONOCHROME);

	if(started)
	{
		if (0 == m_lcd.GetCurrentPageNumber())
		{
			m_lcd.SetProgressBarPosition(pbRedFriendly, redFriendly.PercentComplete());
			m_lcd.SetProgressBarPosition(pbRedEnemy, redEnemy.PercentComplete());
			m_lcd.SetProgressBarPosition(pbBlueFriendly, blueFriendly.PercentComplete());
			m_lcd.SetProgressBarPosition(pbBlueEnemy, blueEnemy.PercentComplete());
			m_lcd.SetProgressBarPosition(pbDragon, dragon.PercentComplete());
			m_lcd.SetProgressBarPosition(pbBaron, baron.PercentComplete());
		}
		else if(1 == m_lcd.GetCurrentPageNumber())
		{
			redFriendly.UpdateCountdownText(m_lcd, txtRedFriendlyTimer);
			redEnemy.UpdateCountdownText(m_lcd, txtRedEnemyTimer);
			blueFriendly.UpdateCountdownText(m_lcd, txtBlueFriendlyTimer);
			blueEnemy.UpdateCountdownText(m_lcd, txtBlueEnemyTimer);
			dragon.UpdateCountdownText(m_lcd, txtDragonTimer);
			baron.UpdateCountdownText(m_lcd, txtBaronTimer);
		}

		if(notify)
		{
			redFriendly.CheckMessages();
			redEnemy.CheckMessages();
			blueFriendly.CheckMessages();
			blueEnemy.CheckMessages();
			dragon.CheckMessages();
			baron.CheckMessages();
		}
	}
}

VOID LeagueOfLegendsHelperDlg::StartMatch()
{
	redFriendly.Start();
	redEnemy.Start();
	blueFriendly.Start();
	blueEnemy.Start();
	dragon.Start();
	baron.Start();

	started = true;
}

VOID LeagueOfLegendsHelperDlg::EndMatch()
{
	started = false;
}

VOID LeagueOfLegendsHelperDlg::FlashColors()
{
	colors = true;
	int hue, lum, sat;
	lum = 120;
	sat = 240;

	for (hue = 0; hue < 240; hue++)
	{
		COLORREF rgbColor = ::ColorHLSToRGB(hue, lum, sat);
		int red = GetRValue(rgbColor);
		int green = GetGValue(rgbColor);
		int blue = GetBValue(rgbColor);
		m_lcd.SetBackground(rgbColor);
		m_lcd.Update();
		Sleep(static_cast<DWORD>(10));
	}
	colors = false;
}