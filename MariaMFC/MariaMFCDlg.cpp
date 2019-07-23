/*
	Create date: 2019-07-24
	ProjectName: MariaMFC
	Filename: MariaMFCDlg.cpp
	Author: Dodo (rabbit.white@daum.net / jungwy@kumoh.ac.kr)
	Description:
	2019-07-24 / MariaDB 64bit (10.4)버전으로 작성함 / dodo

*/


// MariaMFCDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MariaMFC.h"
#include "MariaMFCDlg.h"
#include "afxdialogex.h"
#include <string>
#include <mysql.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMariaMFCDlg 대화 상자

CMariaMFCDlg::CMariaMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MARIAMFC_DIALOG, pParent)
	, m_txtId(_T(""))
	, m_txtEmail(_T(""))
	, m_birthdate(COleDateTime::GetCurrentTime())
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMariaMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DatagridView, m_datagridview);
	DDX_Text(pDX, IDC_TxtID, m_txtId);
	DDV_MaxChars(pDX, m_txtId, 20);
	DDX_Text(pDX, IDC_Txt_Email, m_txtEmail);
	DDV_MaxChars(pDX, m_txtEmail, 100);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER, m_birthdate);
}

BEGIN_MESSAGE_MAP(CMariaMFCDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_ALERT, &CMariaMFCDlg::OnBnClickedBtnAlert)
	ON_BN_CLICKED(IDC_BTN_SELECT, &CMariaMFCDlg::OnBnClickedBtnSelect)
	ON_BN_CLICKED(IDC_BTN_INSERT, &CMariaMFCDlg::OnBnClickedBtnInsert)
END_MESSAGE_MAP()


// CMariaMFCDlg 메시지 처리기

BOOL CMariaMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// m_lst_dataview
	m_datagridview.InsertColumn(0, _T("번호"), NULL, 50);
	m_datagridview.InsertColumn(1, _T("이름"), NULL, 150);
	m_datagridview.InsertColumn(2, _T("생년월일"), NULL, 20);

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMariaMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMariaMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMariaMFCDlg::OnBnClickedBtnAlert()
{
	CString strTxt = CString(_T("안녕하세요"));
	MessageBoxW((LPCTSTR)strTxt);

	UpdateData(TRUE);	// 필드 값 최신화
	MessageBox((LPCTSTR)m_txtId, _T("알림(Alert)"), MB_ICONINFORMATION);
}


void CMariaMFCDlg::OnBnClickedBtnSelect()
{

	LVITEM firstitem;

	char* host = "hostName";
	char* userId = "userID";
	char* passwd = "password";
	char* dbName = "dbName";
	int port = 3306;

	/* MariaDB(MySQL 연동) */
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;

	mysql_init(&mysql);
	if (mysql_real_connect(&mysql, host, userId, passwd, dbName, port, NULL, 0) == NULL)
	{
		return;
	}
	else {
		mysql_query(&mysql, "set names euckr");	// 한글 깨짐 보완
	}

	char* szQuery = "select * from member";

	if (mysql_query(&mysql, szQuery)) {
		return;
	}

	if ((res = mysql_store_result(&mysql)) == NULL) {
		return;
	}

	row = mysql_fetch_row(res);

	while (row)
	{
		std::string name = row[1];
		//std::string type = row[2];

		CString tmp;
		tmp = name.c_str();

		firstitem = { 0 };

		firstitem.mask = LVIF_TEXT;
		firstitem.iItem = 0;

		// 데이터 삽입 - Firstitem
		m_datagridview.InsertItem(&firstitem);

		// 데이터 값 넣기 - Firstitem
		m_datagridview.SetItemText(0, 0, _T("1번"));
		m_datagridview.SetItemText(0, 1, tmp);
		m_datagridview.SetItemText(0, 2, _T("도도1-2"));

		row = mysql_fetch_row(res);
	}

	mysql_free_result(res);

	mysql_close(&mysql);

}


void CMariaMFCDlg::OnBnClickedBtnInsert()
{

	LVITEM firstitem;

	char* host = "hostName";
	char* userId = "userID";
	char* passwd = "password";
	char* dbName = "dbName";
	int port = 3306;

	/* MariaDB(MySQL 연동) */
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;

	UpdateData(TRUE);	// 필드 값 최신화

	mysql_init(&mysql);
	if (mysql_real_connect(&mysql, host, userId, passwd, dbName, port, NULL, 0) == NULL)
	{
		return;
	}
	else {
		mysql_query(&mysql, "set names euckr");			// 한글 깨짐 보완
	}

	const int KO_KR_MAX_SIZE = 128;						// 한글 지원(여유공간)
	CString csTemp = (LPCTSTR)m_txtId;
	char* strName = new char[(strlen(CT2A(csTemp)) + 1) + KO_KR_MAX_SIZE];
	strcpy_s(strName, sizeof(strName) + KO_KR_MAX_SIZE, CT2A(csTemp));
	
	csTemp.Empty();
	csTemp.Format(_T("%04d-%02d-%02d %d:%d:%d"), m_birthdate.GetYear(),
												m_birthdate.GetMonth(),
												m_birthdate.GetDay(),
												m_birthdate.GetHour(),
												m_birthdate.GetMinute(),
												m_birthdate.GetSecond());


	char* strBirthDate = new char[(strlen(CT2A(csTemp)) + 1) + KO_KR_MAX_SIZE];
	strcpy_s(strBirthDate, sizeof(strBirthDate) + KO_KR_MAX_SIZE, CT2A(csTemp));

	char szQuery[256 + KO_KR_MAX_SIZE];
	int len = sprintf_s(szQuery, sizeof(szQuery), "insert into member(name, birthdate) values('%s', '%s')", strName, strBirthDate);

	if (mysql_query(&mysql, szQuery)) {
		return;
	}

	int check = _CrtDumpMemoryLeaks();

	mysql_close(&mysql);
}
