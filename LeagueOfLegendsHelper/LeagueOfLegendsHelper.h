#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CColorAndMonoApp:
// See ColorAndMono.cpp for the implementation of this class
//

class LeagueOfLegendsHelperApp : public CWinApp
{
public:
    LeagueOfLegendsHelperApp();

    // Overrides
public:
    virtual BOOL InitInstance();
    virtual INT ExitInstance();

    // Implementation

    DECLARE_MESSAGE_MAP()

protected:
    HANDLE m_hMutex;
    ULONG_PTR m_gdiplusToken;
    HANDLE ClaimMutex(LPCTSTR szMutex, LPCTSTR szWndClassName);
};

extern LeagueOfLegendsHelperApp theApp;