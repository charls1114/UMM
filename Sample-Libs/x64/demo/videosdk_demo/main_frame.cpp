// line 2291 파이핑 코드 추가
// line 2424 코드 추가
// line 2992 수정 (코드 맨 아래)

#include "stdafx.h"
//#include "zoom_video_sdk_include_interface.h"
#include "main_frame.h"
#include "utils.h"
#include "join_session_wnd_ui.h"
#include "title_bar_wnd_ui.h"
#include "bottom_bar_wnd_ui.h"
#include "zoom_video_sdk_interface.h"
#include "leave_session_tips_wnd_ui.h"
#include "session_info_wnd_ui.h"
#include "session_settings_wnd_ui.h"
#include "share_select_apps_wnd_ui.h"
#include "videosdk_demo_mgr.h"
#include "zoom_videosdk_renderer_def.h"
#include "zoom_videosdk_renderer_api.h"
#include "zoom_video_sdk_password_handler_interface.h"
#include "zoom_videosdk_renderer_interface.h"
#include "video_show_mgr.h"
#include "share_tool_bar_wnd_ui.h"
#include "chat_content_wnd_ui.h"
#include "turn_page_wnd_ui.h"
#include "message_tip_wnd_ui.h"
#include "statistics_wnd_ui.h"
#include "reaction_bar_wnd.h"
#include "bullet_screen_wnd.h"
#include "lower_thirds_info_wnd.h"
#include "feed_back_wnd.h"
#include "feedback_tips_wnd.h"
#include "more_pop_wnd.h"
#include "lowerthirds_setting_wnd_ui.h"
#include "switch_camera_wnd_ui.h"
#include "remote_control_camera_wnd_ui.h"
#include "camera_control_request_received_wnd_ui.h"
#include "stop_camera_control_wnd_ui.h"
#include "gallery_more_pop_wnd_ui.h"
#include "control_camera_tip_wnd_ui.h"
#include "control_camera_pop_wnd_ui.h"
#include "top_tip_wnd_ui.h"
#include "switch_camera_pop_wnd_ui.h"

#include "video_layout_item_base.h"
#include <thread>
#include <atomic>
#include <locale>
#include <codecvt>
#include "ProcessManager.hpp"
//custom message
#define WM_ICON_NOTIFY WM_USER + 1

#define CPATION_HEIGHT 100
#define BOTTOM_BAR_HEIGHT 48
#define HIDE_BOTTOM_BAR_TIMER 1000
#define HIDE_ERROR_TIP_TIMER 1001
#define HIDE_CONTROLL_CAMERA_POP_WND_TIMER 1002
#define HIDE_TOP_TIP_TIMER 1003

#define VIDEO_CANTAINER_DEFAULT_INSET 20,20,20,20
#define DEFAULT_ROW_GAP 20
#define DEFAULT_COLUMN_GAP 20
#define BOTTOM_BAR_MARGIN 20
#define CHAT_CONTENT_LEFT_MARGIN 200
#define SESSION_SETTING_WND_GAP 10
#define DEFAULT_BTN_GAP 10
#define SMALL_ICON_SIZE 24
#define DEFAULT_ITEM_HEIGHT 35
#define DEFAULT_ITEM_WIDTH 180
#define TURN_PAGE_MARIGN 5

#define LOWERTHIRDS_PADDING_LEFT 8
#define LOWERTHIRDS_PADDING_TOP 42

#define LOWERTHIRDS_WND_FULL_HEIGHT 62
#define LOWERTHIRDS_WND_LITE_HEIGHT 42

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
	DestroyAllOwnedWnd();

	if (video_show_mgr_)
	{
		delete video_show_mgr_;
		video_show_mgr_ = nullptr;
	}

	UninitVideoSDK();
}

void CMainFrame::DestroyAllOwnedWnd()
{
	if (join_session_wnd_)
	{
		if (::IsWindow(join_session_wnd_->GetHWND()))
		{
			DestroyWindow(join_session_wnd_->GetHWND());
		}
		delete join_session_wnd_;
		join_session_wnd_ = nullptr;
	}

	if (title_bar_wnd_)
	{
		if (::IsWindow(title_bar_wnd_->GetHWND()))
		{
			DestroyWindow(title_bar_wnd_->GetHWND());
		}
		delete title_bar_wnd_;
		title_bar_wnd_ = nullptr;
	}

	if (switch_camera_wnd_)
	{
		if (::IsWindow(switch_camera_wnd_->GetHWND()))
		{
			DestroyWindow(switch_camera_wnd_->GetHWND());
		}
		delete switch_camera_wnd_;
		switch_camera_wnd_ = nullptr;
	}
	if (remote_control_camera_wnd_)
	{
		if (::IsWindow(remote_control_camera_wnd_->GetHWND()))
		{
			DestroyWindow(remote_control_camera_wnd_->GetHWND());
		}
		delete remote_control_camera_wnd_;
		remote_control_camera_wnd_ = nullptr;
	}
	if (control_camera_tip_wnd_)
	{
		if (::IsWindow(control_camera_tip_wnd_->GetHWND()))
		{
			DestroyWindow(control_camera_tip_wnd_->GetHWND());
		}
		delete control_camera_tip_wnd_;
		control_camera_tip_wnd_ = nullptr;
	}
	if (control_camera_pop_wnd_)
	{
		if (::IsWindow(control_camera_pop_wnd_->GetHWND()))
		{
			DestroyWindow(control_camera_pop_wnd_->GetHWND());
		}
		delete control_camera_pop_wnd_;
		control_camera_pop_wnd_ = nullptr;
	}
	if (switch_camera_pop_wnd_)
	{
		if (::IsWindow(switch_camera_pop_wnd_->GetHWND()))
		{
			DestroyWindow(switch_camera_pop_wnd_->GetHWND());
		}
		delete switch_camera_pop_wnd_;
		switch_camera_pop_wnd_ = nullptr;
	}

	if (bottom_bar_wnd_)
	{
		if (::IsWindow(bottom_bar_wnd_->GetHWND()))
		{
			DestroyWindow(bottom_bar_wnd_->GetHWND());
		}
		DestroyWindow(bottom_bar_wnd_->GetHWND());
		delete bottom_bar_wnd_;
		bottom_bar_wnd_ = nullptr;
	}

	if (share_tool_bar_wnd_)
	{
		if (::IsWindow(share_tool_bar_wnd_->GetHWND()))
		{
			DestroyWindow(share_tool_bar_wnd_->GetHWND());
		}
		delete share_tool_bar_wnd_;
		share_tool_bar_wnd_ = nullptr;
	}

	for (auto iter = cmd_channel_wnd_map_.begin(); iter != cmd_channel_wnd_map_.end(); iter++)
	{
		IParseChannelCmdWnd* cmd_channel_wnd = iter->second;
		if (cmd_channel_wnd)
		{
			if (::IsWindow(cmd_channel_wnd->GetHWND()))
				DestroyWindow(cmd_channel_wnd->GetHWND());

			cmd_channel_wnd = nullptr;
		}
	}
	
}


CMainFrame& CMainFrame::GetInstance()
{
	static CMainFrame s_MainFramInstance;
	return s_MainFramInstance;
}

void CMainFrame::InitVideoSDK()
{
	ZoomVideoSDKInitParams init_params;
	init_params.domain = _T("https://go.zoom.us");	
	init_params.enableLog = true;
	init_params.logFilePrefix = _T("zoom_win_video_demo");
	init_params.videoRawDataMemoryMode = ZoomVideoSDKRawDataMemoryModeHeap;
	init_params.shareRawDataMemoryMode = ZoomVideoSDKRawDataMemoryModeHeap;
	init_params.audioRawDataMemoryMode = ZoomVideoSDKRawDataMemoryModeHeap;
	init_params.enableIndirectRawdata = false;

	ZoomVideoSDKMgr::GetInst().Init(this, init_params);
}

void CMainFrame::UninitVideoSDK()
{
	ZoomVideoSDKMgr::GetInst().UnInit();
}

void CMainFrame::InitControls()
{
	// 각 객체 컨트롤
	CreateJoinSessionWnd();
	CreateTitleBarWnd();
	CreateSwitchCamera();
	CreateRemoteControlCamera();
	CreateCameraControlRequestReceivedWnd();
	CreateStopCameraControlWnd();
	CreateGalleryMorePopWnd();
	CreateControlCameraTipWnd();
	CreateControlCameraPopWnd();
	CreateTopTipWnd();
	CreateSwitchCameraPopWnd();
	CreateBottomBarWnd();
	CreateLeaveSessionTipsWnd();
	CreateSessionInfoWnd();
	CreateSessionSettingsWnd();
	CreateLowerThirdsSettingWnd();
	CreateShareSelectAppsWnd();
	CreateEmojiBarWnd();
	CreateBulletScreenWnd();
	CreateLowerThirdsInfoWnd();
	CreateChatContentWnd();
	CreateTurnPageWnd();
	CreateMessageTipWnd();
	CreateStopShareWnd();
	CreateFeedBackLaunchWnd();
	CreateFeedBackSubmitWnd();
	CreateMorePopWnd();
}

void CMainFrame::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UpdateAllOwnedWndPos();

	ShowFeedBackLaunchWnd(false);
	ShowFeedBackSubmitWnd(false);
}
static int i = 0;
void CMainFrame::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (wParam == HIDE_BOTTOM_BAR_TIMER)
	{
		KillBottomBarHideTimer();
		ShowBottomBarWnd(false);
		ShowEmojiBarWnd(false);
	}
	else if (wParam == HIDE_ERROR_TIP_TIMER)
	{
		KillTimer(m_hWnd, HIDE_ERROR_TIP_TIMER);
		ShowMessageTipWnd(false);
	}
	else if (wParam == HIDE_CONTROLL_CAMERA_POP_WND_TIMER)
	{
		KillTimer(m_hWnd, HIDE_CONTROLL_CAMERA_POP_WND_TIMER);
		ShowControlCameraPopWnd(false);
	}
	else if (wParam == HIDE_TOP_TIP_TIMER)
	{
		KillTimer(m_hWnd, HIDE_TOP_TIP_TIMER);
		ShowTopTipWnd(false);
	}
}

void CMainFrame::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (wParam == 1)
	{
		if (title_bar_wnd_ && ::IsWindow(title_bar_wnd_->GetHWND()))
		{
			title_bar_wnd_->ShowWindow(true);
		}
	}
	else
	{
		HideAllOwnedWnd();
	}
}

void CMainFrame::PopupAppWindow()
{
	if (join_session_wnd_ && ::IsWindow(join_session_wnd_->GetHWND()))
	{
		if (::IsWindowVisible(join_session_wnd_->GetHWND()) && !::IsWindowVisible(m_hWnd)) //current is in join session window
		{
			::SetForegroundWindow(join_session_wnd_->GetHWND());
		}
		else
		{
			if (::IsIconic(m_hWnd)) //if minimize
			{
				::ShowWindow(CMainFrame::GetInstance().GetHWND(), SW_RESTORE);
			}
			::SetForegroundWindow(m_hWnd);
		}
	}
}

void CMainFrame::OnCopyData(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	COPYDATASTRUCT Copy_data = *(COPYDATASTRUCT*)lParam;
	if (Copy_data.dwData == POPUP_EXIST_APP)
	{
		PopupAppWindow();
	}
}

void CMainFrame::OnIconNotify(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (lParam == WM_LBUTTONDOWN)
	{
		PopupAppWindow();
	}
}

void CMainFrame::UpdateParticipantNum()
{
	if (bottom_bar_wnd_)
	{
		bottom_bar_wnd_->UpdateParticipantNum();
	}

	if (share_tool_bar_wnd_)
	{
		share_tool_bar_wnd_->UpdateParticipantNum();
	}
}

void CMainFrame::UpdateFeedBackNum(int feedback_num)
{
	if (bottom_bar_wnd_)
	{
		bottom_bar_wnd_->UpdateFeedBackNum(feedback_num);
	}
}

void CMainFrame::UpdateAllOwnedWndPos()
{
	UpdateTtileBarWndPos();
	UpdateBottomBarWndPos();
	UpdateChatContentWndPos();
	UpdateTurnUpPageWndPos();
	UpdateTurnDownPageWndPos();
	UpdateMessageTipWndPos();
	UpdateLowerThirdsWndPos();
	UpdateBulletScreenWndPos();
	UpdateSwitchCameraWndPos();
	UpdateRemoteControlCameraWndPos();
	UpdateControlCameraTipWndPos();
	UpdateCameraControlRequestReceivedWndPos();
	UpdateTopTipWndPos();
}

void CMainFrame::UpdateCameraControlRequestReceivedWndPos()
{
	if (camera_control_request_wnd_ && ::IsWindow(camera_control_request_wnd_->GetHWND()))
	{
		RECT rect = GetMainFrameCenterRect(camera_control_request_wnd_->GetHWND());
		::SetWindowPos(camera_control_request_wnd_->GetHWND(), NULL, rect.left, rect.top, 0, 0, SWP_NOSIZE);
	}
}
void CMainFrame::UpdateControlCameraTipWndPos()
{
	if (control_camera_tip_wnd_ && ::IsWindow(control_camera_tip_wnd_->GetHWND()))
	{
		RECT rcMain = { 0 };
		GetWindowRect(m_hWnd, &rcMain);
		SetWindowPos(control_camera_tip_wnd_->GetHWND(), HWND_TOP, rcMain.left + DEFAULT_BTN_GAP, rcMain.top + DEFAULT_ROW_GAP * 4, SMALL_ICON_SIZE, SMALL_ICON_SIZE, SWP_NOACTIVATE);
	}
	if (control_camera_pop_wnd_ && ::IsWindow(control_camera_pop_wnd_->GetHWND()))
	{
		RECT rcWindow = { 0 };
		::GetWindowRect(control_camera_tip_wnd_->GetHWND(), &rcWindow);
		POINT pt_temp = { rcWindow.left, rcWindow.top };
		int iXpos = 0, iYpos = 0;
		iXpos = pt_temp.x;
		iYpos = pt_temp.y + DEFAULT_COLUMN_GAP + CHAT_CONTENT_LEFT_MARGIN;

		::SetWindowPos(control_camera_pop_wnd_->GetHWND(), NULL, iXpos, iYpos, 0, 0, SWP_NOSIZE);
	}
}
void CMainFrame::UpdateSwitchCameraWndPos()
{
	if (switch_camera_wnd_ && ::IsWindow(switch_camera_wnd_->GetHWND()))
	{
		RECT rcMain = { 0 };
		GetWindowRect(m_hWnd, &rcMain);
		SetWindowPos(switch_camera_wnd_->GetHWND(), HWND_TOP, rcMain.left + DEFAULT_BTN_GAP, rcMain.top + DEFAULT_ROW_GAP * 4, SMALL_ICON_SIZE, SMALL_ICON_SIZE, SWP_NOACTIVATE);
	}
	if (switch_camera_pop_wnd_ && ::IsWindow(switch_camera_pop_wnd_->GetHWND()))
	{
		RECT rcWindow = { 0 };
		::GetWindowRect(switch_camera_wnd_->GetHWND(), &rcWindow);
		POINT pt_temp = { rcWindow.left, rcWindow.top };
		int iXpos = 0, iYpos = 0;
		iXpos = pt_temp.x;
		iYpos = pt_temp.y + DEFAULT_COLUMN_GAP + CHAT_CONTENT_LEFT_MARGIN;

		::SetWindowPos(switch_camera_pop_wnd_->GetHWND(), NULL, iXpos, iYpos, 0, 0, SWP_NOSIZE);
	}
}
void CMainFrame::UpdateRemoteControlCameraWndPos()
{
	if (remote_control_camera_wnd_ && ::IsWindow(remote_control_camera_wnd_->GetHWND()))
	{
		RECT rcMain = { 0 };
		GetWindowRect(m_hWnd, &rcMain);

		int gallery_width = 0;
		if (video_show_mgr_)
		{
			gallery_width = video_show_mgr_->GetGalleryContainerGalleryWidth();
		}
		if (is_show_gallery_)
		{
			
			SetWindowPos(remote_control_camera_wnd_->GetHWND(), HWND_TOPMOST, rcMain.left + 700, rcMain.top + 450, 152, 96, SWP_NOACTIVATE);
		}
		else
		{
			SetWindowPos(remote_control_camera_wnd_->GetHWND(), HWND_TOPMOST, rcMain.left + 700 + gallery_width + 20, rcMain.top + 450, 152, 96, SWP_NOACTIVATE);
		}
	}
}
void CMainFrame::UpdateTopTipWndPos()
{
	if (!top_tip_wnd_ || !::IsWindow(top_tip_wnd_->GetHWND()))
		return;
	RECT rcClient;
	GetClientRect(top_tip_wnd_->GetHWND(), &rcClient);
	int client_width = top_tip_wnd_->GetWndWidth();
	int client_height = rcClient.bottom - rcClient.top;
	RECT rcMain = { 0 };
	GetWindowRect(m_hWnd, &rcMain);

	if (is_show_gallery_)
	{
		int gallery_width = 0;
		if (video_show_mgr_)
		{
			gallery_width = video_show_mgr_->GetGalleryContainerGalleryWidth();
		}
		SetWindowPos(top_tip_wnd_->GetHWND(), HWND_TOP, rcMain.left + (rcMain.right - rcMain.left - gallery_width)/2 - client_width/2, rcMain.top + 50, client_width, client_height, SWP_NOACTIVATE);
	}
	else
	{
		SetWindowPos(top_tip_wnd_->GetHWND(), HWND_TOP, rcMain.left+ (rcMain.right - rcMain.left) / 2 - client_width / 2, rcMain.top+50, client_width, client_height, SWP_NOACTIVATE);
	}
}

void CMainFrame::UpdateTtileBarWndPos()
{
	if (title_bar_wnd_ && ::IsWindow(title_bar_wnd_->GetHWND()))
	{
		RECT rcMain = { 0 };
		GetWindowRect(m_hWnd, &rcMain);

		if (is_show_gallery_)
		{
			int gallery_width = 0;
			if (video_show_mgr_)
			{
				gallery_width = video_show_mgr_->GetGalleryContainerGalleryWidth();
			}
			SetWindowPos(title_bar_wnd_->GetHWND(), HWND_TOP, rcMain.left, rcMain.top, rcMain.right - rcMain.left - gallery_width, CPATION_HEIGHT, SWP_NOACTIVATE);
		}
		else
		{
			SetWindowPos(title_bar_wnd_->GetHWND(), HWND_TOP, rcMain.left, rcMain.top, rcMain.right - rcMain.left, CPATION_HEIGHT, SWP_NOACTIVATE);
		}
	}
}

void CMainFrame::UpdateBulletScreenWndPos()
{
	if (bullet_screen_wnd_ && ::IsWindow(bullet_screen_wnd_->GetHWND()))
	{
		RECT rcMain = { 0 };
		GetWindowRect(m_hWnd, &rcMain);

		CDuiRect rcBulletScreen;
		GetClientRect(bullet_screen_wnd_->GetHWND(), &rcBulletScreen);

		SetWindowPos(bullet_screen_wnd_->GetHWND(), HWND_TOP, rcMain.left + LOWERTHIRDS_PADDING_LEFT, rcMain.top + LOWERTHIRDS_PADDING_TOP, \
			rcBulletScreen.GetWidth(), rcBulletScreen.GetHeight(), SWP_NOACTIVATE);

	}
}

void CMainFrame::UpdateLowerThirdsWndPos()
{
	if (lower_thirds_info_wnd_ && ::IsWindow(lower_thirds_info_wnd_->GetHWND()))
	{
		RECT rcMain = { 0 };
		GetWindowRect(m_hWnd, &rcMain);

		CDuiRect rcLowerThirds;
		GetClientRect(lower_thirds_info_wnd_->GetHWND(), &rcLowerThirds);
				
		SetWindowPos(lower_thirds_info_wnd_->GetHWND(), HWND_TOP, rcMain.left + LOWERTHIRDS_PADDING_LEFT, rcMain.top + LOWERTHIRDS_PADDING_TOP,\
			rcLowerThirds.GetWidth(), rcLowerThirds.GetHeight(), SWP_NOACTIVATE);
		
	}
}

void CMainFrame::UpdateBottomBarWndPos()
{
	if (bottom_bar_wnd_ && ::IsWindow(bottom_bar_wnd_->GetHWND()))
	{
		RECT rcMain = { 0 };
		GetWindowRect(m_hWnd, &rcMain);

		if (is_show_gallery_)
		{
			int gallery_width = 0;
			if (video_show_mgr_)
			{
				gallery_width = video_show_mgr_->GetGalleryContainerGalleryWidth();
			}
			SetWindowPos(bottom_bar_wnd_->GetHWND(), HWND_TOP, rcMain.left, rcMain.bottom - BOTTOM_BAR_HEIGHT - BOTTOM_BAR_MARGIN, rcMain.right - rcMain.left - gallery_width, BOTTOM_BAR_HEIGHT, SWP_NOACTIVATE);
		}
		else
		{
			SetWindowPos(bottom_bar_wnd_->GetHWND(), HWND_TOP, rcMain.left, rcMain.bottom - BOTTOM_BAR_HEIGHT - BOTTOM_BAR_MARGIN, rcMain.right - rcMain.left, BOTTOM_BAR_HEIGHT, SWP_NOACTIVATE);
		}	
	}
}

void CMainFrame::UpdateChatContentWndPos(bool show)
{
	if (chat_content_wnd_ && ::IsWindow(chat_content_wnd_->GetHWND()))
	{
		RECT rect_chat_content_wnd = { 0 };
		GetClientRect(chat_content_wnd_->GetHWND(), &rect_chat_content_wnd);
		int chat_content_wnd_height = rect_chat_content_wnd.bottom - rect_chat_content_wnd.top;

		bool is_bottom_bar_visible = false;
		if (bottom_bar_wnd_ && ::IsWindow(bottom_bar_wnd_->GetHWND()))
		{
			is_bottom_bar_visible = ::IsWindowVisible(bottom_bar_wnd_->GetHWND());
		}

		int x_pos = 0, y_pos = 0;
		if (is_bottom_bar_visible)
		{
			RECT rect_bottom_screen = { 0 };
			if (bottom_bar_wnd_ && ::IsWindow(bottom_bar_wnd_->GetHWND()))
			{
				GetWindowRect(bottom_bar_wnd_->GetHWND(), &rect_bottom_screen);
				x_pos = rect_bottom_screen.left + CHAT_CONTENT_LEFT_MARGIN;
				y_pos = rect_bottom_screen.top - chat_content_wnd_height;
			}
		}
		else
		{
			RECT rcMain = { 0 };
			GetWindowRect(m_hWnd, &rcMain);

			x_pos = rcMain.left + CHAT_CONTENT_LEFT_MARGIN;
			y_pos = rcMain.bottom - BOTTOM_BAR_MARGIN - chat_content_wnd_height;
		}

		UINT uFlags = SWP_NOSIZE | SWP_NOACTIVATE;
		if (show) //not use function ShowWindow, because ShowWindow can get edit focus,not want
		{
			uFlags |= SWP_SHOWWINDOW;
		}

		::SetWindowPos(chat_content_wnd_->GetHWND(), NULL, x_pos, y_pos, 0, 0, uFlags);
	}
}

void CMainFrame::UpdateTurnUpPageWndPos()
{
	if (turn_up_page_wnd_ && ::IsWindow(turn_up_page_wnd_->GetHWND()))
	{
		RECT rcClient;
		GetClientRect(turn_up_page_wnd_->GetHWND(), &rcClient);

		int iXpos = 0, iYpos = 0;

		int gallery_width = 0;
		if (video_show_mgr_)
		{
			gallery_width = video_show_mgr_->GetGalleryContainerGalleryWidth();
		}

		RECT rect_main;
		GetWindowRect(m_hWnd, &rect_main);
		int client_width = rcClient.right - rcClient.left;
		iXpos = rect_main.right - gallery_width + (gallery_width - client_width) / 2;
		iYpos = rect_main.top + TURN_PAGE_MARIGN;

		::SetWindowPos(turn_up_page_wnd_->GetHWND(), NULL, iXpos, iYpos, 0, 0, SWP_NOSIZE);
	}
}

void CMainFrame::UpdateTurnDownPageWndPos()
{
	if (turn_down_page_wnd_ && ::IsWindow(turn_down_page_wnd_->GetHWND()))
	{
		RECT rcClient;
		GetClientRect(turn_down_page_wnd_->GetHWND(), &rcClient);

		int iXpos = 0, iYpos = 0;

		int gallery_width = 0;
		if (video_show_mgr_)
		{
			gallery_width = video_show_mgr_->GetGalleryContainerGalleryWidth();
		}

		RECT rect_main;
		GetWindowRect(m_hWnd, &rect_main);
		int client_width = rcClient.right - rcClient.left;
		int client_height = rcClient.bottom - rcClient.top;
		iXpos = rect_main.right - gallery_width + (gallery_width - client_width) / 2;
		iYpos = rect_main.bottom - TURN_PAGE_MARIGN - client_height;

		::SetWindowPos(turn_down_page_wnd_->GetHWND(), NULL, iXpos, iYpos, 0, 0, SWP_NOSIZE);
	}
}

void CMainFrame::UpdateShareToolBarWndPos()
{
	if (share_tool_bar_wnd_ && ::IsWindow(share_tool_bar_wnd_->GetHWND()))
	{
		RECT rect_client;
		GetClientRect(share_tool_bar_wnd_->GetHWND(), &rect_client);
		int width = rect_client.right - rect_client.left;
		SetWindowPos(share_tool_bar_wnd_->GetHWND(), HWND_TOPMOST, GetSystemMetrics(SM_CXSCREEN) - width - 50, 50, 0, 0, SWP_NOSIZE);
		share_tool_bar_wnd_->ShowWindow(true);
	}
}

void CMainFrame::UpdateMessageTipWndPos()
{
	if(!bottom_bar_wnd_ || !IsWindow(bottom_bar_wnd_->GetHWND()))
		return;

	if (!message_tip_wnd_ || !::IsWindow(message_tip_wnd_->GetHWND()))
		return;

	if (!chat_content_wnd_ || !::IsWindow(chat_content_wnd_->GetHWND()))
		return;

	RECT rcClient;
	GetClientRect(message_tip_wnd_->GetHWND(), &rcClient);
	int client_width = message_tip_wnd_->GetWndWidth();
	int client_height = rcClient.bottom - rcClient.top;

	int iXpos = 0, iYpos = 0;
	RECT rect;
	if (::IsWindowVisible(chat_content_wnd_->GetHWND()))
	{
		GetWindowRect(chat_content_wnd_->GetHWND(), &rect);
		iXpos = rect.left;
		iYpos = rect.top - client_height - 10;
	}
	else if(::IsWindowVisible(bottom_bar_wnd_->GetHWND()))
	{
		GetWindowRect(bottom_bar_wnd_->GetHWND(), &rect);
		iXpos = rect.left + CHAT_CONTENT_LEFT_MARGIN;
		iYpos = rect.top - client_height - 10;
	}
	else
	{
		GetWindowRect(m_hWnd, &rect);
		iXpos = rect.left + CHAT_CONTENT_LEFT_MARGIN;
		iYpos = rect.bottom - client_height - 10;
	}

	::SetWindowPos(message_tip_wnd_->GetHWND(), NULL, iXpos, iYpos, client_width, client_height, SWP_NOACTIVATE);
}

void CMainFrame::HideOrShowTurnPageWnd()
{
	if (is_show_gallery_)
	{
		int total_page = video_show_mgr_->GetGalleryContainerTotalPage();
		int cur_page = video_show_mgr_->GetGalleryContainerCurPage();

		if (total_page > cur_page)
		{
			if (cur_page == 1)
			{
				ShowTurnUpPageWnd(false);
				ShowTurnDownPageWnd(true);
			}
			else
			{
				ShowTurnUpPageWnd(true);
				ShowTurnDownPageWnd(true);
			}
		}
		else if (total_page == cur_page)
		{
			if (total_page > 1)
			{
				ShowTurnUpPageWnd(true);
				ShowTurnDownPageWnd(false);
			}
			else
			{
				ShowTurnUpPageWnd(false);
				ShowTurnDownPageWnd(false);
			}
		}
	}
	else
	{
		ShowTurnUpPageWnd(false);
		ShowTurnDownPageWnd(false);
	}
}

void CMainFrame::HideTurnPageWnd()
{
	ShowTurnUpPageWnd(false);
	ShowTurnDownPageWnd(false);
}

void CMainFrame::HideAllOwnedWnd()
{
	if (title_bar_wnd_ && ::IsWindow(title_bar_wnd_->GetHWND()))
	{
		title_bar_wnd_->ShowWindow(false);
	}

	ShowRemoteControlCameraWnd(false);
	ShowCameraControlRequestReceivedWnd(false);
	ShowStopCameraControlWnd(false);
	ShowTopTipWnd(false);
	ShowBottomBarWnd(false);
	ShowEmojiBarWnd(false);
	ShowMessageTipWnd(false);	
	HideTurnPageWnd();
}

void CMainFrame::InsertContentToChatContentWnd(const zchar_t* user_name, const zchar_t* content)
{
	if (chat_content_wnd_ && ::IsWindow(chat_content_wnd_->GetHWND()))
	{
		chat_content_wnd_->InsertChatContent(user_name,content);
	}
}

void CMainFrame::OnLeaveSessionUIUpdate()
{
	if (chat_content_wnd_ && ::IsWindow(chat_content_wnd_->GetHWND()))
	{
		chat_content_wnd_->ClearChatContent();
		chat_content_wnd_->ShowWindow(false);
	}

	if (join_session_wnd_ && ::IsWindow(join_session_wnd_->GetHWND()))
	{
		join_session_wnd_->BackToInitPage();
		join_session_wnd_->ShowWindow(true);
	}

	for (auto iter = cmd_channel_wnd_map_.begin(); iter != cmd_channel_wnd_map_.end(); iter++)
	{
		IParseChannelCmdWnd* cmd_channel_wnd = iter->second;
		if (cmd_channel_wnd)
		{
			::ShowWindow(cmd_channel_wnd->GetHWND(), false);
		}
	}

	HideAllOwnedWnd();

	StopShare();

	ShowWindow(false);
}

void CMainFrame::OnJoinMettingJoinFailed(int error_code)
{
	if (join_session_wnd_)
	{
		join_session_wnd_->OnJoinMettingJoinFailed(error_code);
	}
}

void CMainFrame::OnInvalidParamterNotification()
{
	if (join_session_wnd_)
		join_session_wnd_->OnInvalidParamterNotification();
}

void CMainFrame::OnMeetingDisconnecting()
{
	if (video_show_mgr_)
	{
		is_show_gallery_ = false;
		video_show_mgr_->ContainerInit();
	}

	StartPreview();
}

void CMainFrame::SetNotifyIconData(NOTIFYICONDATA& nid)
{
	SecureZeroMemory(&nid, sizeof(NOTIFYICONDATA)); 
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = m_hWnd;
	nid.uID = 1;
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.hIcon = ::LoadIcon(m_PaintManager.GetInstance(), (LPCWSTR)IDI_ICON_MAIN);
	lstrcpyn(nid.szTip, APP_NAME, sizeof(nid.szTip) / sizeof(TCHAR));
	nid.uCallbackMessage = WM_ICON_NOTIFY;
}

void CMainFrame::AddTheTray()
{
	NOTIFYICONDATA	nid;
	SetNotifyIconData(nid);

	::Shell_NotifyIcon(NIM_ADD, &nid);
}

void CMainFrame::DeleteTheTray()
{
	NOTIFYICONDATA	nid;
	SetNotifyIconData(nid);

	::Shell_NotifyIcon(NIM_DELETE, &nid);
}

void CMainFrame::InitWindow()
{
	InitControls();

	InitVideoSDK();

	SetIcon(IDI_ICON_MAIN);
	AddTheTray();
}

CDuiString CMainFrame::GetSkinFolder()
{
	return _T("");
}

CDuiString CMainFrame::GetSkinFile()
{
	return to_wstring(IDXML_MAINFRAM_UI).c_str();
}

LPCTSTR CMainFrame::GetWindowClassName(void) const
{
	return _T("video_sdk_demo");
}

UILIB_RESOURCETYPE CMainFrame::GetResourceType() const
{
	return UILIB_RESOURCE;
}

LPCTSTR CMainFrame::GetResourceID() const
{
	return _T("SKINDATA");
}

CControlUI* CMainFrame::CreateControl(LPCTSTR pstrClass)
{
	return nullptr;
}

LRESULT CMainFrame::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	else if (wParam == VK_F1)
	{
		
	}
	return FALSE;
}

void CMainFrame::CreateJoinSessionWnd()
{
	if (join_session_wnd_ == nullptr)
	{
		join_session_wnd_ = new CJoinSessionWndUI;
	}

	if (join_session_wnd_)
	{
		join_session_wnd_->Create(nullptr, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);		
		join_session_wnd_->CenterWindow();
		join_session_wnd_->ShowWindow(true);
	}
}

void CMainFrame::CreateTitleBarWnd()
{
	if (title_bar_wnd_ == nullptr)
	{
		title_bar_wnd_ = new CTitleBarWndUI;
	}

	if (title_bar_wnd_)
	{
		title_bar_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		title_bar_wnd_->ShowWindow(false);
	}
}

void CMainFrame::CreateSwitchCameraPopWnd()
{
	if (switch_camera_pop_wnd_ == nullptr)
	{
		switch_camera_pop_wnd_ = new CSwitchCameraPopWnd;
	}

	if (switch_camera_pop_wnd_)
	{
		switch_camera_pop_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		switch_camera_pop_wnd_->ShowWindow(false);
	}
}
void CMainFrame::CreateControlCameraPopWnd()
{
	if (control_camera_pop_wnd_ == nullptr)
	{
		control_camera_pop_wnd_ = new CControlCameraPopWnd;
	}

	if (control_camera_pop_wnd_)
	{
		control_camera_pop_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		control_camera_pop_wnd_->ShowWindow(false);
	}
}
void CMainFrame::CreateControlCameraTipWnd()
{
	if (control_camera_tip_wnd_ == nullptr)
	{
		control_camera_tip_wnd_ = new CControlCameraTipWndUI;
	}

	if (control_camera_tip_wnd_)
	{
		control_camera_tip_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		control_camera_tip_wnd_->ShowWindow(false);
	}
}
void CMainFrame::CreateSwitchCamera()
{
	if (switch_camera_wnd_ == nullptr)
	{
		switch_camera_wnd_ = new CSwitchCameraWndUI;
	}

	if (switch_camera_wnd_)
	{
		switch_camera_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		switch_camera_wnd_->ShowWindow(false);
	}
}
void CMainFrame::CreateRemoteControlCamera()
{
	if (remote_control_camera_wnd_ == nullptr)
	{
		remote_control_camera_wnd_ = new CRemoteControlCameraWndUI;
	}

	if (remote_control_camera_wnd_)
	{
		remote_control_camera_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		remote_control_camera_wnd_->ShowWindow(false);
	}
}
void CMainFrame::CreateCameraControlRequestReceivedWnd()
{
	if (camera_control_request_wnd_ == nullptr)
	{
		camera_control_request_wnd_ = new CCameraControlRequestReceivedWndUI;
	}

	if (camera_control_request_wnd_)
	{
		camera_control_request_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		camera_control_request_wnd_->ShowWindow(false);
	}
}
void CMainFrame::CreateStopCameraControlWnd()
{
	if (stop_camera_control_wnd_ == nullptr)
	{
		stop_camera_control_wnd_ = new CStopCameraControlWndUI;
	}

	if (stop_camera_control_wnd_)
	{
		stop_camera_control_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		stop_camera_control_wnd_->ShowWindow(false);
	}
}
void CMainFrame::CreateGalleryMorePopWnd()
{
	if (gallery_more_pop_wnd_ == nullptr)
	{
		gallery_more_pop_wnd_ = new CGalleryMorePopWnd();
	}

	if (gallery_more_pop_wnd_)
	{
		gallery_more_pop_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		gallery_more_pop_wnd_->ShowWindow(false);
	}
}
void CMainFrame::CreateTopTipWnd()
{
	if (top_tip_wnd_ == nullptr)
	{
		top_tip_wnd_ = new TopTipWndUI();
	}

	if (top_tip_wnd_)
	{
		top_tip_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		top_tip_wnd_->ShowWindow(false);
	}
}

void CMainFrame::CreateBottomBarWnd()
{
	if (bottom_bar_wnd_ == nullptr)
	{
		bottom_bar_wnd_ = new CBottomBarWndUI;
	}

	if (bottom_bar_wnd_)
	{
		bottom_bar_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		bottom_bar_wnd_->ShowWindow(false);
	}
}

void CMainFrame::CreateLeaveSessionTipsWnd()
{
	if (leave_session_tips_wnd_ == nullptr)
	{
		leave_session_tips_wnd_ = new CLeaveSessionTipsWndUI;
	}

	if (leave_session_tips_wnd_)
	{
		leave_session_tips_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		leave_session_tips_wnd_->ShowWindow(false);
	}
}

void CMainFrame::CreateSessionInfoWnd()
{
	if (session_infos_wnd_ == nullptr)
	{
		session_infos_wnd_ = new CSessionInfoWndUI;
	}

	if (session_infos_wnd_)
	{
		session_infos_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		session_infos_wnd_->ShowWindow(false);
	}
}

void CMainFrame::CreateSessionSettingsWnd()
{
	if (session_setting_wnd_ == nullptr)
	{
		session_setting_wnd_ = new CSessionSettingsWndUI();
	}

	if (session_setting_wnd_)
	{
		session_setting_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		session_setting_wnd_->ShowWindow(false);
	}
}

void CMainFrame::CreateLowerThirdsSettingWnd()
{
	if (lower_thirds_setting_wnd_ == nullptr)
	{
		lower_thirds_setting_wnd_ = new CLowerThirdsSettingWndUI();
	}

	if (lower_thirds_setting_wnd_)
	{
		lower_thirds_setting_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		lower_thirds_setting_wnd_->ShowWindow(false);
	}
}

void CMainFrame::CreateShareSelectAppsWnd()
{
	if (share_select_apps_wnd_ == nullptr)
	{
		share_select_apps_wnd_ = new ShareSelectAppsWndUI();
	}

	if (share_select_apps_wnd_)
	{
		share_select_apps_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		share_select_apps_wnd_->ShowWindow(false);
	}
}

void CMainFrame::CreateEmojiBarWnd()
{
	if (reaction_bar_wnd_ == nullptr)
	{
		reaction_bar_wnd_ = new CReactionBarWnd();
	}

	if (reaction_bar_wnd_)
	{
		reaction_bar_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		reaction_bar_wnd_->ShowWindow(false);
	}
}

void CMainFrame::CreateMorePopWnd()
{
	if (more_pop_wnd_ == nullptr)
	{
		more_pop_wnd_ = new CMorePopWnd();
	}

	if (more_pop_wnd_)
	{
		more_pop_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		more_pop_wnd_->ShowWindow(false);
	}
}

void CMainFrame::CreateBulletScreenWnd()
{
	if (bullet_screen_wnd_ == nullptr)
	{
		bullet_screen_wnd_ = new CBulletScreenWnd();
	}

	if (bullet_screen_wnd_)
	{
		bullet_screen_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		bullet_screen_wnd_->ShowWindow(false);
		cmd_channel_wnd_map_[CmdChannelType::CMD_REACTION] = bullet_screen_wnd_;
	}
}

void CMainFrame::CreateLowerThirdsInfoWnd()
{
	if (lower_thirds_info_wnd_ == nullptr)
		lower_thirds_info_wnd_ = new CLowerThirdsInfoWnd();

	if (lower_thirds_info_wnd_)
	{
		lower_thirds_info_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		lower_thirds_info_wnd_->ShowWindow(false);
		cmd_channel_wnd_map_[CmdChannelType::CMD_LOWER_THIRD] = lower_thirds_info_wnd_;
	}
}

void CMainFrame::CreateFeedBackLaunchWnd()
{
	if (feedback_launch_wnd_ == nullptr)
		feedback_launch_wnd_ = new CFeedBackLaunchWnd();

	if (feedback_launch_wnd_)
	{
		feedback_launch_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		feedback_launch_wnd_->ShowWindow(false);
		cmd_channel_wnd_map_[CmdChannelType::CMD_FEEDBACK_SUBMIT] = feedback_launch_wnd_;
	}
}

void CMainFrame::CreateFeedBackSubmitWnd()
{
	if (feedback_submit_wnd_ == nullptr)
		feedback_submit_wnd_ = new CFeedBackSubmitWnd();

	if (feedback_submit_wnd_)
	{
		feedback_submit_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		feedback_submit_wnd_->ShowWindow(false);
		cmd_channel_wnd_map_[CmdChannelType::CMD_FEEDBACK_LAUNCH] = feedback_submit_wnd_;
	}
}
void CMainFrame::CreateStatisticsWnd()
{
	if (statistics_wnd_)
	{
		delete statistics_wnd_;
		statistics_wnd_ = nullptr;
	}
	if (statistics_wnd_ == nullptr)
		statistics_wnd_ = new StatisticsWndUI();

	if (statistics_wnd_)
	{
		statistics_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		statistics_wnd_->ShowWindow(false);
		
	}
}

void CMainFrame::CreateStopShareWnd()
{
	if (share_tool_bar_wnd_)
	{
		delete share_tool_bar_wnd_;
		share_tool_bar_wnd_ = nullptr;
	}

	if (share_tool_bar_wnd_ == nullptr)
	{
		share_tool_bar_wnd_ = new ShareToolBarWndUI();
	}

	if (share_tool_bar_wnd_ && !::IsWindow(share_tool_bar_wnd_->GetHWND()))
	{
		share_tool_bar_wnd_->Create(nullptr, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		share_tool_bar_wnd_->ShowWindow(false);
	}
}

void CMainFrame::CreateChatContentWnd()
{
	if (chat_content_wnd_ == nullptr)
	{
		chat_content_wnd_ = new ChatContentWndUI();
	}

	if (chat_content_wnd_)
	{
		chat_content_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		//-----[code:bananackck] 채팅창크게
		chat_content_wnd_->ResizeClient(720, 100);
		chat_content_wnd_->ShowWindow(false);
	}
}

void CMainFrame::CreateTurnPageWnd()
{
	if (turn_up_page_wnd_ == nullptr)
	{
		turn_up_page_wnd_ = new TurnPageWndUI();
	}

	if (turn_up_page_wnd_)
	{
		turn_up_page_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		turn_up_page_wnd_->HideDownPageBtn();
		turn_up_page_wnd_->ShowWindow(false);
	}

	if (turn_down_page_wnd_ == nullptr)
	{
		turn_down_page_wnd_ = new TurnPageWndUI();
	}

	if (turn_down_page_wnd_)
	{
		turn_down_page_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		turn_down_page_wnd_->HideUpPageBtn();
		turn_down_page_wnd_->ShowWindow(false);
	}
}

void CMainFrame::CreateMessageTipWnd()
{
	if (message_tip_wnd_ == nullptr)
	{
		message_tip_wnd_ = new MessageTipWndUI();
	}

	if (message_tip_wnd_)
	{
		message_tip_wnd_->Create(m_hWnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		message_tip_wnd_->ShowWindow(false);
	}
}

LRESULT CMainFrame::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	return __super::MessageHandler(uMsg, wParam, lParam, bHandled);
}

LRESULT CMainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_MOVE:
		{
			OnMove(uMsg, wParam, lParam);
			break;
		}
		case WM_TIMER:
		{
			OnTimer(uMsg, wParam, lParam);
			break;
		}
		case WM_SHOWWINDOW:
		{
			OnShowWindow(uMsg, wParam, lParam);
			break;
		}
		case WM_COPYDATA:
		{
			OnCopyData(uMsg, wParam, lParam);
		}
		case WM_ICON_NOTIFY:
		{
			OnIconNotify(uMsg, wParam, lParam);
		}
	}
	
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT	CMainFrame::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (::GetAsyncKeyState(MK_LBUTTON) < 0)
	{
		if (IsZoomed(m_hWnd)) //maximize
			return 0;

		RECT rcCaption;
		GetClientRect(m_hWnd, &rcCaption);

		rcCaption.bottom = rcCaption.top + CPATION_HEIGHT;

		RECT size_box = m_PaintManager.GetSizeBox();
		rcCaption.left += size_box.left;
		rcCaption.top += size_box.top;
		rcCaption.right -= size_box.right;

		POINT pt;
		GetCursorPos(&pt);
		::ScreenToClient(m_PaintManager.GetPaintWindow(), &pt);
		if (::PtInRect(&rcCaption, pt))
		{
			return HTCAPTION;
		}
	}

	return __super::OnNcHitTest(uMsg, wParam, lParam, bHandled);
}

LRESULT CMainFrame::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostMessage(m_hWnd, WM_QUIT, NULL, NULL);

	return __super::OnDestroy(uMsg, wParam, lParam, bHandled);
}

LRESULT CMainFrame::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::OnLButtonDown(uMsg, wParam, lParam, bHandled);
}

LRESULT CMainFrame::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bool is_in_session = ZoomVideoSDKMgr::GetInst().IsInSession();

	if (!is_in_session)
		return __super::OnLButtonUp(uMsg, wParam, lParam, bHandled);

	ShowBottomBarWnd(true);

	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	if (video_show_mgr_)
	{
		video_show_mgr_->OnLButtonUp(pt);
	}

	return __super::OnLButtonUp(uMsg, wParam, lParam, bHandled);
}

LRESULT CMainFrame::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bool is_in_session = ZoomVideoSDKMgr::GetInst().IsInSession();

	if (video_show_mgr_ && is_in_session && is_show_gallery_)
	{
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		video_show_mgr_->OnMouseMove(pt);
	}
	
	return __super::OnMouseMove(uMsg, wParam, lParam, bHandled);
}

LRESULT CMainFrame::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{	
	if (!IsIconic(m_hWnd))
	{
		if (video_show_mgr_)
		{
			RECT rect_main = { 0 };
			GetClientRect(m_hWnd, &rect_main);

			RendererRect rect_renderer = { rect_main.left,rect_main.top,rect_main.right - rect_main.left,rect_main.bottom - rect_main.top };
			video_show_mgr_->OnSize(rect_renderer);

			HideOrShowTurnPageWnd();
		}

		UpdateAllOwnedWndPos();
	}

	return __super::OnSize(uMsg, wParam, lParam, bHandled);	
}

void CMainFrame::StartPreview()
{
	if (video_show_mgr_)
	{
		IZoomVideoSDKUser* myself = ZoomVideoSDKMgr::GetInst().GetMySelf();
		if (join_session_wnd_ && join_session_wnd_->IsSelectedVideoOff())
		{
			video_show_mgr_->AddRendererVideoItem(myself);
			video_show_mgr_->AddUserAvatarIcon(myself);
		}
		else
		{
			video_show_mgr_->SubscribeAllUser();
		}
	}

	//update ui
	UpdateTtileBarWndPos();
	UpdateBottomBarWndPos();
	UpdateSwitchCameraWndPos();
	UpdateRemoteControlCameraWndPos();
	UpdateControlCameraTipWndPos();
	UpdateCameraControlRequestReceivedWndPos();

	ShowBottomBarWnd(false);
	HideTurnPageWnd();

	if (title_bar_wnd_)
	{
		title_bar_wnd_->UpdateSessionName(_T("connecting..."));
	}
}

void CMainFrame::ShowBottomBarWnd(bool bShow)
{
	//-----[code:bananackck] 채팅 항상 보이게
	ShowChatContentWnd(true);
	if (bottom_bar_wnd_ && ::IsWindow(bottom_bar_wnd_->GetHWND()))
	{
		bottom_bar_wnd_->ShowWindow(bShow);

		UpdateChatContentWndPos();
		ShowChatContentWnd(bShow);

		UpdateMessageTipWndPos();
	}
}

void CMainFrame::SetBottomBarHideTimer()
{
	SetTimer(m_hWnd, HIDE_BOTTOM_BAR_TIMER, 5 * 1000, NULL);
}

void CMainFrame::KillBottomBarHideTimer()
{
	KillTimer(m_hWnd, HIDE_BOTTOM_BAR_TIMER);
}

void CMainFrame::ShowLeaveSessionTipsWnd(bool bShow)
{
	if (leave_session_tips_wnd_ && ::IsWindow(leave_session_tips_wnd_->GetHWND()))
	{
		if(bShow)
			ShowSessionSettingWnd(false);
		leave_session_tips_wnd_->ShowWindow(bShow);
		leave_session_tips_wnd_->CheckIsHost();

		RECT rect = GetMainFrameCenterRect(leave_session_tips_wnd_->GetHWND());
		::SetWindowPos(leave_session_tips_wnd_->GetHWND(), NULL, rect.left, rect.top, 0, 0, SWP_NOSIZE);
	}
}

void CMainFrame::ShowSessionInfoWnd(RECT rc)
{
	if (session_infos_wnd_ && ::IsWindow(session_infos_wnd_->GetHWND()))
	{
		session_infos_wnd_->ShowWindow(true);
		session_infos_wnd_->UpdateSessionInfoUI();

		RECT rcClient;
		GetClientRect(session_infos_wnd_->GetHWND(), &rcClient);

		int iXpos = 0, iYpos = 0;
		iXpos = rc.left + (rc.right - rc.left) / 2 - (rcClient.right - rcClient.left) / 2;
		iYpos = rc.bottom + 10;

		::SetWindowPos(session_infos_wnd_->GetHWND(), NULL, iXpos, iYpos, 0, 0, SWP_NOSIZE);
	}
}

void CMainFrame::ShowSessionSettingWnd(bool show, RECT rc)
{
	if (session_setting_wnd_ && ::IsWindow(session_setting_wnd_->GetHWND()))
	{
		if (show && !::IsWindowVisible(session_setting_wnd_->GetHWND()))
		{
			ShowLeaveSessionTipsWnd(false);
			bool is_in_session = ZoomVideoSDKMgr::GetInst().IsInSession();

			RECT rcClient;
			GetClientRect(session_setting_wnd_->GetHWND(), &rcClient);

			int iXpos = 0, iYpos = 0;
			if (!is_in_session)
			{
				iXpos = rc.left + ((rc.right - rc.left) - (rcClient.right - rcClient.left)) / 2;
				iYpos = rc.top + ((rc.bottom - rc.top) - (rcClient.bottom - rcClient.top)) / 2;;
			}
			else
			{
				iXpos = rc.right - SESSION_SETTING_WND_GAP - (rcClient.right - rcClient.left);
				iYpos = rc.top - SESSION_SETTING_WND_GAP - (rcClient.bottom - rcClient.top);
			}

			::SetWindowPos(session_setting_wnd_->GetHWND(), NULL, iXpos, iYpos, 0, 0, SWP_NOSIZE);
			
			session_setting_wnd_->ShowWindow(true);
			session_setting_wnd_->UpdateSettingsInfoUI();
			session_setting_wnd_->UpdateControl();
		}
		else
		{
			session_setting_wnd_->ShowWindow(false);
		}
	}
}

void CMainFrame::ShowLowerThirdsSettingWndUI(bool show,RECT rc)
{
	if (lower_thirds_setting_wnd_ && ::IsWindow(lower_thirds_setting_wnd_->GetHWND()))
	{
		if (show && !::IsWindowVisible(lower_thirds_setting_wnd_->GetHWND()))
		{
			RECT rcClient;
			GetClientRect(lower_thirds_setting_wnd_->GetHWND(), &rcClient);

			int iXpos = 0, iYpos = 0;
			
			iXpos = (rc.left + (rc.right - rc.left) / 2) - (rcClient.right - rcClient.left) / 2;
			iYpos = (rc.top + (rc.bottom - rc.top) / 2) - (rcClient.bottom - rcClient.top) / 2;
			
			::SetWindowPos(lower_thirds_setting_wnd_->GetHWND(), NULL, iXpos, iYpos, 0, 0, SWP_NOSIZE);

			lower_thirds_setting_wnd_->ShowWindow(true);
			lower_thirds_setting_wnd_->UpdateControl();
		}
		else
		{
			session_setting_wnd_->ShowWindow(false);
		}
	}
}

void CMainFrame::ShowShareSelectAppsWnd(bool show, RECT rc)
{
	if (share_select_apps_wnd_ && ::IsWindow(share_select_apps_wnd_->GetHWND()))
	{
		if (show)
		{
			share_select_apps_wnd_->ShowWindow(true);
			share_select_apps_wnd_->UpdateAllApps();

			RECT rcClient;
			GetClientRect(share_select_apps_wnd_->GetHWND(), &rcClient);

			int iXpos = 0, iYpos = 0;
			iXpos = rc.right + 250 - (rcClient.right - rcClient.left);
			iYpos = rc.top - SESSION_SETTING_WND_GAP - (rcClient.bottom - rcClient.top);

			::SetWindowPos(share_select_apps_wnd_->GetHWND(), NULL, iXpos, iYpos, 0, 0, SWP_NOSIZE);
		}
		else
		{
			share_select_apps_wnd_->ShowWindow(false);
		}
	}
}

void CMainFrame::ShowEmojiBarWnd(bool show, RECT rc)
{
	if (reaction_bar_wnd_ && ::IsWindow(reaction_bar_wnd_->GetHWND()))
	{
		if (show)
		{
			RECT rcClient;
			GetClientRect(reaction_bar_wnd_->GetHWND(), &rcClient);

			int iXpos = 0, iYpos = 0;
			iXpos = rc.right + 250 - (rcClient.right - rcClient.left);
			iYpos = rc.top - SESSION_SETTING_WND_GAP - (rcClient.bottom - rcClient.top);

			::SetWindowPos(reaction_bar_wnd_->GetHWND(), NULL, iXpos, iYpos, 0, 0, SWP_NOSIZE);
			reaction_bar_wnd_->ShowWindow(true);
		}
		else
		{
			reaction_bar_wnd_->ShowWindow(false);
		}
	}
}

void CMainFrame::ShowMorePopWnd(bool show, RECT rc)
{
	if (more_pop_wnd_ && ::IsWindow(more_pop_wnd_->GetHWND()))
	{
		if (show)
		{
			RECT rcClient;
			GetClientRect(more_pop_wnd_->GetHWND(), &rcClient);

			int iXpos = 0, iYpos = 0;
			iXpos = rc.left;
			iYpos = rc.top - SESSION_SETTING_WND_GAP - (rcClient.bottom - rcClient.top);

			::SetWindowPos(more_pop_wnd_->GetHWND(), NULL, iXpos, iYpos, 0, 0, SWP_NOSIZE);
			more_pop_wnd_->ShowWindow(true);
		}
		else
		{
			more_pop_wnd_->ShowWindow(false);
		}
	}
}

void CMainFrame::ShowChatContentWnd(bool show)
{
	if (chat_content_wnd_ && ::IsWindow(chat_content_wnd_->GetHWND()))
	{
		if (show && chat_content_wnd_->HasChatContent())
		{
			chat_content_wnd_->ShowWindow(true);
		}	
		else
			chat_content_wnd_->ShowWindow(false);

		UpdateMessageTipWndPos();
	}
}

void CMainFrame::SetActivityVideoEmojiPos(POINT emoji_pos)
{
	if (video_show_mgr_)
	{
		video_show_mgr_->SetActivityVideoEmojiPos(emoji_pos);
	}
	
}

void CMainFrame::ShowUserEmoji(IZoomVideoSDKUser* pUser, std::wstring reaction_type, bool is_need_check)
{
	if (!video_show_mgr_ || !reaction_bar_wnd_)
		return;

	int res_id = reaction_bar_wnd_->GetResIdByReactionType(reaction_type);
	if (res_id == -1)
		return;

	int result_res_id = res_id;
	if (is_need_check)
		result_res_id = reaction_bar_wnd_->AddUserReactionCheck(pUser, res_id);
	
	video_show_mgr_->OnUserEmojiStatusChanged(pUser, result_res_id);
}

void CMainFrame::ShowBulletScreenWnd(bool show)
{
	if (bullet_screen_wnd_ && ::IsWindow(bullet_screen_wnd_->GetHWND()))
	{
		if (show)
		{
			CDuiRect rcClient;
			GetClientRect(m_hWnd, &rcClient);

			POINT ptLeftTop = { rcClient.left,rcClient.top };
			POINT ptRightBottom = { rcClient.right,rcClient.bottom };
			ClientToScreen(m_hWnd, &ptLeftTop);
			ClientToScreen(m_hWnd, &ptRightBottom);

			::SetWindowPos(bullet_screen_wnd_->GetHWND(), HWND_TOPMOST, ptLeftTop.x, ptLeftTop.y, \
				(ptRightBottom.x - ptLeftTop.x), (ptRightBottom.y - ptLeftTop.y), SWP_FRAMECHANGED);

			bullet_screen_wnd_->ShowWindow(true);		
		}
		else
			bullet_screen_wnd_->ShowWindow(false);
	}
}

void CMainFrame::ShowLowerThirdsInfoWnd(bool show)
{
	if (lower_thirds_info_wnd_ && ::IsWindow(lower_thirds_info_wnd_->GetHWND()))
	{
		if (show)
		{
			if (session_setting_wnd_ && !session_setting_wnd_->IsShowLowerThirdsInfoWnd())
				return;

			if (!ZoomVideoSDKMgr::GetInst().IsInSession())
				return;
			
			IZoomVideoSDKUser* pSelectUser = GetGalleryContainerSelectUser();
			std::wstring username, description, color, isenable;
			if (pSelectUser)
			{
				LowerThirdInfo lower_third_info = lower_thirds_info_wnd_->GetUserLowerThirdInfo(pSelectUser);
				username = lower_third_info.lower_third_name;
				description = lower_third_info.lower_third_description;
				color = lower_third_info.lower_third_color;
			}
			else			
				GetLowerThirdInfoFromIniFile(username, description, color, isenable);				
			

			if (username.empty())
			{
				lower_thirds_info_wnd_->ShowWindow(false);
				POINT emoji_middle_point = { LOWERTHIRDS_PADDING_LEFT, LOWERTHIRDS_PADDING_TOP };
				SetActivityVideoEmojiPos(emoji_middle_point);
				return;
			}

			lower_thirds_info_wnd_->SetLowerThirdsInfo(username.data(), description.data(), color.data());

			RECT rcMain = { 0 };
			GetWindowRect(m_hWnd, &rcMain);

			int nPosX = rcMain.left + LOWERTHIRDS_PADDING_LEFT;
			int nPosY = rcMain.top + LOWERTHIRDS_PADDING_TOP;

			CDuiRect rcWnd;
			GetClientRect(lower_thirds_info_wnd_->GetHWND(), &rcWnd);
			int iWndWidth  = rcWnd.GetWidth();
			int iWndHeight = LOWERTHIRDS_WND_FULL_HEIGHT;
			if (description.empty())
				iWndHeight = LOWERTHIRDS_WND_LITE_HEIGHT;
			
			::SetWindowPos(lower_thirds_info_wnd_->GetHWND(), HWND_TOP, nPosX, nPosY, iWndWidth, iWndHeight, SWP_FRAMECHANGED);

			lower_thirds_info_wnd_->ShowWindow(true);

			POINT emoji_middle_point = { iWndWidth + LOWERTHIRDS_PADDING_LEFT, LOWERTHIRDS_PADDING_TOP };
			SetActivityVideoEmojiPos(emoji_middle_point);
		}
		else
		{
			lower_thirds_info_wnd_->ShowWindow(false);

			POINT emoji_middle_point = { LOWERTHIRDS_PADDING_LEFT, LOWERTHIRDS_PADDING_TOP };
			SetActivityVideoEmojiPos(emoji_middle_point);
		}
			
	}
}

void CMainFrame::ShowTurnUpPageWnd(bool show)
{
	if (turn_up_page_wnd_ && ::IsWindow(turn_up_page_wnd_->GetHWND()))
	{
		if (show)
		{
			turn_up_page_wnd_->ShowWindow(true);

			UpdateTurnUpPageWndPos();
		}
		else
		{
			turn_up_page_wnd_->ShowWindow(false);
		}
	}
}

void CMainFrame::ShowTurnDownPageWnd(bool show)
{
	if (turn_down_page_wnd_ && ::IsWindow(turn_down_page_wnd_->GetHWND()))
	{
		if (show)
		{
			turn_down_page_wnd_->ShowWindow(true);

			UpdateTurnDownPageWndPos();
		}
		else
		{
			turn_down_page_wnd_->ShowWindow(false);
		}
	}
}

void CMainFrame::ShowMessageTipWnd(bool show, wstring error_text)
{
	if (message_tip_wnd_ && ::IsWindow(message_tip_wnd_->GetHWND()))
	{
		if (show)
		{
			message_tip_wnd_->ShowWindow(true);
			SetTimer(m_hWnd, HIDE_ERROR_TIP_TIMER, 5 * 1000,nullptr);
			message_tip_wnd_->SetErrorTipText(error_text);

			UpdateMessageTipWndPos();
		}
		else
		{
			message_tip_wnd_->ShowWindow(false);
		}
	}
}

void CMainFrame::ShowShareToolBarWnd(bool show, bool isCameraShare)
{
	if (share_tool_bar_wnd_ && ::IsWindow(share_tool_bar_wnd_->GetHWND()))
	{
		if (show)
		{
			share_tool_bar_wnd_->ShowWindow(true);
			UpdateShareToolBarWndPos();
			if (isCameraShare)
			{
				::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
			}
			else
			{
				::ShowWindow(m_hWnd, SW_MINIMIZE);
			}
		}
		else
		{
			share_tool_bar_wnd_->ShowWindow(false);
			if (is_full_screen_)
			{
				::ShowWindow(m_hWnd, SW_MAXIMIZE);
			}
			else
			{
				::ShowWindow(m_hWnd, SW_NORMAL);
			}
		}
	}
}

void CMainFrame::ShowStatisticsWnd()
{
	CreateStatisticsWnd();
	if (statistics_wnd_ && ::IsWindow(statistics_wnd_->GetHWND()))
	{
		HWND hwnd = statistics_wnd_->GetHWND();
		RECT rc = GetMainFrameCenterRect(hwnd);
		::SetWindowPos(hwnd, NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE);

		if (session_setting_wnd_)
		{
			session_setting_wnd_->ShowWindow(false);
		}
		statistics_wnd_->ShowWindow(true);
	}
}

void CMainFrame::ShowFeedBackLaunchWnd(bool show, RECT rc)
{
	if (feedback_launch_wnd_ && ::IsWindow(feedback_launch_wnd_->GetHWND()))
	{
		if (show)
		{
			RECT rcClient;
			GetClientRect(feedback_launch_wnd_->GetHWND(), &rcClient);

			int iXpos = 0, iYpos = 0;
			iXpos = rc.left;
			iYpos = rc.top - SESSION_SETTING_WND_GAP - (rcClient.bottom - rcClient.top);

			::SetWindowPos(feedback_launch_wnd_->GetHWND(), NULL, iXpos, iYpos, 0, 0, SWP_NOSIZE);
		}
		
		feedback_launch_wnd_->ShowWindow(show);
	}
	
}

void CMainFrame::ShowFeedBackSubmitWnd(bool show)
{
	if (feedback_submit_wnd_ && ::IsWindow(feedback_submit_wnd_->GetHWND()))
	{
		if (show)
		{
			RECT rcClient;
			GetClientRect(feedback_submit_wnd_->GetHWND(), &rcClient);

			RECT rcMainWnd;
			GetClientRect(m_hWnd, &rcMainWnd);
			POINT ptLeftTop = { rcMainWnd.left,rcMainWnd.top };
			POINT ptRightBottom = { rcMainWnd.right,rcMainWnd.bottom };
			ClientToScreen(m_hWnd, &ptLeftTop);
			ClientToScreen(m_hWnd, &ptRightBottom);

			int iXpos = 0, iYpos = 0;
			iXpos = ptRightBottom.x - (rcClient.right - rcClient.left) - BOTTOM_BAR_MARGIN * 2;
			iYpos = ptRightBottom.y - BOTTOM_BAR_HEIGHT - (rcClient.bottom - rcClient.top) - BOTTOM_BAR_MARGIN;

			::SetWindowPos(feedback_submit_wnd_->GetHWND(), NULL, iXpos, iYpos, 0, 0, SWP_NOSIZE);
		}

		feedback_submit_wnd_->ShowWindow(show);
	}
}

void CMainFrame::ShowFeedBackTipsWnd(bool show, RECT rc)
{
	if (!feedback_tips_wnd_)
	{
		feedback_tips_wnd_ = new CFeedBackTipsWnd();
		
		if (feedback_tips_wnd_)
		{
			HWND hwnd = feedback_launch_wnd_ ? feedback_launch_wnd_->GetHWND() : NULL;
			feedback_tips_wnd_->Create(hwnd, nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);			
		}
	}
	if (feedback_tips_wnd_)
	{
		feedback_tips_wnd_->ShowWindow(show);

		if (show)
		{
			RECT rcClient;
			GetClientRect(feedback_tips_wnd_->GetHWND(), &rcClient);

			::SetWindowPos(feedback_tips_wnd_->GetHWND(), NULL, rc.left, rc.top - (rcClient.bottom - rcClient.top) - SESSION_SETTING_WND_GAP, 0, 0, SWP_NOSIZE);
		}		
	}
}

IZoomVideoSDKUser* CMainFrame::GetCameraControlRequester()
{
	if (camera_control_request_wnd_)
	{
		return camera_control_request_wnd_->GetRequester();
	}
	return NULL;
}
void CMainFrame::StartShare2ndCamera()
{
	ZoomVideoSDKErrors err = ZoomVideoSDKMgr::GetInst().StartShare2ndCamera();
	if (err == ZoomVideoSDKErrors_Success)
	{
		ShowShareToolBarWnd(true, true);
		if (ZoomVideoSDKMgr::GetInst().GetNumberOfCameras() > 2)
		{
			ShowSwitchShareCameraButton(true);
		}
	}

	ShowShareSelectAppsWnd(false);
}
void CMainFrame::SwitchShareCamera()
{
	video_show_mgr_->SwitchShareCamera();
}
void CMainFrame::ShareSelectedCamera(std::wstring deviceID)
{
	video_show_mgr_->ShareSelectedCamera(deviceID);
}
void CMainFrame::ShowControlCameraTipButton(bool show)
{
	if (control_camera_tip_wnd_ && ::IsWindow(control_camera_tip_wnd_->GetHWND()))
	{
		control_camera_tip_wnd_->ShowWindow(show);
	}
}
void CMainFrame::ShowSwitchShareCameraButton(bool show)
{
	if (switch_camera_wnd_ && ::IsWindow(switch_camera_wnd_->GetHWND()))
	{
		switch_camera_wnd_->ShowWindow(show);
	}
}
void CMainFrame::ShowRemoteControlCameraWnd(bool show)
{
	if (remote_control_camera_wnd_ && ::IsWindow(remote_control_camera_wnd_->GetHWND()))
	{
		remote_control_camera_wnd_->ShowWindow(show);
	}
}
void CMainFrame::ShowCameraControlRequestReceivedWnd(bool bShow)
{
	if (camera_control_request_wnd_ && ::IsWindow(camera_control_request_wnd_->GetHWND()))
	{
		if (bShow)
		{
			RECT rect = GetMainFrameCenterRect(camera_control_request_wnd_->GetHWND());
			::SetWindowPos(camera_control_request_wnd_->GetHWND(), NULL, rect.left, rect.top, 0, 0, SWP_NOSIZE);
			camera_control_request_wnd_->ShowWindow(true);
		}
		else
		{
			camera_control_request_wnd_->ShowWindow(false);
		}
	}
}
void CMainFrame::ShowStopCameraControlWnd(bool bShow)
{
	if (stop_camera_control_wnd_ && ::IsWindow(stop_camera_control_wnd_->GetHWND()))
	{
		stop_camera_control_wnd_->ShowWindow(bShow);
		RECT rect = GetMainFrameCenterRect(stop_camera_control_wnd_->GetHWND());
		::SetWindowPos(stop_camera_control_wnd_->GetHWND(), NULL, rect.left, rect.top, 0, 0, SWP_NOSIZE);
	}
}
void CMainFrame::ShowGalleryMorePopWnd(bool show, POINT pt)
{
	if (gallery_more_pop_wnd_ && ::IsWindow(gallery_more_pop_wnd_->GetHWND()))
	{
		if (show)
		{
			int iXpos = 0, iYpos = 0;
			iXpos = pt.x;
			iYpos = pt.y + DEFAULT_COLUMN_GAP;

			::SetWindowPos(gallery_more_pop_wnd_->GetHWND(), NULL, iXpos, iYpos, 0, 0, SWP_NOSIZE);

			IZoomVideoSDKUser* selected_user = GetGalleryContainerSelectUser();
			IZoomVideoSDKUser* myself = ZoomVideoSDKMgr::GetInst().GetMySelf();
			if (!selected_user || !myself)
			{
				return;
			}
			bool myself_is_host_or_manager = (myself->isHost() || myself->isManager()) ? true : false;
			bool selected_user_is_host_or_manager = (selected_user->isHost() || selected_user->isManager()) ? true : false;
			if (myself_is_host_or_manager && !selected_user_is_host_or_manager)
			{
				gallery_more_pop_wnd_->UpdateUIForRemove(true);
			}
			else
			{
				gallery_more_pop_wnd_->UpdateUIForRemove(false);
			}
			gallery_more_pop_wnd_->ShowWindow(true);
		}
		else
		{
			gallery_more_pop_wnd_->ShowWindow(false);
		}
	}
}
void CMainFrame::ShowControlCameraPopWnd(bool show, POINT pt)
{
	if (control_camera_pop_wnd_ && ::IsWindow(control_camera_pop_wnd_->GetHWND()))
	{
		if (show)
		{
			SetTimer(m_hWnd, HIDE_CONTROLL_CAMERA_POP_WND_TIMER, 2 * 1000, nullptr);
			std::wstring text;
			std::wstring userName;
			IZoomVideoSDKUser* controller = GetCameraControlRequester();
			if (controller && controller->getUserName())
			{
				userName = controller->getUserName();
				text = userName + _T(" can now control your camera");
			}
			control_camera_pop_wnd_->SetText(text);

			int iXpos = 0, iYpos = 0;
			iXpos = pt.x;
			iYpos = pt.y + DEFAULT_COLUMN_GAP + CHAT_CONTENT_LEFT_MARGIN;
			RECT rcClient;
			GetClientRect(control_camera_pop_wnd_->GetHWND(), &rcClient);
			int client_width = control_camera_pop_wnd_->GetWndWidth();
			int client_height = rcClient.bottom - rcClient.top;
			::SetWindowPos(control_camera_pop_wnd_->GetHWND(), NULL, iXpos, iYpos, client_width, client_height, SWP_NOACTIVATE);
			control_camera_pop_wnd_->ShowWindow(true);
		}
		else
		{
			control_camera_pop_wnd_->ShowWindow(false);
		}
	}
}
void CMainFrame::ShowTopTipWnd(bool show, wstring error_text)
{
	if (top_tip_wnd_ && ::IsWindow(top_tip_wnd_->GetHWND()))
	{
		if (show)
		{
			top_tip_wnd_->ShowWindow(true);
			SetTimer(m_hWnd, HIDE_TOP_TIP_TIMER, 5 * 1000, nullptr);
			top_tip_wnd_->SetTopTipText(error_text);

			UpdateTopTipWndPos();
		}
		else
		{
			top_tip_wnd_->ShowWindow(false);
		}
	}
}
void CMainFrame::UpdateGalleryMoreWndUIForCameraControl(bool isApproved)
{
	if (gallery_more_pop_wnd_)
	{
		gallery_more_pop_wnd_->UpdateUIForCameraControlStatus(isApproved);
	}
}
void CMainFrame::ShowSwitchCameraPopWnd(bool show, POINT pt)
{
	if (switch_camera_pop_wnd_ && ::IsWindow(switch_camera_pop_wnd_->GetHWND()))
	{
		if (show)
		{
			uint32_t camera_numbers = ZoomVideoSDKMgr::GetInst().GetNumberOfCameras();
			if (camera_numbers <= 2)
			{
				return;
			}
			IVideoSDKVector<IZoomVideoSDKCameraDevice*>* pCameraList = ZoomVideoSDKMgr::GetInst().GetCameraList();
			if (!pCameraList)
			{
				return;
			}
			int index = 0;
			for (int i = 0; i < pCameraList->GetCount(); i++)
			{
				IZoomVideoSDKCameraDevice* pCameraDevice = pCameraList->GetItem(i);
				std::wstring deviceName;
				std::wstring deviceID;
				if (pCameraDevice && pCameraDevice->getDeviceName() && pCameraDevice->getDeviceId())
				{
					deviceName = pCameraDevice->getDeviceName();
					deviceID = pCameraDevice->getDeviceId();
					if (!pCameraDevice->isSelectedDevice() && 
						deviceID != ZoomVideoSDKMgr::GetInst().GetSharingCameraID()
						)
					{
						switch_camera_pop_wnd_->SetBtnText(index, deviceName, deviceID);
						index++;
					}
				}
			}

			int iXpos = 0, iYpos = 0;
			iXpos = pt.x;
			iYpos = pt.y + DEFAULT_COLUMN_GAP + CHAT_CONTENT_LEFT_MARGIN;
			RECT rcClient;
			GetClientRect(switch_camera_pop_wnd_->GetHWND(), &rcClient);
			int client_width = DEFAULT_ITEM_WIDTH;
			int client_height = DEFAULT_ITEM_HEIGHT * (camera_numbers - 2);
			::SetWindowPos(switch_camera_pop_wnd_->GetHWND(), NULL, iXpos, iYpos, client_width, client_height, SWP_NOACTIVATE);
			switch_camera_pop_wnd_->ShowWindow(true);
		}
		else
		{
			switch_camera_pop_wnd_->ShowWindow(false);
		}
	}
}

void CMainFrame::StartScreenShare(wstring screen_id)
{
	ZoomVideoSDKShareOption option;
	option.isWithDeviceAudio = false;
	option.isOptimizeForSharedVideo = false;
	ZoomVideoSDKErrors err = ZoomVideoSDKMgr::GetInst().StartShareScreen(screen_id.c_str(), option);
	if (err == ZoomVideoSDKErrors_Success)
	{
		ShowShareToolBarWnd(true);
	}

	ShowShareSelectAppsWnd(false);
}

void CMainFrame::StartShareView(HWND hwnd)
{
	ZoomVideoSDKShareOption option;
	option.isWithDeviceAudio = false;
	option.isOptimizeForSharedVideo = false;
	ZoomVideoSDKErrors err = ZoomVideoSDKMgr::GetInst().StartShareView(hwnd, option);
	if (err == ZoomVideoSDKErrors_Success)
	{
		ShowShareToolBarWnd(true);
	}

	ShowShareSelectAppsWnd(false);
}

void CMainFrame::StopShare()
{
	ShowShareToolBarWnd(false);
	ShowBottomBarWnd(false);
	ShowSwitchShareCameraButton(false);
	ZoomVideoSDKErrors err = ZoomVideoSDKMgr::GetInst().StopShare();
}

void CMainFrame::SendChatToAll(const zchar_t* msgContent)
{
	ZoomVideoSDKMgr::GetInst().SendChatToAll(msgContent);
}

void CMainFrame::UpdateRenderModeLowerThird(IZoomVideoSDKUser* pUser, wstring lower_third_name, wstring lower_third_description, LowerThirdColorInfo& color_info)
{
	if (!video_show_mgr_ )
		return;
	
	video_show_mgr_->OnUpdateRenderModeLowerThird(pUser, lower_third_name, lower_third_description, color_info);	
}

void CMainFrame::ShowParticipantGallery(bool bShow)
{
	if (!video_show_mgr_)
		return;
	
	ShowSessionSettingWnd(false);
	VideoRenderLayoutType layout_type = video_show_mgr_->GetCurrentLayoutType();

	if (layout_type == kVideoRenderLayoutType1x6)
	{
		video_show_mgr_->OnGalleryContainerShowGallery(bShow);

		is_show_gallery_ = bShow;

		//update ui
		UpdateTtileBarWndPos();
		UpdateBottomBarWndPos();
		UpdateSwitchCameraWndPos();
		UpdateRemoteControlCameraWndPos();
		UpdateControlCameraTipWndPos();
		UpdateCameraControlRequestReceivedWndPos();
		UpdateTopTipWndPos();
		HideOrShowTurnPageWnd();
	}
}

bool CMainFrame::IsParticipantGalleryShown() const
{
	return is_show_gallery_;
}

void CMainFrame::SetCommandChannelConnect(bool is_connect)
{
	is_command_channel_connected_ = is_connect;
}

bool CMainFrame::IsCommandChannelConnect()
{
	return is_command_channel_connected_;
}

void CMainFrame::stt_mt_piping()
{
	OutputDebugStringA("program start");

	try {
		pipe_cpptopy::PER_PIPEINFO pipeInfo;
		wstring tmpInput = L"../project/stt_mt.py";

		/* 자식 프로세스 생성 (Python 인터프리터 실행) */
		prcManager.CreateChildProcess(tmpInput.c_str());

		/* 파이프 초기화 */
		prcManager.CreateChildPipe();
		pipeInfo.clear();

		/* 비동기적으로 읽기 및 쓰기를 처리 */
		atomic<bool> isRunning(true);

		// 파이프 읽기 쓰레드
		run_read_pipe = true;
		readThread = new thread([&]() {
			try {
				char chBuf[BUFSIZE];
				BOOL fSuccess;
				int bReadNext;
				DWORD cbRead;
				do
				{
					do
					{
						// Read from the pipe.
						RtlZeroMemory(chBuf, BUFSIZE);
						fSuccess = ReadFile(
							prcManager.GetPipe(),    // pipe handle
							chBuf,                   // buffer to receive reply
							BUFSIZE * sizeof(char), // size of buffer
							&cbRead,                 // number of bytes read
							NULL);                   // not overlapped
						if (!fSuccess && GetLastError() != ERROR_MORE_DATA)
						{
							OutputDebugString(L"ReadFile() error!!!\n");
							break;
						}
						OutputDebugStringA(chBuf);
						wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
						wstring wstr = converter.from_bytes(chBuf);

						// zchar_t* 포인터로 사용 (즉, wchar_t*)
						const zchar_t* stt_text = wstr.c_str();
						OutputDebugString(stt_text);
						ZoomVideoSDKMgr::GetInst().SendChatToAll(stt_text);
					} while (!fSuccess); // repeat loop if ERROR_MORE_DATA

				}while (run_read_pipe);
			}
			catch (exception& e) {
				const string & error = string("Read Thread Exception: ") + e.what() + "\n";
				OutputDebugStringA(error.c_str());
			}
		});
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << '\n';
		return;
	}
}

void CMainFrame::JoinSession(ZoomVideoSDKSessionContext& session_context)
{
	IZoomVideoSDKSession* pSession = ZoomVideoSDKMgr::GetInst().JoinSession(session_context);
	if (pSession)
	{
		//ai 모델 프로그램 멀티 스레드로 실행
		this->username = session_context.userName;
		stt_mt_piping();
		if (nullptr == renderer_)
		{
			renderer_ = CreateZoomVideoSDKRendererObj();
			if (renderer_)
			{
				renderer_->SetParentHandle((void*)m_hWnd);
				renderer_->SetVideoItemBkColor(RendererColor(58, 59, 59));
				renderer_->SetRendererBkColor(RendererColor(35, 36, 36, 255));
			}
		}

		if (!video_show_mgr_)
		{
			RECT rect_main = { 0 };
			GetClientRect(m_hWnd, &rect_main);

			RendererRect rect_renderer = { rect_main.left,rect_main.top,rect_main.right - rect_main.left,rect_main.bottom - rect_main.top };

			video_show_mgr_ = new VideoShowMgr(renderer_, rect_renderer);
		}

		VideoContainerParam param;

		param.rect_inset = { 0,0,0,0 };
		param.row_gap = DEFAULT_ROW_GAP;
		param.column_gap = DEFAULT_COLUMN_GAP;
		param.layout_type = kVideoRenderLayoutType1x6;
		param.mode = render_mode_;

		SwitchVideoRenderLayoutType(param);

		if (video_show_mgr_)
		{
			is_show_gallery_ = false;
			video_show_mgr_->ContainerInit();
		}

		StartPreview();

		join_session_wnd_->ShowWindow(false);
		ShowWindow(true);
	}
}

void CMainFrame::LeaveSession(bool end)
{
	ZoomVideoSDKErrors err = ZoomVideoSDKMgr::GetInst().LeaveSession(end);
}

void CMainFrame::HanldeRenderModeChanged(RendererMode mode)
{
	render_mode_ = mode;
	if (video_show_mgr_)
		video_show_mgr_->SwitchRenderMode(mode);
}

void CMainFrame::SwitchVideoRenderLayoutType(VideoContainerParam& param)
{
	if (video_show_mgr_)
	{
		video_show_mgr_->SwitchVideoRenderLayoutType(param);
	}
}

CSessionSettingsWndUI* CMainFrame::GetSessionSettingsWndUI() const
{
	return session_setting_wnd_;
}

void CMainFrame::OnGalleryContainerSelectUserChange()
{
	if (!video_show_mgr_ || !lower_thirds_info_wnd_)
		return;

	
	VideoLayoutItemBase* video_container_item = video_show_mgr_->GetGalleryContainerSelectItem();
	if (video_container_item)
	{
		IZoomVideoSDKUser* pUser = video_container_item->GetUser();		
		LowerThirdInfo lower_thirs_info = lower_thirds_info_wnd_->GetUserLowerThirdInfo(pUser);

		if (!pUser)
			return;

		if (lower_thirs_info.lower_third_name.empty())
			ShowLowerThirdsInfoWnd(false);
		else
		{
			ShowLowerThirdsInfoWnd(true);
			this->SetLowerThirdsInfo(lower_thirs_info.lower_third_name.data(), lower_thirs_info.lower_third_description.data(), lower_thirs_info.lower_third_color.data());
		}			
	}
}

IZoomVideoSDKUser* CMainFrame::GetGalleryContainerSelectUser()
{
	if (video_show_mgr_)
	{
		VideoLayoutItemBase* video_container_item = video_show_mgr_->GetGalleryContainerSelectItem();
		if (video_container_item)
			return video_container_item->GetUser();
	}
	return NULL;
}

void CMainFrame::SetLowerThirdsInfo(std::wstring username, std::wstring description, std::wstring color)
{
	if (lower_thirds_info_wnd_)
	{
		lower_thirds_info_wnd_->SetLowerThirdsInfo(username.data(), description.data(), color.data());
	}
}

LowerThirdColorInfo CMainFrame::GetLowerThirdsColorInfo(const std::wstring& colorKey)
{
	if (lower_thirds_info_wnd_)
	{
		return lower_thirds_info_wnd_->GetLowerThirdsColorInfo(colorKey);
	}
	else
	{
		LowerThirdColorInfo lower_third_color_info;
		return lower_third_color_info;
	}
}

LowerThirdInfo CMainFrame::GetUserLowerThirdInfo(IZoomVideoSDKUser* pUser)
{
	LowerThirdInfo lower_third_info;
	if (!pUser)
		return lower_third_info;

	if (lower_thirds_info_wnd_)
		return lower_thirds_info_wnd_->GetUserLowerThirdInfo(pUser);
	else
		return lower_third_info;
}

void CMainFrame::GetLowerThirdInfoFromIniFile(std::wstring& username, std::wstring& description, std::wstring& color, std::wstring& enable)
{
	if (lower_thirds_setting_wnd_)
	{
		return lower_thirds_setting_wnd_->GetLowerThirdInfoFromIniFile(username, description, color, enable);
	}
}

std::wstring CMainFrame::GetLowerThirdConfPath()
{
	if (lower_thirds_setting_wnd_)
	{
		return lower_thirds_setting_wnd_->GetLowerThirdConfPath();
	}
	else
	{
		return _T("");
	}
}

int CMainFrame::GetUserReactionResId(IZoomVideoSDKUser* pUser)
{
	if (reaction_bar_wnd_)
	{
		return reaction_bar_wnd_->GetUserReactionResId(pUser);
	}
	else
	{
		return 0;
	}

}

void CMainFrame::RemoveUserReaction(IZoomVideoSDKUser* pUser)
{
	if (video_show_mgr_)
	{
		video_show_mgr_->OnUserEmojiStatusChanged(pUser, 0);		
	}	
}

void CMainFrame::HandleTurnUpPageEvent()
{
	if (video_show_mgr_)
	{
		video_show_mgr_->OnTurnUpPage();
	}

	//update ui
	HideOrShowTurnPageWnd();
}

void CMainFrame::HandleTurnDownPageEvent()
{
	if (video_show_mgr_)
	{
		video_show_mgr_->OnTurnDownPage();
	}

	//update ui
	HideOrShowTurnPageWnd();
}

void CMainFrame::OnJoinSessionCloseBtnClick()
{
	CleaupZoomVideoSDKRendererModule();
	DeleteTheTray();
	Close();
}

void CMainFrame::OnMinimizeBtnClick()
{
	::ShowWindow(m_hWnd, SW_MINIMIZE);
}

void CMainFrame::OnFullScreenBtnClick()
{
	if (::IsZoomed(m_hWnd)) //maximize
	{
		::ShowWindow(CMainFrame::GetInstance().GetHWND(), SW_RESTORE);
		is_full_screen_ = false;
	}
	else
	{
		::ShowWindow(CMainFrame::GetInstance().GetHWND(), SW_MAXIMIZE);
		is_full_screen_ = true;
	}
}

RECT CMainFrame::GetMainFrameCenterRect(HWND hwnd)
{
	RECT rect_center = { 0,0,0,0 };

	if (hwnd == NULL)
	{
		return rect_center;
	}

	CRect rect_main;
	GetWindowRect(m_hWnd, &rect_main);

	CRect rect_dlg;
	GetWindowRect(hwnd, &rect_dlg);

	rect_center.left = rect_main.left + (rect_main.Width() - rect_dlg.Width()) / 2;
	rect_center.top = rect_main.top + (rect_main.Height() - rect_dlg.Height()) / 2;
	rect_center.right = rect_center.left + rect_dlg.Width();
	rect_center.bottom = rect_center.top + rect_dlg.Height();

	return rect_center;
}

void CMainFrame::onSessionJoin()
{
	if (join_session_wnd_ && ::IsWindow(join_session_wnd_->GetHWND()))
	{
		join_session_wnd_->ShowWindow(false);
	}

	ShowWindow(true);

	ShowBottomBarWnd(false);

	if (bottom_bar_wnd_)	
		bottom_bar_wnd_->OnSessionJoin();
	
	if (lower_thirds_info_wnd_)
		lower_thirds_info_wnd_->OnSessionJoin();

	bool isHost = ZoomVideoSDKMgr::GetInst().IsHost();
	if (feedback_launch_wnd_ && isHost)
		feedback_launch_wnd_->OnSessionJoin();

	const zchar_t* szSessionName = ZoomVideoSDKMgr::GetInst().GetSessionName();
	if (title_bar_wnd_)	
		title_bar_wnd_->UpdateSessionName(szSessionName);	
		
	UpdateParticipantNum();
	
	if (session_setting_wnd_ && session_setting_wnd_->IsShowLowerThirdsInfoWnd())
	{
		ShowLowerThirdsInfoWnd(true);
	}

	if (session_infos_wnd_)
	{
		session_infos_wnd_->UpdateSessionInfoUI();
	}
}

void CMainFrame::onSessionLeave()
{
	if (video_show_mgr_)
	{
		video_show_mgr_->OnSessionLeave();
	}

	prcManager.Dispose();
	run_read_pipe = false;
	readThread->join();
	//update ui
	OnLeaveSessionUIUpdate();
}

void CMainFrame::onError(ZoomVideoSDKErrors errorCode, int detailErrorCode)
{
	if (errorCode == ZoomVideoSDKErrors_Session_Disconnecting || errorCode == ZoomVideoSDKErrors_Session_Reconnecting)
	{
		OnMeetingDisconnecting();
	}
	else if (errorCode == ZoomVideoSDKErrors_Invalid_Parameter)
	{
		OnInvalidParamterNotification();
	}
	else
	{
		OnJoinMettingJoinFailed(errorCode);
	}
}

void CMainFrame::onUserJoin(IZoomVideoSDKUserHelper* pUserHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{
	if (video_show_mgr_)
	{
		video_show_mgr_->OnUserJoin(userList);
	}

	//update ui
	UpdateParticipantNum();
	if (session_infos_wnd_)
	{
		session_infos_wnd_->UpdateSessionInfoUI();
	}

	HideOrShowTurnPageWnd();
}

void CMainFrame::onUserLeave(IZoomVideoSDKUserHelper* pUserHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{
	if (video_show_mgr_)
	{
		video_show_mgr_->OnUserLeave(userList);
	}

	if (lower_thirds_info_wnd_)
	{
		lower_thirds_info_wnd_->OnUserLeave(userList);
	}
	//update ui
	if (session_infos_wnd_)
	{
		session_infos_wnd_->UpdateSessionInfoUI();
	}

	if (camera_control_request_wnd_)
	{
		camera_control_request_wnd_->OnUserLeave(userList);
	}
	
	ZoomVideoSDKMgr::GetInst().OnUserLeave(userList);

	UpdateParticipantNum();
	HideOrShowTurnPageWnd();
}

bool CMainFrame::JudgeIsIncludeMySelf(IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{
	IZoomVideoSDKUser* myself = ZoomVideoSDKMgr::GetInst().GetMySelf();

	bool is_inculde_myself = false;
	int count = userList->GetCount();
	for (int i = 0; i < count; i++)
	{
		IZoomVideoSDKUser* user = userList->GetItem(i);
		if (!user) continue;

		if (user == myself)
		{
			is_inculde_myself = true;
			break;
		}
	}

	return is_inculde_myself;
}

void CMainFrame::onUserVideoStatusChanged(IZoomVideoSDKVideoHelper* pVideoHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{
	if (!pVideoHelper || !userList || !video_show_mgr_)
		return;

	if (JudgeIsIncludeMySelf(userList))
	{
		//update ui
		bool is_video_on = false;
		if (ZoomVideoSDKMgr::GetInst().IsMyselfVideoOn())
			is_video_on = true;
		
		if (bottom_bar_wnd_)
		{
			bottom_bar_wnd_->UpdateUIForVideoStatus(is_video_on);
		}

		if (share_tool_bar_wnd_)
		{
			share_tool_bar_wnd_->UpdateUIForVideoStatus(is_video_on);
		}

		if (!video_show_mgr_)
			return;

		if (video_show_mgr_ && is_video_on)
		{
			IZoomVideoSDKUser* myself = ZoomVideoSDKMgr::GetInst().GetMySelf();
			video_show_mgr_->Subscribe(myself, RAW_DATA_TYPE_VIDEO);
		}
	}
}

void CMainFrame::onUserAudioStatusChanged(IZoomVideoSDKAudioHelper* pAudioHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{
	if (!pAudioHelper || !userList || !video_show_mgr_)
		return;

	video_show_mgr_->OnUserAudioStatusChanged(pAudioHelper, userList);

	//update ui
	if (JudgeIsIncludeMySelf(userList))
	{
		bool is_audio_on = false;
		if (!ZoomVideoSDKMgr::GetInst().IsMyselfAudioMuted())
			is_audio_on = true;

		if (is_audio_on)
		{
			ShowMessageTipWnd(true, _T("You are unmuted now"));
		}
		else
		{
			ShowMessageTipWnd(true, _T("You are muted now"));
		}

		if (bottom_bar_wnd_)
		{
			bottom_bar_wnd_->UpdateUIForAudioStatus(is_audio_on);
		}

		if (share_tool_bar_wnd_)
		{
			share_tool_bar_wnd_->UpdateUIForAudioStatus(is_audio_on);
		}
	}
}

void CMainFrame::onUserShareStatusChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, ZoomVideoSDKShareStatus status, ZoomVideoSDKShareType type)
{
	if (!pUser)
		return;

	if (video_show_mgr_)
	{
		video_show_mgr_->OnUserShareStatusChanged(pShareHelper, pUser, status, type);
	}

	//update ui
	wstring user_name = pUser->getUserName();
	wstring str_message;
	if (!pUser->GetSharePipe())
		return;
	ZoomVideoSDKShareStatus share_status = pUser->GetSharePipe()->getShareStatus();
	if (share_status == ZoomVideoSDKShareStatus_Start || share_status == ZoomVideoSDKShareStatus_Resume)
	{
		str_message = user_name +_T(" is sharing");
	}
	else
	{
		str_message = user_name + _T(" sharing stopped");
	}
	ShowMessageTipWnd(true, str_message.c_str());
}

void CMainFrame::onLiveStreamStatusChanged(IZoomVideoSDKLiveStreamHelper* pLiveStreamHelper, ZoomVideoSDKLiveStreamStatus status)
{

}

void CMainFrame::onChatNewMessageNotify(IZoomVideoSDKChatHelper* pChatHelper, IZoomVideoSDKChatMessage* messageItem)
{
	if (!messageItem)
		return;
	IZoomVideoSDKUser* send_user = messageItem->getSendUser();
	if (send_user)
	{
		const zchar_t* user_name = send_user->getUserName();
		const zchar_t* content = messageItem->getContent();

		UpdateChatContentWndPos(true);
		InsertContentToChatContentWnd(user_name, content);
	}
}

void CMainFrame::onUserHostChanged(IZoomVideoSDKUserHelper* pUserHelper, IZoomVideoSDKUser* pUser)
{
	if (pUser)
	{
		const zchar_t* szUserName = pUser->getUserName();
		ZoomVideoSDKAudioStatus audioStatus = pUser->getAudioStatus();
		if (audioStatus.isMuted)
		{

		}
		if (pUser->GetVideoPipe())
			ZoomVideoSDKVideoStatus videoStatus = pUser->GetVideoPipe()->getVideoStatus();
	}

	if (bottom_bar_wnd_)
		bottom_bar_wnd_->OnSessionJoin();
}

void CMainFrame::onUserActiveAudioChanged(IZoomVideoSDKAudioHelper* pAudioHelper, IVideoSDKVector<IZoomVideoSDKUser*>* list)
{

}

void CMainFrame::onSessionNeedPassword(IZoomVideoSDKPasswordHandler* handler)
{
	if (!handler)
		return;

	if (join_session_wnd_)
		join_session_wnd_->OnJoinPasswordWrong();

	handler->leaveSessionIgnorePassword();
}

void CMainFrame::onSessionPasswordWrong(IZoomVideoSDKPasswordHandler* handler)
{
	if (!handler)
		return;

	if (join_session_wnd_)
		join_session_wnd_->OnJoinPasswordWrong();

	handler->leaveSessionIgnorePassword();
}

void CMainFrame::onMixedAudioRawDataReceived(AudioRawData* data_)
{
}

void CMainFrame::onOneWayAudioRawDataReceived(AudioRawData* data_, IZoomVideoSDKUser* pUser) 
{

}
void CMainFrame::onSharedAudioRawDataReceived(AudioRawData* data_)
{
}

void CMainFrame::onUserManagerChanged(IZoomVideoSDKUser* pUser)
{

}

void CMainFrame::onUserNameChanged(IZoomVideoSDKUser* pUser)
{

}

void CMainFrame::onCameraControlRequestResult(IZoomVideoSDKUser* pUser, bool isApproved)
{
	wstring user_name = pUser->getUserName();
	wstring str_message;
	if (isApproved)
	{
		str_message = _T("You can now control ") + user_name + _T("'s camera");
		ShowRemoteControlCameraWnd(true);
	}
	else
	{
		str_message = user_name + _T(" declined your request");
	}
	UpdateGalleryMoreWndUIForCameraControl(isApproved);
	ShowTopTipWnd(true, str_message);
}

void CMainFrame::onCameraControlRequestReceived(IZoomVideoSDKUser* pUser, ZoomVideoSDKCameraControlRequestType requestType, IZoomVideoSDKCameraControlRequestHandler* pCameraControlRequestHandler)
{
	std::wstring text;
	std::wstring userName;
	if (pUser && pUser->getUserName())
	{
		userName = pUser->getUserName();
	}
	if (requestType == ZoomVideoSDKCameraControlRequestType_RequestControl && pCameraControlRequestHandler)
	{
		camera_control_request_wnd_->SetRequester(pUser);
		camera_control_request_wnd_->SetCameraControlRequestHandler(pCameraControlRequestHandler);
		ShowCameraControlRequestReceivedWnd(true);
	}
	if (requestType == ZoomVideoSDKCameraControlRequestType_GiveUpControl)
	{
		text = userName + _T(" stopped controlling your camera");
		ShowTopTipWnd(true, text);
		ShowControlCameraTipButton(false);
		ShowControlCameraPopWnd(false);
	}
}

void CMainFrame::onCommandReceived(IZoomVideoSDKUser* pSender, const zchar_t* strCmd)
{
	wstring wstrCmd(strCmd);
	if (wstrCmd.empty())
		return;

	std::vector<std::wstring> cmd_elems;
	split(wstrCmd, _T('|'), cmd_elems);

	if (cmd_elems.size() == 0)
		return;

	CmdChannelType nCmdType = (CmdChannelType)_ttoi(cmd_elems[0].c_str());

	IParseChannelCmdWnd* channelCmdWnd = NULL;
	if (cmd_channel_wnd_map_.count(nCmdType) > 0)	
		channelCmdWnd = cmd_channel_wnd_map_[nCmdType];
	
	if (channelCmdWnd)
		channelCmdWnd->OnParseChannelCmd(pSender,cmd_elems);		
}

void CMainFrame::onCommandChannelConnectResult(bool isSuccess)
{
	this->SetCommandChannelConnect(isSuccess);

	if (isSuccess && lower_thirds_setting_wnd_)
	{
		std::wstring username, description, color, isenable;
		CMainFrame::GetInstance().GetLowerThirdInfoFromIniFile(username, description, color, isenable);

		if (username.empty())
			return;
		
		lower_thirds_setting_wnd_->SendLowerThirdSettingInfo(username, description, color);
	}
}

void CMainFrame::onCloudRecordingStatus(RecordingStatus status, IZoomVideoSDKRecordingConsentHandler* pHandler)
{

}

void CMainFrame::onHostAskUnmute()
{
	CMainFrame::GetInstance().ShowMessageTipWnd(true, _T("The host would like you to unmute"));
}

void CMainFrame::onInviteByPhoneStatus(PhoneStatus status, PhoneFailedReason reason)
{

}

void CMainFrame::onCalloutJoinSuccess(IZoomVideoSDKUser* pUser, const zchar_t* phoneNumber)
{

}

void CMainFrame::onMultiCameraStreamStatusChanged(ZoomVideoSDKMultiCameraStreamStatus status, IZoomVideoSDKUser* pUser, IZoomVideoSDKRawDataPipe* pVideoPipe)
{
}

void CMainFrame::onMicSpeakerVolumeChanged(unsigned int micVolume, unsigned int speakerVolume)
{

}

void CMainFrame::onAudioDeviceStatusChanged(ZoomVideoSDKAudioDeviceType type, ZoomVideoSDKAudioDeviceStatus status)
{

}

void CMainFrame::onTestMicStatusChanged(ZoomVideoSDK_TESTMIC_STATUS status)
{

}

void CMainFrame::onSelectedAudioDeviceChanged()
{

}

void CMainFrame::onLiveTranscriptionStatus(ZoomVideoSDKLiveTranscriptionStatus status)
{

}

void CMainFrame::onLiveTranscriptionMsgReceived(const zchar_t* ltMsg, IZoomVideoSDKUser* pUser, ZoomVideoSDKLiveTranscriptionOperationType type)
{

}

void CMainFrame::onLiveTranscriptionMsgError(ILiveTranscriptionLanguage* spokenLanguage, ILiveTranscriptionLanguage* transcriptLanguage)
{

}

/*
	chat window를 제거하는 함수 구현
	아래 함수 삽입
*/
void CMainFrame::RemoveChatContentWnd()
{
	// null이 아닌 경우 즉, 채팅창이 띄워진 경우만 동작
	if(chat_content_wnd_ != nullptr)
	{
		chat_content_wnd_->ShowWindow(false);
	}
}