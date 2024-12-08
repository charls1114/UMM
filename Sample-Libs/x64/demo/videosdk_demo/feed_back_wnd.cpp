#include "stdafx.h"
#include "feed_back_wnd.h"
#include "utils.h"
#include "main_frame.h"

#define HIDE_SUBMIT_WND_TIMER    1005
#define RESET_LAUNCH_BTN_TIMER    1006

void CFeedBackLaunchWnd::InitWindow()
{
	InitControl();
}

void CFeedBackLaunchWnd::InitControl()
{
	btn_feedback_launch_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_feedback_launch")));		
	btn_feedback_close_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_feedback_close")));
	pushed_layout_ = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("pushed_layout")));
	launch_layout_main_ = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("launch_layout_main")));
	lable_responses_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lable_responses")));
}

LRESULT CFeedBackLaunchWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

void CFeedBackLaunchWnd::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (wParam == RESET_LAUNCH_BTN_TIMER)
	{
		SwitchLaunchPushedBtn(true);
	}
}

void CFeedBackLaunchWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender == btn_feedback_launch_)
			OnLaunchFeebBackBtnClick();
		else if (msg.pSender == btn_feedback_close_)
			OnCloseFeebBackBtnClick();
	}
}

void CFeedBackLaunchWnd::SwitchLaunchPushedBtn(bool is_show_launch)
{
	if (!btn_feedback_launch_ || !pushed_layout_)
		return;

	btn_feedback_launch_->SetVisible(is_show_launch);
	pushed_layout_->SetVisible(!is_show_launch);

	if (!is_show_launch)
		SetTimer(m_hWnd, RESET_LAUNCH_BTN_TIMER, 60 * 1000, NULL);
}

void CFeedBackLaunchWnd::OnSessionJoin()
{
	InitFeedBackLaunch();
	CMainFrame::GetInstance().UpdateFeedBackNum(0);
}

void CFeedBackLaunchWnd::InitFeedBackLaunch()
{
	SwitchLaunchPushedBtn(true);

	feedback_total_usernum = 0;
	feedback_user_map_.clear();

	UpdateFeedBackView();
}

void CFeedBackLaunchWnd::OnParseChannelCmd(IZoomVideoSDKUser* pSender, const std::vector<std::wstring >& cmdVector)
{
	if (cmdVector.size() < 2 || !pSender)
		return;

	std::wstring strCmdType = cmdVector[0];
	std::wstring strFeedBack = cmdVector[1];
	

	CmdChannelType nCmdType = (CmdChannelType)_ttoi(strCmdType.c_str());
	if (CmdChannelType::CMD_FEEDBACK_SUBMIT == nCmdType)
	{
		std::wstring userId = pSender->getUserID();

		if (feedback_user_map_.count(userId) > 0)
			feedback_user_map_[userId] = strFeedBack;
		else		
			feedback_user_map_.insert(make_pair(userId, strFeedBack));
		
		UpdateFeedBackView();
		CMainFrame::GetInstance().UpdateFeedBackNum(feedback_user_map_.size());
	}
}

void CFeedBackLaunchWnd::SetFeedBackUserNum()
{
	if (!launch_layout_main_)
		return;

	feedback_total_usernum = feedback_user_map_.size();
	for (int i = 0; i < launch_layout_main_->GetCount(); i++)
	{
		CVerticalLayoutUI* pItemUI = (CVerticalLayoutUI*)launch_layout_main_->GetItemAt(i);
		if (!pItemUI)
			continue;

		CProgressUI* pVoteProgress = static_cast<CProgressUI*>(pItemUI->FindSubControl(_T("vote_progress")));
		if (pVoteProgress)
			pVoteProgress->SetMaxValue(feedback_total_usernum);				
	}
}

void CFeedBackLaunchWnd::UpdateFeedBackView()
{
	if (!launch_layout_main_ || !lable_responses_)
		return;

	CDuiString strResponses;
	strResponses.Format(_T("%d responses"), feedback_user_map_.size());
	lable_responses_->SetText(strResponses);

	map<std::wstring, int>	feedback_result_map;
	feedback_result_map.clear();
	for (auto iter = feedback_user_map_.begin(); iter != feedback_user_map_.end(); iter++)
	{
		if (feedback_result_map.count(iter->second) > 0)
		{
			auto result_iter = feedback_result_map.find(iter->second);
			result_iter->second++;
		}
		else
			feedback_result_map.insert(make_pair(iter->second,1));
	}

	int itemCount = launch_layout_main_->GetCount();
	for (int i = 0; i < itemCount; i++)
	{
		CVerticalLayoutUI* pItemUI = (CVerticalLayoutUI*)launch_layout_main_->GetItemAt(i);
		if (!pItemUI)
			continue;

		std::wstring item_userdata = pItemUI->GetUserData().GetData();
		
		int progress_value = 0;
		if (feedback_result_map.count(item_userdata) > 0)
		{
			auto item_iter = feedback_result_map.find(item_userdata);			
			progress_value = item_iter->second;
		}
		
		CProgressUI* pVoteProgress = static_cast<CProgressUI*>(pItemUI->FindSubControl(_T("vote_progress")));
		if (pVoteProgress)
		{
			pVoteProgress->SetMaxValue(feedback_user_map_.size());
			pVoteProgress->SetValue(progress_value);
		}

		if (feedback_user_map_.size() == 0)
			continue;

		int percent = 100 * ((float)progress_value / (float)feedback_user_map_.size());
		CDuiString strPercent;

		CLabelUI* pVotePercentLabel = static_cast<CProgressUI*>(pItemUI->FindSubControl(_T("vote_item_percent")));	
		if (pVotePercentLabel)
		{
			strPercent.Format(_T("%d%%"),percent);
			pVotePercentLabel->SetText(strPercent);
		}
		
		CLabelUI* pVoteNumLabel = static_cast<CProgressUI*>(pItemUI->FindSubControl(_T("vote_num")));
		if (pVoteNumLabel)
		{
			strPercent.Format(_T("%d responses"), progress_value);
			pVoteNumLabel->SetText(strPercent);
		}		
	}	
}

bool CFeedBackLaunchWnd::CheckIsFirstUserFeedBack()
{
	std::wstring file_name = CMainFrame::GetInstance().GetLowerThirdConfPath();

	TCHAR tempStr[MAX_PATH] = { 0 };
	GetPrivateProfileString(_T("FeedBack"), _T("IsFirst"), NULL, tempStr, sizeof(tempStr), file_name.data());
	std::wstring bFirst = tempStr;
	if (bFirst.empty() || bFirst != _T("0"))
	{
		std::wstring file_name = CMainFrame::GetInstance().GetLowerThirdConfPath();
		::WritePrivateProfileString(_T("FeedBack"), _T("IsFirst"), _T("0"), file_name.data());
		return true;
	}
	else
	{
		return false;
	}	
}

void CFeedBackLaunchWnd::OnLaunchFeebBackBtnClick()
{
	if (CheckIsFirstUserFeedBack())
	{
		RECT rc = { 0 };
		if (!btn_feedback_launch_)	return;

		rc = btn_feedback_launch_->GetPos();
		POINT ptLeftTop = { rc.left,rc.top };
		POINT ptRightBottom = { rc.right,rc.bottom };
		ClientToScreen(m_hWnd, &ptLeftTop);
		ClientToScreen(m_hWnd, &ptRightBottom);
		rc = { ptLeftTop.x,ptLeftTop.y,ptRightBottom.x,ptRightBottom.y };

		CMainFrame::GetInstance().ShowFeedBackTipsWnd(true, rc);
	}
	else
	{
		SwitchLaunchPushedBtn(false);

		SetFeedBackUserNum();

		CDuiString cmdStr;
		cmdStr.Format(_T("%d"), CmdChannelType::CMD_FEEDBACK_LAUNCH);

		if (!CMainFrame::GetInstance().IsCommandChannelConnect())
			return;

		ZoomVideoSDKErrors err = ZoomVideoSDKMgr::GetInst().SendCommand(NULL, cmdStr.GetData());

		if (ZoomVideoSDKErrors_Success != err)
		{
			wstring errStr = ZoomVideoSDKMgr::GetInst().GetErrorStringByErrorCode(err);
			MessageBoxW(NULL, errStr.data(), _T("Error"), NULL);
		}
	}

}

void CFeedBackLaunchWnd::OnCloseFeebBackBtnClick()
{
	this->ShowWindow(false);
}



// ================class CFeedBackSubmitWnd================
void CFeedBackSubmitWnd::InitWindow()
{
	InitControl();
	InitEmojiUserDataMap();
}

void CFeedBackSubmitWnd::InitControl()
{
	btn_submit_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_feedback_submit")));
	btn_close_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_close")));

	option_highly_satisfied_ = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("option_highly_satisfied")));
	option_satisfied_ = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("option_satisfied")));
	option_neutral_ = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("option_neutral")));
	option_dissatisfied_ = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("option_dissatisfied")));
	option_very_dissatisfied_ = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("option_very_dissatisfied")));

	lable_submit_finish_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lable_submit_finish")));
	lable_feedback_title_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lable_feedback_title")));
	lable_submit_tips_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lable_submit_tips")));

	layout_submit_before_ = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("layout_submit_before")));
	layout_submit_after_ = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("layout_submit_after")));
}

void CFeedBackSubmitWnd::InitEmojiUserDataMap()
{
	emoji_userdata_map_.clear();
	emoji_userdata_map_.insert(make_pair(_T("verySatisfied"), _T("Very Satisfied")));
	emoji_userdata_map_.insert(make_pair(_T("satisfied"), _T("Satisfied")));
	emoji_userdata_map_.insert(make_pair(_T("neutral"), _T("Neutral")));
	emoji_userdata_map_.insert(make_pair(_T("unsatisfied"), _T("Unsatisfied")));
	emoji_userdata_map_.insert(make_pair(_T("veryUnsatisfied"), _T("Very Unsatisfied")));
	
}

LRESULT CFeedBackSubmitWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

void CFeedBackSubmitWnd::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (wParam == HIDE_SUBMIT_WND_TIMER)
	{
		SwitchSubmitBeforeAndAfter(false);
	}
}

void CFeedBackSubmitWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender == btn_submit_)
			OnSubmitBtnClick();
		if (msg.pSender == btn_close_)
			this->ShowWindow(false);
	}
	else if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		if (msg.pSender == option_highly_satisfied_ || msg.pSender == option_satisfied_ ||
			msg.pSender == option_neutral_ || msg.pSender == option_dissatisfied_ ||
			msg.pSender == option_very_dissatisfied_)
		{
			HandleFeedBackOptionChange(msg.pSender);
		}
	}
}

void CFeedBackSubmitWnd::OnParseChannelCmd(IZoomVideoSDKUser* pSender, const std::vector<std::wstring >& cmdVector)
{
	if (cmdVector.size() < 1)
		return;

	std::wstring strCmdType = cmdVector[0];
	CmdChannelType nCmdType = (CmdChannelType)_ttoi(strCmdType.c_str());

	if (CMD_FEEDBACK_LAUNCH != nCmdType)
		return;

	CMainFrame::GetInstance().ShowFeedBackSubmitWnd(true);
}

void CFeedBackSubmitWnd::SwitchSubmitBeforeAndAfter(bool is_submit)
{
	if (!layout_submit_before_ || !layout_submit_after_)
		return;

	layout_submit_before_->SetVisible(!is_submit);
	layout_submit_after_->SetVisible(is_submit);
	if (is_submit)	
		SetTimer(m_hWnd, HIDE_SUBMIT_WND_TIMER, 5000, NULL);
	else
	{
		this->ShowWindow(false);
		KillTimer(m_hWnd, HIDE_SUBMIT_WND_TIMER);
	}
	
}

void CFeedBackSubmitWnd::OnSubmitBtnClick()
{
	SwitchSubmitBeforeAndAfter(true);

	if (m_feedback_select_.IsEmpty())
		return;

	CDuiString cmdStr;
	cmdStr.Format(_T("%d|%s"), CmdChannelType::CMD_FEEDBACK_SUBMIT, m_feedback_select_.GetData());

	IZoomVideoSDKUser* pUser = ZoomVideoSDKMgr::GetInst().GetSessionHost();	
		
	if (!CMainFrame::GetInstance().IsCommandChannelConnect())
		return;

	ZoomVideoSDKErrors err = ZoomVideoSDKMgr::GetInst().SendCommand(pUser, cmdStr.GetData());

	if (ZoomVideoSDKErrors_Success != err)
	{
		wstring errStr = ZoomVideoSDKMgr::GetInst().GetErrorStringByErrorCode(err);
		MessageBoxW(NULL, errStr.data(), _T("Error"), NULL);
	}
}

void CFeedBackSubmitWnd::HandleFeedBackOptionChange(CControlUI* pControl)
{
	if (!pControl || !m_feedback_select_ || !btn_submit_ || !lable_submit_tips_)
		return;

	m_feedback_select_ = pControl->GetUserData();
	
	if (!lable_submit_tips_->IsVisible())
		lable_submit_tips_->SetVisible(true);

	auto iter = emoji_userdata_map_.find(m_feedback_select_.GetData());
	if (iter != emoji_userdata_map_.end())	
		lable_submit_tips_->SetText(iter->second.data());
	
	if (!btn_submit_->IsEnabled())	
		btn_submit_->SetEnabled();
}