// PacMan.cpp : ����Ӧ�ó������ڵ㡣
//
#include "stdafx.h"
#include "PacMan.h"

#define MAX_LOADSTRING 100
#define WINLENTH 700 //700
#define WINHIGHT 740 //740
#define STAGEBEG 1
#define STAGEEND 12
#define MAXVISSPEED 18 //��С������18ms
#define MIDVISSPEED 48
#define MINVISSPEED 98
#define SERACHGAP  5000 //5s
#define IDT_TIME 111
#define IDT_SEARC 121

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
HDC hGameStart = NULL;
HDC hComdc = NULL;
HBITMAP hbGameMap = NULL;

//��Ϸ���ƶ�����:
static PacMan *pPacMan;
static PacMove *pEne1;
static PacMove *pEne2;
static PacMove *pEne3;
static GreenEne *pEne4;
Stage* Map = NULL;
unsigned int VisSpeed = MIDVISSPEED; 
std::fstream check("stage.dat", ios::in | ios::out | ios::binary);//������Դ

// �˴���ģ���а����ĺ�����ǰ������:

//�ڴ��ͷź���:
template <typename T>
void ReleaseM(T pointer);
unsigned int SaveStage(unsigned int i=0);
void ResetPacMove();

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int, HWND&);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	unsigned int stage = STAGEBEG;

	if (IDYES == MessageBox(NULL, TEXT("Start Game?"), TEXT("PacTips"), MB_YESNO | MB_ICONQUESTION))
	{
		if (IDYES == MessageBox(NULL, TEXT("Continue?"), TEXT("PacTips"), MB_YESNO | MB_ICONQUESTION))
		{
			stage = SaveStage(0);
		}
		else
		{
			stage = STAGEBEG;
		}
	}
	else
	{
		return 0;
	}

	//��ʼ��λ��
	unsigned int NUMS = STAGEEND;
	pPacMan = new PacMan(PACSHOW_R, PACSHOW_C);
	pEne1   = new RedEne(ENESHOW_R, ENESHOW_C);
	pEne2   = new RedEne(ENESHOW_R, ENESHOW_C);
	pEne3   = new BlueEne(ENESHOW_R, ENESHOW_C);
	pEne4   = new GreenEne(ENESHOW_R, ENESHOW_C);

	Map = new Stage(stage);

	if (!Map->InitialMap())
		exit(EXIT_FAILURE);

	PacMove::pMapStage = Map;
	Enemy::pacman = pPacMan;//

	MSG msg;
	HACCEL hAccelTable;
	HWND hwnd;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PACMAN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow, hwnd))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PACMAN));

	// ����Ϣѭ��:
	while ((pPacMan->GetToward() != CATCH) && (stage <= NUMS))
	{
		if (pPacMan->Win() && (stage <= NUMS))
		{
			HDC hdc;
			stage++;
			SaveStage(stage);
			hdc = GetDC(hwnd);
			ResetPacMove();

			if (stage <= NUMS)
			{
				if (IDNO == MessageBox(hwnd, TEXT("You Win this stage, continue?"), TEXT("PacTips"), MB_YESNO))
				{
					PostQuitMessage(0);
					break;
				}
				//��������
				RECT rect;
				rect.bottom = WINHIGHT;
				rect.top = 0;
				rect.left = 0;
				rect.right = WINLENTH;
				FillRect(hdc, &rect, CreateSolidBrush(RGB(255,255,255)));
				
				Map->StageWin();
				if (!(Map->InitialMap()))
				{
					MessageBox(hwnd, TEXT("error: Inital Map Wrong"), TEXT("PacTips"), MB_ICONERROR | MB_ICONERROR);
					exit(EXIT_FAILURE);
				}
				//�ػ��ͼ
				PacMove::pMapStage->DrawMap(hdc);
			}

			DeleteDC(hdc);
			continue;
		}

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	if (pPacMan->GetToward() == CATCH)
	{
		char buffer[MAX_LOADSTRING];
		sprintf_s(buffer, "You Lose, ~~o(>_<)o ~~, Score: %u\0", pPacMan->GetScore());
		MessageBox(hwnd, buffer, TEXT("PacTips"), MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(hwnd, TEXT("You Win, �r(�s���t)�q"), TEXT("PacTips"), MB_OK | MB_ICONINFORMATION);
	}

	//�ͷ��ڴ��:
	pEne4->FindPac.Destroy();
	ReleaseM(pPacMan);
	ReleaseM(pEne1);
	ReleaseM(pEne2);
	ReleaseM(pEne3);
	ReleaseM(pEne4);
	ReleaseM(Map);

	return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_VREDRAW | CS_HREDRAW;	
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PACMAN));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PACMAN);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, HWND &hwnd)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hwnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME, //ȥ����󻯺ʹ��ڴ�С�ػ�
      CW_USEDEFAULT, CW_USEDEFAULT, WINLENTH, WINHIGHT, NULL, NULL, hInstance, NULL);

   if (!hwnd)
   {
      return FALSE;
   }

   ShowWindow(hwnd, SW_SHOWNORMAL);
   UpdateWindow(hwnd);

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	//HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		SetTimer(hWnd, IDT_TIME, VisSpeed, NULL);
		SetTimer(hWnd, IDT_SEARC, SERACHGAP, NULL);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			pPacMan->Catch();
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_TIMER:
		switch (wParam)
		{	
		case IDT_TIME:
		{
			if ((pPacMan->GetToward()) == CATCH)
			{
				KillTimer(hWnd, IDT_TIME);
				break;
			}
			if (pPacMan->Win())
				break;

			//��ͼ״̬�ػ�
			HDC hdc = GetDC(hWnd);

			pEne1->MoveBehavior();
			pEne2->MoveBehavior();
			pEne3->MoveBehavior();
			pEne4->MoveBehavior();
			pPacMan->MoveBehavior();

			PacMove::pMapStage->DrawPea(hdc);
			pEne1->DrawBlank(hdc);
			pEne2->DrawBlank(hdc);
			pEne3->DrawBlank(hdc);
			pEne4->DrawBlank(hdc);
			pPacMan->DrawBlank(hdc);

			pEne1->Draw(hdc);
			pEne2->Draw(hdc);
			pEne3->Draw(hdc);
			pEne4->Draw(hdc);
			pPacMan->Draw(hdc);

			DeleteDC(hdc);
			break;
		}
		//��Ѱ
		case IDT_SEARC:
			if (!pEne4->GreenStack.IsEmpty())
			{
				break;
			}
			if (Map != NULL)
			{
				pEne4->FindPac.Destroy();
				pEne4->FindPac.SEARCH(Map->BarrierMap, pEne4->GetRow(), pEne4->GetCol(), 
				pPacMan->GetRow(), pPacMan->GetCol(), pEne4->GreenStack);
			}
			else
				break;
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
		case 'W':
		case 'w':
			pPacMan->SetTwCmd(UP);
			break;
		case VK_DOWN:
		case 'S':
		case 's':
			pPacMan->SetTwCmd(DOWN);
			break;
		case VK_LEFT:
		case 'A':
		case 'a':
			pPacMan->SetTwCmd(LEFT);
			break;
		case VK_RIGHT:
		case 'D':
		case 'd':
			pPacMan->SetTwCmd(RIGHT);
			break;
		case VK_PAUSE:
		case VK_SPACE:
		case 'P':
		case 'p':
			pPacMan->SysPause();
			break;
		case VK_PRIOR:
			if (VisSpeed > MAXVISSPEED)
			{
				VisSpeed -= 4;
				SetTimer(hWnd, IDT_TIME, VisSpeed, NULL);
			}
			break;
		case VK_NEXT:
			if (VisSpeed < MINVISSPEED)
			{
				VisSpeed += 4;
				SetTimer(hWnd, IDT_TIME, VisSpeed, NULL);
			}
			break;
		case VK_HOME:
			VisSpeed = MIDVISSPEED;
			SetTimer(hWnd, IDT_TIME, VisSpeed, NULL);
			break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		HDC hdc;
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ���������ͼ����...
		PacMove::pMapStage->DrawMap(hdc);
		EndPaint(hWnd, &ps);
		DeleteDC(hdc);
		break;
	case WM_CLOSE:
		if (IDYES == MessageBox(hWnd, TEXT("Do you want to quit?"), TEXT("PacMessage"), MB_YESNO | MB_ICONQUESTION))
			DestroyWindow(hWnd);
	case WM_DESTROY:
//		pPacMan->Catch();
		KillTimer(hWnd, IDT_TIME);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

template <typename T>
void ReleaseM(T pointer)
{
	if (pointer != NULL)
		delete pointer;
}

void ResetPacMove()
{
	pPacMan->SetPosition(PACSHOW_R, PACSHOW_C);
	pPacMan->SetTwCmd(LEFT);
	pEne1->SetPosition(ENESHOW_R, ENESHOW_C);
	pEne2->SetPosition(ENESHOW_R, ENESHOW_C);
	pEne3->SetPosition(ENESHOW_R, ENESHOW_C);
	pEne4->SetPosition(ENESHOW_R, ENESHOW_C);
	
}

//��ȡ�ؿ�ʱӦ�βε���0
unsigned int SaveStage(unsigned int i)
{
	if (i >= STAGEEND)
		i = STAGEBEG;

	unsigned int stagesave = i;

	if (i == 0)
	{	
		int fstage = 0;
		fstream indata("save.dat", ios::in | ios::binary);
		indata.read(reinterpret_cast<char *>(&fstage), sizeof(unsigned int));
		indata.close();

		return fstage;
	}
	else
	{
		fstream outdata("save.dat", ios::out | ios::binary);
		outdata.write(reinterpret_cast<const char *>(&stagesave), sizeof(stagesave));
		outdata.close();

		return stagesave;
	}
}