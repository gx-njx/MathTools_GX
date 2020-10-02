
// MathToolsDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MathTools.h"
#include "MathToolsDlg.h"
#include "afxdialogex.h"
#include "Matrix.h"
#include "Sudoku.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMathToolsDlg 对话框



CMathToolsDlg::CMathToolsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MATHTOOLS_DIALOG, pParent)
	, input(_T(""))
	, output(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMathToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INPUT, input);
	DDX_Text(pDX, IDC_OUTPUT, output);
}

BEGIN_MESSAGE_MAP(CMathToolsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_EquationsCal, &CMathToolsDlg::OnBnClickedEquationscal)
	ON_BN_CLICKED(IDC_SudokuCal, &CMathToolsDlg::OnBnClickedSudokucal)
END_MESSAGE_MAP()


// CMathToolsDlg 消息处理程序

BOOL CMathToolsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMathToolsDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMathToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMathToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMathToolsDlg::OnBnClickedEquationscal()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CStringArray lines, tem;
	int nline = SplitStringArray(input, 13, lines);
	CMatrix A(nline, nline);
	CMatrix X(nline, 1);
	CMatrix L(nline, 1);
	for (int i = 0; i < nline; i++) {
		int nnum = SplitStringArray(lines[i], ',', tem);
		if (nnum < nline + 1) {
			MessageBox( _T("系数不足"), _T("输入错误"), 0);
			return;
		}
		for (int j = 0; j < nnum - 1; j++) {
			A(i, j) = _tstof(tem[j]);
		}
		L(i, 0) = _tstof(tem[nnum - 1]);
	}
	X = A.Inv() * L;
	CString tems;
	output.Empty();
	for (int i = 0; i < X.Row(); i++) {
		tems.Format(_T("%d=%.8lf\r\n"), i + 1, X(i, 0));
		output = output + _T("x") + tems;
	}
	UpdateData(false);
}

int CMathToolsDlg::SplitStringArray(CString str, char split, CStringArray& aStr)
{
	int startIdx = 0;
	int idx = str.Find(split, startIdx);
	aStr.RemoveAll();//先清空

	while (-1 != idx)
	{
		CString sTmp = str.Mid(startIdx, idx - startIdx);
		aStr.Add(sTmp);
		startIdx = idx + 1;
		idx = str.Find(split, startIdx);
	}
	CString sTmp = str.Right(str.GetLength() - startIdx);
	if (!sTmp.IsEmpty())
		aStr.Add(sTmp);
	return aStr.GetSize();
}


void CMathToolsDlg::OnBnClickedSudokucal()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CStringArray lines, tem;
	int sudo[9][9];
	int nline = SplitStringArray(input, 13, lines);
	if (nline < 9) {
		MessageBox(_T("参数不足,空缺请填充0"), _T("输入错误"), 0);
		return;
	}
	for (int i = 0; i < nline; i++) {
		int nnum = SplitStringArray(lines[i], ',', tem);
		if (nnum < 9) {
			MessageBox(_T("参数不足,空缺请填充0"), _T("输入错误"), 0);
			return;
		}
		for (int j = 0; j < nnum; j++) {
			sudo[i][j] = _tstoi(tem[j]);
		}
	}
	Sudoku Sudo(sudo);
	bool tag = Sudo.Callback(0, 0);
	output = _T("");
	CString tem2;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			tem2.Format(_T("%d,"), Sudo.sudo[i][j]);
			if (j == 8)tem2.Remove(',');
			output += tem2;
		}
		output += _T("\r\n");
	}
	UpdateData(false);
}
