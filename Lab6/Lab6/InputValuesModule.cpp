#include "pch.h"
#include "framework.h"
#include "InputValuesModule.h"

HINSTANCE hInstCurrent;

long n_MOD1;
long Min_MOD1;
long Max_MOD1;

const int allValues = 3;

HWND hWndDataCreator = NULL;

static INT_PTR CALLBACK InputValues_MOD1(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
static INT_PTR CALLBACK Warning_MOD1(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
static void OnOk(HWND hDlg);
static void OnCancel(HWND hDlg);
static void OnClose(HWND hDlg);
int SendCopyData(HWND hWndDest, HWND hWndSrc, void* lp, long cb);

int Func_MOD1(HINSTANCE hInst, HWND hWnd)
{
    return DialogBox(hInst, MAKEINTRESOURCE(IDD_INPUT), hWnd, InputValues_MOD1);
}

INT_PTR CALLBACK InputValues_MOD1(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    switch (iMessage)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            OnOk(hDlg);
            return (INT_PTR)TRUE;
            break;
        case IDCANCEL:
            OnCancel(hDlg);
            return (INT_PTR)TRUE;
            break;
        }
        break;
    case WM_CLOSE:
    {
        OnClose(hDlg);
    }
    break;
    default: break;
    }
    return FALSE;
}

void OnOk(HWND hDlg)
{
    n_MOD1 = GetDlgItemInt(hDlg, IDC_EDIT_N, NULL, FALSE);
    Min_MOD1 = GetDlgItemInt(hDlg, IDC_EDIT_MIN, NULL, FALSE);
    Max_MOD1 = GetDlgItemInt(hDlg, IDC_EDIT_MAX, NULL, FALSE);

    if (n_MOD1 == NULL  || Min_MOD1 == NULL || Max_MOD1 == NULL )
    {
        DialogBox(hInstCurrent, MAKEINTRESOURCE(IDD_WARNING_NULL), hDlg, Warning_MOD1);
        return;
    }

    if (Max_MOD1 > 9)
    {
        DialogBox(hInstCurrent, MAKEINTRESOURCE(IDD_WARNING_MAX), hDlg, Warning_MOD1);
        return;
    }

    if (Min_MOD1 <= Max_MOD1)
    {
        hWndDataCreator = FindWindow("OBJECT2", NULL);
        if (hWndDataCreator == NULL)
        {
            WinExec("Object2.exe", SW_SHOW);
            hWndDataCreator = FindWindow("OBJECT2", NULL);
        }
        else {
            InvalidateRect(hWndDataCreator, 0, TRUE);
        }

        long params[allValues] = { n_MOD1, Min_MOD1, Max_MOD1 };

        SendCopyData(hWndDataCreator, GetParent(hDlg), params, sizeof(params));

        return;
    }
    else
    {
        DialogBox(hInstCurrent, MAKEINTRESOURCE(IDD_WARNING_VALUES),
            hDlg, Warning_MOD1);
        return;
    }
}

INT_PTR CALLBACK Warning_MOD1(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    switch (iMessage)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            EndDialog(hDlg, 0);
            return (INT_PTR)TRUE;
            break;
        }
        break;
    case WM_CLOSE:
    {
        OnClose(hDlg);
    }
    break;
    default: break;
    }
    return FALSE;
}

void OnCancel(HWND hDlg)
{
    EndDialog(hDlg, 0);
}

void OnClose(HWND hDlg)
{
    EndDialog(hDlg, 0);
}

int SendCopyData(HWND hWndDest, HWND hWndSrc, void* lp, long cb)
{
    COPYDATASTRUCT cds{};
    cds.dwData = 1;
    cds.cbData = cb;
    cds.lpData = lp;
    return SendMessage(hWndDest, WM_COPYDATA, (WPARAM)hWndSrc, (LPARAM)&cds);
}