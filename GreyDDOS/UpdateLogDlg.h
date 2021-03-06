#pragma once


// CUpdateLogDlg 对话框

class CUpdateLogDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUpdateLogDlg)

public:
	CUpdateLogDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CUpdateLogDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UPDATE_LOG_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_updateLog;
	afx_msg void OnSetfocusUpdateLog();
};
