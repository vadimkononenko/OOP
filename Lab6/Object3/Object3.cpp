#include "framework.h"
#include "pch.h"
#include "Object3.h"
#include "Resource.h"
#include <string>
using namespace std;

#define MAX_LOADSTRING 100

HINSTANCE hInst;                    
WCHAR szTitle[MAX_LOADSTRING];      
WCHAR szWindowClass[MAX_LOADSTRING];

char bufferText[1024];
int n_MOD3;
int* buffer;
int determinant;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
long GetTextFromClipboard(HWND, char*, long);
void OnCopyData(HWND hWnd, WPARAM wParam, LPARAM lParam);
int CalculateDeterminant(int * matrix, int rows);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_OBJECT3, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OBJECT3));

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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_OBJECT3));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_OBJECT3);
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
        SetWindowPos(hWnd, HWND_BOTTOM, 341, 40, 300, 300, SWP_DEFERERASE);

        GetTextFromClipboard(hWnd, bufferText, sizeof(bufferText));
    }
        break;
    case WM_COPYDATA:
    {
        OnCopyData(hWnd, wParam, lParam);

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
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        string tempBufferForMatrixString;

        

        if (bufferText != tempBufferForMatrixString) {
            buffer = new int[n_MOD3 * n_MOD3];

            tempBufferForMatrixString = bufferText;

            string num;

            int tempCounter = 0;
            while (tempCounter != n_MOD3 * n_MOD3)
            {
                num = tempBufferForMatrixString[tempCounter * 2];// multiply 2 cuz after num is space in buffer

                buffer[tempCounter] = stod(num);
                tempCounter++;
            }

            int determinant = CalculateDeterminant(buffer, n_MOD3);
            CHAR buf[100];
            sprintf_s(buf, ("%d"), determinant);

            TextOutA(hdc, 0, 0, buf, 10);
        }

        EndPaint(hWnd, &ps);
    }

        break;
    default:
        return DefWindowProcW(hWnd, message, wParam, lParam);
    }
    return 0;
}

int CalculateDeterminant(int * matrix, int rows) {
    int determinant = 0;
    if (rows == 1) {
        determinant = matrix[0];
    }
    else if (rows == 2) {
        determinant = matrix[0] * matrix[3] - matrix[1] * matrix[2];
    }
    else {
        for (int i = 0; i < rows; i++) {
            int nextSize = rows - 1;
            int d;
            int* newMatrix = new int[nextSize * nextSize];
            int divine = (i % 2 == 0) ? 1 : -1;
            int counter = 0;
            for (int j = rows; j < rows * rows; j++) {
                if ((j) % rows == i) continue;
                newMatrix[counter] = matrix[j];
                counter++;
            }
            d = CalculateDeterminant(newMatrix, nextSize);

            determinant += divine * d * matrix[i];
            delete[] newMatrix;
        }
    }
    return determinant;
}

void OnCopyData(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    COPYDATASTRUCT* cds;
    cds = (COPYDATASTRUCT*)lParam;
    long* p = (long*)cds->lpData;
    n_MOD3 = p[0];
}

long GetTextFromClipboard(HWND hWnd, char* dest, long maxsize)
{
    HGLOBAL hglb;
    LPTSTR lptstr;
    long size, res;
    res = 0;
    if (!IsClipboardFormatAvailable(CF_TEXT)) return 0;
    if (!OpenClipboard(hWnd)) return 0;
    hglb = GetClipboardData(CF_TEXT);
    if (hglb != NULL)
    {
        lptstr = (LPTSTR)GlobalLock(hglb);
        if (lptstr != NULL)
        {
            size = strlen((char*)lptstr);
            if (size > maxsize)
            {
                lptstr[maxsize] = 0;
                size = strlen((char*)lptstr);
            }
            strcpy_s(dest, maxsize, (char*)lptstr);
            res = size;
            GlobalUnlock(hglb);
        }
    }
    CloseClipboard();
    return res;
}