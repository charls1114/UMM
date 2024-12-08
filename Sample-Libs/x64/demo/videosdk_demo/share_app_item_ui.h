#ifndef _SHARE_APP_ITEM_UI_H_
#define _SHARE_APP_ITEM_UI_H_

#include "enum_apps_info_mgr_comm_def.h"

class ShareAppItemUI : public COptionUI
{
public:
	ShareAppItemUI();
	~ShareAppItemUI();

	void SetCustomImage(HBITMAP hbitmap);
	void SetTitleName(wstring name);

	void set_app_info(const AppInfo& app);
	AppInfo get_app_info() const;

	SIZE ShareAppItemUI::GetThumbnailSize(SIZE size);

public:
	virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
private:
	HBITMAP	 app_thumbnail_hbitmap_;
	wstring name_;
	AppInfo app_;
	SIZE thumbnail_size_;
};

#endif //_SHARE_APP_ITEM_UI_H_