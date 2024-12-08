#ifndef _STOP_SHARE_WND_H_
#define _STOP_SHARE_WND_H_

class ShareToolBarWndUI : public WindowImplBase
{
public:
	ShareToolBarWndUI();
	~ShareToolBarWndUI();

	void UpdateParticipantNum();
	void UpdateUIForAudioStatus(bool is_audio_on);
	void UpdateUIForVideoStatus(bool is_video_on);

protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetWindowClassName() const { return _T("StopShareWndUI"); }
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT	OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	void InitControls();
	void AddNoHitTestControls(CControlUI* controls);
	bool IsHitInNoHitTestControls(const POINT& pt);

	void DoMutedAudioBtnClick();
	void DoUnmutedAudioBtnClick();
	void DoMutedVideoBtnClick();
	void DoUnmutedVideoBtnClick();
	void DoParticipantBtnClick();

	void DoStopShareBtnClick();
private:
	vector<CControlUI*>  vect_no_hittest_controls_;
	CButtonUI* stop_share_btn_;
	CButtonUI* btn_muted_audio_ = nullptr;
	CButtonUI* btn_unmuted_audio_ = nullptr;
	CButtonUI* btn_muted_video_ = nullptr;
	CButtonUI* btn_unmuted_video_ = nullptr;
	CButtonUI* btn_participant_ = nullptr;
	CLabelUI* label_participants_ = nullptr;
};

#endif //_STOP_SHARE_WND_H_