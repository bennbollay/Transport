//	SelectShipScreen.cpp
//
//	Show select ship screen

#include "PreComp.h"
#include "Transcendence.h"
#include "XMLUtil.h"

#include "SDL.h"

const COLORREF RGB_TOP_BAR =					CG16bitImage::RGBValue(0, 2, 10);

const int INTRO_DISPLAY_WIDTH =				1024;
const int INTRO_DISPLAY_HEIGHT =			512;

void CTranscendenceWnd::AnimateSelectShip (void)

//	AnimateSelectShip
//
//	Paint the select ship screen

	{
	//	Paint the top section, which is blank

	m_Screen.Fill(m_rcIntroTop.left,
			m_rcIntroTop.top,
			RectWidth(m_rcIntroTop),
			RectHeight(m_rcIntroTop),
			RGB_TOP_BAR);

	//	Paint displays

	m_ButtonBarDisplay.Update();
	m_ButtonBarDisplay.Paint(m_Screen);

	m_ShipClassDisplay.Update();
	m_ShipClassDisplay.Paint(m_Screen);

	//	Update the screen

	BltScreen();
	}

void CTranscendenceWnd::OnCharSelectShip (char chChar, DWORD dwKeyData)

//	OnCharSelectShip
//
//	Handle OnChar

	{
	if (m_ButtonBarDisplay.OnChar(chChar))
		return;
	}

void CTranscendenceWnd::OnDblClickSelectShip (int x, int y, DWORD dwFlags)

//	OnDblClickSelectShip
//
//	Handle double-click

	{
	if (m_ButtonBarDisplay.OnLButtonDoubleClick(x, y))
		return;
	}

void CTranscendenceWnd::OnKeyDownSelectShip (int iVirtKey, DWORD dwKeyData)

//	OnKeyDownSelectShip
//
//	Handle OnKeyDown

	{
	if (m_ButtonBarDisplay.OnKeyDown(iVirtKey))
		return;

	if (m_ShipClassDisplay.OnKeyDown(iVirtKey))
		return;

	switch (iVirtKey)
		{
		case SDLK_ESCAPE:
			DoCommand(CMD_SELECT_SHIP_CANCEL);
			break;

		case SDLK_RETURN:
			DoCommand(CMD_SELECT_SHIP);
			break;
		}
	}

void CTranscendenceWnd::OnLButtonDownSelectShip (int x, int y, DWORD dwFlags)

//	OnLButtonDownSelectShip
//
//	Handle OnLButtonDown

	{
	if (m_ButtonBarDisplay.OnLButtonDown(x, y))
		return;
	}

void CTranscendenceWnd::OnMouseMoveSelectShip (int x, int y, DWORD dwFlags)

//	OnMouseMoveSelectShip
//
//	Handle OnMouseMove

	{
	m_ButtonBarDisplay.OnMouseMove(x, y);
	}

ALERROR CTranscendenceWnd::StartSelectShip (void)

//	StartSelectShip
//
//	Show the select ship screen

	{
	RECT rcRect;

	//	Use widescreen topology

	int cyBarHeight = (g_cyScreen - INTRO_DISPLAY_HEIGHT) / 2;
	m_rcIntroTop.top = 0;
	m_rcIntroTop.left = 0;
	m_rcIntroTop.bottom = cyBarHeight;
	m_rcIntroTop.right = g_cxScreen;

	m_rcIntroMain.top = cyBarHeight;
	m_rcIntroMain.left = 0;
	m_rcIntroMain.bottom = g_cyScreen - cyBarHeight;
	m_rcIntroMain.right = g_cxScreen;

	m_rcIntroBottom.top = g_cyScreen - cyBarHeight;
	m_rcIntroBottom.left = 0;
	m_rcIntroBottom.bottom = g_cyScreen;
	m_rcIntroBottom.right = g_cxScreen;

	//	Create the buttons

	m_ButtonBar.Init();

	m_ButtonBar.AddButton(CMD_PREV_SHIP,
			NULL_STR,
			NULL_STR,
			NULL_STR,
			8,
			CButtonBarData::alignCenter,
			CButtonBarData::styleMedium);

	m_ButtonBar.AddButton(CMD_SELECT_SHIP,
			CONSTLIT("Select Ship"),
			CONSTLIT("Select this ship"),
			CONSTLIT("S"),
			3,
			CButtonBarData::alignCenter);

	m_ButtonBar.AddButton(CMD_NEXT_SHIP,
			NULL_STR,
			NULL_STR,
			NULL_STR,
			9,
			CButtonBarData::alignCenter,
			CButtonBarData::styleMedium);

	m_ButtonBar.AddButton(CMD_SELECT_SHIP_CANCEL,
			CONSTLIT("Cancel"),
			CONSTLIT("Cancel New Game"),
			CONSTLIT("C"),
			5,
			CButtonBarData::alignRight);

	m_ButtonBarDisplay.SetFontTable(&m_Fonts);
	m_ButtonBarDisplay.Init(this, &m_ButtonBar, m_rcIntroBottom);

	//	Create the ship class display

	rcRect = m_rcIntroTop;
	rcRect.bottom = m_rcIntroBottom.top;
	m_ShipClassDisplay.Init(this, rcRect, m_Options.bDebugGame);

	//	Done

	m_State = gsSelectShip;
	ShowCursor(true);

	return NOERROR;
	}

void CTranscendenceWnd::StopSelectShip (void)

//	StopSelectShip
//
//	Stop select ship screen

	{
	ASSERT(m_State == gsSelectShip);

	m_ShipClassDisplay.CleanUp();
	m_ButtonBarDisplay.CleanUp();
	m_ButtonBar.CleanUp();

	//	Hide cursor

	ShowCursor(false);
	}

