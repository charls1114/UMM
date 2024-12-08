#ifndef _STATISTICS_WND_UI_H_
#define _STATISTICS_WND_UI_H_

class StatisticsWndUI : public WindowImplBase
{
public:
	StatisticsWndUI();
	~StatisticsWndUI();

protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetWindowClassName() const { return _T("StatisticsWndUI"); }
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void Notify(TNotifyUI& msg);

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void InitControls();
	void DoCloseBtnClicked();
	void DoAudioOptionClicked();
	void DoVideoOptionClicked();
	void DoShareOptionClicked();

	void UpdateAudioList();
	void UpdateVideoList();
	void UpdateShareList();

	void OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	COptionUI* option_audio_;
	COptionUI* option_video_;
	COptionUI* option_share_;

	CListUI* statistics_list_;
};

#endif //_STATISTICS_WND_UI_H_