/*
	Trabalho 1- Fundamentos da Computação.
	Aluna: Ana Lara Alves Garcia.
	Compilador: Visual Studio Code
	Sistema Operacional: Windows
*/

#include <windows.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#define MENU_DEC_BIN 1
#define MENU_DEC_OCT 2
#define MENU_DEC_HEX 3
#define MENU_SAIR 7

#define MENU_BIN_DEC 8
#define MENU_BIN_OCT 9
#define MENU_BIN_HEX 10

#define MENU_OCT_DEC 11
#define MENU_OCT_BIN 12
#define MENU_OCT_HEX 13

#define MENU_HEX_DEC 4
#define MENU_HEX_BIN 5
#define MENU_HEX_OCT 6

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildWindowProcedure(HWND, UINT, WPARAM, LPARAM);

void AddMenus(HWND);
void CreateChildWindow(HWND, int type);
void decimalParaBinario(int decimal, char *binario);
void decimalParaOctal(int decimal, char *octal);
void decimalParaHexa(int decimal, char *hexa);
bool validarEntradaDecimal(const char *str);
bool validarEntradaBinaria(const char *str);
bool validarEntradaOctal(const char *str);
bool validarEntradaHexadecimal(const char *str);
void binarioParaDecimal(const char *binario, int *decimal);
void octalParaDecimal(const char *octal, int *decimal);
void hexaParaDecimal(const char *hexadecimal, int *decimal);

// Escopo global
HMENU hMenu;
HWND hEdit, hValueLabel;
HWND hChildWindow = NULL; // Variável global para a janela filha
int calculationType;

// API do Windows
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    WNDCLASSW wc = {0};
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"myWindowClass";
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClassW(&wc)) {
        return -1;
    }

    wc.lpszClassName = L"myChildWindowClass";
    wc.lpfnWndProc = ChildWindowProcedure;

    if (!RegisterClassW(&wc)) {
        return -1;
    }

    CreateWindowW(L"myWindowClass", L"Conversao de Bases", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);

    MSG msg = {0};

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_COMMAND:
            switch (wp) {
                case MENU_DEC_BIN:
                case MENU_DEC_OCT:
                case MENU_DEC_HEX:
                case MENU_HEX_DEC:
                case MENU_HEX_BIN:
                case MENU_HEX_OCT:
                case MENU_BIN_DEC:
                case MENU_BIN_OCT:
                case MENU_BIN_HEX:
                case MENU_OCT_DEC:
                case MENU_OCT_BIN:
                case MENU_OCT_HEX:
                    if (hChildWindow == NULL) {
                        CreateChildWindow(hWnd, wp);
                        EnableMenuItem(hMenu, MENU_SAIR, MF_GRAYED);
                    }
                    break;
                case MENU_SAIR:
                    DestroyWindow(hWnd);
                    break;
            }
            break;
        case WM_CREATE:
            AddMenus(hWnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProcW(hWnd, msg, wp, lp);
    }
    return 0;
}

void AddMenus(HWND hWnd) {
    hMenu = CreateMenu();
    HMENU hDecimalMenu = CreateMenu(); // menu DECIMAL
    HMENU hBinarioMenu = CreateMenu(); // menu BINARIO
    HMENU hOctalMenu = CreateMenu(); // menu OCTAL
    HMENU hHexaMenu = CreateMenu(); // menu HEXA

    // MENU CONVERSOES DE DECIMAL PARA TODAS AS BASES
    AppendMenu(hDecimalMenu, MF_STRING, MENU_DEC_BIN, "Decimal para Binario");
    AppendMenu(hDecimalMenu, MF_STRING, MENU_DEC_OCT, "Decimal para Octal");
    AppendMenu(hDecimalMenu, MF_STRING, MENU_DEC_HEX, "Decimal para Hexadecimal");

    // MENU CONVERSOES DE BINARIO PARA TODAS AS BASES
    AppendMenu(hBinarioMenu, MF_STRING, MENU_BIN_DEC, "Binario para Decimal");
    AppendMenu(hBinarioMenu, MF_STRING, MENU_BIN_OCT, "Binario para Octal");
    AppendMenu(hBinarioMenu, MF_STRING, MENU_BIN_HEX, "Binario para Hexadecimal");

    // MENU CONVERSOES DE OCTAL PARA TODAS AS BASES
    AppendMenu(hOctalMenu, MF_STRING, MENU_OCT_DEC, "Octal para Decimal");
    AppendMenu(hOctalMenu, MF_STRING, MENU_OCT_BIN, "Octal para Binario");
    AppendMenu(hOctalMenu, MF_STRING, MENU_OCT_HEX, "Octal para Hexadecimal");

    // MENU CONVERSOES DE HEXA PARA TODAS AS BASES
    AppendMenu(hHexaMenu, MF_STRING, MENU_HEX_DEC, "Hexadecimal para Decimal");
    AppendMenu(hHexaMenu, MF_STRING, MENU_HEX_BIN, "Hexadecimal para Binario");
    AppendMenu(hHexaMenu, MF_STRING, MENU_HEX_OCT, "Hexadecimal para Octal");

    // MENU PRINCIPAL
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hDecimalMenu, "Entrada Decimal");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hBinarioMenu, "Entrada Binario");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hOctalMenu, "Entrada Octal");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hHexaMenu, "Entrada Hexadecimal");
    AppendMenu(hMenu, MF_STRING, MENU_SAIR, "Sair");

    SetMenu(hWnd, hMenu);
}

void CreateChildWindow(HWND hWnd, int type) {
    calculationType = type;
    LPCWSTR windowTitle;
    LPCWSTR inputLabel;

    switch (type) {
        case MENU_DEC_BIN:
            windowTitle = L"Conversao Decimal para Binario";
            inputLabel = L"Entre com o numero na base Decimal:";
            break;
        case MENU_DEC_OCT:
            windowTitle = L"Conversao Decimal para Octal";
            inputLabel = L"Entre com o numero na base Decimal:";
            break;
        case MENU_DEC_HEX:
            windowTitle = L"Conversao Decimal para Hexadecimal";
            inputLabel = L"Entre com o numero na base Decimal:";
            break;
        case MENU_HEX_DEC:
            windowTitle = L"Conversao Hexadecimal para Decimal";
            inputLabel = L"Entre com o numero na base Hexadecimal:";
            break;
        case MENU_HEX_BIN:
            windowTitle = L"Conversao Hexadecimal para Binario";
            inputLabel = L"Entre com o numero na base Hexadecimal:";
            break;
        case MENU_HEX_OCT:
            windowTitle = L"Conversao Hexadecimal para Octal";
            inputLabel = L"Entre com o numero na base Hexadecimal:";
            break;
        case MENU_BIN_DEC:
            windowTitle = L"Conversao Binario para Decimal";
            inputLabel = L"Entre com o numero na base Binaria:";
            break;
        case MENU_BIN_OCT:
            windowTitle = L"Conversao Binario para Octal";
            inputLabel = L"Entre com o numero na base Binaria:";
            break;
        case MENU_BIN_HEX:
            windowTitle = L"Conversao Binario para Hexadecimal";
            inputLabel = L"Entre com o numero na base Binaria:";
            break;
        case MENU_OCT_DEC:
            windowTitle = L"Conversao Octal para Decimal";
            inputLabel = L"Entre com o numero na base Octal:";
            break;
        case MENU_OCT_BIN:
            windowTitle = L"Conversao Octal para Binario";
            inputLabel = L"Entre com o numero na base Octal:";
            break;
        case MENU_OCT_HEX:
            windowTitle = L"Conversao Octal para Hexadecimal";
            inputLabel = L"Entre com o numero na base Octal:";
            break;
        default:
            windowTitle = L"Conversao";
            inputLabel = L"Entre com o numero:";
    }

    hChildWindow = CreateWindowW(L"myChildWindowClass", windowTitle, WS_OVERLAPPED | WS_VISIBLE | WS_SYSMENU, 150, 150, 300, 200, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"Static", inputLabel, WS_VISIBLE | WS_CHILD, 20, 20, 200, 40, hChildWindow, NULL, NULL, NULL);
}

// Função para validar a entrada em decimal
bool validarEntradaDecimal(const char *str) {
    while (*str) {
        if (!isdigit(*str)) {
            return false;
        }
        str++;
    }
    return true;
}

// Função para validar a entrada em binário
bool validarEntradaBinaria(const char *str) {
    while (*str) {
        if (*str != '0' && *str != '1') {
            return false;
        }
        str++;
    }
    return true;
}

// Função para validar a entrada em octal
bool validarEntradaOctal(const char *str) {
    while (*str) {
        if (*str < '0' || *str > '7') {
            return false;
        }
        str++;
    }
    return true;
}

// Função para validar a entrada em hexadecimal
bool validarEntradaHexadecimal(const char *str) {
    while (*str) {
        if (!isdigit(*str) && !((*str >= 'A' && *str <= 'F') || (*str >= 'a' && *str <= 'f'))) {
            return false;
        }
        str++;
    }
    return true;
}

// Função conversão decimal para binário
void decimalParaBinario(int decimal, char *binario) {
    int i = 0;

    if (decimal == 0) {
        binario[i++] = '0';
        binario[i] = '\0';
        return;
    }
    while (decimal > 0) {
        binario[i++] = (decimal % 2) + '0';
        decimal /= 2;
    }
    binario[i] = '\0';

    // Invertendo a string para obter o valor correto
    int tam = strlen(binario);
    for (int j = 0; j < tam / 2; j++) {
        char temp = binario[j];
        binario[j] = binario[tam - j - 1];
        binario[tam - j - 1] = temp;
    }
}

// Função conversão decimal para octal
void decimalParaOctal(int decimal, char *octal) {
    int i = 0;

    if (decimal == 0) {
        octal[i++] = '0';
        octal[i] = '\0';
        return;
    }
    while (decimal > 0) {
        octal[i++] = (decimal % 8) + '0';
        decimal /= 8;
    }
    octal[i] = '\0';

    // Invertendo a string para obter o valor correto
    int len = strlen(octal);
    for (int j = 0; j < len / 2; j++) {
        char temp = octal[j];
        octal[j] = octal[len - j - 1];
        octal[len - j - 1] = temp;
    }
}

// Função conversão decimal para hexa
void decimalParaHexa(int decimal, char *hexa) {
    int i = 0;

    if (decimal == 0) {
        hexa[i++] = '0';
        hexa[i] = '\0';
        return;
    }
    while (decimal > 0) {
        int aux = decimal % 16;

        if (aux < 10) {
            hexa[i++] = aux + '0';
        } else {
            hexa[i++] = (aux - 10) + 'A';
        }

        decimal /= 16;
    }

    hexa[i] = '\0';

    // Invertendo a string para obter o valor correto
    int len = strlen(hexa);
    for (int j = 0; j < len / 2; j++) {
        char temp = hexa[j];
        hexa[j] = hexa[len - j - 1];
        hexa[len - j - 1] = temp;
    }
}

// Função conversão binário para decimal
void binarioParaDecimal(const char *binario, int *decimal) {
    *decimal = 0;
    while (*binario) {
        *decimal = (*decimal << 1) + (*binario++ - '0');
    }
}

// Função conversão octal para decimal
void octalParaDecimal(const char *octal, int *decimal) {
    *decimal = 0;
    while (*octal) {
        *decimal = (*decimal * 8) + (*octal++ - '0');
    }
}

// Função conversão hexadecimal para decimal
void hexaParaDecimal(const char *hexadecimal, int *decimal) {
    *decimal = 0;
    while (*hexadecimal) {
        if (isdigit(*hexadecimal)) {
            *decimal = (*decimal * 16) + (*hexadecimal - '0');
        } else if (*hexadecimal >= 'A' && *hexadecimal <= 'F') {
            *decimal = (*decimal * 16) + (*hexadecimal - 'A' + 10);
        } else if (*hexadecimal >= 'a' && *hexadecimal <= 'f') {
            *decimal = (*decimal * 16) + (*hexadecimal - 'a' + 10);
        }
        hexadecimal++;
    }
}

LRESULT CALLBACK ChildWindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            hEdit = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 20, 60, 100, 20, hWnd, (HMENU)1, NULL, NULL);
            CreateWindowW(L"Static", L"Conversao:", WS_VISIBLE | WS_CHILD, 20, 110, 150, 20, hWnd, NULL, NULL, NULL);
            hValueLabel = CreateWindowW(L"Static", L"", WS_VISIBLE | WS_CHILD, 20, 140, 200, 20, hWnd, NULL, NULL, NULL);
            break;
        case WM_COMMAND:
            if (HIWORD(wp) == EN_CHANGE && LOWORD(wp) == 1) {
                char buffer[256];
                GetWindowText(hEdit, buffer, 256);
                char result[256];
                bool invalido = false;

                switch (calculationType) {
                    case MENU_DEC_BIN:
                    case MENU_DEC_OCT:
                    case MENU_DEC_HEX:
                        if (!validarEntradaDecimal(buffer)) {
                            invalido = true;
                        } else {
                            int inputValor = atoi(buffer);
                            switch (calculationType) {
                                case MENU_DEC_BIN:
                                    decimalParaBinario(inputValor, result);
                                    break;
                                case MENU_DEC_OCT:
                                    decimalParaOctal(inputValor, result);
                                    break;
                                case MENU_DEC_HEX:
                                    decimalParaHexa(inputValor, result);
                                    break;
                                default:
                                    strcpy(result, "Conversao nao implementada");
                            }
                        }
                        break;

                    case MENU_BIN_DEC:
                    case MENU_BIN_OCT:
                    case MENU_BIN_HEX:
                        if (!validarEntradaBinaria(buffer)) {
                            invalido = true;
                        } else {
                            int inputValor;
                            binarioParaDecimal(buffer, &inputValor);
                            switch (calculationType) {
                                case MENU_BIN_DEC:
                                    sprintf(result, "%d", inputValor);
                                    break;
                                case MENU_BIN_OCT:
                                    decimalParaOctal(inputValor, result);
                                    break;
                                case MENU_BIN_HEX:
                                    decimalParaHexa(inputValor, result);
                                    break;
                                default:
                                    strcpy(result, "Conversao nao implementada");
                            }
                        }
                        break;

                    case MENU_OCT_DEC:
                    case MENU_OCT_BIN:
                    case MENU_OCT_HEX:
                        if (!validarEntradaOctal(buffer)) {
                            invalido = true;
                        } else {
                            int inputValor;
                            octalParaDecimal(buffer, &inputValor);
                            switch (calculationType) {
                                case MENU_OCT_DEC:
                                    sprintf(result, "%d", inputValor);
                                    break;
                                case MENU_OCT_BIN:
                                    decimalParaBinario(inputValor, result);
                                    break;
                                case MENU_OCT_HEX:
                                    decimalParaHexa(inputValor, result);
                                    break;
                                default:
                                    strcpy(result, "Conversao nao implementada");
                            }
                        }
                        break;

                    case MENU_HEX_DEC:
                    case MENU_HEX_BIN:
                    case MENU_HEX_OCT:
                        if (!validarEntradaHexadecimal(buffer)) {
                            invalido = true;
                        } else {
                            int inputValor;
                            hexaParaDecimal(buffer, &inputValor);
                            switch (calculationType) {
                                case MENU_HEX_DEC:
                                    sprintf(result, "%d", inputValor);
                                    break;
                                case MENU_HEX_BIN:
                                    decimalParaBinario(inputValor, result);
                                    break;
                                case MENU_HEX_OCT:
                                    decimalParaOctal(inputValor, result);
                                    break;
                                default:
                                    strcpy(result, "Conversao nao implementada");
                            }
                        }
                        break;

                    default:
                        strcpy(result, "Conversao nao implementada");
                }

                if (invalido) {
                    strcpy(buffer, "Entrada Invalida");
                } else {
                    sprintf(buffer, "%s", result);
                }

                SetWindowText(hValueLabel, buffer);
            }
            break;
        case WM_DESTROY:
            EnableMenuItem(hMenu, MENU_SAIR, MF_ENABLED);
            hChildWindow = NULL;
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProcW(hWnd, msg, wp, lp);
    }

    return 0;
}
