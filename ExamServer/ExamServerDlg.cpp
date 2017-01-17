
// ExamServerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ExamServer.h"
#include "ExamServerDlg.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CExamServerDlg 대화 상자



CExamServerDlg::CExamServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EXAMSERVER_DIALOG, pParent)//객체생성자
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


// CExamServerDlg 메시지 처리기

BOOL CExamServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();	
	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	my_socket.setupsocket(18000, (char)1);
	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	/*
	mh_listen_socket = socket(AF_INET, SOCK_STREAM, 0);//AF_INET : 인터넷 기본 TCP 주소, 0은 2번째 인자 버전과 자동으로 프로토콜을 연결해줌
	sockaddr_in srv_addr;
	srv_addr.sin_family = AF_INET;//10.104.196.150
	srv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");//ip는 할당된 ip에 따른다: cmd->ipconfig
	srv_addr.sin_port = htons(18000);//htonl=host byte order to network order, l=unsigned long int)

	bind(mh_listen_socket, (LPSOCKADDR)&srv_addr, sizeof(srv_addr));//bind인프라에 연결하는 행위
	//소켓을 네트워크 카드에 연결	
	listen(mh_listen_socket, 1);//몇 개 까지 받을거냐(동시에 처리하는 것이 아님,)빠르게 만들려면 소켓을 여러개 만들어 리슨도 여러개 만듬
	*/
	AddEventString("서비스를 시작합니다."); 
	WSAAsyncSelect(my_socket.get_socket(), m_hWnd, 25001, FD_ACCEPT);//비동기함수
	//WSAAsyncSelect(mh_listen_socket, m_hWnd, 25001, FD_ACCEPT);//비동기함수
	//accept(h_socket, (LPSOCKADDR)&srv_addr, );실제 사용자가 접속할 때까지 프로그램이 응답상태에 빠지는 특성을 보완한것.
	//h_socket에서 FD_ACCEPT 이벤트가 발생하면 25001메세지를 이 대화상자에 보내라는 것-->응답없음 상태에 빠지지않음


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CExamServerDlg::OnPaint()
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
HCURSOR CExamServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CExamServerDlg::AddEventString(const char *ap_string)
{//리스트 박스에 문자 출력 및 커서 자동이동하는 함수
 //CString이 const char로 값을 제한하기 때문에 char*는 더 넓은 범위를 뜻하기 때문에 char*로 CString을 다룰 때 캐스팅이 필요하여
 //con char *로 변수를 선언하여 사용한다
 //LPCTSTR -> const char *
	while (m_event_list.GetCount() > 3000) {
		m_event_list.DeleteString(0);
	}
	int index = m_event_list.InsertString(-1, ap_string);
	m_event_list.SetCurSel(index);


}

afx_msg LRESULT CExamServerDlg::On25001(WPARAM wParam, LPARAM lParam)
{//wParam -> 이벤트가 발생한 소켓의 핸들이 넘어옴, lParam에 소켓에 발생한 메세지가 옴FD_ACCEPT 
	sockaddr_in client_addr;
	int sockaddr_in_size = sizeof(sockaddr_in);
	SOCKET h_socket = my_socket.create_accpet(&client_addr);
	//SOCKET h_socket = accept(my_socket.get_socket(), (LPSOCKADDR)&client_addr, &sockaddr_in_size);
	//소켓 자리에 wParam을 적어도 됨
	//2번 째 인자는 접속하는 client의 ip를 알 수 있게하는 역할(발신자 표시)
	//클론 소켓으로, listen 소켓을 복제함. listen 소켓은 사용되지않고 accept 소켓만 사용됨
	int i;
	for (i = 0; i < MAX_USER_COUNT; i++) {
		if (m_user_list[i].h_socket == INVALID_SOCKET)break;
	}

	if (i < MAX_USER_COUNT) {//빈 방이 있는 상태
		m_user_list[i].h_socket = h_socket;
		strcpy(m_user_list[i].ip_address, inet_ntoa(client_addr.sin_addr));
		//client address 복사
		WSAAsyncSelect(m_user_list[i].h_socket, m_hWnd, 25002, FD_READ | FD_CLOSE);
		//FD_WRITE는 서버에서 데이터 보낼떄 이 메세지에는 비동기를 걸지 않음-> 보내는 시점을 서버가 알기 때문
		//FD_READ 상대편이 데이터를 보낼 때 발생되는 메세지
		//FD_CLOSE 상대편이 끊을 때
		// | 비트 연산자를 사용해야함, or를 하면 오버라이트 되서 마지막에 쓰인 메세지만 받게됨
		CString str;
		str.Format("%s에서 접속했습니다.", m_user_list[i].ip_address);
	}
	else {//꽉 찬 상태
		AddEventString("관리 최대 인원 초과!\n");
		closesocket(h_socket);//accept용 소켓 클로즈->필요가 없으므로
		//끊어진 client 다시 접속 시도, -> 끊고,, 악순환 반복 -->리슨 소켓을 닫으면 아예 못들어옴.
	}
	return 0;
}


void CExamServerDlg::OnDestroy()
{//윈도우 파괴직전 생기는 메세지
	CDialogEx::OnDestroy();
	//창 닫기전 쓰고 있는 소켓이 있다면 모두 클로즈
	for (int i = 0; i < MAX_USER_COUNT; i++) {
		if (m_user_list[i].h_socket != INVALID_SOCKET) {
			closesocket(m_user_list[i].h_socket);
		}
	}
}
void CExamServerDlg::SendFrameData(SOCKET ah_socket, char a_message_id, unsigned short int a_body_size, char* ap_send_data)//헤더와 데이터의 content를 보내는 함수
{
	char *p_send_data = new char[4 + a_body_size];//헤더 4 byte
	*p_send_data = 27;//헤더 구성
	*(p_send_data + 1) = a_message_id;
	*(unsigned short *)(p_send_data + 2) = a_body_size;//일시적으로 char*를 short*에 맞게 변위를 조절
	
	memcpy(p_send_data + 4, ap_send_data, a_body_size);//데이터를 복사
	send(ah_socket, p_send_data, a_body_size + 4, 0);//send(동기화함수->다 진행될때까지) 클라이언트에게 전송
	delete[] p_send_data;//전송후 삭제
}

afx_msg LRESULT CExamServerDlg::On25002(WPARAM wParam, LPARAM lParam)
{//lParam에는 FD_READ or FD_CLOSE 의 정보를 담고있음.
 //wParam과 socket의 형은 unsigned int형
	CString str;
	if (WSAGETSELECTEVENT(lParam) == FD_READ) {//32bit값 lParam에서 상위 또는 하위의 이벤트 값을 추출함
		//수신의 약속을 정해서 보낸다. 프로토콜에 따라 다르지만 보통 1.보내는 데이터의 크기가 얼만지 밝히고(header), 이후 내용(body)을 확인함
		//데이터 크기를 나타내는 범위는 2~8K 정도가 적절(2byte범위내), 2.추가로 헤더에 자신의 클라이언트가 맞는지 고유값을 넣어 확인
		//3.mesg id에 어떤 데이터의 타입이 올지 알려준다 *1~3 여기서 쓰일 헤더를 정의*

		//FD_READ-> 발생조건 1:수신버퍼에 데이터가 없을 때, 최초 새로운 데이터가 수신되는 경우
		//발생조건 2: recv함수로 데이터를 읽었는데 수신 버퍼에 데이터가 남아 있는 경우 또 발생
		//발생조건 3: 비동기함수 설정시에 데이터가 있는 경우(너무 빨리 데이터가 수신버퍼에 차게되는 경우)

		//속도가 느린 client가 속도가 빠른 server에 보내면 수신버퍼가 빨리 비워져서 FD_READ가 많이 발생해 문제발생
		//원인은 비동기 함수를 사용해서 FD_READ를 발생하게끔 했기 때문		
		WSAAsyncSelect(wParam, m_hWnd, 25002, FD_CLOSE);//-->비동기에서 FD_READ를 제외시킴
		//정한 규약에 따라..
		char key;
		
		recv(wParam, &key, 1, 0);//1바이트 읽어서 key값에 설정. 4번쨰 인자는 recv 옵션설정 0:default
		if (key == 27) {//자신의 클라이언트에서 보낸 건지 확인
			char message_id;
			//현재 수신버퍼에서 설정한 크기만큼 읽는 함수..수신버퍼에 그보다 적은 양이 있으면
			//적은 양만큼 읽어준다.->신뢰성이 없는 함수임-> 실제로 읽은 사이즈를 반환하는 특성이 있기 때문에 이를 활용
			
			recv(wParam, &message_id, 1, 0);

			unsigned short body_size;
			recv(wParam, (char *)&body_size, 2, 0);
			char* p_body_data = NULL;
			if (body_size > 0) {//0보다 큰 사이즈로 데이터가 들어왔을경우를 위해				
				p_body_data = new char[body_size];
				int total = 0, x, retry = 0;
				while (total < body_size) {//recv가 수신한 사이즈에 맞게 content를 받음
					x = recv(wParam, p_body_data + total, body_size - total, 0);//데이터content 받음, 시작 주소는 +total, 받는 사이즈는 -total
					if (x == SOCKET_ERROR)break;//client가 비정상 종료하여 recv가 에러값을 반환하면 break될수 있도록
					total = total + x;
					if (total < body_size) {
						Sleep(50);//5밀리 세컨드 쉬어서 자신의 빠른 속도를 어느정도 조절해 수신 버퍼의 양을 높힐 수 있도록 한다.
						retry++;
						if (retry > 5)break;//재시도 5번 넘으면 break
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
		//현상 말고 원인에 집중하기		
	}
	else {//FD_CLOSE
		closesocket(wParam);//서버 소켓 클로즈
		for (int i = 0; i < MAX_USER_COUNT; i++) {
			if (m_user_list[i].h_socket == wParam) {//배열로 관리하는 소켓정보도 수정
				m_user_list[i].h_socket = INVALID_SOCKET;
				str.Format("사용자가 종료했습니다.: %s", m_user_list[i].ip_address);
				AddEventString(str);
				break;
			}
		}
	}
	return 0;
}
