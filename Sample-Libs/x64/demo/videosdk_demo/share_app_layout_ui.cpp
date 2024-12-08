#include "stdafx.h"
#include "share_app_layout_ui.h"
#include "share_app_item_ui.h"
#include "enum_apps_info_mgr.h"
#include <string>
#include "Utils/stb_image.h"
#include "main_frame.h"
#include "videosdk_demo_mgr.h"

#define APP_ITEM_OPTION_GROUP _T("AllAppsOption")

ShareAppLayoutUI::ShareAppLayoutUI()
{
	
}

ShareAppLayoutUI::~ShareAppLayoutUI()
{

}

CControlUI* ShareAppLayoutUI::CreateControl(LPCTSTR pstrClass)
{
	if (_tcscmp(pstrClass, _T("ShareAppItem")) == 0)
	{
		return new ShareAppItemUI();
	}

	return nullptr;
}

HBITMAP ShareAppLayoutUI::GetBitmap(const wstring& image)
{
	LPBYTE pImage = NULL;
	int x, y, n;
	pImage = stbi_load_from_memory((LPBYTE)image.c_str(), image.length(), &x, &y, &n, 4);
	if (!pImage) {
		return NULL;
	}
	BITMAPINFO bmi;
	::ZeroMemory(&bmi, sizeof(BITMAPINFO));   
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = x;
	bmi.bmiHeader.biHeight = -y;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = x * y * 4;

	bool bAlphaChannel = false;
	LPBYTE pDest = NULL;
	HBITMAP hBitmap = ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);
	if (!hBitmap) {
		return NULL;
	}
	for (int i = 0; i < x * y; i++)
	{
		pDest[i * 4 + 3] = pImage[i * 4 + 3];
		if (pDest[i * 4 + 3] < 255)
		{
			pDest[i * 4] = (BYTE)(DWORD(pImage[i * 4 + 2]) * pImage[i * 4 + 3] / 255);
			pDest[i * 4 + 1] = (BYTE)(DWORD(pImage[i * 4 + 1]) * pImage[i * 4 + 3] / 255);
			pDest[i * 4 + 2] = (BYTE)(DWORD(pImage[i * 4]) * pImage[i * 4 + 3] / 255);
			bAlphaChannel = true;
		}
		else
		{
			pDest[i * 4] = pImage[i * 4 + 2];
			pDest[i * 4 + 1] = pImage[i * 4 + 1];
			pDest[i * 4 + 2] = pImage[i * 4];
		}

		if (*(DWORD*)(&pDest[i * 4]) == 0) {
			pDest[i * 4] = (BYTE)0;
			pDest[i * 4 + 1] = (BYTE)0;
			pDest[i * 4 + 2] = (BYTE)0;
			pDest[i * 4 + 3] = (BYTE)0;
			bAlphaChannel = true;
		}
	}

	stbi_image_free(pImage);
	return hBitmap;
}

void ShareAppLayoutUI::AddItem(ShareAppItemUI* item, const AppInfo& app, HBITMAP hbitmap, wstring title)
{
	if (item && hbitmap)
	{
		item->SetTitleName(title);
		item->SetCustomImage(hbitmap);
		item->set_app_info(app);
		item->SetGroup(LPCTSTR(APP_ITEM_OPTION_GROUP));
		Add(item);
	}
}

void ShareAppLayoutUI::UpdateAppItem()
{
	RemoveAll();

	//add screen item
	AppList screen_list = EnumAppsInfoMgr::Instance().GetDisplayScreenList();
	for (size_t i = 0; i < screen_list.size(); i++)
	{
		CDialogBuilder dlgBuilder;
		ShareAppItemUI* app_item = static_cast<ShareAppItemUI*>(dlgBuilder.Create(IDXML_SHARE_APP_ITEM_UI, _T("xml"), this, m_pManager));
		if (app_item)
		{
			SIZE item_size = GetItemSize();
			SIZE thumbnail_size = app_item->GetThumbnailSize(item_size);
			wstring thumbnail_image = EnumAppsInfoMgr::Instance().GetDisplayScreenThumnailImage(screen_list[i], thumbnail_size.cx, thumbnail_size.cy);
			HBITMAP hbitmap = GetBitmap(thumbnail_image);

			if (hbitmap)
			{
				wstring title = _T("Screen ") + to_wstring(i + 1);

				AddItem(app_item, screen_list[i], hbitmap, title);
			}
		}
	}
	//add 2nd camera item
	if (ZoomVideoSDKMgr::GetInst().GetNumberOfCameras() > 1)
	{
		CDialogBuilder dlgBuilder;
		ShareAppItemUI* app_item = static_cast<ShareAppItemUI*>(dlgBuilder.Create(IDXML_SHARE_APP_ITEM_UI, _T("xml"), this, m_pManager));
		if (app_item)
		{
			HBITMAP hbitmap = EnumAppsInfoMgr::Instance().LoadBitmapFromResource(IDB_PNG_SHARE_2ND_CAMERA);
			SIZE item_size = GetItemSize();
			SIZE thumbnail_size = app_item->GetThumbnailSize(item_size);

			wstring thumbnail_image = EnumAppsInfoMgr::Instance().GetWindowPNGImage(hbitmap, thumbnail_size.cx, thumbnail_size.cy);
			HBITMAP hbitmap_adjust = GetBitmap(thumbnail_image);

			if (hbitmap_adjust)
			{
				wstring title = _T("Content from 2nd Camera");
				AppInfo secondCameraShare;
				secondCameraShare.share_type = kSecondCameraShare;
				AddItem(app_item, secondCameraShare, hbitmap_adjust, title);
			}
		}
	}
	
	//add app item
	AppList app_list = EnumAppsInfoMgr::Instance().GetAllAppsList();
	for (size_t i = 0; i < app_list.size(); i++)
	{
		if (app_list[i].title == MAIN_FRAME_TITLE)
			continue;

		CDialogBuilder dlgBuilder;
		ShareAppItemUI* app_item = static_cast<ShareAppItemUI*>(dlgBuilder.Create(IDXML_SHARE_APP_ITEM_UI, _T("xml"), this, m_pManager));
		if (app_item)
		{
			SIZE item_size = GetItemSize();
			SIZE thumbnail_size = app_item->GetThumbnailSize(item_size);
			wstring thumbnail_image = EnumAppsInfoMgr::Instance().GetAppThumbnailImage(app_list[i], thumbnail_size.cx, thumbnail_size.cy);
			HBITMAP hbitmap = GetBitmap(thumbnail_image);
			if (hbitmap)
			{
				wstring title = app_list[i].title;
				AddItem(app_item, app_list[i], hbitmap, title);
			}
		}
	}

	if (GetCount() > 0)
	{
		ShareAppItemUI* item = static_cast<ShareAppItemUI*>(GetItemAt(0)); //default the first item is selected
		if (item)
		{
			item->Selected(true, false);
		}
	}
}

AppInfo ShareAppLayoutUI::GetSelectItemAppInfo()
{
	AppInfo app;

	for (size_t i = 0; i < m_items.GetSize(); i++)
	{
		ShareAppItemUI* app_item = static_cast<ShareAppItemUI*>(m_items[i]);
		if (app_item)
		{
			if (app_item->IsSelected())
			{
				app = app_item->get_app_info();
				break;
			}
		}
		
	}

	return app;
}