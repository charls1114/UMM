#pragma once

class CJoinSessionWndUI : public WindowImplBase
{
public:
	CJoinSessionWndUI();
	~CJoinSessionWndUI();

public:
	void OnInvalidParamterNotification();
	void OnJoinMettingJoinFailed(int error_code);
	void OnJoinPasswordWrong();
	void BackToInitPage();
	bool IsSelectedVideoOff();
protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetWindowClassName() const { return _T("VideoSDKDemoJoinSessionUI"); }
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void Notify(TNotifyUI& msg);
	
private:

	void InitControls();
	void DoJoinSessionBtnClick();
	void DoCreateSessionBtnClick();
	void DoJoinBtnClick();
	void DoCloseBtnClick();
	void DoSettingBtnClick();
	void DoBackBtnClick();
	void DoErrorOkBtnClick();
	void ShowJoinOrCreateSessionPage(bool is_join);
	void ShowInitPage();
	void ShowErrorPage();
	void ShowBtnJoinOrCreate(bool show_join);

	void JudgeHideInvalidParamter();
	void JudgeHideWrongPWDTip();
	void JudgeHideNameEmptyErrorTip();
	void OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam);
	wstring FormatString(wstring str,int error_code);
private:

	CTabLayoutUI* tab_layout_ = nullptr;

	CRichEditUI* edit_session_name_ = nullptr;
	CRichEditUI* edit_user_name_ = nullptr;
	CRichEditUI* edit_session_psw_ = nullptr;
	CButtonUI* btn_setting_ = nullptr;
	CButtonUI* btn_join_ = nullptr;
	CButtonUI* btn_create_ = nullptr;
	CLabelUI* label_join_create_title_ = nullptr;
	CCheckBoxUI* check_box_turn_off_video_ = nullptr;
	CCheckBoxUI* check_box_mute_audio_ = nullptr;
	wstring token_;

	CVerticalLayoutUI* init_layout_ = nullptr;
	CHorizontalLayoutUI* join_create_session_layout_ = nullptr;
	CVerticalLayoutUI* error_layout_ = nullptr;
	CLabelUI* label_error_ = nullptr;
	CLabelUI* label_invalid_paramter_ = nullptr;
	CLabelUI* label_your_name_error_ = nullptr;
	CLabelUI* labe_password_error_ = nullptr;
	CButtonUI* btn_error_ok = nullptr;
	CDuiString session_name_;
	CDuiString session_password_;
};