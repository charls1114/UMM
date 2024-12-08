#include "stdafx.h"
#include "reaction_bar_wnd.h"
#include "main_frame.h"
#include "videosdk_demo_mgr.h"

#define REACTION_CHANGE_TIMER 1003
#define HIDE_EMOJI_TIME 5

CReactionBarWnd::CReactionBarWnd()
{
}

CReactionBarWnd::~CReactionBarWnd()
{
}

LRESULT CReactionBarWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

void CReactionBarWnd::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (wParam == REACTION_CHANGE_TIMER)
	{
		CheckUserReacton();
	}
}

bool CReactionBarWnd::IsUserRaiseHand(IZoomVideoSDKUser* pUser)
{
	if (!pUser)
		return false;

	auto iter = user_raise_hand_map_.find(pUser);
	if (iter != user_raise_hand_map_.end())
		return true;
	else
		return false;
}

void CReactionBarWnd::AddRaiseHandUser(IZoomVideoSDKUser* pUser)
{
	if (!pUser)
		return;

	user_raise_hand_map_[pUser] = true;
}

void CReactionBarWnd::RemoveRaiseHandUser(IZoomVideoSDKUser* pUser)
{
	if (!pUser)
		return;

	user_raise_hand_map_.erase(pUser);
}

void CReactionBarWnd::CheckUserReacton()
{
	if (user_reaction_check_map_.size() == 0)
		return;

	for (auto iter = user_reaction_check_map_.begin(); iter != user_reaction_check_map_.end(); iter++)
	{
		UserReaction user_reaction = iter->second;
		
		int dff = difftime(std::time(0), user_reaction.reaction_time);
		if (dff > HIDE_EMOJI_TIME)
		{
			if (IsUserRaiseHand(iter->first))
				CMainFrame::GetInstance().ShowUserEmoji(iter->first, _T("raisehand"),false);			
			else			
				CMainFrame::GetInstance().RemoveUserReaction(iter->first);				
					
			user_reaction_check_map_.erase(iter);
		}
	}
}

void CReactionBarWnd::InitControl()
{
	btn_raise_hand_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_raise_hand")));
	btn_free_hand_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_free_hand")));
	
}

void CReactionBarWnd::InitWindow()
{	
	InitControl();
	InitReactionMap();
	SetTimer(m_hWnd, REACTION_CHANGE_TIMER, 1000, NULL);
}

void CReactionBarWnd::InitReactionMap()
{
	reaction_type_map_.insert(make_pair(_T("clap"), IDB_EMOJI_CLAP));
	reaction_type_map_.insert(make_pair(_T("thumbsup"), IDB_EMOJI_THUMBSUP));
	reaction_type_map_.insert(make_pair(_T("joy"), IDB_EMOJI_JOY));
	reaction_type_map_.insert(make_pair(_T("hushed"), IDB_EMOJI_HUSHED));
	reaction_type_map_.insert(make_pair(_T("heart"), IDB_EMOJI_HEART));
	reaction_type_map_.insert(make_pair(_T("tada"), IDB_EMOJI_TADA));
	reaction_type_map_.insert(make_pair(_T("raisehand"), IDB_EMOJI_RAISEDHAND));
	reaction_type_map_.insert(make_pair(_T("lowerhand"), IDB_EMOJI_RAISEDHAND)); 

	user_raise_hand_map_.clear();
}

int CReactionBarWnd::GetResIdByReactionType(std::wstring reaction_type)
{
	if (reaction_type_map_.count(reaction_type) > 0)
		return reaction_type_map_[reaction_type];

	return -1;
}

int CReactionBarWnd::AddUserReactionCheck(IZoomVideoSDKUser* pUser, int res_id)
{
	if (res_id == -1)
		return 0;

	UserReaction user_reaction;
	auto iter = user_reaction_check_map_.find(pUser);
	if (iter != user_reaction_check_map_.end())
		user_reaction = iter->second;
	
	user_reaction.reaction_time = std::time(0);
	if (res_id == IDB_EMOJI_RAISEDHAND)
	{
		bool is_raise_hand = IsUserRaiseHand(pUser);
		is_raise_hand ? RemoveRaiseHandUser(pUser) : AddRaiseHandUser(pUser);
		is_raise_hand = IsUserRaiseHand(pUser);
		
		if (is_raise_hand)
		{
			return IDB_EMOJI_RAISEDHAND;
		}
		else
		{
			user_reaction.reaction_res_id = user_reaction.reaction_res_id;
		}		
	}
	else
	{
		user_reaction.reaction_res_id = res_id;		
	}
	
	user_reaction_check_map_[pUser] = user_reaction;

	return user_reaction.reaction_res_id;
}

int	CReactionBarWnd::GetUserReactionResId(IZoomVideoSDKUser* pUser)
{
	if (!pUser)
		return 0;

	if (IsUserRaiseHand(pUser))
		return IDB_EMOJI_RAISEDHAND;

	auto iter = user_reaction_check_map_.find(pUser);
	if (iter != user_reaction_check_map_.end())
	{
		UserReaction user_reaction = iter->second;
		return user_reaction.reaction_res_id;
	}
	else
	{
		return 0;
	}
}

void CReactionBarWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btn_emoji_clap")  || msg.pSender->GetName() == _T("btn_emoji_thumbsup") ||
			msg.pSender->GetName() == _T("btn_emoji_joy")   || msg.pSender->GetName() == _T("btn_emoji_hushed")   ||
			msg.pSender->GetName() == _T("btn_emoji_heart") || msg.pSender->GetName() == _T("btn_emoji_tada")     ||
			msg.pSender->GetName() == _T("btn_raise_hand"))
		{
			DoEmojiBtnClick(msg.pSender);
		}
		
	}

	__super::Notify(msg);
}

LRESULT CReactionBarWnd::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::OnSetFocus(uMsg, wParam, lParam, bHandled);
}

LRESULT CReactionBarWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	this->ShowWindow(false);
	return __super::OnKillFocus(uMsg, wParam, lParam, bHandled);
}

void CReactionBarWnd::DoEmojiBtnClick(CControlUI* pControl)
{
	if (!pControl)	return;

	CDuiString strEmoji = pControl->GetUserData();
	if (strEmoji.IsEmpty())	return;
	
	if (strEmoji == _T("raisehand"))
	{
		pControl->SetText(_T("Lower Hand"));
		pControl->SetUserData(_T("lowerhand"));
	}	
	else if (strEmoji == _T("lowerhand"))
	{
		pControl->SetText(_T("Raise Hand"));
		pControl->SetUserData(_T("raisehand"));
	}
		
	CDuiString cmdStr;
	cmdStr.Format(_T("%d|%s"), CmdChannelType::CMD_REACTION, strEmoji.GetData());

	IZoomVideoSDKUser* pUser = ZoomVideoSDKMgr::GetInst().GetMySelf();
	CMainFrame::GetInstance().onCommandReceived(pUser, cmdStr.GetData());

	if (!CMainFrame::GetInstance().IsCommandChannelConnect())
		return;

	ZoomVideoSDKErrors err = ZoomVideoSDKMgr::GetInst().SendCommand(NULL, cmdStr.GetData());
	
	if (ZoomVideoSDKErrors_Success != err)
	{
		wstring errStr = ZoomVideoSDKMgr::GetInst().GetErrorStringByErrorCode(err);
		MessageBoxW(NULL, errStr.data(), _T("Error"), NULL);
	}
}