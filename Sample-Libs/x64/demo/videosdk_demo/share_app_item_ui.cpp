#include "stdafx.h"
#include "share_app_item_ui.h"

#define TITLE_TEXT_HEIGHT 20
#define MARGIN 10

ShareAppItemUI::ShareAppItemUI()
{

}

ShareAppItemUI::~ShareAppItemUI()
{
	if (app_thumbnail_hbitmap_)
	{
		DeleteObject(app_thumbnail_hbitmap_);
		app_thumbnail_hbitmap_ = NULL;
	}
}

void ShareAppItemUI::SetCustomImage(HBITMAP hbitmap)
{
	app_thumbnail_hbitmap_ = hbitmap;
	Invalidate();
}

void ShareAppItemUI::SetTitleName(wstring name)
{
	name_ = name;
	Invalidate();
}

void ShareAppItemUI::set_app_info(const AppInfo& app)
{
	app_ = app;
}

AppInfo ShareAppItemUI::get_app_info() const
{
	return app_;
}

SIZE ShareAppItemUI::GetThumbnailSize(SIZE size)
{
	thumbnail_size_.cx = size.cx - MARGIN * 2;
	thumbnail_size_.cy = size.cy - MARGIN * 2 - TITLE_TEXT_HEIGHT;

	return thumbnail_size_;
}

bool ShareAppItemUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
	CControlUI::DoPaint(hDC, rcPaint, pStopControl);

	CRect item_rect = m_rcItem;

	CRect rect_thumbnail = item_rect;
	rect_thumbnail.left += MARGIN;
	rect_thumbnail.top += MARGIN;
	rect_thumbnail.right = rect_thumbnail.left + thumbnail_size_.cx;
	rect_thumbnail.bottom = rect_thumbnail.top + thumbnail_size_.cy;

	RECT textpadding = GetTextPadding();

	CRect rect_name = item_rect;
	rect_name.left += textpadding.left;
	rect_name.right -= textpadding.right;
	rect_name.top = rect_name.bottom - MARGIN - TITLE_TEXT_HEIGHT;

	//draw app thumbnail
	if (app_thumbnail_hbitmap_)
	{
		BITMAP bitmap = { 0 };
		GetObject(app_thumbnail_hbitmap_, sizeof(BITMAP), (LPVOID)&bitmap);
		if (bitmap.bmWidth > 0 && bitmap.bmHeight > 0)
		{
			HDC mem_dc = ::CreateCompatibleDC(hDC);
			HBITMAP old_bmp = (HBITMAP)SelectObject(mem_dc, app_thumbnail_hbitmap_);
			BitBlt(hDC, rect_thumbnail.left, rect_thumbnail.top, bitmap.bmWidth, bitmap.bmHeight, mem_dc, 0, 0, SRCCOPY);
			SelectObject(mem_dc, old_bmp);
			DeleteDC(mem_dc);
		}
	}

	//draw title text
	bool is_selected = IsSelected();
	DWORD dwColor = is_selected ? GetSelectedTextColor() : GetTextColor();
	CRenderEngine::DrawText(hDC, m_pManager, rect_name, name_.c_str(), dwColor, 0, DT_SINGLELINE | DT_VCENTER | DT_CENTER | DT_END_ELLIPSIS);

	return true;
}