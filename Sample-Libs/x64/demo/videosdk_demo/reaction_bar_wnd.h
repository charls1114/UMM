#ifndef _REACTION_BAR_WND_UI_H_
#define _REACTION_BAR_WND_UI_H_

//enum ReactionType;

struct UserReaction
{
	std::time_t reaction_time;
	int  reaction_res_id;

	UserReaction()
	{
		reaction_time = std::time(0);
		reaction_res_id = 0;
	}
};

class CReactionBarWnd : public WindowImplBase
{
public:
	CReactionBarWnd();
	~CReactionBarWnd();

public:
	int	GetResIdByReactionType(std::wstring reaction_type);
	int	AddUserReactionCheck(IZoomVideoSDKUser* pUser, int res_id);
	int	GetUserReactionResId(IZoomVideoSDKUser* pUser);
protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder() { return _T(""); }
	virtual CDuiString GetSkinFile() { return to_wstring(IDXML_REACTION_BAR_WND_UI).c_str(); }
	virtual UILIB_RESOURCETYPE GetResourceType() const { return UILIB_RESOURCE; };
	virtual LPCTSTR GetWindowClassName() const { return _T("ReactionBarWnd"); }
	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	void InitControl();
	void InitReactionMap();
	void CheckUserReacton();
	void AddRaiseHandUser(IZoomVideoSDKUser* pUser);
	void RemoveRaiseHandUser(IZoomVideoSDKUser* pUser);
	bool IsUserRaiseHand(IZoomVideoSDKUser* pUser);
	void DoEmojiBtnClick(CControlUI* pControl);	

private:
	map<std::wstring, int> reaction_type_map_;
	map<IZoomVideoSDKUser*, UserReaction> user_reaction_check_map_;
	map<IZoomVideoSDKUser*, bool> user_raise_hand_map_;
	CButtonUI* btn_raise_hand_ = nullptr;
	CButtonUI* btn_free_hand_ = nullptr;
};

#endif