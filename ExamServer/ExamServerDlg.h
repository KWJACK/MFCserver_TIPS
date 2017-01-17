
// ExamServerDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "MySocket.h"
#define MAX_USER_COUNT 100
struct UserData {
	SOCKET h_socket;// -1�̸� �Ⱦ��� ���� -1�� �ƴϸ� ���� ����
	char ip_address[16];//255.255.255.255 15�ڸ�+ /0-> 16��
};
// CExamServerDlg ��ȭ ����
class CExamServerDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CExamServerDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.//��ü������
	void AddEventString(const char *ap_string);
// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXAMSERVER_DIALOG };
#endif
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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
