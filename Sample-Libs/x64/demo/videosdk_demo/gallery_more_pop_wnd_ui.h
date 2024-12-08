#ifndef _GALLERY_MORE_POP_WND_UI_H_
#define _GALLERY_MORE_POP_WND_UI_H_

class CGalleryMorePopWnd : public WindowImplBase
{
public:
	CGalleryMorePopWnd();
	~CGalleryMorePopWnd();
	void UpdateUIForCameraControlStatus(bool isShowStop);
	void UpdateUIForRemove(bool isShowRemove);
protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder() { return _T(""); }
	virtual CDuiString GetSkinFile() { return to_wstring(IDXML_GALLERY_MORE_POP_WND_UI).c_str(); }
	virtual UILIB_RESOURCETYPE GetResourceType() const { return UILIB_RESOURCE; };
	virtual LPCTSTR GetWindowClassName() const { return _T("GalleryMorePopWnd"); }
	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	
private:
	void InitControl();	
	void DoRequestOrStopCameraControlBtnClick();	
	void DoRemoveBtnClick();

private:
	bool m_is_show_stop;
	CButtonUI* btn_request_or_stop_camera_control_ = nullptr;
	CButtonUI* btn_remove_ = nullptr;
};

#endif