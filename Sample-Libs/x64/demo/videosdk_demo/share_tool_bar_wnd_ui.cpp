#include "stdafx.h"
#include "share_tool_bar_wnd_ui.h"
#include "main_frame.h"
#include "videosdk_demo_mgr.h"

ShareToolBarWndUI::ShareToolBarWndUI():stop_share_btn_(nullptr)
{

}

ShareToolBarWndUI::~ShareToolBarWndUI()
{

}

void ShareToolBarWndUI::InitControls()
{
	stop_share_btn_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_stop_share")));
	btn_muted_audio_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_muted_audio")));
	btn_unmuted_audio_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_unmuted_audio")));
	btn_muted_video_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_muted_video")));
	btn_unmuted_video_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_unmuted_video")));
	btn_participant_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_participant")));
	label_participants_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_participant_num")));
}

void ShareToolBarWndUI::InitWindow()
{
	InitControls();

	AddNoHitTestControls(stop_share_btn_);
	AddNoHitTestControls(btn_muted_audio_);
	AddNoHitTestControls(btn_unmuted_audio_);
	AddNoHitTestControls(btn_muted_video_);
	AddNoHitTestControls(btn_unmuted_video_);
	AddNoHitTestControls(btn_participant_);
	AddNoHitTestControls(label_participants_);
}

CDuiString ShareToolBarWndUI::GetSkinFolder()
{
	return _T("");
}

CDuiString ShareToolBarWndUI::GetSkinFile()
{
	return to_wstring(IDXML_SHARE_TOOL_BAR_WND_UI).c_str();
}

UILIB_RESOURCETYPE ShareToolBarWndUI::GetResourceType() const
{
	return UILIB_RESOURCE;
}

LRESULT ShareToolBarWndUI::ResponseDefaultKeyEvent(WPARAM wParam)
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

void ShareToolBarWndUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender == stop_share_btn_)
		{
			DoStopShareBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_muted_audio"))
		{
			DoMutedAudioBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_unmuted_audio"))
		{
			DoUnmutedAudioBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_muted_video"))
		{
			DoMutedVideoBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_unmuted_video"))
		{
			DoUnmutedVideoBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_participant"))
		{
			DoParticipantBtnClick();
		}
	}

	__super::Notify(msg);
}

LRESULT	ShareToolBarWndUI::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (::GetAsyncKeyState(MK_LBUTTON) < 0)
	{
		RECT rect_client;
		GetClientRect(m_hWnd, &rect_client);

		POINT pt;
		GetCursorPos(&pt);
		::ScreenToClient(m_PaintManager.GetPaintWindow(), &pt);
		if (!IsHitInNoHitTestControls(pt))
		{
			if (::PtInRect(&rect_client, pt))
			{
				return HTCAPTION;
			}
		}
	}

	return __super::OnNcHitTest(uMsg, wParam, lParam, bHandled);
}

void ShareToolBarWndUI::AddNoHitTestControls(CControlUI* controls)
{
	if (controls)
	{
		vect_no_hittest_controls_.push_back(controls);
	}
}

bool ShareToolBarWndUI::IsHitInNoHitTestControls(const POINT& pt)
{
	for (size_t i = 0; i < vect_no_hittest_controls_.size(); ++i)
	{
		RECT rect(vect_no_hittest_controls_[i]->GetPos());
		if (::PtInRect(&rect, pt))
			return true;
	}

	return false;
}

void ShareToolBarWndUI::DoMutedAudioBtnClick()
{
	ZoomVideoSDKMgr::GetInst().UnmuteAudio();
}

void ShareToolBarWndUI::DoUnmutedAudioBtnClick()
{
	ZoomVideoSDKMgr::GetInst().MuteAudio();
}

void ShareToolBarWndUI::DoMutedVideoBtnClick()
{
	ZoomVideoSDKErrors err = ZoomVideoSDKMgr::GetInst().UnmuteVideo();
	if (err == ZoomVideoSDKErrors_Session_Video_Error)
	{
		CMainFrame::GetInstance().ShowMessageTipWnd(true, _T("Please select another video camera in settings."));
	}
}

void ShareToolBarWndUI::DoUnmutedVideoBtnClick()
{
	ZoomVideoSDKMgr::GetInst().MuteVideo();
}

void ShareToolBarWndUI::DoParticipantBtnClick()
{
	if (CMainFrame::GetInstance().IsParticipantGalleryShown())
		CMainFrame::GetInstance().ShowParticipantGallery(false);
	else
		CMainFrame::GetInstance().ShowParticipantGallery(true);
}

void ShareToolBarWndUI::DoStopShareBtnClick()
{
	CMainFrame::GetInstance().StopShare();
}

void ShareToolBarWndUI::UpdateParticipantNum()
{
	if (nullptr == label_participants_)
		return;

	int user_count = ZoomVideoSDKMgr::GetInst().GetUserCountInSession();

	if (user_count >= 0)
		label_participants_->SetVisible(true);
	else
		label_participants_->SetVisible(false);

	TCHAR szNum[64] = { 0 };
	wsprintf(szNum, _T("%d"), user_count);  
	label_participants_->SetText(szNum);
}

void ShareToolBarWndUI::UpdateUIForAudioStatus(bool is_audio_on)
{
	if (!btn_muted_audio_ || !btn_unmuted_audio_)
		return;

	btn_muted_audio_->SetVisible(!is_audio_on);
	btn_unmuted_audio_->SetVisible(is_audio_on);
}

void ShareToolBarWndUI::UpdateUIForVideoStatus(bool is_video_on)
{
	if (!btn_muted_video_ || !btn_unmuted_video_)
		return;

	btn_muted_video_->SetVisible(!is_video_on);
	btn_unmuted_video_->SetVisible(is_video_on);
}