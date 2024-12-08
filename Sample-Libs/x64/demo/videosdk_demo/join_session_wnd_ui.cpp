#include "stdafx.h"
#include "join_session_wnd_ui.h"
#include "main_frame.h"
#include "session_settings_wnd_ui.h"
#include "zoom_videosdk_renderer_api.h"

#define HIDE_INVALID_PARAMTER_LABEL 1001
#define HIDE_WRONG_PASSWORD_LABEL   1002
#define HIDE_YOUR_NAME_ERROR_TIP    1003

CJoinSessionWndUI::CJoinSessionWndUI()
{

}

CJoinSessionWndUI::~CJoinSessionWndUI()
{

}

void CJoinSessionWndUI::InitControls()
{
	tab_layout_ = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("join_session_tablayout")));

	edit_session_name_ = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("edit_session_name")));
	edit_user_name_ = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("edit_your_name")));
	edit_session_psw_ = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("edit_password")));
	btn_setting_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_settings")));
	btn_join_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_join")));
	btn_create_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_create")));
	label_join_create_title_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_join_create_a_session")));
	check_box_turn_off_video_ = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chk_turn_off_video")));
	check_box_mute_audio_ = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chk_mute_audio")));

	join_create_session_layout_ = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("join_create_a_session")));
	init_layout_ = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("init_layout")));
	error_layout_ = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("error_layout")));
	label_invalid_paramter_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_session_error")));
	label_error_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("error_text")));
	btn_error_ok = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_error_ok")));
	label_your_name_error_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_your_name_error")));
	labe_password_error_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_password_error")));
}

void CJoinSessionWndUI::InitWindow()
{
	InitControls();

	SetIcon(IDI_ICON_MAIN);
}

CDuiString CJoinSessionWndUI::GetSkinFolder()
{
	return _T("");
}

CDuiString CJoinSessionWndUI::GetSkinFile()
{
	return to_wstring(IDXML_JOIN_SESSION_WND_UI).c_str();
}

UILIB_RESOURCETYPE CJoinSessionWndUI::GetResourceType() const
{
	return UILIB_RESOURCE;
}

LRESULT CJoinSessionWndUI::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CJoinSessionWndUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("close"))
		{
			DoCloseBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_join_session"))
		{
			DoJoinSessionBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_create_session"))
		{
			DoCreateSessionBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_back"))
		{
			DoBackBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_join"))
		{
			DoJoinBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_create"))
		{
			DoJoinBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_settings"))
		{
			DoSettingBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_error_ok"))
		{
			DoErrorOkBtnClick();
		}
	}
	__super::Notify(msg);
}

LRESULT CJoinSessionWndUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_TIMER:
		{
			OnTimer(uMsg, wParam, lParam);
			break;
		}
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

void CJoinSessionWndUI::DoJoinSessionBtnClick()
{
	CSessionSettingsWndUI* setting_wnd = CMainFrame::GetInstance().GetSessionSettingsWndUI();

	if (!setting_wnd)
		return;

	token_ = setting_wnd->GetToken();
	if (token_.empty())
	{
		DoSettingBtnClick();
		return;
	}

	setting_wnd->ShowWindow(false);

	if (btn_setting_)
	{
		btn_setting_->SetVisible(false);
	}

	VideoSDKRendererType renderer_type = setting_wnd->GetRendererType();
	VideoSDKRenderInitParam init_param;
	init_param.frame_rate = 30;
	init_param.renderer_type = renderer_type;

	VideoSDKRenderInitErrorType error = InitZoomVideoSDKRendererModule(init_param);

	ShowJoinOrCreateSessionPage(true);
}

void CJoinSessionWndUI::DoCreateSessionBtnClick()
{
	CSessionSettingsWndUI* setting_wnd = CMainFrame::GetInstance().GetSessionSettingsWndUI();

	if (!setting_wnd)
		return;

	token_ = setting_wnd->GetToken();
	if (token_.empty())
	{
		DoSettingBtnClick();
		return;
	}

	setting_wnd->ShowWindow(false);

	if (btn_setting_)
	{
		btn_setting_->SetVisible(false);
	}
	VideoSDKRendererType renderer_type = setting_wnd->GetRendererType();
	VideoSDKRenderInitParam init_param;
	init_param.frame_rate = 30;
	init_param.renderer_type = renderer_type;

	VideoSDKRenderInitErrorType error = InitZoomVideoSDKRendererModule(init_param);
	ShowJoinOrCreateSessionPage(false);
}

void CJoinSessionWndUI::DoJoinBtnClick()
{
	if (!edit_session_name_ || !edit_session_psw_ || !edit_user_name_
		||!check_box_turn_off_video_ || !check_box_mute_audio_)
		return;

	//session name
	session_name_ = edit_session_name_->GetText();
	//user name
	CDuiString sUserName = edit_user_name_->GetText();
	if (sUserName.IsEmpty())
	{
		if (label_your_name_error_)
		{
			label_your_name_error_->SetVisible(true);
			SetTimer(m_hWnd, HIDE_YOUR_NAME_ERROR_TIP, 100, NULL);
		}
		return;
	}
	//password
	session_password_ = edit_session_psw_->GetText();
	//is turn off video
	bool is_video_on = !check_box_turn_off_video_->GetCheck();
	//is mute audio
	bool is_mute_audio = check_box_mute_audio_->GetCheck();

	ZoomVideoSDKSessionContext session_context;
	session_context.sessionName = session_name_;
	session_context.sessionPassword = session_password_;
	session_context.userName = sUserName;
	session_context.token = token_.c_str();
	session_context.videoOption.localVideoOn = is_video_on;
	session_context.audioOption.connect = true;
	session_context.audioOption.mute = is_mute_audio;
	
	CMainFrame::GetInstance().JoinSession(session_context);
}

void CJoinSessionWndUI::DoCloseBtnClick()
{
	CMainFrame::GetInstance().OnJoinSessionCloseBtnClick();
}

void CJoinSessionWndUI::DoSettingBtnClick()
{
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	POINT ptLeftTop = { rc.left,rc.top };
	POINT ptRightBottom = { rc.right,rc.bottom };
	ClientToScreen(m_hWnd, &ptLeftTop);
	ClientToScreen(m_hWnd, &ptRightBottom);
	RECT rcScreen = { ptLeftTop.x,ptLeftTop.y,ptRightBottom.x,ptRightBottom.y };

	CMainFrame::GetInstance().ShowSessionSettingWnd(true,rcScreen);
}

void CJoinSessionWndUI::DoBackBtnClick()
{
	ShowInitPage();
}

void CJoinSessionWndUI::DoErrorOkBtnClick()
{
	ShowJoinOrCreateSessionPage(true);

	KillTimer(m_hWnd, HIDE_WRONG_PASSWORD_LABEL);
}

void CJoinSessionWndUI::ShowJoinOrCreateSessionPage(bool is_join)
{
	if (tab_layout_ && join_create_session_layout_)
	{
		tab_layout_->SelectItem(join_create_session_layout_);
	}

	ShowBtnJoinOrCreate(is_join);
	
	if (edit_session_name_)
	{
		edit_session_name_->SetFocus();
	}
}

void CJoinSessionWndUI::ShowInitPage()
{
	if (btn_setting_)
	{
		btn_setting_->SetVisible(true);
	}

	if (tab_layout_ && init_layout_)
	{
		tab_layout_->SelectItem(init_layout_);
	}
}

void CJoinSessionWndUI::ShowErrorPage()
{
	if (tab_layout_ && init_layout_)
	{
		tab_layout_->SelectItem(error_layout_);
	}
}

void CJoinSessionWndUI::ShowBtnJoinOrCreate(bool show_join)
{
	if (!btn_join_ || !btn_create_ ||!label_join_create_title_)
		return;

	if (edit_session_name_)
	{
		edit_session_name_->SetText(_T(""));
	}

	if (edit_user_name_)
	{
		edit_user_name_->SetText(_T(""));
	}

	if (edit_session_psw_)
	{
		edit_session_psw_->SetText(_T(""));
	}

	if (label_invalid_paramter_)
	{
		label_invalid_paramter_->SetVisible(false);
	}

	if (label_your_name_error_)
	{
		label_your_name_error_->SetVisible(false);
	}

	if (labe_password_error_)
	{
		labe_password_error_->SetVisible(false);
	}

	btn_join_->SetVisible(show_join);
	btn_create_->SetVisible(!show_join);

	if (show_join)
	{
		label_join_create_title_->SetText(_T("Join a Session"));
	}
	else
	{
		label_join_create_title_->SetText(_T("Create a Session"));
	}
}

void CJoinSessionWndUI::OnInvalidParamterNotification()
{
	if (label_invalid_paramter_)
		label_invalid_paramter_->SetVisible(true);

	SetTimer(m_hWnd, HIDE_INVALID_PARAMTER_LABEL, 100, NULL);
}

void CJoinSessionWndUI::OnJoinPasswordWrong()
{
	ShowErrorPage();
	if (label_error_)	
		label_error_->SetText(_T("Wrong password.Please enter again."));

	SetTimer(m_hWnd, HIDE_WRONG_PASSWORD_LABEL, 5000, NULL);
}

void CJoinSessionWndUI::BackToInitPage()
{
	if (!error_layout_ || !tab_layout_)
		return;

	if (error_layout_ != tab_layout_->GetItemAt(tab_layout_->GetCurSel())) //current page is not error page,back to init page.
	{
		ShowInitPage();
	}
}

bool CJoinSessionWndUI::IsSelectedVideoOff()
{
	if (check_box_turn_off_video_)
	{
		return check_box_turn_off_video_->GetCheck();
	}

	return false;
}

wstring CJoinSessionWndUI::FormatString(wstring str, int error_code)
{
	TCHAR temp_str[256] = { 0 };
	wsprintf(temp_str, _T("%s(Error Code: %d)"), str.c_str(),error_code);   

	return temp_str;
}

void CJoinSessionWndUI::OnJoinMettingJoinFailed(int error_code)
{
	if (label_error_)
	{
		wstring str_error = FormatString(_T("Unable to join or create session.Please try again."), error_code);
		label_error_->SetText(str_error.c_str());
	}

	ShowErrorPage();
}

void CJoinSessionWndUI::JudgeHideInvalidParamter()
{
	if (!edit_session_name_ || !label_invalid_paramter_)
		return;

	if (session_name_ != edit_session_name_->GetText())
	{
		label_invalid_paramter_->SetVisible(false);
		KillTimer(m_hWnd, HIDE_INVALID_PARAMTER_LABEL);
	}
}

void CJoinSessionWndUI::JudgeHideWrongPWDTip()
{
	ShowJoinOrCreateSessionPage(true);
	KillTimer(m_hWnd, HIDE_WRONG_PASSWORD_LABEL);
}

void CJoinSessionWndUI::JudgeHideNameEmptyErrorTip()
{
	if (!label_your_name_error_ || !edit_user_name_)
		return;

	if (!(edit_user_name_->GetText().IsEmpty()))
	{
		label_your_name_error_->SetVisible(false);
		KillTimer(m_hWnd, HIDE_YOUR_NAME_ERROR_TIP);
	}
}

void CJoinSessionWndUI::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (wParam == HIDE_INVALID_PARAMTER_LABEL)
	{
		JudgeHideInvalidParamter();
	}
	else if (wParam == HIDE_WRONG_PASSWORD_LABEL)
	{
		JudgeHideWrongPWDTip();
	}
	else if (wParam == HIDE_YOUR_NAME_ERROR_TIP)
	{
		JudgeHideNameEmptyErrorTip();
	}
}