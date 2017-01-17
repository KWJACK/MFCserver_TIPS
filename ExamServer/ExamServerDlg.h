
// ExamServerDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "MySocket.h"
#define MAX_USER_COUNT 100
struct UserData {
	SOCKET h_socket;// -1이면 안쓰는 소켓 -1이 아니면 쓰는 소켓
	char ip_address[16];//255.255.255.255 15자리+ /0-> 16개
};
// CExamServerDlg 대화 상자
class CExamServerDlg : public CDialogEx
{
// 생성입니다.
public:
	CExamServerDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.//객체생성자
	void AddEventString(const char *ap_string);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXAMSERVER_DIALOG };
#endif
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	MySocket my_socket;
	CListBox m_event_list;
	//SOCKET mh_listen_socket;
	UserData m_user_list[MAX_USER_COUNT];
protected:
	afx_msg LRESULT On25001(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnDestroy();
	void SendFrameData(SOCKET ah_socket, char a_message_id, unsigned short int a_body_size, char * ap_send_data);
protected:
	afx_msg LRESULT On25002(WPARAM wParam, LPARAM lParam);
};
