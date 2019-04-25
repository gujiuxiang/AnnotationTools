
// MFCApplication1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DirDialog.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include <locale>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef UNICODE //Test to see if we're using wchar_ts or not.
typedef std::wstring StringType;
#else
typedef std::string StringType;
#endif


//Defines a system-wide hot key.
BOOL WINAPI RegisterHotKey(//https://msdn.microsoft.com/en-us/library/ms646309(v=vs.85).aspx
	_In_opt_ HWND hWnd,            //接收热键触发事件的窗口句柄
	_In_     int  id,                //热键的ID,应用程序ID范围 [0x0000,0xBFFF].DLL范围[0xC000~0xFFFF]
	_In_     UINT fsModifiers,    //声明组合键中的 Alt(MOD_ALT)、Ctrl(MOD_CONTROL)、Shift(MOD_SHIFT)、Win键(MOD_WIN)、MOD_NOREPEAT 或者其组合
	_In_     UINT vk                //普通按键的代码(例如'A')，其他见https://msdn.microsoft.com/en-us/library/dd375731(v=vs.85).aspx
	);

//Frees a hot key previously registered by the calling thread.
BOOL WINAPI UnregisterHotKey(//https://msdn.microsoft.com/en-us/library/ms646327(v=vs.85).aspx
	_In_opt_ HWND hWnd,            //接收热键触发事件的窗口句柄
	_In_     int  id                //热键的ID
	);
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CMFCApplication1Dlg 对话框



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCApplication1Dlg::IDD, pParent)
	, m_data_path(_T(""))
	, m_bbox_sum(0)
	, m_bbox_id_current(1)
	, m_annos_ch(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_predlg_open_flag = FALSE;
	m_bbox_sum = 0;
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PATH, m_data_path);
	DDX_Control(pDX, IDC_STATIC_MAIN_PEV, m_staMain);
	DDX_Control(pDX, IDC_STATIC_BBOX, m_staBox);
	DDX_Text(pDX, IDC_EDIT_BBOX_SUM, m_bbox_sum);
	DDX_Text(pDX, IDC_EDIT_CURRENT_BBOX, m_bbox_id_current);
	DDX_Text(pDX, IDC_EDIT1_ANNOS, m_annos_ch);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_DATA_PATH, &CMFCApplication1Dlg::OnBnClickedButtonDataPath)
	ON_STN_CLICKED(IDC_STATIC_MAIN_PEV, &CMFCApplication1Dlg::OnStnClickedStaticMainPev)
	ON_EN_CHANGE(IDC_EDIT_BBOX_SUM, &CMFCApplication1Dlg::OnEnChangeEditBboxSum)
	ON_EN_CHANGE(IDC_EDIT_PATH, &CMFCApplication1Dlg::OnEnChangeEditPath)
	ON_STN_CLICKED(IDC_STATIC_BBOX, &CMFCApplication1Dlg::OnStnClickedStaticBbox)
	ON_WM_HOTKEY()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	//注册热键 Ctrl + S(s)
	RegisterHotKey(this->GetSafeHwnd(), 1001, MOD_CONTROL, 'a');
	RegisterHotKey(this->GetSafeHwnd(), 1002, MOD_CONTROL, 'A');
	RegisterHotKey(this->GetSafeHwnd(), 1003, MOD_CONTROL, 'd');
	RegisterHotKey(this->GetSafeHwnd(), 1004, MOD_CONTROL, 'D');
	//注册热键 Ctrl + F1(不重复，不会连续收到多次)
	//RegisterHotKey(this->GetSafeHwnd(), 1005, MOD_CONTROL | MOD_NOREPEAT, VK_RETURN);
	RegisterHotKey(this->GetSafeHwnd(), 1005, VK_RETURN, VK_RETURN);
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication1Dlg::OnPaint()
{
	// Set the icon for this dialog. The framework does this automatically  
	// when the application's main window is not a dialog  
	SetIcon(m_hIcon, TRUE);    // Set big icon  
	SetIcon(m_hIcon, FALSE);   // Set small icon  

	m_redcolor = RGB(255, 0, 0);                      // 红色  
	m_bluecolor = RGB(0, 0, 255);                     // 蓝色  
	m_textcolor = RGB(255, 255, 255);                 // 文本颜色设置为白色  
	m_redbrush.CreateSolidBrush(m_redcolor);      // 红色背景色  
	m_bluebrush.CreateSolidBrush(m_bluecolor);    // 蓝色背景色  

	CPaintDC dc(this); // 用于绘制的设备上下文
	

	SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

	// 使图标在工作区矩形中居中
	int cxIcon = GetSystemMetrics(SM_CXICON);
	int cyIcon = GetSystemMetrics(SM_CYICON);
	CRect rect;
	GetClientRect(&rect);
	dc.FillSolidRect(rect, RGB(0, 0, 0));   //设置为绿色背景

	int x = (rect.Width() - cxIcon + 1) / 2;
	int y = (rect.Height() - cyIcon + 1) / 2;

	// 绘制图标
	dc.DrawIcon(x, y, m_hIcon);
	if (m_predlg_open_flag)
	{
		ShowImage();
	}
}

void CMFCApplication1Dlg::ShowImage()
{

	CDC *pDC_bbox = GetDlgItem(IDC_STATIC_BBOX)->GetDC();
	CRect PicBBoxRect;
	GetDlgItem(IDC_STATIC_BBOX)->GetWindowRect(&PicBBoxRect);
	HDC hDC_bbox = pDC_bbox->GetSafeHdc();


	CDC *pDC = GetDlgItem(IDC_STATIC_MAIN_PEV)->GetDC();
	HDC hDC = pDC->GetSafeHdc();

	CvPoint p0, p1;
	// 绘制黑色背景
	CRect PicBoxRect;
	GetDlgItem(IDC_STATIC_MAIN_PEV)->GetWindowRect(&PicBoxRect);
	m_Current_pciHeight = PicBoxRect.Height();
	m_Current_pciWidth = PicBoxRect.Width();

	int PicBoxWidth = PicBoxRect.right - PicBoxRect.left;   //获取Picture控件的宽和高
	int PicBoxHeight = PicBoxRect.bottom - PicBoxRect.top;
	int ImgWidth = IplImgMain->width;							//获取图片的宽和高
	int ImgHeight = IplImgMain->height;
	int CenterX = (int)(PicBoxWidth - ImgWidth) / 2;        // 使图片的显示位置正好在控件的正中
	int CenterY = (int)(PicBoxHeight - ImgHeight) / 2;

	float width = (float)IplImgMain->width;;
	float height = (float)IplImgMain->height;
	float c = 0.0f;
	float x = 0.0f;
	float y = 0.0f;

	// 按适合宽度模式处理
	width = (float)PicBoxRect.Width();
	c = width / (IplImgMain->width);

	if (c * (IplImgMain->height) < PicBoxRect.Height()) // 如果没有超界
	{
		height = c * (IplImgMain->height);
	}
	else // 超界则采用适合高度模式
	{
		height = (float)PicBoxRect.Height();
		c = height / (IplImgMain->height);
		width = c * (IplImgMain->width);
	}

	if ((UINT)PicBoxRect.Width() > width)
	{
		x = ((UINT)PicBoxRect.Width() - width) / 2.0f;
	}

	if ((UINT)PicBoxRect.Height() > height)
	{
		y = ((UINT)PicBoxRect.Height() - height) / 2.0f;
	}

	CenterX = x;
	CenterY = y;
	ImgWidth = width;
	ImgHeight = height;

	
	bbox_show = bbox;

	CvSize size_main = cvSize(IplImgMain->width, IplImgMain->height);//区域大小
	IplImage *IplImgMain_copy = cvCreateImage(size_main, IplImgMain->depth, IplImgMain->nChannels);//创建目标图像
	cvCopy(IplImgMain, IplImgMain_copy); //复制图像
	while (bbox_show)
	{
		p0.x = bbox_show->x;
		p0.y = bbox_show->y;
		p1.x = bbox_show->x + bbox_show->w;
		p1.y = bbox_show->y + bbox_show->h;

		if (bbox_show->bbox_tag == m_bbox_id_current)
		{
			//ScreenToClient(&PicBoxRect);
			CvSize size = cvSize(bbox_show->w, bbox_show->h);//区域大小
			cvSetImageROI(IplImgMain_copy, cvRect(bbox_show->x, bbox_show->y, bbox_show->w, bbox_show->h));//设置源图像ROI
			IplImgBBox = cvCreateImage(size, IplImgMain_copy->depth, IplImgMain_copy->nChannels);//创建目标图像
			cvCopy(IplImgMain_copy, IplImgBBox); //复制图像
			cvResetImageROI(IplImgMain_copy);
			
			m_Current_bboxHeight = PicBBoxRect.Height();
			m_Current_bboxWidth = PicBBoxRect.Width();

			int PicBBoxWidth = PicBBoxRect.right - PicBBoxRect.left;   //获取Picture控件的宽和高
			int PicBBoxHeight = PicBBoxRect.bottom - PicBBoxRect.top;
			int BBoxImgWidth = IplImgBBox->width;							//获取图片的宽和高
			int BBoxImgHeight = IplImgBBox->height;
			int BBoxCenterX = (int)(PicBBoxWidth - BBoxImgWidth) / 2;        // 使图片的显示位置正好在控件的正中
			int BBoxCenterY = (int)(PicBBoxWidth - BBoxImgHeight) / 2;


			float bbox_width = (float)IplImgBBox->width;;
			float bbox_height = (float)IplImgBBox->height;
			float bbox_c = 0.0f;
			float bbox_x = 0.0f;
			float bbox_y = 0.0f;

			// 按适合宽度模式处理
			bbox_width = (float)PicBBoxRect.Width();
			bbox_c = bbox_width / (IplImgBBox->width);

			if (bbox_c * (IplImgBBox->height) < PicBBoxRect.Height()) // 如果没有超界
			{
				bbox_height = bbox_c * (IplImgBBox->height);
			}
			else // 超界则采用适合高度模式
			{
				bbox_height = (float)PicBBoxRect.Height();
				bbox_c = bbox_height / (IplImgBBox->height);
				bbox_width = bbox_c * (IplImgBBox->width);
			}

			if ((UINT)PicBBoxRect.Width() > bbox_width)
			{
				bbox_x = ((UINT)PicBBoxRect.Width() - bbox_width) / 2.0f;
			}

			if ((UINT)PicBoxRect.Height() > bbox_height)
			{
				bbox_y = ((UINT)PicBBoxRect.Height() - bbox_height) / 2.0f;
			}

			BBoxCenterX = bbox_x;
			BBoxCenterY = bbox_y;
			BBoxImgWidth = bbox_width;
			BBoxImgHeight = bbox_height;

			//所设置的4个点为Picture控件中图片显示的左上和右下的坐标
			SetRect(PicBBoxRect, BBoxCenterX, BBoxCenterY, BBoxCenterX + BBoxImgWidth, BBoxCenterY + BBoxImgHeight);


			//所设置的4个点为Picture控件中图片显示的左上和右下的坐标
			SetRect(PicBoxRect, CenterX, CenterY, CenterX + ImgWidth, CenterY + ImgHeight);
			cvRectangle(IplImgMain_copy, p0, p1, CV_RGB(0, 0, 255), 2, 8, 0);

			uchar * a = (uchar *)malloc(256 * sizeof(uchar));
			for (int i = 0; i<256; i++){
				a[i] = bbox_show->bbox_annos[i];
			}
			//text.putText(IplImgMain_copy, (const char *)a, p0, CV_RGB(255, 0, 0));
		}
		else
		{
			//所设置的4个点为Picture控件中图片显示的左上和右下的坐标
			SetRect(PicBoxRect, CenterX, CenterY, CenterX + ImgWidth, CenterY + ImgHeight);
			cvRectangle(IplImgMain_copy, p0, p1, CV_RGB(255, 0, 0), 2, 8, 0);
		}

		bbox_show = bbox_show->next;
	}		

	CBrush myBrush(RGB(0, 0, 0));    // dialog background color
	CBrush *pOld = pDC_bbox->SelectObject(&myBrush);
	BOOL bRes = pDC_bbox->PatBlt(0, 0, PicBBoxRect.Width(), PicBBoxRect.Height(), PATCOPY);
	CvvImage bimg;
	bimg.CopyOf(IplImgBBox);
	bimg.DrawToHDC(hDC_bbox, &PicBBoxRect);
	bimg.Destroy();   // 释放CvvImage


	CvvImage cimg;
	cimg.CopyOf(IplImgMain_copy);
	cimg.DrawToHDC(hDC, &PicBoxRect);
	cimg.Destroy();   // 释放CvvImage
	cvReleaseImage(&IplImgMain_copy);
	ReleaseDC(pDC);
	ReleaseDC(pDC_bbox);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{

	return static_cast<HCURSOR>(m_hIcon);
}

char * CMFCApplication1Dlg::CString2ConstChar(CString FilePath)
{
	LPCTSTR p = FilePath.GetBuffer(); //将CString转换成LPCTSTR
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_ACP, NULL, p, -1, NULL, 0, NULL, FALSE);
	char * ch = new char[dwMinSize];
	WideCharToMultiByte(CP_OEMCP, NULL, p, -1, ch, dwMinSize, NULL, FALSE);
	return ch;
}

const wchar_t *GetWC(const char *c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}
//http://www.it610.com/article/2003763.htm
void CMFCApplication1Dlg::OnBnClickedButtonDataPath()
{
	// TODO:  在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE);///TRUE为OPEN对话框，FALSE为SAVE AS对话框

	if (dlg.DoModal() == IDOK)
	{
		m_data_path = dlg.GetPathName();
		IplImgMain = cvLoadImage(CString2ConstChar((m_data_path.GetBuffer())));
		
		bool bLoad = false;
		CString base_name = m_data_path;
		CString base_save_name = m_data_path;
		base_save_name.Replace(GetWC(".jpg"), GetWC("_data_bak.xml"));
		base_name.Replace(GetWC(".jpg"), GetWC("_data.xml"));

		bLoad = xml.Load(base_name);
		if (!bLoad)
		{
			AfxMessageBox(_T("没有找到XML文件"), MB_OK | MB_ICONSTOP);
			return;
		}
		else
		{
			//back up
			xml.Save((MCD_CSTR)(base_save_name));//默认创建路径为该工程的根目录

			//load xml data
			m_predlg_open_flag = TRUE;
			m_bbox_sum = 0;
			m_bbox_id_current = 1;
			bbox = (bbox_node *)malloc(sizeof(bbox_node));
			bbox->next = NULL;
			bbox_tail = bbox;

			xml.ResetMainPos();//把xml对象指向的位置初始化，使其指向文件开始
			xml.FindElem();//查找任意元素，此处查找到的第一个元素即为根元素
			xml.IntoElem();//进入Root
			if (!xml.FindElem(_T("Layers")))
			{
				AfxMessageBox(_T("没有找到XML文件 Layers"), MB_OK | MB_ICONSTOP);
			}
			else
			{
				xml.IntoElem();//进入Root
				if (!xml.FindElem(_T("Layer")))
					AfxMessageBox(_T("没有找到XML文件 Layer"), MB_OK | MB_ICONSTOP);
				else
				{
					xml.IntoElem();//进入Root
					if (!xml.FindElem(_T("Shapes")))
						AfxMessageBox(_T("没有找到XML文件 Shapes"), MB_OK | MB_ICONSTOP);
					else
					{
						xml.IntoElem();//进入Root
						while (xml.FindElem(_T("Shape")))//不能使用if，因为要遍历所有的Student元素
						{
							CMarkup xml_tmp = xml;
							m_bbox_sum++;
							CString strTagName;//元素名称
							CString strData;//元素内容

							bbox_new = (bbox_node *)malloc(sizeof(bbox_node));

							//xml_tmp.IntoElem();//进入Root
							//xml_tmp.FindElem(_T("Rectangle"));
							//xml_tmp2 = xml_tmp;
							//xml_tmp2.IntoElem();//进入Root
							xml_tmp2.FindElem(_T("BlockText"));
							xml_tmp2.IntoElem();//进入Root
							xml_tmp2.FindElem(_T("Text"));
							strData = xml_tmp.GetData();////获取元素内容
							//bbox_new->bbox_annos = strData;
							strcpy(bbox_new->bbox_annos, (const char *)(LPCTSTR)strData);

							
							xml_tmp.IntoElem();//进入Root
							xml_tmp.FindElem(_T("Data"));
							xml_tmp.IntoElem();//进入Root
							xml_tmp.FindElem(_T("Extent"));


							strData = xml_tmp.GetAttrib(_T("X"));////获取元素内容
							bbox_new->x = _tstof((LPCTSTR)strData); 
							strData = xml_tmp.GetAttrib(_T("Y"));////获取元素内容
							bbox_new->y = _tstof((LPCTSTR)strData);
							strData = xml_tmp.GetAttrib(_T("Width"));////获取元素内容
							bbox_new->w = _tstof((LPCTSTR)strData);
							strData = xml_tmp.GetAttrib(_T("Height"));////获取元素内容
							bbox_new->h = _tstof((LPCTSTR)strData);
							strData = xml_tmp.GetAttrib(_T("Height"));////获取元素内容
							bbox_new->h = _tstof((LPCTSTR)strData);
							bbox_new->bbox_tag = m_bbox_sum;
							bbox_new->next = NULL;
							bbox_tail->next = bbox_new;
							bbox_tail = bbox_new;
						}
						bbox_tail->next = NULL;
					}
				}
			}

			

			m_predlg_open_flag = TRUE;				//设置目录打开标志，只有打开了以后，才可以显示图像
			ShowImage();
		}
	}
	else
		UpdateData(true);
	
	
	UpdateData(FALSE);
}


void CMFCApplication1Dlg::OnStnClickedStaticMainPev()
{
	// TODO:  在此添加控件通知处理程序代码
}


void CAboutDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


void CMFCApplication1Dlg::OnEnChangeEditBboxSum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CMFCApplication1Dlg::OnEnChangeEditPath()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CMFCApplication1Dlg::OnStnClickedStaticBbox()
{
	// TODO:  在此添加控件通知处理程序代码
}


void CMFCApplication1Dlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nHotKeyId) {
	case 1001:
	case 1002:
		if (m_bbox_id_current  > 1)
			m_bbox_id_current--;
		ShowImage();
		break;
	case 1003:
	case 1004:
		if (m_bbox_id_current < m_bbox_sum)
			m_bbox_id_current++;
		ShowImage();
		break;
	case 1005:
		
		if (m_bbox_id_current < m_bbox_sum)
			m_bbox_id_current++;
		ShowImage();
		break;
	default:
		ShowImage();
		break;
	}
	UpdateData(FALSE);
	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}


BOOL CMFCApplication1Dlg::DestroyWindow()
{
	// TODO:  在此添加专用代码和/或调用基类

	return CDialogEx::DestroyWindow();
}


void CMFCApplication1Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO:  在此处添加消息处理程序代码
	//解除热键注册
	UnregisterHotKey(this->GetSafeHwnd(), 1001);
	UnregisterHotKey(this->GetSafeHwnd(), 1002);
	UnregisterHotKey(this->GetSafeHwnd(), 1003);
	UnregisterHotKey(this->GetSafeHwnd(), 1004);
	UnregisterHotKey(this->GetSafeHwnd(), 1005);
}

BOOL CMFCApplication1Dlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam)
	{
		if (GetFocus() == GetDlgItem(IDC_EDIT1_ANNOS))  //根据不同控件焦点判断是那个在执行  
		{
			CString Current_input_annos;

			UpdateData(TRUE);
			Current_input_annos = m_annos_ch;

			if (m_bbox_id_current > m_bbox_sum)
			{
				//int a = MessageBox("输入完成，请打开下一个图片?？", "Warning!!", MB_YESNO | MB_ICONWARNING);
				m_bbox_id_current--;
				UpdateData(FALSE);
				int a = AfxMessageBox(_T("输入完成，请打开下一个图片？", MB_YESNO | MB_ICONQUESTION));
				//int a = MessageBox(_T("输入完成，请打开下一个图片？", "Warning!!"), MB_YESNO | MB_ICONWARNING);
				if (a == IDYES)
				{
					m_bbox_id_current = 0;
				}
				else
					AfxMessageBox(_T("需要重新打开，否则出错！"));
			}
			else
			{ 
				m_bbox_id_current++;
				ShowImage();
				m_annos_ch = "";
				UpdateData(FALSE);

				bbox_save = bbox;
				UINT count=1;

				IplImgMain = cvLoadImage(CString2ConstChar((m_data_path.GetBuffer())));

				bool bLoad = false;
				CString base_name = m_data_path;
				base_name.Replace(GetWC(".jpg"), GetWC("_data.xml"));

				bLoad = xml.Load(base_name);
				if (!bLoad)
				{
					AfxMessageBox(_T("没有找到XML文件"), MB_OK | MB_ICONSTOP);
				}
				else
				{
					m_predlg_open_flag = TRUE;
					bbox_save = (bbox_node *)malloc(sizeof(bbox_node));
					bbox_save->next = NULL;
					bbox_tail = bbox_save;

					xml.ResetMainPos();//把xml对象指向的位置初始化，使其指向文件开始
					xml.FindElem();//查找任意元素，此处查找到的第一个元素即为根元素
					xml.IntoElem();//进入Root
					if (!xml.FindElem(_T("Layers")))
					{
						AfxMessageBox(_T("没有找到XML文件 Layers"), MB_OK | MB_ICONSTOP);
					}
					else
					{
						xml.IntoElem();//进入Root
						if (!xml.FindElem(_T("Layer")))
							AfxMessageBox(_T("没有找到XML文件 Layer"), MB_OK | MB_ICONSTOP);
						else
						{
							xml.IntoElem();//进入Root
							if (!xml.FindElem(_T("Shapes")))
								AfxMessageBox(_T("没有找到XML文件 Shapes"), MB_OK | MB_ICONSTOP);
							else
							{
								xml.IntoElem();//进入Root
								while (xml.FindElem(_T("Shape")))//不能使用if，因为要遍历所有的Student元素
								{
									CMarkup xml_temp = xml;
									count++;

									xml_temp.IntoElem();//进入Root
									if (!xml_temp.FindElem(_T("BlockText")))
									{
										AfxMessageBox(_T("没有找到XML文件 BlockText"), MB_OK | MB_ICONSTOP);
									}
									else
									{
										xml_temp.IntoElem();//进入Root
										if (!xml_temp.FindElem(_T("Text")))
										{
											AfxMessageBox(_T("没有找到XML文件 Text"), MB_OK | MB_ICONSTOP);
										}
										else
										{
											//bbox_new->bbox_annos = strData;
											if (count == m_bbox_id_current)
											{
												CString strData;
												xml_temp.SetData(Current_input_annos);////设置子元素内容
												//char *ch = "\\psf\Home\Desktop\Test\902_data.xml";
												//CString save_name(ch);
												xml_temp.Save((MCD_CSTR)(base_name));//默认创建路径为该工程的根目录
												break;
											}
										}
									}
								}
								bbox_tail->next = NULL;
							}
						}
					}
				}
			}
		}
	}
	return 0; //这里必须返回0 否则运行报错  
}