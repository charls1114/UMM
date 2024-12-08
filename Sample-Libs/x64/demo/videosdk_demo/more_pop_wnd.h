#ifndef _MORE_POP_WND_UI_H_
#define _MORE_POP_WND_UI_H_

enum ReactionType;

class CMorePopWnd : public WindowImplBase
{
public:
	CMorePopWnd();
	~CMorePopWnd();

protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder() { return _T(""); }
	virtual CDuiString GetSkinFile() { return to_wstring(IDXML_MORE_POP_WND_UI).c_str(); }
	virtual UILIB_RESOURCETYPE GetResourceType() const { return UILIB_RESOURCE; };
	virtual LPCTSTR GetWindowClassName() const { return _T("MorePopWnd"); }
	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	
private:
	void InitControl();	
	void DoFeedBackBtnClick();	

private:
	CButtonUI* btn_free_hand_ = nullptr;
};

#endif