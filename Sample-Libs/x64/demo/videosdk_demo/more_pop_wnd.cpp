#include "stdafx.h"
#include "more_pop_wnd.h"
#include "main_frame.h"

CMorePopWnd::CMorePopWnd()
{
}

CMorePopWnd::~CMorePopWnd()
{
}


void CMorePopWnd::InitControl()
{	
}

void CMorePopWnd::InitWindow()
{	
	InitControl();
}

void CMorePopWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btn_feedback"))
		{
			DoFeedBackBtnClick();
		}
		this->ShowWindow(false);
	}

	__super::Notify(msg);
}

LRESULT CMorePopWnd::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::OnSetFocus(uMsg, wParam, lParam, bHandled);
}

LRESULT CMorePopWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	this->ShowWindow(false);
	return __super::OnKillFocus(uMsg, wParam, lParam, bHandled);
}

void CMorePopWnd::DoFeedBackBtnClick()
{
	CMainFrame::GetInstance().ShowFeedBackSubmitWnd(true);
}