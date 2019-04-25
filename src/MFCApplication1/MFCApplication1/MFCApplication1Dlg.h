
// MFCApplication1Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "math.h"
#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"
#include "Markup.h"
#include "string.h"
#include <locale>
// CMFCApplication1Dlg 对话框

struct bbox_node {
	float x;
	float y;
	float w;
	float h;
	UINT bbox_tag;
	char bbox_annos[256];
	bbox_node *next;
};

class CMFCApplication1Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication1Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_data_path;
	afx_msg void OnBnClickedButtonDataPath();
	CStatic m_staMain;
	CStatic m_staBox;
	CBrush m_redbrush, m_bluebrush;
	COLORREF m_redcolor, m_bluecolor, m_textcolor;
	afx_msg void OnStnClickedStaticMainPev();
	void ShowImage();
	char * CString2ConstChar(CString FilePath);
	IplImage *IplImgMain;
	IplImage *IplImgBBox;
	BOOL m_predlg_open_flag;
	UINT m_Current_pciHeight;
	UINT m_Current_pciWidth;
	UINT m_Current_PrePciWidth;
	UINT m_Current_PrePciHeight;
	UINT m_Current_bboxHeight;
	UINT m_Current_bboxWidth;
	UINT m_Current_PrebboxWidth;
	UINT m_Current_PrebboxHeight;
	CMarkup xml;
	CMarkup xml_tmp;
	CMarkup xml_tmp2;
	UINT m_bbox_sum;
	UINT m_bbox_id_current;
	afx_msg void OnEnChangeEditBboxSum();
	afx_msg void OnEnChangeEditPath();
	bbox_node *bbox, *bbox_new, *bbox_tail;
	bbox_node *bbox_show, *bbox_save;
	afx_msg void OnStnClickedStaticBbox();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	virtual BOOL DestroyWindow();
	afx_msg void OnDestroy();
	CString m_annos_ch;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
