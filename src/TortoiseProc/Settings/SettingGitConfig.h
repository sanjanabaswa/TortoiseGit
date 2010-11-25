#pragma once

#include "SettingsPropPage.h"
#include "Tooltip.h"
#include "registry.h"
#include "afxwin.h"
// CSettingGitConfig dialog

class CSettingGitConfig : public ISettingsPropPage
{
	DECLARE_DYNAMIC(CSettingGitConfig)

public:
	CSettingGitConfig();
	virtual ~CSettingGitConfig();
	UINT GetIconID() {return IDI_GITCONFIG;}
// Dialog Data
	enum { IDD = IDD_SETTINGIT_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();
    BOOL OnApply();
	
	int m_ChangeMask;
	enum
	{
		GIT_NAME=0x1,
		GIT_EMAIL=0x2,
		GIT_CRLF=0x4,
		GIT_SAFECRLF=0x8,
	};
	DECLARE_MESSAGE_MAP()
public:
    CString m_UserName;
    CString m_UserEmail;
    BOOL m_bGlobal;
    afx_msg void OnBnClickedCheckGlobal();
    afx_msg void OnEnChangeGitUsername();
    afx_msg void OnEnChangeGitUseremail();
	BOOL m_bAutoCrlf;
	BOOL m_bSafeCrLf;
	afx_msg void OnBnClickedCheckAutocrlf();
	afx_msg void OnBnClickedCheckSafecrlf();
};
