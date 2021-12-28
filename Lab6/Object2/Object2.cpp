#include "framework.h"
#include "pch.h"
#include "Object2.h"
#include <random>
#include "Resource.h"
#include <iostream>
using namespace std;

#define MAX_LOADSTRING 100

HINSTANCE hInst;                    
WCHAR szTitle[MAX_LOADSTRING];      
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
int RandomInt(int low, int high);
static int Count(int element);
void OnCopyData(HWND hWnd, WPARAM wParam, LPARAM lParam);
int PutTextToClipboard(HWND hWnd, char* src);
void StartObj3(HWND hWnd);
void CreateMatrix(HWND hWnd);
int SendCopyData(HWND hWndDest, HWND hWndSrc, void* lp, long cb);

int const allValues = 3;
int values_MOD2[allValues];
HWND hWndDataCreator = NULL;

int n_MOD2;
int Min_MOD2;
int Max_MOD2;

BOOL Counter = FALSE;

std::string copyMatrix = "";

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_OBJECT2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OBJECT2));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_OBJECT2));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_OBJECT2);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        srand(time(0));
        SetWindowPos(hWnd, HWND_BOTTOM, 40, 40, 300, 300, SWP_DEFERERASE);
    }
    break;
    case WM_COPYDATA:
    {
        OnCopyData(hWnd, wParam, lParam);

        if (n_MOD2 > 0)
        {
            CreateMatrix(hWnd);
        }

        InvalidateRect(hWnd, 0, TRUE);
    }
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProcW(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_PAINT:
    {
        RECT rc = { 0 };
        GetClientRect(hWnd, &rc);
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        char* cstr = new char[copyMatrix.size() + 1];
        strcpy_s(cstr, copyMatrix.size() + 1, copyMatrix.c_str());
        PutTextToClipboard(hWnd, cstr);

        DrawTextA(hdc, cstr, -1, &rc, DT_TOP);

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
    {
        PostQuitMessage(0);
    }
    break;

    default:
        return DefWindowProcW(hWnd, message, wParam, lParam);
    }
    return 0;
}

void CreateMatrix(HWND hWnd)
{
    int** matrix = new int* [n_MOD2];
    for (int i = 0; i < n_MOD2; ++i)
    {
        matrix[i] = new int[n_MOD2];
    }

    for (int i = 0; i < n_MOD2; ++i)
    {
        for (int j = 0; j < n_MOD2; ++j)
        {
            matrix[i][j] = RandomInt(Min_MOD2, Max_MOD2);

            copyMatrix += to_string(matrix[i][j]);
            if (j < n_MOD2-1)
            {
                copyMatrix += " ";
            }
        }

        if (i < n_MOD2)
        {
            copyMatrix += "\n";
        }
    }

    for (int i = 0; i < n_MOD2; ++i)
    {
        delete[] matrix[i];
    }
    delete[] matrix;
}

int SendCopyData(HWND hWndDest, HWND hWndSrc, void* lp, long cb)
{
    COPYDATASTRUCT cds{};
    cds.dwData = 1;
    cds.cbData = cb;
    cds.lpData = lp;
    return SendMessage(hWndDest, WM_COPYDATA, (WPARAM)hWndSrc, (LPARAM)&cds);
}

int RandomInt(int low, int high)
{
    return rand() % high + low;
}

int Count(int element)
{
    int count_MOD1 = 0;
    while (element != 0)
    {
        element = element / 10;
        ++count_MOD1;
    }
    return count_MOD1;
}

void OnCopyData(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    COPYDATASTRUCT* cds;
    cds = (COPYDATASTRUCT*)lParam;
    long* p = (long*)cds->lpData;
    n_MOD2 = p[0];
    Min_MOD2 = p[1];
    Max_MOD2 = p[2];
}

int PutTextToClipboard(HWND hWnd, char* src)
{
    HGLOBAL hglbCopy;
    BYTE* pTmp;
    long len;
    if (src == NULL) return 0;
    if (src[0] == 0) return 0;
    len = strlen(src);
    hglbCopy = GlobalAlloc(GHND, len + 1);
    if (hglbCopy == NULL) return FALSE;
    pTmp = (BYTE*)GlobalLock(hglbCopy);
    memcpy(pTmp, src, len + 1);
    GlobalUnlock(hglbCopy);
    if (!OpenClipboard(hWnd))
    {
        GlobalFree(hglbCopy);
        return 0;
    }
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hglbCopy);
    CloseClipboard();

    StartObj3(hWnd);
    return 1;
}

void StartObj3(HWND hWnd)
{
    hWndDataCreator = FindWindow("OBJECT3", NULL);
    if (hWndDataCreator == NULL)
    {
        WinExec("Object3.exe", SW_SHOW);
        hWndDataCreator = FindWindow("OBJECT3", NULL);
    }
    else {
        InvalidateRect(hWndDataCreator, 0, TRUE);
    }
    
    long params[allValues] = { n_MOD2};

    SendCopyData(hWndDataCreator, hWnd, params, sizeof(params));
}
