#include "stdafx.h"
#include "statistics_wnd_ui.h"
#include "videosdk_demo_mgr.h"
#include <atlbase.h>
#include <atlconv.h>

#define STATISTICS_UPDATE_TIMER 10010

StatisticsWndUI::StatisticsWndUI():option_audio_(nullptr), option_video_(nullptr), option_share_(nullptr), statistics_list_(nullptr)
{

}

StatisticsWndUI::~StatisticsWndUI()
{

}

void StatisticsWndUI::InitControls()
{
	option_audio_ = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("option_audio")));
	option_video_ = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("option_video")));
	option_share_ = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("option_share")));
	statistics_list_ = static_cast<CListUI*>(m_PaintManager.FindControl(_T("statistic_list")));

	UpdateAudioList();
	SetTimer(m_hWnd, STATISTICS_UPDATE_TIMER, 1000 * 5, nullptr);
}

void StatisticsWndUI::InitWindow()
{
	InitControls();
}

CDuiString StatisticsWndUI::GetSkinFolder()
{
	return _T("");
}

CDuiString StatisticsWndUI::GetSkinFile()
{
	return to_wstring(IDXML_STATISTICS_WND_UI).c_str();
}

UILIB_RESOURCETYPE StatisticsWndUI::GetResourceType() const
{
	return UILIB_RESOURCE;
}

LRESULT StatisticsWndUI::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void StatisticsWndUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btn_close"))
		{
			DoCloseBtnClicked();
		}
		else if (msg.pSender->GetName() == _T("option_audio"))
		{
			DoAudioOptionClicked();
		}
		else if (msg.pSender->GetName() == _T("option_video"))
		{
			DoVideoOptionClicked();
		}
		else if (msg.pSender->GetName() == _T("option_share"))
		{
			DoShareOptionClicked();
		}
	}
}

LRESULT StatisticsWndUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_TIMER:
	{
		OnTimer(uMsg, wParam, lParam);
		break;
	}
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

void StatisticsWndUI::DoCloseBtnClicked()
{
	KillTimer(m_hWnd, STATISTICS_UPDATE_TIMER);
	Close();
}

void StatisticsWndUI::DoAudioOptionClicked()
{
	UpdateAudioList();
}

void StatisticsWndUI::DoVideoOptionClicked()
{
	UpdateVideoList();
}

void StatisticsWndUI::DoShareOptionClicked()
{
	UpdateShareList();
}

void StatisticsWndUI::UpdateAudioList()
{
	if (!statistics_list_)
		return;

	statistics_list_->RemoveAll();

	ZoomVideoSDKSessionAudioStatisticInfo send_info;
	ZoomVideoSDKSessionAudioStatisticInfo recv_info;
	ZoomVideoSDKMgr::GetInst().GetSessionAudioStatisticInfo(send_info, recv_info);

	//frequency
	CDialogBuilder builder;
	CListContainerElementUI* item_frequency = static_cast<CListContainerElementUI*>(builder.Create(IDXML_STATISTICS_LIST_ITEM, _T("xml"), this, &m_PaintManager));

	if (item_frequency)
	{
		CLabelUI* label_statistics = static_cast<CLabelUI*>(item_frequency->FindSubControl(_T("label_statistical_items")));
		if (label_statistics)
		{
			label_statistics->SetText(_T("frequency"));
		}

		CLabelUI* label_send = static_cast<CLabelUI*>(item_frequency->FindSubControl(_T("label_send")));
		if (label_send)
		{
			if (send_info.frequency <= 0)
			{
				label_send->SetText(_T("-"));
			}
			else
			{
				TCHAR send[64] = { 0 };
				wsprintf(send, _T("%d KHz"), send_info.frequency);   
				label_send->SetText(send);
			}
		}

		CLabelUI* label_recv = static_cast<CLabelUI*>(item_frequency->FindSubControl(_T("label_receive")));
		if (label_recv)
		{
			if (recv_info.frequency <= 0)
			{
				label_recv->SetText(_T("-"));
			}
			else
			{
				TCHAR recv[64] = { 0 };
				wsprintf(recv, _T("%d KHz"), recv_info.frequency);   
				label_recv->SetText(recv);
			}
		}

		statistics_list_->Add(item_frequency);
	}

	//latency
	CListContainerElementUI* item_latency = static_cast<CListContainerElementUI*>(builder.Create(IDXML_STATISTICS_LIST_ITEM, _T("xml"), this, &m_PaintManager));

	if (item_latency)
	{
		CLabelUI* label_statistics = static_cast<CLabelUI*>(item_latency->FindSubControl(_T("label_statistical_items")));
		if (label_statistics)
		{
			label_statistics->SetText(_T("latency"));
		}

		CLabelUI* label_send = static_cast<CLabelUI*>(item_latency->FindSubControl(_T("label_send")));
		if (label_send)
		{
			if (send_info.latency <= 0)
			{
				label_send->SetText(_T("-"));
			}
			else
			{
				TCHAR send[64] = { 0 };
				wsprintf(send, _T("%d ms"), send_info.latency);    
				label_send->SetText(send);
			}
		}

		CLabelUI* label_recv = static_cast<CLabelUI*>(item_latency->FindSubControl(_T("label_receive")));
		if (label_recv)
		{
			if (recv_info.latency <= 0)
			{
				label_recv->SetText(_T("-"));
			}
			else
			{
				TCHAR recv[64] = { 0 };
				wsprintf(recv, _T("%d ms"), recv_info.latency);   
				label_recv->SetText(recv);
			}
		}

		statistics_list_->Add(item_latency);
	}

	//jitter
	CListContainerElementUI* item_jitter = static_cast<CListContainerElementUI*>(builder.Create(IDXML_STATISTICS_LIST_ITEM, _T("xml"), this, &m_PaintManager));

	if (item_jitter)
	{
		CLabelUI* label_statistics = static_cast<CLabelUI*>(item_jitter->FindSubControl(_T("label_statistical_items")));
		if (label_statistics)
		{
			label_statistics->SetText(_T("jitter"));
		}

		CLabelUI* label_send = static_cast<CLabelUI*>(item_jitter->FindSubControl(_T("label_send")));
		if (label_send)
		{
			if (send_info.Jitter <= 0)
			{
				label_send->SetText(_T("-"));
			}
			else
			{
				TCHAR send[64] = { 0 };
				wsprintf(send, _T("%d ms"), send_info.Jitter);    
				label_send->SetText(send);
			}
		}

		CLabelUI* label_recv = static_cast<CLabelUI*>(item_jitter->FindSubControl(_T("label_receive")));
		if (label_recv)
		{
			if (recv_info.Jitter <= 0)
			{
				label_recv->SetText(_T("-"));
			}
			else
			{
				TCHAR recv[64] = { 0 };
				wsprintf(recv, _T("%d ms"), recv_info.Jitter);     
				label_recv->SetText(recv);
			}
		}

		statistics_list_->Add(item_jitter);
	}

	//packetLossAvg
	CListContainerElementUI* item_packetLossAvg = static_cast<CListContainerElementUI*>(builder.Create(IDXML_STATISTICS_LIST_ITEM, _T("xml"), this, &m_PaintManager));

	if (item_jitter)
	{
		CLabelUI* label_statistics = static_cast<CLabelUI*>(item_packetLossAvg->FindSubControl(_T("label_statistical_items")));
		if (label_statistics)
		{
			label_statistics->SetText(_T("packetLossAvg"));
		}

		CLabelUI* label_send = static_cast<CLabelUI*>(item_packetLossAvg->FindSubControl(_T("label_send")));
		if (label_send)
		{
			if (send_info.packetLossAvg <= 0.001f)
			{
				label_send->SetText(_T("-"));
			}
			else
			{
				char send[64] = { 0 };
				sprintf(send, "%.02f %%", send_info.packetLossAvg);     
				USES_CONVERSION;
				label_send->SetText(A2W(send));
			}
		}

		CLabelUI* label_recv = static_cast<CLabelUI*>(item_packetLossAvg->FindSubControl(_T("label_receive")));
		if (label_recv)
		{
			if (recv_info.packetLossAvg <= 0.01f)
			{
				label_recv->SetText(_T("-"));
			}
			else
			{
				char recv[64] = { 0 };
				sprintf(recv, "%.02f %%", recv_info.packetLossAvg);    
				USES_CONVERSION;
				label_recv->SetText(A2W(recv));
			}
		}

		statistics_list_->Add(item_packetLossAvg);
	}

	//packetLossMax
	CListContainerElementUI* item_packetLossMax = static_cast<CListContainerElementUI*>(builder.Create(IDXML_STATISTICS_LIST_ITEM, _T("xml"), this, &m_PaintManager));

	if (item_jitter)
	{
		CLabelUI* label_statistics = static_cast<CLabelUI*>(item_packetLossMax->FindSubControl(_T("label_statistical_items")));
		if (label_statistics)
		{
			label_statistics->SetText(_T("packetLossMax"));
		}

		CLabelUI* label_send = static_cast<CLabelUI*>(item_packetLossMax->FindSubControl(_T("label_send")));
		if (label_send)
		{
			if (send_info.packetLossMax <= 0.01f)
			{
				label_send->SetText(_T("-"));
			}
			else
			{
				char send[64] = { 0 };
				sprintf(send, "%.02f %%", send_info.packetLossMax);     
				USES_CONVERSION;
				label_send->SetText(A2W(send));
			}
		}

		CLabelUI* label_recv = static_cast<CLabelUI*>(item_packetLossMax->FindSubControl(_T("label_receive")));
		if (label_recv)
		{
			if (recv_info.packetLossMax <= 0.01f)
			{
				label_recv->SetText(_T("-"));
			}
			else
			{
				char recv[64] = { 0 };
				sprintf(recv, "%.02f %%", recv_info.packetLossMax);      
				USES_CONVERSION;
				label_recv->SetText(A2W(recv));
			}
		}

		statistics_list_->Add(item_packetLossMax);
	}
}

void StatisticsWndUI::UpdateVideoList()
{
	if (!statistics_list_)
		return;

	statistics_list_->RemoveAll();

	ZoomVideoSDKSessionASVStatisticInfo send_info;
	ZoomVideoSDKSessionASVStatisticInfo recv_info;
	ZoomVideoSDKMgr::GetInst().GetSessionVideoStatisticInfo(send_info, recv_info);

	//frame_width
	CDialogBuilder builder;
	CListContainerElementUI* item_frame_width = static_cast<CListContainerElementUI*>(builder.Create(IDXML_STATISTICS_LIST_ITEM, _T("xml"), this, &m_PaintManager));

	if (item_frame_width)
	{
		CLabelUI* label_statistics = static_cast<CLabelUI*>(item_frame_width->FindSubControl(_T("label_statistical_items")));
		if (label_statistics)
		{
			label_statistics->SetText(_T("frame_width"));
		}

		CLabelUI* label_send = static_cast<CLabelUI*>(item_frame_width->FindSubControl(_T("label_send")));
		if (label_send)
		{
			if (send_info.frame_width <= 0)
			{
				label_send->SetText(_T("-"));
			}
			else
			{
				TCHAR send[64] = { 0 };
				wsprintf(send, _T("%d"), send_info.frame_width);     
				label_send->SetText(send);
			}
		}

		CLabelUI* label_recv = static_cast<CLabelUI*>(item_frame_width->FindSubControl(_T("label_receive")));
		if (label_recv)
		{
			if (recv_info.frame_width <= 0)
			{
				label_recv->SetText(_T("-"));
			}
			else
			{
				TCHAR recv[64] = { 0 };
				wsprintf(recv, _T("%d"), recv_info.frame_width);     
				label_recv->SetText(recv);
			}
		}

		statistics_list_->Add(item_frame_width);
	}

	//frame_height
	CListContainerElementUI* item_frame_height = static_cast<CListContainerElementUI*>(builder.Create(IDXML_STATISTICS_LIST_ITEM, _T("xml"), this, &m_PaintManager));

	if (item_frame_height)
	{
		CLabelUI* label_statistics = static_cast<CLabelUI*>(item_frame_height->FindSubControl(_T("label_statistical_items")));
		if (label_statistics)
		{
			label_statistics->SetText(_T("frame_height"));
		}

		CLabelUI* label_send = static_cast<CLabelUI*>(item_frame_height->FindSubControl(_T("label_send")));
		if (label_send)
		{
			if (send_info.frame_height <= 0)
			{
				label_send->SetText(_T("-"));
			}
			else
			{
				TCHAR send[64] = { 0 };
				wsprintf(send, _T("%d"), send_info.frame_height);    
				label_send->SetText(send);
			}
		}

		CLabelUI* label_recv = static_cast<CLabelUI*>(item_frame_height->FindSubControl(_T("label_receive")));
		if (label_recv)
		{
			if (recv_info.frame_height <= 0)
			{
				label_recv->SetText(_T("-"));
			}
			else
			{
				TCHAR recv[64] = { 0 };
				wsprintf(recv, _T("%d"), recv_info.frame_height);    
				label_recv->SetText(recv);
			}
		}

		statistics_list_->Add(item_frame_height);
	}

	//fps
	CListContainerElementUI* item_fps = static_cast<CListContainerElementUI*>(builder.Create(IDXML_STATISTICS_LIST_ITEM, _T("xml"), this, &m_PaintManager));

	if (item_fps)
	{
		CLabelUI* label_statistics = static_cast<CLabelUI*>(item_fps->FindSubControl(_T("label_statistical_items")));
		if (label_statistics)
		{
			label_statistics->SetText(_T("fps"));
		}

		CLabelUI* label_send = static_cast<CLabelUI*>(item_fps->FindSubControl(_T("label_send")));
		if (label_send)
		{
			if (send_info.fps <= 0)
			{
				label_send->SetText(_T("-"));
			}
			else
			{
				TCHAR send[64] = { 0 };
				wsprintf(send, _T("%d"), send_info.fps);    
				label_send->SetText(send);
			}
		}

		CLabelUI* label_recv = static_cast<CLabelUI*>(item_fps->FindSubControl(_T("label_receive")));
		if (label_recv)
		{
			if (recv_info.fps <= 0)
			{
				label_recv->SetText(_T("-"));
			}
			else
			{
				TCHAR recv[64] = { 0 };
				wsprintf(recv, _T("%d"), recv_info.fps);   
				label_recv->SetText(recv);
			}
		}

		statistics_list_->Add(item_fps);
	}

	//latency
	CListContainerElementUI* item_latency = static_cast<CListContainerElementUI*>(builder.Create(IDXML_STATISTICS_LIST_ITEM, _T("xml"), this, &m_PaintManager));

	if (item_latency)
	{
		CLabelUI* label_statistics = static_cast<CLabelUI*>(item_latency->FindSubControl(_T("label_statistical_items")));
		if (label_statistics)
		{
			label_statistics->SetText(_T("latency"));
		}

		CLabelUI* label_send = static_cast<CLabelUI*>(item_latency->FindSubControl(_T("label_send")));
		if (label_send)
		{
			if (send_info.latency <= 0)
			{
				label_send->SetText(_T("-"));
			}
			else
			{
				TCHAR send[64] = { 0 };
				wsprintf(send, _T("%d ms"), recv_info.latency);    
				label_send->SetText(send);
			}
		}

		CLabelUI* label_recv = static_cast<CLabelUI*>(item_latency->FindSubControl(_T("label_receive")));
		if (label_recv)
		{
			if (recv_info.latency <= 0)
			{
				label_recv->SetText(_T("-"));
			}
			else
			{
				TCHAR recv[64] = { 0 };
				wsprintf(recv, _T("%d ms"), recv_info.latency);    
				label_recv->SetText(recv);
			}
		}

		statistics_list_->Add(item_latency);
	}

	//jitter
	CListContainerElementUI* item_jitter = static_cast<CListContainerElementUI*>(builder.Create(IDXML_STATISTICS_LIST_ITEM, _T("xml"), this, &m_PaintManager));

	if (item_jitter)
	{
		CLabelUI* label_statistics = static_cast<CLabelUI*>(item_jitter->FindSubControl(_T("label_statistical_items")));
		if (label_statistics)
		{
			label_statistics->SetText(_T("jitter"));
		}

		CLabelUI* label_send = static_cast<CLabelUI*>(item_jitter->FindSubControl(_T("label_send")));
		if (label_send)
		{
			if (send_info.Jitter <= 0)
			{
				label_send->SetText(_T("-"));
			}
			else
			{
				TCHAR send[64] = { 0 };
				wsprintf(send, _T("%d ms"), recv_info.Jitter);   
				label_send->SetText(send);
			}
		}

		CLabelUI* label_recv = static_cast<CLabelUI*>(item_jitter->FindSubControl(_T("label_receive")));
		if (label_recv)
		{
			if (recv_info.Jitter <= 0)
			{
				label_recv->SetText(_T("-"));
			}
			else
			{
				TCHAR recv[64] = { 0 };
				wsprintf(recv, _T("%d ms"), recv_info.Jitter);     
				label_recv->SetText(recv);
			}
		}

		statistics_list_->Add(item_jitter);
	}
}

void StatisticsWndUI::UpdateShareList()
{
	if (!statistics_list_)
		return;

	statistics_list_->RemoveAll();

	ZoomVideoSDKSessionASVStatisticInfo send_info;
	ZoomVideoSDKSessionASVStatisticInfo recv_info;
	ZoomVideoSDKMgr::GetInst().GetSessionShareStatisticInfo(send_info, recv_info);

	//frame_width
	CDialogBuilder builder;
	CListContainerElementUI* item_frame_width = static_cast<CListContainerElementUI*>(builder.Create(IDXML_STATISTICS_LIST_ITEM, _T("xml"), this, &m_PaintManager));

	if (item_frame_width)
	{
		CLabelUI* label_statistics = static_cast<CLabelUI*>(item_frame_width->FindSubControl(_T("label_statistical_items")));
		if (label_statistics)
		{
			label_statistics->SetText(_T("frame_width"));
		}

		CLabelUI* label_send = static_cast<CLabelUI*>(item_frame_width->FindSubControl(_T("label_send")));
		if (label_send)
		{
			if (send_info.frame_width <= 0)
			{
				label_send->SetText(_T("-"));
			}
			else
			{
				TCHAR send[64] = { 0 };
				wsprintf(send, _T("%d"), send_info.frame_width);   
				label_send->SetText(send);
			}
		}

		CLabelUI* label_recv = static_cast<CLabelUI*>(item_frame_width->FindSubControl(_T("label_receive")));
		if (label_recv)
		{
			if (recv_info.frame_width <= 0)
			{
				label_recv->SetText(_T("-"));
			}
			else
			{
				TCHAR recv[64] = { 0 };
				wsprintf(recv, _T("%d"), recv_info.frame_width);    
				label_recv->SetText(recv);
			}
		}

		statistics_list_->Add(item_frame_width);
	}

	//frame_height
	CListContainerElementUI* item_frame_height = static_cast<CListContainerElementUI*>(builder.Create(IDXML_STATISTICS_LIST_ITEM, _T("xml"), this, &m_PaintManager));

	if (item_frame_height)
	{
		CLabelUI* label_statistics = static_cast<CLabelUI*>(item_frame_height->FindSubControl(_T("label_statistical_items")));
		if (label_statistics)
		{
			label_statistics->SetText(_T("frame_height"));
		}

		CLabelUI* label_send = static_cast<CLabelUI*>(item_frame_height->FindSubControl(_T("label_send")));
		if (label_send)
		{
			if (send_info.frame_height <= 0)
			{
				label_send->SetText(_T("-"));
			}
			else
			{
				TCHAR send[64] = { 0 };
				wsprintf(send, _T("%d"), send_info.frame_height);  
				label_send->SetText(send);
			}
		}

		CLabelUI* label_recv = static_cast<CLabelUI*>(item_frame_height->FindSubControl(_T("label_receive")));
		if (label_recv)
		{
			if (recv_info.frame_height <= 0)
			{
				label_recv->SetText(_T("-"));
			}
			else
			{
				TCHAR recv[64] = { 0 };
				wsprintf(recv, _T("%d"), recv_info.frame_height);    
				label_recv->SetText(recv);
			}
		}

		statistics_list_->Add(item_frame_height);
	}

	//fps
	CListContainerElementUI* item_fps = static_cast<CListContainerElementUI*>(builder.Create(IDXML_STATISTICS_LIST_ITEM, _T("xml"), this, &m_PaintManager));

	if (item_fps)
	{
		CLabelUI* label_statistics = static_cast<CLabelUI*>(item_fps->FindSubControl(_T("label_statistical_items")));
		if (label_statistics)
		{
			label_statistics->SetText(_T("fps"));
		}

		CLabelUI* label_send = static_cast<CLabelUI*>(item_fps->FindSubControl(_T("label_send")));
		if (label_send)
		{
			if (send_info.fps <= 0)
			{
				label_send->SetText(_T("-"));
			}
			else
			{
				TCHAR send[64] = { 0 };
				wsprintf(send, _T("%d"), send_info.fps);   
				label_send->SetText(send);
			}
		}

		CLabelUI* label_recv = static_cast<CLabelUI*>(item_fps->FindSubControl(_T("label_receive")));
		if (label_recv)
		{
			if (recv_info.fps <= 0)
			{
				label_recv->SetText(_T("-"));
			}
			else
			{
				TCHAR recv[64] = { 0 };
				wsprintf(recv, _T("%d"), recv_info.fps);    
				label_recv->SetText(recv);
			}
		}

		statistics_list_->Add(item_fps);
	}

	//latency
	CListContainerElementUI* item_latency = static_cast<CListContainerElementUI*>(builder.Create(IDXML_STATISTICS_LIST_ITEM, _T("xml"), this, &m_PaintManager));

	if (item_latency)
	{
		CLabelUI* label_statistics = static_cast<CLabelUI*>(item_latency->FindSubControl(_T("label_statistical_items")));
		if (label_statistics)
		{
			label_statistics->SetText(_T("latency"));
		}

		CLabelUI* label_send = static_cast<CLabelUI*>(item_latency->FindSubControl(_T("label_send")));
		if (label_send)
		{
			if (send_info.latency <= 0)
			{
				label_send->SetText(_T("-"));
			}
			else
			{
				TCHAR send[64] = { 0 };
				wsprintf(send, _T("%d ms"), recv_info.latency);    
				label_send->SetText(send);
			}
		}

		CLabelUI* label_recv = static_cast<CLabelUI*>(item_latency->FindSubControl(_T("label_receive")));
		if (label_recv)
		{
			if (recv_info.latency <= 0)
			{
				label_recv->SetText(_T("-"));
			}
			else
			{
				TCHAR recv[64] = { 0 };
				wsprintf(recv, _T("%d ms"), recv_info.latency);     
				label_recv->SetText(recv);
			}
		}

		statistics_list_->Add(item_latency);
	}

	//jitter
	CListContainerElementUI* item_jitter = static_cast<CListContainerElementUI*>(builder.Create(IDXML_STATISTICS_LIST_ITEM, _T("xml"), this, &m_PaintManager));

	if (item_jitter)
	{
		CLabelUI* label_statistics = static_cast<CLabelUI*>(item_jitter->FindSubControl(_T("label_statistical_items")));
		if (label_statistics)
		{
			label_statistics->SetText(_T("jitter"));
		}

		CLabelUI* label_send = static_cast<CLabelUI*>(item_jitter->FindSubControl(_T("label_send")));
		if (label_send)
		{
			if (send_info.Jitter <= 0)
			{
				label_send->SetText(_T("-"));
			}
			else
			{
				TCHAR send[64] = { 0 };
				wsprintf(send, _T("%d ms"), recv_info.Jitter);    
				label_send->SetText(send);
			}
		}

		CLabelUI* label_recv = static_cast<CLabelUI*>(item_jitter->FindSubControl(_T("label_receive")));
		if (label_recv)
		{
			if (recv_info.Jitter <= 0)
			{
				label_recv->SetText(_T("-"));
			}
			else
			{
				TCHAR recv[64] = { 0 };
				wsprintf(recv, _T("%d ms"), recv_info.Jitter);    
				label_recv->SetText(recv);
			}
		}

		statistics_list_->Add(item_jitter);
	}
}

void StatisticsWndUI::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (wParam == STATISTICS_UPDATE_TIMER)
	{
		if (!option_audio_ || !option_video_ || !option_share_)
			return;

		if (option_audio_->IsSelected())
		{
			UpdateAudioList();
		}
		else if (option_video_->IsSelected())
		{
			UpdateVideoList();
		}
		else if (option_share_->IsSelected())
		{
			UpdateShareList();
		}
	}
}