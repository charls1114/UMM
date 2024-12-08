#include "stdafx.h"
#include "gallery_more_pop_wnd_ui.h"
#include "main_frame.h"
#include "videosdk_demo_mgr.h"

CGalleryMorePopWnd::CGalleryMorePopWnd()
{
	m_is_show_stop = false;
}

CGalleryMorePopWnd::~CGalleryMorePopWnd()
{
	m_is_show_stop = false;
}


void CGalleryMorePopWnd::InitControl()
{	
	btn_request_or_stop_camera_control_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_request_or_stop_camera_control")));
	btn_remove_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_remove")));
}

void CGalleryMorePopWnd::InitWindow()
{	
	InitControl();
}                                                                                                                                       

void CGalleryMorePopWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btn_request_or_stop_camera_control"))
		{
			DoRequestOrStopCameraControlBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_remove"))
		{
			DoRemoveBtnClick();
		}
		this->ShowWindow(false);
	}

	__super::Notify(msg);
}

LRESULT CGalleryMorePopWnd::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::OnSetFocus(uMsg, wParam, lParam, bHandled);
}

LRESULT CGalleryMorePopWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	this->ShowWindow(false);
	return __super::OnKillFocus(uMsg, wParam, lParam, bHandled);
}

void CGalleryMorePopWnd::DoRequestOrStopCameraControlBtnClick()
{
	if (!m_is_show_stop)
	{
		ZoomVideoSDKErrors err = ZoomVideoSDKErrors_Unknown;
		err = ZoomVideoSDKMgr::GetInst().RequestControlRemoteCamera();
		if (err == ZoomVideoSDKErrors_Success && ZoomVideoSDKMgr::GetInst().GetRemoteControlCameraUser())
		{
			std::wstring selected_username = ZoomVideoSDKMgr::GetInst().GetRemoteControlCameraUser()->getUserName();
			std::wstring str_message = _T("Requesting camera control from ") + selected_username + _T(" ...");
			CMainFrame::GetInstance().ShowTopTipWnd(true, str_message);
		}
	}
	else
	{
		CMainFrame::GetInstance().ShowStopCameraControlWnd(true);
	}
	
}

void CGalleryMorePopWnd::DoRemoveBtnClick()
{
	IZoomVideoSDKUser* selected_user = CMainFrame::GetInstance().GetGalleryContainerSelectUser();
	ZoomVideoSDKMgr::GetInst().RemoveUser(selected_user);
}

void CGalleryMorePopWnd::UpdateUIForCameraControlStatus(bool isShowStop)
{
	if (!btn_request_or_stop_camera_control_)
		return;
	DWORD textcolor = 0XED4D6A;
	if (isShowStop)
	{
		btn_request_or_stop_camera_control_->SetText(_T("Stop Camera Control"));
		
	}
	else
	{
		textcolor = 0XFFFFFF;
		btn_request_or_stop_camera_control_->SetText(_T("Request Camera Control"));
	}
	btn_request_or_stop_camera_control_->SetTextColor(textcolor);
	m_is_show_stop = isShowStop;
}
void CGalleryMorePopWnd::UpdateUIForRemove(bool isShowRemove)
{
	if (!btn_remove_)
		return;
	if (!isShowRemove)
	{
		ResizeClient(180, 42);
	}
	else
	{
		ResizeClient(180, 70);
	}
	btn_remove_->SetVisible(isShowRemove);
}