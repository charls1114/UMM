#ifndef _FEED_BACK_WND_H_
#define _FEED_BACK_WND_H_

#include "videosdk_demo_mgr.h"

class CFeedBackLaunchWnd : public WindowImplBase ,public IParseChannelCmdWnd
{
public:
	CFeedBackLaunchWnd() {};
	~CFeedBackLaunchWnd() {};

public:
	void	OnSessionJoin();
	virtual HWND GetHWND() { return m_hWnd; }
	virtual void OnParseChannelCmd(IZoomVideoSDKUser* pSender, const std::vector<std::wstring >& cmdVector);	
	
protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder() { return _T(""); }
	virtual CDuiString GetSkinFile() { return to_wstring(IDXML_FEEDBACK_LAUNCH_WND_UI).c_str(); }
	virtual UILIB_RESOURCETYPE GetResourceType() const { return UILIB_RESOURCE; };
	virtual LPCTSTR GetWindowClassName() const { return _T("FeedBackLaunchWnd"); }
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void Notify(TNotifyUI& msg);
	virtual void OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	void	InitControl();
	void	InitFeedBackLaunch();
	void	UpdateFeedBackView();
	void	SetFeedBackUserNum();
	void	OnLaunchFeebBackBtnClick();
	void	OnCloseFeebBackBtnClick();	
	void	SwitchLaunchPushedBtn(bool is_show_launch);
	bool	CheckIsFirstUserFeedBack();
private:
	CButtonUI* btn_feedback_launch_ = nullptr;
	CButtonUI* btn_feedback_close_ = nullptr;
	CHorizontalLayoutUI* pushed_layout_ = nullptr;
	CVerticalLayoutUI* launch_layout_main_ = nullptr;
	CLabelUI* lable_responses_ = nullptr;
	
	int feedback_total_usernum = 0;
	map<std::wstring, std::wstring> feedback_user_map_;
	
};


class CFeedBackSubmitWnd : public WindowImplBase, public IParseChannelCmdWnd
{
public:
	CFeedBackSubmitWnd() {};
	~CFeedBackSubmitWnd() {};

public:
	virtual void OnParseChannelCmd(IZoomVideoSDKUser* pSender, const std::vector<std::wstring >& cmdVector);
	virtual HWND GetHWND() { return m_hWnd; }
protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder() { return _T(""); }
	virtual CDuiString GetSkinFile() { return to_wstring(IDXML_FEEDBACK_SUBMIT_WND_UI).c_str(); }
	virtual UILIB_RESOURCETYPE GetResourceType() const { return UILIB_RESOURCE; };
	virtual LPCTSTR GetWindowClassName() const { return _T("FeedBackSubmitWnd"); }	
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void Notify(TNotifyUI& msg);
	virtual void OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	void	InitControl();	
	
	void	OnSubmitBtnClick();
	void	SwitchSubmitBeforeAndAfter(bool is_submit);
	void	HandleFeedBackOptionChange(CControlUI* pControl);
	void	InitEmojiUserDataMap();
private:
	CButtonUI* btn_submit_ = nullptr;
	CButtonUI* btn_close_ = nullptr;
	COptionUI* option_highly_satisfied_ = nullptr;
	COptionUI* option_satisfied_ = nullptr;
	COptionUI* option_neutral_ = nullptr;
	COptionUI* option_dissatisfied_ = nullptr;
	COptionUI* option_very_dissatisfied_ = nullptr;

	CLabelUI* lable_submit_finish_ = nullptr;
	CLabelUI* lable_feedback_title_ = nullptr;
	CLabelUI* lable_submit_tips_ = nullptr;

	CHorizontalLayoutUI* layout_submit_before_ = nullptr;
	CHorizontalLayoutUI* layout_submit_after_ = nullptr;
	map<std::wstring, std::wstring> emoji_userdata_map_;
	

	CDuiString  m_feedback_select_;
};
#endif