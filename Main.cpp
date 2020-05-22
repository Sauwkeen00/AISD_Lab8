#include "Node.h"
#include "BinaryTree.h"
#include "resource.h"
#include <windows.h>
#include <string>


//-------- инициализируем кнопки 
#define idButton1 1
#define idButton2 2
#define idButton3 3
#define idButton4 4
#define idButton5 5
#define idButton6 6


using namespace std;

HWND hWinRect;
HINSTANCE hWinInstance; // дискриптор окна

unsigned button = 0; // присваиваем нажатию 0 

BinaryTree *tree = new BinaryTree(); 

// инициализуерм функции для обработки нажатий кнопки
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ButtonClick1(HWND);
void ButtonClick2(HWND);
void ButtonClick3(HWND);
void ButtonClick4(HWND);
void ButtonClick5(HWND);
//void ButtonClick6(HWND);

// заполняем структуру окна
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hMainWnd;
	MSG msg;
	WNDCLASSEX wc; // пользовательская структура

	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"Form";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) // регистрируем окно
	{
		MessageBox(NULL, L"Cannot register class", L"Error", MB_OK);
		return 0;
	}

	// ------ создаем окно и задаем его название
	hMainWnd = CreateWindow(L"Form", L"BinaryTree", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0, (HWND)NULL, (HMENU)NULL, (HINSTANCE)hInstance, NULL);

	// создаем кнопки с названием (строка 62-75)
	CreateWindow(L"BUTTON", L"Добавить", WS_CHILD | WS_VISIBLE,
		10, 10, 120, 30, hMainWnd, (HMENU)idButton1, hInstance, NULL);

	CreateWindow(L"BUTTON", L"Удалить", WS_CHILD | WS_VISIBLE,
		10, 50, 120, 30, hMainWnd, (HMENU)idButton2, hInstance, NULL);

	CreateWindow(L"BUTTON", L"Глубина", WS_CHILD | WS_VISIBLE,
		10, 90, 120, 30, hMainWnd, (HMENU)idButton3, hInstance, NULL);

	CreateWindow(L"BUTTON", L"Высота", WS_CHILD | WS_VISIBLE,
		10, 130, 120, 30, hMainWnd, (HMENU)idButton4, hInstance, NULL);

	CreateWindow(L"BUTTON", L"Задание", WS_CHILD | WS_VISIBLE,
		10, 170, 120, 30, hMainWnd, (HMENU)idButton5, hInstance, NULL);
	
	//CreateWindow(L"BUTTON", L"Поиск", WS_CHILD | WS_VISIBLE,
	//	10, 219, 120, 30, hMainWnd, (HMENU)idButton6, hInstance, NULL);

	hWinRect = hMainWnd; // дискриптор окна
	hWinInstance = hInstance;

	// обработчик ошибки, если окно не удалось создать 
	if (!hMainWnd) 
	{
		MessageBox(NULL, L"Cannot create main window", L"Error", MB_OK);
		return 0;
	}

	ShowWindow(hMainWnd, nCmdShow); // показываем окно пользователю

	while (GetMessage(&msg, NULL, 0, 0)) // показывать сообщения пользователю
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
// обрабатываем результаты работы с программой клиентом
{
	HDC hDC;
	PAINTSTRUCT ps;
	RECT rect;

	switch (uMsg) 
	{
		case WM_COMMAND: // привязываем к кнопкам конкретные функциии 
			if (HIWORD(wParam) == BN_CLICKED)
				switch (LOWORD(wParam))
				{
					case idButton1:
						ButtonClick1(hWnd);
						break;
					case idButton2:
						ButtonClick2(hWnd);
						break;
					case idButton3:
						ButtonClick3(hWnd);
						break;
					case idButton4:
						ButtonClick4(hWnd);
						break;
					case idButton5:
						ButtonClick5(hWnd);
						break;
				}
			break;

		case WM_PAINT: // кейс для рисования дерева
			hDC = BeginPaint(hWnd, &ps); // начало отрисовки

			GetClientRect(hWnd, &rect); // рисуем в клиентской области
			FillRect(hDC, &rect, (HBRUSH)(COLOR_WINDOW + 1));
	
			if (tree->getRoot() != NULL)
				tree->orderDraw(tree->getRoot(), 0, hDC, rect);

			EndPaint(hWnd, &ps); // окончили рисовать и вышли
			break;

		case WM_CLOSE: // кейс закрыть окно, вместе с тем: удалить дерево, освободить дискриптор и выйти 
			delete tree; 
			DestroyWindow(hWnd);
			break;
		case WM_DESTROY: // аварийное завершение 
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

INT_PTR CALLBACK MyDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) // создаем диалоговое окно
{
	switch (iMsg)
	{
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDC_BTN_OK:
					wchar_t value[50] = { 0 };
					GetDlgItemText(hDlg, IDC_EDIT, value, 50);
					if (!wcslen(value)) // выдавать сообщение если ничего не введено
					{
						MessageBox(hDlg, L"Введите значение узла!", L"Поле ввода пусто", MB_ICONASTERISK);
					}
					else
					{
						switch (button) 
						{
							case 1: // привязываем к первой кнопке функцию добавления узла
								tree->addNode(tree->getRoot(), NULL, _wtof(value));
								tree->orderLCR(tree->getRoot(), 0); // очищаем то, что нарисовано
								InvalidateRect(hWinRect, 0, 0); // перерисовываем
								break;
							case 2:
							{
								if (tree->getRoot() == NULL) // удаления элемента 
									break;
								tree->removeNode(tree->getRoot(), _wtof(value));
								tree->orderLCR(tree->getRoot(), 0);
								InvalidateRect(hWinRect, 0, 0);
								break;
							}
							case 3: 
							{
								if (tree->getRoot() == NULL) // определение глубины узла
									break;

								int depth = tree->getNodeDepth(tree->search(tree->getRoot(), _wtof(value)));
								MessageBox(NULL, to_wstring(depth).c_str(), L"Глубина", NULL);
								break;
							}
							case 4:
							{
								if (tree->getRoot() == NULL) // определение высоты узла
									break;

								int height = tree->getNodeHeight(tree->search(tree->getRoot(), _wtof(value)));
								MessageBox(NULL, to_wstring(height).c_str(), L"Высота", NULL);
								break;
							}
						}

						EndDialog(hDlg, TRUE);
					}
			}
			break;

		case WM_CLOSE: // закрытие
			EndDialog(hDlg, FALSE);
			break;

		case WM_DESTROY: // разрушение
			DestroyWindow(hDlg);
			break;

		default:
			return FALSE;
	}

	return TRUE;
}

// создание функций для кнопок (строка 229-269)
void ButtonClick1(HWND hWnd)
{
	button = 1;
	CreateDialog(hWinInstance, MAKEINTRESOURCE(DLG_NAME), hWnd, MyDlgProc);
}

void ButtonClick2(HWND hWnd)
{
	button = 2;
	CreateDialog(hWinInstance, MAKEINTRESOURCE(DLG_NAME), hWnd, MyDlgProc);
}

void ButtonClick3(HWND hWnd)
{
	button = 3;
	CreateDialog(hWinInstance, MAKEINTRESOURCE(DLG_NAME), hWnd, MyDlgProc);
}

void ButtonClick4(HWND hWnd)
{
	button = 4;
	CreateDialog(hWinInstance, MAKEINTRESOURCE(DLG_NAME), hWnd, MyDlgProc);
}

void ButtonClick5(HWND hWnd)
{
	if (tree->getRoot() == NULL)
		return;

	Node *v;
	if ((v = tree->getMiddleNodeOnLevel(tree->getRoot())) != NULL)
	{
		MessageBox(NULL, (L"Будет удалена вершина с ключом: " +
			to_wstring(v->getValue())).c_str(), L"Внимание", NULL);
		tree->removeNode(tree->getRoot(), v->getValue());
		tree->orderLCR(tree->getRoot(), 0);
		InvalidateRect(hWinRect, 0, 0);
	}
	else
		MessageBox(NULL, L"Не найдено!", L"Внимание", NULL);
}