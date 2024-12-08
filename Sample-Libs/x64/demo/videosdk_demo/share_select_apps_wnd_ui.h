#ifndef _SHARE_SELECT_APPS_WND_H_
#define _SHARE_SELECT_APPS_WND_H_

class ShareAppLayoutUI;
class ShareSelectAppsWndUI : public WindowImplBase
{
public:
	ShareSelectAppsWndUI();
	~ShareSelectAppsWndUI();

	void UpdateAllApps();

protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetWindowClassName() const { return _T("VideoSDKDemoShareSelectAppsUI"); }
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void Notify(TNotifyUI& msg);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
protected:
	virtual LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

private:
	void InitControls();

	void DoCloseBtnClick();
	void DoCancelBtnClick();
	void DoShareBtnClick();
private:
	ShareAppLayoutUI* share_app_layout_;
};

#endif  //_SHARE_SELECT_APPS_WND_H_