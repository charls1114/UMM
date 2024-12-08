#include "stdafx.h"
#include "feedback_tips_wnd.h"

CFeedBackTipsWnd::CFeedBackTipsWnd()
{
}

CFeedBackTipsWnd::~CFeedBackTipsWnd()
{
}

LRESULT CFeedBackTipsWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_TIMER:
	{
		break;
	}
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}


void CFeedBackTipsWnd::InitControl()
{
	btn_close_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_close")));
	
}

void CFeedBackTipsWnd::InitWindow()
{	
	InitControl();	
}

LRESULT CFeedBackTipsWnd::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::OnSetFocus(uMsg, wParam, lParam, bHandled);
}

LRESULT CFeedBackTipsWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	this->ShowWindow(false);
	return __super::OnKillFocus(uMsg, wParam, lParam, bHandled);
}

void CFeedBackTipsWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btn_close"))
		{
			DoCloseBtnClick();
		}		
	}

	__super::Notify(msg);
}

void CFeedBackTipsWnd::DoCloseBtnClick()
{
	this->ShowWindow(false);
}