#include "stdafx.h"
#include "share_select_apps_wnd_ui.h"
#include "main_frame.h"
#include "share_app_layout_ui.h"
#include "videosdk_demo_mgr.h"

ShareSelectAppsWndUI::ShareSelectAppsWndUI():share_app_layout_(nullptr)
{

}

ShareSelectAppsWndUI::~ShareSelectAppsWndUI()
{

}

CControlUI* ShareSelectAppsWndUI::CreateControl(LPCTSTR pstrClass)
{
	if (_tcscmp(pstrClass, _T("ShareAppLayout")) == 0)
	{
		return new ShareAppLayoutUI();
	}

	return nullptr;
}

void ShareSelectAppsWndUI::InitControls()
{
	share_app_layout_ = static_cast<ShareAppLayoutUI*>(m_PaintManager.FindControl(_T("all_apps_layout")));
}

void ShareSelectAppsWndUI::InitWindow()
{
	InitControls();
}

CDuiString ShareSelectAppsWndUI::GetSkinFolder()
{
	return _T("");
}

CDuiString ShareSelectAppsWndUI::GetSkinFile()
{
	return to_wstring(IDXML_SHARE_SELECT_APPS_WND_UI).c_str();
}

UILIB_RESOURCETYPE ShareSelectAppsWndUI::GetResourceType() const
{
	return UILIB_RESOURCE;
}

LRESULT ShareSelectAppsWndUI::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void ShareSelectAppsWndUI::UpdateAllApps()
{
	if (share_app_layout_)
	{
		share_app_layout_->UpdateAppItem();
	}
}

void ShareSelectAppsWndUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btn_close"))
		{
			DoCloseBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_cancel"))
		{
			DoCancelBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_share"))
		{
			DoShareBtnClick();
		}
	}
}

LRESULT ShareSelectAppsWndUI::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ShowWindow(false);
	return __super::OnKillFocus(uMsg, wParam, lParam, bHandled);
}

void ShareSelectAppsWndUI::DoCloseBtnClick()
{
	CMainFrame::GetInstance().ShowShareSelectAppsWnd(false);
}
void ShareSelectAppsWndUI::DoCancelBtnClick()
{
	CMainFrame::GetInstance().ShowShareSelectAppsWnd(false);
}

void ShareSelectAppsWndUI::DoShareBtnClick()
{

	AppInfo select_app = share_app_layout_->GetSelectItemAppInfo();

	switch (select_app.share_type)
	{
	case kScreenShare:
	{
		CMainFrame::GetInstance().StartScreenShare(select_app.device);
		break;
	}
	case kWindowShare:
	{
		CMainFrame::GetInstance().StartShareView(reinterpret_cast<HWND>(select_app.id));
		break;
	}
	case kSecondCameraShare:
	{
		CMainFrame::GetInstance().StartShare2ndCamera();
		break;
	}
	default:
		break;
	}
}