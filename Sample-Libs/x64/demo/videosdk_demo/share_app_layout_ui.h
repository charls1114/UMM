#ifndef _SHARE_APP_LAYOUT_UI_H_
#define _SHARE_APP_LAYOUT_UI_H_

#include "enum_apps_info_mgr_comm_def.h"

#include <Windows.h>
#include <gdiplus.h>
#include <iostream>

class ShareAppItemUI;
class ShareAppLayoutUI : public CTileLayoutUI, public IDialogBuilderCallback
{
public:
	ShareAppLayoutUI();
	~ShareAppLayoutUI();

	void UpdateAppItem();

	AppInfo GetSelectItemAppInfo();

public:
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);

private:
	HBITMAP GetBitmap(const wstring& image);
	void AddItem(ShareAppItemUI* item, const AppInfo& app,HBITMAP hbitmap, wstring title);
};

#endif //_SHARE_APP_LAYOUT_UI_H_