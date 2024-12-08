#ifndef _FEEDBACK_TIPS_WND_UI_H_
#define _FEEDBACK_TIPS_WND_UI_H_

class CFeedBackTipsWnd : public WindowImplBase
{
public:
	CFeedBackTipsWnd();
	~CFeedBackTipsWnd();

protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder() { return _T(""); }
	virtual CDuiString GetSkinFile() { return to_wstring(IDXML_FEEDBACK_TIPS_WND_UI).c_str(); }
	virtual UILIB_RESOURCETYPE GetResourceType() const { return UILIB_RESOURCE; };
	virtual LPCTSTR GetWindowClassName() const { return _T("FeedbackTipsWnd"); }
	virtual void Notify(TNotifyUI& msg);	
	virtual LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	void InitControl();
	void DoCloseBtnClick();
	
private:
	CButtonUI* btn_close_ = nullptr;
};

#endif