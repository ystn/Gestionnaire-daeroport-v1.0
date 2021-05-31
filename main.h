#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <commctrl.h>
#include <math.h>
#include "aeroport.h"
#include "resource.h"

#define NEW_FILE 1
#define OPEN_FILE 2
#define SAVE_FILE 3
#define SAVE_AS_FILE 4
#define CLOSE_FILE 5
#define PRINT_FILE 6
#define EXIT_APPLICATION 7
#define ABOUT_APPLICATION 8
#define MENU_AIRPLANE_NEW_HEAD 11
#define MENU_AIRPLANE_NEW_TAIL 12
#define MENU_AIRPLANE_NEW_POS 13
#define MENU_AIRPLANE_DELETE_HEAD 14
#define MENU_AIRPLANE_DELETE_TAIL 15
#define MENU_AIRPLANE_DELETE_POS 16
#define MENU_AIRPLANE_MOD_ID 17
#define MENU_AIRPLANE_MOD_COMPAGNIE 18
#define SEARCH_ID 20
#define SEARCH_COMPAGNIE 21
#define SEARCH_TAKEOFF_HOUR 22
#define ORDER_LIST 23
#define MENU_COMPAGNIE_NEW 24
#define MENU_COMPAGNIE_DELETE 25
#define SHOW_ALL 26
#define BUTTON_DIALOG_OK 1
#define BUTTON_DIALOG_CANCEL 2

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
bool CALLBACK SetFont(HWND, LPARAM);
LRESULT CALLBACK CreatePlaneTeteDialogueProcedure(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK CreatePlaneQueueDialogueProcedure(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK CreatePlanePosDialogueProcedure(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DeletePlanePosDialogueProcedure(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ResearchPlaneIdDialogueProcedure(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ResearchPlaneCompagnieDialogueProcedure(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ResearchPlaneHeureDialogueProcedure(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ModifyPlaneIdDialogueProcedure(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ModifyPlaneCompagnieDialogueProcedure(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK AddCompagnieDialogueProcedure(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DeleteCompagnieDialogueProcedure(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ViewProcedure(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK CopyrightProcedure(HWND,UINT,WPARAM,LPARAM);
HWND CreateAVerticalScrollBar(HWND, int);
void AddMenus(HWND);
void AddControls(HWND);
HMENU AddFileMenu();
void hUnsavedChanges(HWND);
void hOpenFile(HWND);
void hSaveFile(HWND);
void hAddPlane(HWND);
void registerDialogClass(HINSTANCE,LPCSTR,LRESULT CALLBACK(*)(HWND, UINT, WPARAM, LPARAM));
void registerCopyrightClass(HINSTANCE);
void registerView(HINSTANCE);
void displayDialog(HWND, LPCSTR, LPCSTR, LPCSTR, int, int);
void displayCopyright(HWND);
void hDisplayComponent(HWND ,Liste *, Cellule *, unsigned);
void hRechercheHeure(HWND, Liste *, Cellule *, unsigned);
void hRechercheComp(HWND, Liste *, Cellule *, unsigned);
void hShowAll(HWND, Liste *, Cellule *, unsigned);
int windowHeight(HWND);
void CopieListe();
void refreshView();
void hComboBoxItems(HWND);
void CustomHandleVScroll(HWND,int);
void CustomHandleKeyDown(HWND,UINT);
#endif // MAIN_H_INCLUDED
