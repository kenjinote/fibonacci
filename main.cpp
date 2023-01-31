#pragma comment(linker, "/opt:nowin98")
#include <windows.h>

CHAR szClassName[]="window";
HWND hEdit;

// 再帰（指数時間かかる）
int fib1(int n){
	if(n==0 || n==1){
		return 1;
	}else{
		return fib1(n - 1) + fib1(n - 2);
	}
}
// 動的計画法（多項式時間かかる）
int fib2(int n) {
	int num1 = 1, num2 = 1, tmp = 1;
	for(int i = 1; i < n; i++) {
		tmp = num1 + num2;
		num1 = num2;
		num2 = tmp;
	}
	return tmp;
}

VOID AddLine(LPSTR str)
{
	DWORD len = SendMessage(hEdit,WM_GETTEXTLENGTH,0,0);
	SendMessage(hEdit,EM_SETSEL,(WPARAM)len,(LPARAM)len);
	SendMessage(hEdit,EM_REPLACESEL,(WPARAM)false,(LPARAM)str);
}

void SetFibonacci(void)
{
	int i;
	TCHAR szTemp[128];
	
	for(i=0;i<32;i++){
		wsprintf(szTemp,"%d\r\n", fib2(i));
		AddLine(szTemp);
	}
	SendMessage(hEdit,EM_SETSEL, (WPARAM)0, (LPARAM)-1);
	SetFocus(hEdit);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
    switch (msg){
	case WM_CREATE:
		hEdit=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",WS_CHILD|WS_VISIBLE|WS_VSCROLL|ES_MULTILINE|ES_AUTOHSCROLL|ES_AUTOVSCROLL,0,0,0,0,hWnd,NULL,((LPCREATESTRUCT)(lParam))->hInstance,NULL);
		SetFibonacci();//↑でES_AUTOHSCROLL|ES_AUTOVSCROLLを指定しないと部分的にしか挿入されない
		break;
	case WM_SIZE:
		MoveWindow(hEdit,0,0,LOWORD(lParam),HIWORD(lParam),TRUE);
		break;
	case WM_SETFOCUS:
		SetFocus(hEdit);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return(DefWindowProc(hWnd,msg,wParam,lParam));
    }
    return (0L);
}

EXTERN_C void __cdecl WinMainCRTStartup()
{
    MSG msg;
    STARTUPINFO si;
    WNDCLASS wndclass={CS_HREDRAW|CS_VREDRAW,WndProc,0,0,GetModuleHandle(NULL),NULL,LoadCursor(NULL,IDC_ARROW),(HBRUSH)(COLOR_WINDOW+1),NULL,szClassName};
    RegisterClass(&wndclass);
    HWND hWnd=CreateWindow(szClassName,TEXT("フィボナッチ数列"),WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,GetModuleHandle(NULL),NULL);
    GetStartupInfo(&si);ShowWindow(hWnd,(si.dwFlags&STARTF_USESHOWWINDOW)?si.wShowWindow:SW_SHOWDEFAULT);
    UpdateWindow(hWnd);
    while(GetMessage(&msg,NULL,0,0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    ExitProcess(0);
}

#if _DEBUG
void main(){}
#endif