
// ExamServerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "ExamServer.h"
#include "ExamServerDlg.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CExamServerDlg ��ȭ ����



CExamServerDlg::CExamServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EXAMSERVER_DIALOG, pParent)//��ü������
{	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	for (int i = 0; i < MAX_USER_COUNT; i++)
		m_user_list[i].h_socket = INVALID_SOCKET;
}


void CExamServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EVENT_LIST, m_event_list);
}

BEGIN_MESSAGE_MAP(CExamServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(25001, &CExamServerDlg::On25001)
	ON_WM_DESTROY()
	ON_MESSAGE(25002, &CExamServerDlg::On25002)
END_MESSAGE_MAP()


// CExamServerDlg �޽��� ó����

BOOL CExamServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();	
	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.
	my_socket.setupsocket(18000, (char)1);
	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	/*
	mh_listen_socket = socket(AF_INET, SOCK_STREAM, 0);//AF_INET : ���ͳ� �⺻ TCP �ּ�, 0�� 2��° ���� ������ �ڵ����� ���������� ��������
	sockaddr_in srv_addr;
	srv_addr.sin_family = AF_INET;//10.104.196.150
	srv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");//ip�� �Ҵ�� ip�� ������: cmd->ipconfig
	srv_addr.sin_port = htons(18000);//htonl=host byte order to network order, l=unsigned long int)

	bind(mh_listen_socket, (LPSOCKADDR)&srv_addr, sizeof(srv_addr));//bind������ �����ϴ� ����
	//������ ��Ʈ��ũ ī�忡 ����	
	listen(mh_listen_socket, 1);//�� �� ���� �����ų�(���ÿ� ó���ϴ� ���� �ƴ�,)������ ������� ������ ������ ����� ������ ������ ����
	*/
	AddEventString("���񽺸� �����մϴ�."); 
	WSAAsyncSelect(my_socket.get_socket(), m_hWnd, 25001, FD_ACCEPT);//�񵿱��Լ�
	//WSAAsyncSelect(mh_listen_socket, m_hWnd, 25001, FD_ACCEPT);//�񵿱��Լ�
	//accept(h_socket, (LPSOCKADDR)&srv_addr, );���� ����ڰ� ������ ������ ���α׷��� ������¿� ������ Ư���� �����Ѱ�.
	//h_socket���� FD_ACCEPT �̺�Ʈ�� �߻��ϸ� 25001�޼����� �� ��ȭ���ڿ� ������� ��-->������� ���¿� ����������


	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CExamServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CExamServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CExamServerDlg::AddEventString(const char *ap_string)
{//����Ʈ �ڽ��� ���� ��� �� Ŀ�� �ڵ��̵��ϴ� �Լ�
 //CString�� const char�� ���� �����ϱ� ������ char*�� �� ���� ������ ���ϱ� ������ char*�� CString�� �ٷ� �� ĳ������ �ʿ��Ͽ�
 //con char *�� ������ �����Ͽ� ����Ѵ�
 //LPCTSTR -> const char *
	while (m_event_list.GetCount() > 3000) {
		m_event_list.DeleteString(0);
	}
	int index = m_event_list.InsertString(-1, ap_string);
	m_event_list.SetCurSel(index);


}

afx_msg LRESULT CExamServerDlg::On25001(WPARAM wParam, LPARAM lParam)
{//wParam -> �̺�Ʈ�� �߻��� ������ �ڵ��� �Ѿ��, lParam�� ���Ͽ� �߻��� �޼����� ��FD_ACCEPT 
	sockaddr_in client_addr;
	int sockaddr_in_size = sizeof(sockaddr_in);
	SOCKET h_socket = my_socket.create_accpet(&client_addr);
	//SOCKET h_socket = accept(my_socket.get_socket(), (LPSOCKADDR)&client_addr, &sockaddr_in_size);
	//���� �ڸ��� wParam�� ��� ��
	//2�� ° ���ڴ� �����ϴ� client�� ip�� �� �� �ְ��ϴ� ����(�߽��� ǥ��)
	//Ŭ�� ��������, listen ������ ������. listen ������ �������ʰ� accept ���ϸ� ����
	int i;
	for (i = 0; i < MAX_USER_COUNT; i++) {
		if (m_user_list[i].h_socket == INVALID_SOCKET)break;
	}

	if (i < MAX_USER_COUNT) {//�� ���� �ִ� ����
		m_user_list[i].h_socket = h_socket;
		strcpy(m_user_list[i].ip_address, inet_ntoa(client_addr.sin_addr));
		//client address ����
		WSAAsyncSelect(m_user_list[i].h_socket, m_hWnd, 25002, FD_READ | FD_CLOSE);
		//FD_WRITE�� �������� ������ ������ �� �޼������� �񵿱⸦ ���� ����-> ������ ������ ������ �˱� ����
		//FD_READ ������� �����͸� ���� �� �߻��Ǵ� �޼���
		//FD_CLOSE ������� ���� ��
		// | ��Ʈ �����ڸ� ����ؾ���, or�� �ϸ� ��������Ʈ �Ǽ� �������� ���� �޼����� �ްԵ�
		CString str;
		str.Format("%s���� �����߽��ϴ�.", m_user_list[i].ip_address);
	}
	else {//�� �� ����
		AddEventString("���� �ִ� �ο� �ʰ�!\n");
		closesocket(h_socket);//accept�� ���� Ŭ����->�ʿ䰡 �����Ƿ�
		//������ client �ٽ� ���� �õ�, -> ����,, �Ǽ�ȯ �ݺ� -->���� ������ ������ �ƿ� ������.
	}
	return 0;
}


void CExamServerDlg::OnDestroy()
{//������ �ı����� ����� �޼���
	CDialogEx::OnDestroy();
	//â �ݱ��� ���� �ִ� ������ �ִٸ� ��� Ŭ����
	for (int i = 0; i < MAX_USER_COUNT; i++) {
		if (m_user_list[i].h_socket != INVALID_SOCKET) {
			closesocket(m_user_list[i].h_socket);
		}
	}
}
void CExamServerDlg::SendFrameData(SOCKET ah_socket, char a_message_id, unsigned short int a_body_size, char* ap_send_data)//����� �������� content�� ������ �Լ�
{
	char *p_send_data = new char[4 + a_body_size];//��� 4 byte
	*p_send_data = 27;//��� ����
	*(p_send_data + 1) = a_message_id;
	*(unsigned short *)(p_send_data + 2) = a_body_size;//�Ͻ������� char*�� short*�� �°� ������ ����
	
	memcpy(p_send_data + 4, ap_send_data, a_body_size);//�����͸� ����
	send(ah_socket, p_send_data, a_body_size + 4, 0);//send(����ȭ�Լ�->�� ����ɶ�����) Ŭ���̾�Ʈ���� ����
	delete[] p_send_data;//������ ����
}

afx_msg LRESULT CExamServerDlg::On25002(WPARAM wParam, LPARAM lParam)
{//lParam���� FD_READ or FD_CLOSE �� ������ �������.
 //wParam�� socket�� ���� unsigned int��
	CString str;
	if (WSAGETSELECTEVENT(lParam) == FD_READ) {//32bit�� lParam���� ���� �Ǵ� ������ �̺�Ʈ ���� ������
		//������ ����� ���ؼ� ������. �������ݿ� ���� �ٸ����� ���� 1.������ �������� ũ�Ⱑ ���� ������(header), ���� ����(body)�� Ȯ����
		//������ ũ�⸦ ��Ÿ���� ������ 2~8K ������ ����(2byte������), 2.�߰��� ����� �ڽ��� Ŭ���̾�Ʈ�� �´��� �������� �־� Ȯ��
		//3.mesg id�� � �������� Ÿ���� ���� �˷��ش� *1~3 ���⼭ ���� ����� ����*

		//FD_READ-> �߻����� 1:���Ź��ۿ� �����Ͱ� ���� ��, ���� ���ο� �����Ͱ� ���ŵǴ� ���
		//�߻����� 2: recv�Լ��� �����͸� �о��µ� ���� ���ۿ� �����Ͱ� ���� �ִ� ��� �� �߻�
		//�߻����� 3: �񵿱��Լ� �����ÿ� �����Ͱ� �ִ� ���(�ʹ� ���� �����Ͱ� ���Ź��ۿ� ���ԵǴ� ���)

		//�ӵ��� ���� client�� �ӵ��� ���� server�� ������ ���Ź��۰� ���� ������� FD_READ�� ���� �߻��� �����߻�
		//������ �񵿱� �Լ��� ����ؼ� FD_READ�� �߻��ϰԲ� �߱� ����		
		WSAAsyncSelect(wParam, m_hWnd, 25002, FD_CLOSE);//-->�񵿱⿡�� FD_READ�� ���ܽ�Ŵ
		//���� �Ծ࿡ ����..
		char key;
		
		recv(wParam, &key, 1, 0);//1����Ʈ �о key���� ����. 4���� ���ڴ� recv �ɼǼ��� 0:default
		if (key == 27) {//�ڽ��� Ŭ���̾�Ʈ���� ���� ���� Ȯ��
			char message_id;
			//���� ���Ź��ۿ��� ������ ũ�⸸ŭ �д� �Լ�..���Ź��ۿ� �׺��� ���� ���� ������
			//���� �縸ŭ �о��ش�.->�ŷڼ��� ���� �Լ���-> ������ ���� ����� ��ȯ�ϴ� Ư���� �ֱ� ������ �̸� Ȱ��
			
			recv(wParam, &message_id, 1, 0);

			unsigned short body_size;
			recv(wParam, (char *)&body_size, 2, 0);
			char* p_body_data = NULL;
			if (body_size > 0) {//0���� ū ������� �����Ͱ� ��������츦 ����				
				p_body_data = new char[body_size];
				int total = 0, x, retry = 0;
				while (total < body_size) {//recv�� ������ ����� �°� content�� ����
					x = recv(wParam, p_body_data + total, body_size - total, 0);//������content ����, ���� �ּҴ� +total, �޴� ������� -total
					if (x == SOCKET_ERROR)break;//client�� ������ �����Ͽ� recv�� �������� ��ȯ�ϸ� break�ɼ� �ֵ���
					total = total + x;
					if (total < body_size) {
						Sleep(50);//5�и� ������ ��� �ڽ��� ���� �ӵ��� ������� ������ ���� ������ ���� ���� �� �ֵ��� �Ѵ�.
						retry++;
						if (retry > 5)break;//��õ� 5�� ������ break
					}
				}
			}

			if (message_id == 1){
				int i;
				for (i = 0; i < MAX_USER_COUNT; i++) {
					if (m_user_list[i].h_socket == wParam)break;
				}

				CString str2;
				str2.Format("%s : %s", m_user_list[i].ip_address, p_body_data);
				AddEventString(str2);

				for (i = 0; i < MAX_USER_COUNT; i++) {//broad casting
					if (m_user_list[i].h_socket != INVALID_SOCKET) {
						SendFrameData(m_user_list[i].h_socket, 1, str2.GetLength()+1, (char *)(const char*)str2);
					}					
				}
			}

			if (p_body_data != NULL)delete[] p_body_data;
			WSAAsyncSelect(wParam, m_hWnd, 25002, FD_READ | FD_CLOSE);
		}
		//���� ���� ���ο� �����ϱ�		
	}
	else {//FD_CLOSE
		closesocket(wParam);//���� ���� Ŭ����
		for (int i = 0; i < MAX_USER_COUNT; i++) {
			if (m_user_list[i].h_socket == wParam) {//�迭�� �����ϴ� ���������� ����
				m_user_list[i].h_socket = INVALID_SOCKET;
				str.Format("����ڰ� �����߽��ϴ�.: %s", m_user_list[i].ip_address);
				AddEventString(str);
				break;
			}
		}
	}
	return 0;
}
