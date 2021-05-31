#include "main.h"

HINSTANCE hInst;
HWND hMainWindow, hNum, hCarb, hCons, hHeure1, hHeure2, hAcr, hPos, hId, hTitle, hComboAcr, hView, hComboBox;
TCHAR szClassName[ ] = _T("WindowsApp");
char openedFile[200], globalHour[5], globalAcr[4], globalId[7];
HMENU hMenu;
Liste *liste, *searchResult;
ListeCompagnie *listeCompagnie;
int shownListHeight, posY;
bool unsavedChanges;

int WINAPI WinMain (HINSTANCE hThisInstance,HINSTANCE hPrevInstance,LPSTR lpszArgument,int nCmdShow)
{
    MSG messages;
    WNDCLASSEX wincl;
    hInst = hThisInstance;
    liste = creer_liste();
    listeCompagnie = creer_liste_comp();
    searchResult = creer_liste();
    unsavedChanges = false;

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);

    wincl.hIcon = LoadIcon (hThisInstance, MAKEINTRESOURCE(IDI_ICON));
    wincl.hIconSm = LoadIcon (hThisInstance, MAKEINTRESOURCE(IDI_ICON));
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH) COLOR_WINDOW;

    if (!RegisterClassEx (&wincl))
        return 0;

    registerDialogClass(hThisInstance,"CreatePlane",CreatePlaneTeteDialogueProcedure);
    registerDialogClass(hThisInstance,"CreatePlaneQueue",CreatePlaneTeteDialogueProcedure);
    registerDialogClass(hThisInstance,"CreatePlanePos",CreatePlanePosDialogueProcedure);
    registerDialogClass(hThisInstance,"DeletePlanePos",DeletePlanePosDialogueProcedure);
    registerDialogClass(hThisInstance,"ResearchPlaneId",ResearchPlaneIdDialogueProcedure);
    registerDialogClass(hThisInstance,"ResearchPlaneCompagnie",ResearchPlaneCompagnieDialogueProcedure);
    registerDialogClass(hThisInstance,"ResearchPlaneHour",ResearchPlaneHeureDialogueProcedure);
    registerDialogClass(hThisInstance,"ModifyPlaneId",ModifyPlaneIdDialogueProcedure);
    registerDialogClass(hThisInstance,"ModifyPlaneCompagnie",ModifyPlaneCompagnieDialogueProcedure);
    registerDialogClass(hThisInstance,"AddCompagnie",AddCompagnieDialogueProcedure);
    registerDialogClass(hThisInstance,"DeleteCompagnie",DeleteCompagnieDialogueProcedure);
    registerView(hThisInstance);
    registerCopyrightClass(hThisInstance);

    hMainWindow = CreateWindowEx (0,szClassName,_T("Gestion d'aeroport"),WS_OVERLAPPEDWINDOW|WS_VSCROLL,CW_USEDEFAULT,CW_USEDEFAULT,544,375,HWND_DESKTOP,NULL,hThisInstance,NULL);

    EnumChildWindows(hMainWindow, (WNDENUMPROC)SetFont, (LPARAM)GetStockObject(DEFAULT_GUI_FONT));

    ShowWindow (hMainWindow, nCmdShow);

    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return messages.wParam;
}

LRESULT CALLBACK WindowProcedure (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int val;
    switch (message)
    {
        case WM_SIZE:
        {
            SCROLLINFO si;
            si.cbSize = sizeof(SCROLLINFO);
            si.fMask = SIF_PAGE;
            GetScrollInfo(hWnd,SB_VERT,&si);
            si.nPage = windowHeight(hWnd);
            SetScrollInfo(hWnd,SB_VERT,&si,TRUE);
        }
            break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EnumChildWindows(hMainWindow, (WNDENUMPROC)SetFont, (LPARAM)GetStockObject(DEFAULT_GUI_FONT));
            EndPaint(hWnd, &ps);
        }
        case WM_COMMAND:
            switch (wParam)
            {
                case NEW_FILE:
                    hUnsavedChanges(hWnd);
                    strcpy(openedFile,"");
                    detruire(liste);
                    detruire_comp(listeCompagnie);
                    detruire(searchResult);
                    refreshView();
                    break;
                case OPEN_FILE:
                    hOpenFile(hWnd);
                    break;
                case SAVE_FILE:
                    if(!strcmp(openedFile,""))
                        hSaveFile(hWnd);
                    else
                        enregistrer(openedFile,liste,listeCompagnie);
                    break;
                case SAVE_AS_FILE:
                    hSaveFile(hWnd);
                    break;
                case CLOSE_FILE:
                    hUnsavedChanges(hWnd);
                    strcpy(openedFile,"");
                    detruire(liste);
                    detruire_comp(listeCompagnie);
                    detruire(searchResult);
                    refreshView();
                    break;
                case EXIT_APPLICATION:
                    hUnsavedChanges(hWnd);
                    DestroyWindow(hWnd);
                    break;
                case ABOUT_APPLICATION:
                    displayCopyright(hWnd);
                    ShowWindow(hWnd,SW_NORMAL);
                    break;
                case MENU_AIRPLANE_NEW_HEAD:
                    displayDialog(hWnd,"CreatePlane","Ajouter un avion en tête","Enregistrer",375,290);
                    ShowWindow(hWnd,SW_NORMAL);
                    break;
                case MENU_AIRPLANE_NEW_TAIL:
                    displayDialog(hWnd,"CreatePlaneQueue","Ajouter un avion en queue","Enregistrer",375,290);
                    ShowWindow(hWnd,SW_NORMAL);
                    break;
                case MENU_AIRPLANE_NEW_POS:
                    displayDialog(hWnd,"CreatePlanePos","Ajouter un avion à un position","Enregistrer",375,320);
                    ShowWindow(hWnd,SW_NORMAL);
                    break;
                case MENU_AIRPLANE_DELETE_HEAD:
                    val = MessageBoxEx(hWnd,"Etes-vous sûr de supprimer un avion en tête","Supprimer",MB_YESNO,0);
                    if(val == IDYES)
                    {
                        supprimer_tete_av(liste);
                        unsavedChanges = true;
                        detruire(searchResult);
                        CopieListe();
                        refreshView();
                    }
                    break;
                case MENU_AIRPLANE_DELETE_TAIL:
                    val = MessageBoxEx(hWnd,"Etes-vous sûr de supprimer un avion en queue","Supprimer",MB_YESNO,0);
                    if(val == IDYES)
                    {
                        supprimer_queue_av(liste);
                        unsavedChanges = true;
                        detruire(searchResult);
                        CopieListe();
                        refreshView();
                    }
                    break;
                case MENU_AIRPLANE_DELETE_POS:
                    displayDialog(hWnd,"DeletePlanePos","Supprimer un avion à un position","Supprimer",350,180);
                    ShowWindow(hWnd,SW_NORMAL);
                    break;
                case MENU_AIRPLANE_MOD_ID:
                    displayDialog(hWnd,"ModifyPlaneId","Modifier l'identifiant d'un avion","Modifier",350,200);
                    ShowWindow(hWnd,SW_NORMAL);
                    break;
                case MENU_AIRPLANE_MOD_COMPAGNIE:
                    displayDialog(hWnd,"ModifyPlaneCompagnie","Modifier la compagnie d'un avion","Modifier",350,200);
                    ShowWindow(hWnd,SW_NORMAL);
                    break;
                case SHOW_ALL:
                    CopieListe();
                    refreshView();
                    break;
                case SEARCH_ID:
                    displayDialog(hWnd,"ResearchPlaneId","Recherche un avion par identifiant","Rechercher",350,180);
                    ShowWindow(hWnd,SW_NORMAL);
                    break;
                case SEARCH_COMPAGNIE:
                    displayDialog(hWnd,"ResearchPlaneCompagnie","Recherche un avion par compagnie","Rechercher",350,180);
                    ShowWindow(hWnd,SW_NORMAL);
                    break;
                case SEARCH_TAKEOFF_HOUR:
                    displayDialog(hWnd,"ResearchPlaneHour","Recherche un avion par heure de decollage","Rechercher",350,180);
                    ShowWindow(hWnd,SW_NORMAL);
                    break;
                case ORDER_LIST:
                    trierListe(liste);
                    CopieListe();
                    refreshView();
                    break;
                case MENU_COMPAGNIE_NEW:
                    displayDialog(hWnd,"AddCompagnie","Ajouter compagnie","Ajouter",350,200);
                    ShowWindow(hWnd,SW_NORMAL);
                    break;
                case MENU_COMPAGNIE_DELETE:
                    displayDialog(hWnd,"DeleteCompagnie","Supprimer compagnie","Supprimer",350,180);
                    ShowWindow(hWnd,SW_NORMAL);
                    break;
            }
            break;
        case WM_VSCROLL:
            CustomHandleVScroll(hWnd,wParam);
            break;
        case WM_KEYDOWN:
            CustomHandleKeyDown(hWnd, wParam);
            break;
        case WM_CREATE:
        {
            AddMenus(hWnd);
            AddControls(hWnd);
            SCROLLINFO si;
            si.cbSize = sizeof(SCROLLINFO);
            si.nMin = 0;
            si.nPage = windowHeight(hWnd);
            si.nPos = 0;
            si.fMask = SIF_ALL;
            SetScrollInfo(hWnd,SB_VERT,&si,TRUE);
        }
            break;
        case WM_DESTROY:
            PostQuitMessage (0);
            break;
        default:
            return DefWindowProc (hWnd, message, wParam, lParam);
    }

    return 0;
}

bool CALLBACK SetFont(HWND child, LPARAM font)
{
    SendMessage(child, WM_SETFONT, font, true);
    return true;
}

HMENU AddFileMenu()
{
    HMENU hFile = CreateMenu();
    AppendMenu(hFile,MF_STRING,NEW_FILE,"&Nouveau fichier");
    AppendMenu(hFile,MF_STRING,OPEN_FILE,"&Ouvrir...");
    AppendMenu(hFile,MF_SEPARATOR,(UINT_PTR)NULL,NULL);
    AppendMenu(hFile,MF_STRING,SAVE_FILE,"Enregistrer");
    AppendMenu(hFile,MF_STRING,SAVE_AS_FILE,"&Enregistrer sous...");
    AppendMenu(hFile,MF_SEPARATOR,(UINT_PTR)NULL,NULL);
    AppendMenu(hFile,MF_STRING,CLOSE_FILE,"&Fermer le fichier");
    AppendMenu(hFile,MF_SEPARATOR,(UINT_PTR)NULL,NULL);
    AppendMenu(hFile,MF_STRING,EXIT_APPLICATION,"&Quitter");
    return hFile;
}

HMENU AddEditMenu()
{
    HMENU hEdit = CreateMenu();
    AppendMenu(hEdit,MF_STRING,MENU_AIRPLANE_NEW_HEAD,"Ajouter un avion en tête");
    AppendMenu(hEdit,MF_STRING,MENU_AIRPLANE_NEW_TAIL,"Ajouter un avion en queue");
    AppendMenu(hEdit,MF_STRING,MENU_AIRPLANE_NEW_POS,"Ajouter un avion dans un position");
    AppendMenu(hEdit,MF_SEPARATOR,(UINT_PTR)NULL,NULL);
    AppendMenu(hEdit,MF_STRING,MENU_AIRPLANE_DELETE_HEAD,"Supprimer un avion en tête");
    AppendMenu(hEdit,MF_STRING,MENU_AIRPLANE_DELETE_TAIL,"Supprimer un avion en queue");
    AppendMenu(hEdit,MF_STRING,MENU_AIRPLANE_DELETE_POS,"Supprimer un avion dans un position");
    AppendMenu(hEdit,MF_SEPARATOR,(UINT_PTR)NULL,NULL);
    AppendMenu(hEdit,MF_STRING,MENU_AIRPLANE_MOD_ID,"Modifier l'identifiant d'un avion");
    AppendMenu(hEdit,MF_STRING,MENU_AIRPLANE_MOD_COMPAGNIE,"Modifier la compagnie d'un avion");
    AppendMenu(hEdit,MF_SEPARATOR,(UINT_PTR)NULL,NULL);
    AppendMenu(hEdit,MF_STRING,SHOW_ALL,"Affiche tout");
    AppendMenu(hEdit,MF_STRING,SEARCH_ID,"Recherche par identifiant avion");
    AppendMenu(hEdit,MF_STRING,SEARCH_COMPAGNIE,"Recherche par compagnie");
    AppendMenu(hEdit,MF_STRING,SEARCH_TAKEOFF_HOUR,"Recherche par heure de décollage");
    AppendMenu(hEdit,MF_SEPARATOR,(UINT_PTR)NULL,NULL);
    AppendMenu(hEdit,MF_STRING,ORDER_LIST,"Tri");
    AppendMenu(hEdit,MF_SEPARATOR,(UINT_PTR)NULL,NULL);
    AppendMenu(hEdit,MF_STRING,MENU_COMPAGNIE_NEW,"Ajouter une compagnie");
    AppendMenu(hEdit,MF_STRING,MENU_COMPAGNIE_DELETE,"Supprimer une compagnie");
    return hEdit;
}

HMENU AddHelpMenu()
{
    HMENU hHelp = CreateMenu();
    AppendMenu(hHelp,MF_STRING,ABOUT_APPLICATION,"&A propos...");
    return hHelp;
}

void AddMenus(HWND hwnd)
{
    hMenu = CreateMenu();
    HMENU hFileMenu = AddFileMenu();
    HMENU hEditMenu = AddEditMenu();
    HMENU hHelpMenu = AddHelpMenu();

    // Menu items
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hFileMenu,"&Fichier");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hEditMenu,"&Edit");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hHelpMenu,"&Aide");

    // Rendering the menu
    SetMenu(hwnd,hMenu);
}

void AddControls(HWND hWnd)
{
    CopieListe();
    hView = CreateWindowEx(0,"ViewClass",NULL,WS_CHILDWINDOW|WS_VISIBLE,0,0,450,3000,hWnd,NULL,NULL,NULL);
}



void hUnsavedChanges(HWND hWnd)
{
    if(!liste_vide(liste) && (unsavedChanges || !strcmp(openedFile,"")))
    {
        int val = MessageBox(hWnd,"Est-ce que tu veux enregistrer les modifications","Enregistrer",MB_YESNO);
        if(val==IDYES)
        {
            if(!strcmp(openedFile,""))
                hSaveFile(hWnd);
            else
            {
                enregistrer(openedFile,liste,listeCompagnie);
                unsavedChanges = false;
            }
        }
    }
}

void hOpenFile(HWND hWnd)
{
    hUnsavedChanges(hWnd);
    OPENFILENAME ofn;

    char fileName[200];

    ZeroMemory(&ofn,sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = fileName;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 200;
    ofn.lpstrFilter = "Text files\0*.txt\0Save files\0*.sav\0All files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;

    if(GetOpenFileName(&ofn))
    {
        strncpy(openedFile,ofn.lpstrFile,200);
        detruire(liste);
        detruire_comp(listeCompagnie);
        charger(openedFile,liste,listeCompagnie);
        CopieListe();
        refreshView();
    }
}

void hSaveFile(HWND hWnd)
{
    OPENFILENAME ofn;

    char fileName[100];

    ZeroMemory(&ofn,sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = fileName;
    strcpy(ofn.lpstrFile,"Untitled.txt");
    ofn.nMaxFile = 100;
    ofn.lpstrFilter = "Text files\0*.txt\0Save files\0*.sav\0All files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;

    if(GetSaveFileName(&ofn))
    {
        strncpy(openedFile,ofn.lpstrFile,200);
        enregistrer(openedFile,liste,listeCompagnie);
        unsavedChanges = false;
    }
}

bool hValidNum(char *n)
{
    bool size = strcmp(n,"") && strlen(n)<4;
    bool type = true;
    for(int i = 0; type && i<strlen(n); i++)
    {
        if(!isdigit(n[i]))
            type = false;
    }
    return (size && type);
}

bool hValidAcr(char *n)
{
    bool size = strcmp(n,"") && strlen(n)==3;
    bool type = true;
    for(int i = 0; type && i<strlen(n); i++)
    {
        if(!isupper(n[i]))
            type = false;
    }
    return size && type;
}

bool hValidAcrExist(char *n)
{
    bool exist = false;
    if(recherche_acr_comp(listeCompagnie,n)) exist = true;
    return exist;
}

bool hValidHeure(char *h1, char *h2)
{
    bool length = strlen(h1)<3 && strlen(h2)<3;
    bool limits = atoi(h1)>=0 && atoi(h1)<=23 && atoi(h2)>=0 && atoi(h2)<=59;
    bool size = length && limits;
    bool type = true;
    for(int i = 0; type && i<strlen(h1); i++)
    {
        if(!isdigit(h1[i]))
            type = false;
    }
    for(int i = 0; type && i<strlen(h2); i++)
    {
        if(!isdigit(h2[i]))
            type = false;
    }
    return(size && type);
}

bool hValidId(char *id)
{
    bool length = strcmp(id,"") && strlen(id)==6;
    bool type = true;
    for(int i=0;type && i<3;i++)
    {
        if(!isalpha(id[i]))
            type = false;
    }
    for(int i=3;type && i<6;i++)
    {
        if(!isdigit(id[i]))
            type = false;
    }
    return (length && type);
}

char *hHeureInput(char *h1, char *h2)
{
    char *heure = malloc(5*sizeof(char));
    if(strlen(h1)==1)
    {
        strncpy(heure,"0",1);
        strncpy(heure+1,h1,1);
    }
    else
        strncpy(heure,h1,2);
    if(strlen(h1)==1)
    {
        strncpy(heure+2,"0",1);
        strncpy(heure+3,h2,2);
    }
    else
        strncpy(heure+2,h2,3);
    return heure;
}

Avion *hCreatePlane(HWND hWnd)
{
    unsigned numero;
    int carburant, consommation;
    char *heure, acr[5], n[5],ca[5],co[5], h1[4], h2[4];

    GetWindowText(hNum,n,5);
    GetWindowText(hCarb,ca,5);
    GetWindowText(hCons,co,5);
    GetWindowText(hAcr,acr,5);
    GetWindowText(hHeure1,h1,4);
    GetWindowText(hHeure2,h2,4);

    numero = atoi(n);
    carburant = atoi(ca);
    consommation = atoi(co);
    heure = hHeureInput(h1,h2);

    if(!hValidNum(n))
        MessageBoxEx(hWnd,"Numero incorrect","Error",MB_ICONERROR|MB_OK,0);
    else if(!strcmp(ca,""))
        MessageBoxEx(hWnd,"Carburant incorrect","Error",MB_ICONERROR|MB_OK,0);
    else if(!strcmp(co,""))
        MessageBoxEx(hWnd,"Consommation incorrect","Error",MB_ICONERROR|MB_OK,0);
    else if(!hValidHeure(h1,h2))
        MessageBoxEx(hWnd,"Heure incorrect","Error",MB_ICONERROR|MB_OK,0);
    else if(!hValidAcr(acr))
        MessageBoxEx(hWnd,"Acronyme incorrect","Error",MB_ICONERROR|MB_OK,0);
    else if(!hValidAcrExist(acr))
        MessageBoxEx(hWnd,"Compagnie n'existe pas","Error",MB_ICONERROR|MB_OK,0);
    else
    {
        Avion *av = creer_avion(listeCompagnie,numero,carburant,consommation,heure,acr);
        SetWindowText(hPos,"");
        SetWindowText(hNum,"");
        SetWindowText(hCarb,"");
        SetWindowText(hCons,"");
        SetWindowText(hHeure1,"");
        SetWindowText(hHeure2,"");
        SetWindowText(hAcr,"");
        unsavedChanges = true;
        return av;
    }
    return NULL;
}

void hInsererTete(HWND hWnd)
{
    Avion *av = hCreatePlane(hWnd);
    if(av)
    {
        insere_tete_av(liste,av);
        CopieListe();
        refreshView();
    }
}

void hInsererQueue(HWND hWnd)
{
    Avion *av = hCreatePlane(hWnd);
    if(av)
    {
        insere_queue_av(liste,av);
        CopieListe();
        refreshView();
    }
}

void hInsererPos(HWND hWnd, unsigned pos)
{
    Avion *av = hCreatePlane(hWnd);
    if(av)
    {
        insere_pos_av(liste,av,pos);
        CopieListe();
        refreshView();
    }
}


void hSupprimerPos(HWND hWnd,unsigned pos)
{
    suppression_pos_av(liste,pos-1);
    unsavedChanges = true;
    CopieListe();
    refreshView();
}

void hClose(HWND hWnd)
{
    int val = MessageBoxEx(hWnd,"Est-ce que vous êtes sûr d'annuler l'opération?","Annuler",MB_YESNO|MB_ICONEXCLAMATION,0);
    if(val==IDYES)
    {
        EnableWindow(hMainWindow,true);
        DestroyWindow(hWnd);
    }
}

LRESULT CALLBACK ViewProcedure(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
    switch(message)
    {
        case WM_CREATE:
            CreateWindowEx(0,"Static","Identifiant",WS_VISIBLE|WS_CHILD,25,5,70,13,hWnd,NULL,NULL,NULL);
            CreateWindowEx(0,"Static","Carburant",WS_VISIBLE|WS_CHILD,85,5,50,13,hWnd,NULL,NULL,NULL);
            CreateWindowEx(0,"Static","Consom.",WS_VISIBLE|WS_CHILD,140,5,50,13,hWnd,NULL,NULL,NULL);
            CreateWindowEx(0,"Static","Heure",WS_VISIBLE|WS_CHILD,195,5,50,13,hWnd,NULL,NULL,NULL);
            CreateWindowEx(0,"Static","Compagnie",WS_VISIBLE|WS_CHILD,255,5,100,13,hWnd,NULL,NULL,NULL);
            CreateWindowEx(0,"Static","Acronyme",WS_VISIBLE|WS_CHILD,360,5,50,13,hWnd,NULL,NULL,NULL);
            parcourir(hWnd,searchResult,hDisplayComponent);
            break;
        default:
            return DefWindowProc (hWnd, message, wParam, lParam);
    }
}

void registerView(HINSTANCE hInst)
{
    WNDCLASSEX view;

    view.lpszClassName = "ViewClass";
    view.hInstance = hInst;
    view.lpfnWndProc = ViewProcedure;
    view.style = CS_DBLCLKS;
    view.cbSize = sizeof (WNDCLASSEX);

    view.hIcon = LoadIcon (hInst, MAKEINTRESOURCE(IDI_ICON));
    view.hIconSm = LoadIcon (hInst, MAKEINTRESOURCE(IDI_ICON));
    view.hCursor = LoadCursor (NULL, IDC_ARROW);
    view.lpszMenuName = NULL;
    view.cbClsExtra = 0;
    view.cbWndExtra = 0;
    view.hbrBackground = (HBRUSH) COLOR_WINDOW;

    RegisterClassEx(&view);
}

void registerDialogClass(HINSTANCE hInst, LPCSTR className, LRESULT CALLBACK (*DialogueProcedure)(HWND,UINT,WPARAM,LPARAM))
{
    WNDCLASSEX dialog;

    dialog.lpszClassName = className;
    dialog.hInstance = hInst;
    dialog.lpfnWndProc = DialogueProcedure;
    dialog.style = CS_DBLCLKS;
    dialog.cbSize = sizeof (WNDCLASSEX);

    dialog.hIcon = LoadIcon (hInst, MAKEINTRESOURCE(IDI_ICON));
    dialog.hIconSm = LoadIcon (hInst, MAKEINTRESOURCE(IDI_ICON));
    dialog.hCursor = LoadCursor (NULL, IDC_ARROW);
    dialog.lpszMenuName = NULL;
    dialog.cbClsExtra = 0;
    dialog.cbWndExtra = 0;
    dialog.hbrBackground = (HBRUSH) COLOR_WINDOW;

    RegisterClassEx(&dialog);
}

void displayDialog(HWND hWnd, LPCSTR className, LPCSTR title, LPCSTR button, int xsize, int ysize)
{
    HWND hDlg = CreateWindowEx(0,className,title,WS_VISIBLE | WS_OVERLAPPED | WS_SYSMENU | DS_CENTER, CW_USEDEFAULT, CW_USEDEFAULT, xsize, ysize, hWnd,NULL,NULL,NULL);
    CreateWindowEx(0,"Button",button,WS_VISIBLE|WS_CHILD|BS_FLAT,50,ysize-80,100,30,hDlg,(HMENU)BUTTON_DIALOG_OK,NULL,NULL);
    CreateWindowEx(0,"Button","Annuler",WS_VISIBLE|WS_CHILD|BS_FLAT,175,ysize-80,100,30,hDlg,(HMENU)BUTTON_DIALOG_CANCEL,NULL,NULL);
    EnumChildWindows(hDlg, (WNDENUMPROC)SetFont, (LPARAM)GetStockObject(DEFAULT_GUI_FONT));
    EnableWindow(hMainWindow,false);
}

void AddPlaneControls(HWND hWnd, int column, int xoffset, int yoffset)
{

    CreateWindowEx(0,"Static","Numero de vol:",WS_VISIBLE|WS_CHILD,xoffset,yoffset,170,20,hWnd,NULL,NULL,NULL);
    hNum = CreateWindowEx(0,"Edit","",WS_VISIBLE|WS_CHILD|WS_BORDER,xoffset+column+5,yoffset,30,20,hWnd,NULL,NULL,NULL);
    CreateWindowEx(0,"Static","Carburant:",WS_VISIBLE|WS_CHILD,xoffset,yoffset+30,170,20,hWnd,NULL,NULL,NULL);
    hCarb = CreateWindowEx(0,"Edit","",WS_VISIBLE|WS_CHILD|WS_BORDER,xoffset+column+5,yoffset+30,100,20,hWnd,NULL,NULL,NULL);
    CreateWindowEx(0,"Static","Consommation:",WS_VISIBLE|WS_CHILD,xoffset,yoffset+60,170,20,hWnd,NULL,NULL,NULL);
    hCons = CreateWindowEx(0,"Edit","",WS_VISIBLE|WS_CHILD|WS_BORDER,xoffset+column+5,yoffset+60,100,20,hWnd,NULL,NULL,NULL);
    CreateWindowEx(0,"Static","Heure de décollage:",WS_VISIBLE|WS_CHILD,xoffset,yoffset+90,170,20,hWnd,NULL,NULL,NULL);
    hHeure1 = CreateWindowEx(0,"Edit","",WS_VISIBLE|WS_CHILD|WS_BORDER,xoffset+column+5,yoffset+90,20,20,hWnd,NULL,NULL,NULL);
    CreateWindowEx(0,"Static",":",WS_VISIBLE|WS_CHILD,xoffset+column+27,yoffset+90,3,20,hWnd,NULL,NULL,NULL);
    hHeure2 = CreateWindowEx(0,"Edit","",WS_VISIBLE|WS_CHILD|WS_BORDER,xoffset+column+33,yoffset+90,20,20,hWnd,NULL,NULL,NULL);
    CreateWindowEx(0,"Static","Acronyme de compagnie:",WS_VISIBLE|WS_CHILD,xoffset,yoffset+120,170,20,hWnd,NULL,NULL,NULL);
    hAcr = CreateWindowEx(0,"Edit","",WS_VISIBLE|WS_CHILD|WS_BORDER,xoffset+column+5,yoffset+120,48,20,hWnd,NULL,NULL,NULL);
}

LRESULT CALLBACK CreatePlaneTeteDialogueProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_COMMAND:
            switch(wParam)
            {
            case BUTTON_DIALOG_OK:
                hInsererTete(hWnd);
                break;
            case BUTTON_DIALOG_CANCEL:
                hClose(hWnd);
                break;
            }
            break;
        case WM_CREATE:
            AddPlaneControls(hWnd,170,50,50);

            break;
        case WM_DESTROY:
            EnableWindow(hMainWindow,true);
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc (hWnd, message, wParam, lParam);
    }
}

LRESULT CALLBACK CreatePlaneQueueDialogueProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_COMMAND:
            switch(wParam)
            {
            case BUTTON_DIALOG_OK:
                hInsererQueue(hWnd);
                break;
            case BUTTON_DIALOG_CANCEL:
                hClose(hWnd);
                break;
            }
            break;
        case WM_CREATE:
            AddPlaneControls(hWnd,170,50,50);
            break;
        case WM_DESTROY:
            EnableWindow(hMainWindow,true);
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc (hWnd, message, wParam, lParam);
    }
}

LRESULT CALLBACK CreatePlanePosDialogueProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    unsigned pos;
    char p[4];
    switch(message)
    {
        case WM_COMMAND:
            switch(wParam)
            {
            case BUTTON_DIALOG_OK:
                GetWindowText(hPos,p,4);
                pos = atoi(p);
                hInsererPos(hWnd,pos);
                break;
            case BUTTON_DIALOG_CANCEL:
                hClose(hWnd);
                break;
            }
            break;
        case WM_CREATE:
            CreateWindowEx(0,"Static","Position d'insertion:",WS_VISIBLE|WS_CHILD,50,50,170,20,hWnd,NULL,NULL,NULL);
            hPos = CreateWindowEx(0,"Edit","",WS_VISIBLE|WS_CHILD|WS_BORDER,225,50,30,20,hWnd,NULL,NULL,NULL);
            AddPlaneControls(hWnd,170,50,80);
            break;
        case WM_DESTROY:
            EnableWindow(hMainWindow,true);
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc (hWnd, message, wParam, lParam);
    }
}

LRESULT CALLBACK DeletePlanePosDialogueProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int val;
    char p[6];
    unsigned pos;
    switch(message)
    {
        case WM_COMMAND:
            switch(wParam)
            {
                case BUTTON_DIALOG_OK:
                    GetWindowText(hPos,p,5);
                    if(!strcmp(p,""))
                    {
                        val = MessageBoxEx(hWnd,"Aucune position n'a pas été entrée","Error",MB_ICONERROR|MB_RETRYCANCEL,0);
                        if(val==IDCANCEL)
                            DestroyWindow(hWnd);
                    }else
                    {

                        pos = atoi(p);
                        if(pos<1 || pos>taille_liste_av(liste))
                        {
                            MessageBox(hWnd,"Cette position n'existe pas","Error",MB_OK);
                            SetWindowText(hPos,"");
                        }
                        else
                        {
                            hSupprimerPos(hWnd,pos);
                            EnableWindow(hMainWindow,true);
                            DestroyWindow(hWnd);
                        }
                    }
                    break;
                case BUTTON_DIALOG_CANCEL:
                    hClose(hWnd);
                    break;
            }
            break;
        case WM_CREATE:
            CreateWindowEx(0,"Static","Position de suppression:",WS_VISIBLE|WS_CHILD,50,50,210,20,hWnd,NULL,NULL,NULL);
            hPos = CreateWindowEx(0,"Edit","",WS_VISIBLE|WS_CHILD|WS_BORDER,220,50,30,20,hWnd,NULL,NULL,NULL);
            break;
        case WM_DESTROY:
            EnableWindow(hMainWindow,true);
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc (hWnd, message, wParam, lParam);
    }
}

LRESULT CALLBACK ResearchPlaneIdDialogueProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Cellule *c;
    char id[7];
    switch(message)
    {
        case WM_COMMAND:
            switch(wParam)
            {
            case BUTTON_DIALOG_OK:
                GetWindowText(hId,id,7);
                strncpy(globalId,id,7);
                if(!hValidId(id))
                {
                    MessageBox(hWnd,"Identifiant incorrect","Error",MB_OK);
                }else
                {
                    detruire(searchResult);
                    c = recherche_id(liste,id);
                    insere_queue_av(searchResult,c->cle);
                    refreshView();
                    EnableWindow(hMainWindow,true);
                    DestroyWindow(hWnd);
                }
                break;
            case BUTTON_DIALOG_CANCEL:
                hClose(hWnd);
                break;
            }
            break;
        case WM_CREATE:
            CreateWindowEx(0,"Static","Identifiant de l'avion:",WS_VISIBLE|WS_CHILD,50,50,210,20,hWnd,NULL,NULL,NULL);
            hId = CreateWindowEx(0,"Edit","",WS_VISIBLE|WS_CHILD|WS_BORDER,170,50,100,20,hWnd,NULL,NULL,NULL);
            break;
        case WM_DESTROY:
            EnableWindow(hMainWindow,true);
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc (hWnd, message, wParam, lParam);
    }
}

LRESULT CALLBACK ResearchPlaneCompagnieDialogueProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    char acronyme[4];
    switch(message)
    {
        case WM_COMMAND:
            switch(wParam)
            {
            case BUTTON_DIALOG_OK:
                GetWindowText(hAcr,acronyme,4);
                if(!hValidAcr(acronyme))
                    MessageBoxEx(hWnd,"Acronyme incorrect","Error",MB_ICONERROR|MB_OK,0);
                else
                {
                    strncpy(globalAcr,acronyme,4);
                    detruire(searchResult);
                    parcourir(hWnd,liste,hRechercheComp);
                    refreshView();
                    EnableWindow(hMainWindow,true);
                    DestroyWindow(hWnd);
                }
                break;
            case BUTTON_DIALOG_CANCEL:
                hClose(hWnd);
                break;
            }
            break;
        case WM_CREATE:
            CreateWindowEx(0,"Static","Acronyme de la compagnie:",WS_VISIBLE|WS_CHILD,50,50,210,20,hWnd,NULL,NULL,NULL);
            hAcr = CreateWindowEx(0,"Edit","",WS_VISIBLE|WS_CHILD|WS_BORDER,200,50,50,20,hWnd,NULL,NULL,NULL);
            break;
        case WM_DESTROY:
            EnableWindow(hMainWindow,true);
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc (hWnd, message, wParam, lParam);
    }
}

LRESULT CALLBACK ResearchPlaneHeureDialogueProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    char h1[3], h2[3], *heure;
    switch(message)
    {
        case WM_COMMAND:
            switch(wParam)
            {
            case BUTTON_DIALOG_OK:
                GetWindowText(hHeure1,h1,3);
                GetWindowText(hHeure2,h2,3);
                if(!hValidHeure(h1,h2))
                    MessageBoxEx(hWnd,"Heure incorrect","Error",MB_ICONERROR|MB_OK,0);
                else
                {
                    heure = hHeureInput(h1,h2);
                    strncpy(globalHour,heure,5);
                    detruire(searchResult);
                    parcourir(hWnd,liste,hRechercheHeure);
                    refreshView();
                    EnableWindow(hMainWindow,true);
                    DestroyWindow(hWnd);
                }
                break;
            case BUTTON_DIALOG_CANCEL:
                hClose(hWnd);
                break;
            }
            break;
        case WM_CREATE:
            CreateWindowEx(0,"Static","Heure de décollage:",WS_VISIBLE|WS_CHILD,50,50,170,20,hWnd,NULL,NULL,NULL);
            hHeure1 = CreateWindowEx(0,"Edit","",WS_VISIBLE|WS_CHILD|WS_BORDER,100,50,20,20,hWnd,NULL,NULL,NULL);
            CreateWindowEx(0,"Static",":",WS_VISIBLE|WS_CHILD,123,50,3,20,hWnd,NULL,NULL,NULL);
            hHeure2 = CreateWindowEx(0,"Edit","",WS_VISIBLE|WS_CHILD|WS_BORDER,125,50,20,20,hWnd,NULL,NULL,NULL);
            break;
        case WM_DESTROY:
            EnableWindow(hMainWindow,true);
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc (hWnd, message, wParam, lParam);
    }
}

LRESULT CALLBACK ModifyPlaneIdDialogueProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    char id[7], num[4];
    Cellule *c;
    switch(message)
    {
        case WM_COMMAND:
            switch(wParam)
            {
            case BUTTON_DIALOG_OK:
                GetWindowText(hId,id,7);
                GetWindowText(hNum,num,4);
                if(!hValidNum(num))
                    MessageBoxEx(hWnd,"Numero incorrect","Error",MB_ICONERROR|MB_OK,0);
                else if(!hValidId(id))
                    MessageBoxEx(hWnd,"Identifiant incorrect","Error",MB_ICONERROR|MB_OK,0);
                else
                {
                    c = recherche_id(liste,id);
                    gerer_id(c->cle,atoi(num));
                    unsavedChanges = true;
                    CopieListe();
                    refreshView();
                    EnableWindow(hMainWindow,true);
                    DestroyWindow(hWnd);
                }
                break;
            case BUTTON_DIALOG_CANCEL:
                hClose(hWnd);
                break;
            }
            break;
        case WM_CREATE:
            CreateWindowEx(0,"Static","Identifiant de l'avion:",WS_VISIBLE|WS_CHILD,50,50,195,20,hWnd,NULL,NULL,NULL);
            hId = CreateWindowEx(0,"Edit","",WS_VISIBLE|WS_CHILD|WS_BORDER,180,50,70,20,hWnd,NULL,NULL,NULL);
            CreateWindowEx(0,"Static","Nouveaur Numéro de vol:",WS_VISIBLE|WS_CHILD,50,80,195,20,hWnd,NULL,NULL,NULL);
            hNum = CreateWindowEx(0,"Edit","",WS_VISIBLE|WS_CHILD|WS_BORDER,180,80,70,20,hWnd,NULL,NULL,NULL);
            break;
        case WM_DESTROY:
            EnableWindow(hMainWindow,true);
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc (hWnd, message, wParam, lParam);
    }
}

LRESULT CALLBACK ModifyPlaneCompagnieDialogueProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    char id[7], acronyme[4];
    Cellule *c;
    switch(message)
    {
        case WM_COMMAND:
            switch(wParam)
            {
            case BUTTON_DIALOG_OK:
                GetWindowText(hId,id,7);
                GetWindowText(hAcr,acronyme,4);
                if(!hValidId(id))
                    MessageBoxEx(hWnd,"Identifiant incorrect","Error",MB_ICONERROR|MB_OK,0);
                else if(!hValidAcr(acronyme))
                    MessageBoxEx(hWnd,"Acronyme incorrect","Error",MB_ICONERROR|MB_OK,0);
                else
                {
                    c = recherche_id(liste,id);
                    gerer_compagnie(c->cle,listeCompagnie,acronyme);
                    unsavedChanges = true;
                    CopieListe();
                    refreshView();
                    EnableWindow(hMainWindow,true);
                    DestroyWindow(hWnd);
                }

                break;
            case BUTTON_DIALOG_CANCEL:
                hClose(hWnd);
                break;
            }
            break;
        case WM_CREATE:
            CreateWindowEx(0,"Static","Identifiant de l'avion:",WS_VISIBLE|WS_CHILD,50,50,195,20,hWnd,NULL,NULL,NULL);
            hId = CreateWindowEx(0,"Edit","",WS_VISIBLE|WS_CHILD|WS_BORDER,240,50,70,20,hWnd,NULL,NULL,NULL);
            CreateWindowEx(0,"Static","Acronyme de la nouvelle compagnie:",WS_VISIBLE|WS_CHILD,50,80,195,20,hWnd,NULL,NULL,NULL);
            hAcr = CreateWindowEx(0,"Edit","",WS_VISIBLE|WS_CHILD|WS_BORDER,240,80,70,20,hWnd,NULL,NULL,NULL);
            break;
        case WM_DESTROY:
            EnableWindow(hMainWindow,true);
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc (hWnd, message, wParam, lParam);
    }
}

LRESULT CALLBACK AddCompagnieDialogueProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    char nom[100];
    char acronyme[4];
    Compagnie *comp;
    switch(message)
    {
        case WM_COMMAND:
            switch(wParam)
            {
            case BUTTON_DIALOG_OK:
                GetWindowText(hTitle,nom,100);
                GetWindowText(hAcr,acronyme,4);
                if(!hValidAcr(acronyme))
                    MessageBoxEx(hWnd,"Acronyme incorrect","Error",MB_ICONERROR|MB_OK,0);
                else if(hValidAcrExist(acronyme))
                    MessageBoxEx(hWnd,"Compagnie existe déjà","Error",MB_ICONERROR|MB_OK,0);
                else
                {
                    comp = creer_compagnie(nom,acronyme);
                    insere_comp(listeCompagnie,comp);
                    unsavedChanges = true;
                    EnableWindow(hMainWindow,true);
                    DestroyWindow(hWnd);
                }
                break;
            case BUTTON_DIALOG_CANCEL:
                hClose(hWnd);
                break;
            }
            break;
        case WM_CREATE:
            CreateWindowEx(0,"Static","Nom de la compagnie:",WS_VISIBLE|WS_CHILD,50,50,195,20,hWnd,NULL,NULL,NULL);
            hTitle = CreateWindowEx(0,"Edit","",WS_VISIBLE|WS_CHILD|WS_BORDER,200,50,100,20,hWnd,NULL,NULL,NULL);
            CreateWindowEx(0,"Static","Acronyme de la compagnie:",WS_VISIBLE|WS_CHILD,50,80,195,20,hWnd,NULL,NULL,NULL);
            hAcr = CreateWindowEx(0,"Edit","",WS_VISIBLE|WS_CHILD|WS_BORDER,200,80,50,20,hWnd,NULL,NULL,NULL);
            break;
        case WM_DESTROY:
            EnableWindow(hMainWindow,true);
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc (hWnd, message, wParam, lParam);
    }
}

LRESULT CALLBACK DeleteCompagnieDialogueProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    char acronyme[4];
    CelluleCompagnie *c;
    switch(message)
    {
        case WM_COMMAND:
            switch(wParam)
            {
            case BUTTON_DIALOG_OK:
                GetWindowText(hAcr,acronyme,4);
                if(!hValidAcr(acronyme))
                    MessageBoxEx(hWnd,"Acronyme incorrect","Error",MB_ICONERROR|MB_OK,0);
                else if(!hValidAcrExist(acronyme))
                    MessageBoxEx(hWnd,"Compagnie n'existe pas","Error",MB_ICONERROR|MB_OK,0);
                else
                {
                    c = recherche_acr_comp(listeCompagnie,acronyme);
                    supprimer_compagnie_av(liste,listeCompagnie,c);
                    unsavedChanges = true;
                    CopieListe();
                    refreshView();
                    EnableWindow(hMainWindow,true);
                    DestroyWindow(hWnd);
                }

                break;
            case BUTTON_DIALOG_CANCEL:
                hClose(hWnd);
                break;
            }
            break;
        case WM_CREATE:
            CreateWindowEx(0,"Static","Acronyme de la compagnie:",WS_VISIBLE|WS_CHILD,50,50,195,20,hWnd,NULL,NULL,NULL);
            hAcr = CreateWindowEx(0,"Edit","",WS_VISIBLE|WS_CHILD|WS_BORDER,200,50,50,20,hWnd,NULL,NULL,NULL);
            break;
        case WM_DESTROY:
            EnableWindow(hMainWindow,true);
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc (hWnd, message, wParam, lParam);
    }
}

void hShowAll(HWND hWnd, Liste *liste, Cellule *c, unsigned i)
{
    insere_queue_av(searchResult,c->cle);
}

void hRechercheComp(HWND hWnd, Liste *liste, Cellule *c, unsigned i)
{
    if(!strcmp(c->cle->compagnie->acronyme,globalAcr))
        insere_queue_av(searchResult,c->cle);
}

void hRechercheHeure(HWND hWnd, Liste *liste, Cellule *c, unsigned i)
{
    if(!strcmp(c->cle->heure_decollage,globalHour))
        insere_queue_av(searchResult,c->cle);
}

void CopieListe()
{
    detruire(searchResult);
    parcourir(NULL,liste,hShowAll);
}

void hDisplayComponent(HWND hWnd, Liste *l, Cellule *c, unsigned pos)
{
    Avion *av = c->cle;
    char carburant[7], consommation[7], h1[3], h2[3],acronyme[4], *index = malloc((floor(log10(abs(pos+1)))+2)*sizeof(char));
    itoa(av->consommation,consommation,10);
    itoa(av->carburant,carburant,10);
    strncpy(h1,av->heure_decollage,2);
    h1[2] = '\0';
    strncpy(h2,(av->heure_decollage)+2,2);
    h2[2] = '\0';
    strncpy(acronyme,av->compagnie->acronyme,3);
    acronyme[3] = '\0';
    itoa(pos+1,index,10);
    CreateWindowEx(0,"Static",index,WS_VISIBLE|WS_CHILD,5,5+((pos+1)*25),70,13,hWnd,NULL,NULL,NULL);
    CreateWindowEx(0,"Static","-",WS_VISIBLE|WS_CHILD,18,5+((pos+1)*25),70,13,hWnd,NULL,NULL,NULL);
    CreateWindowEx(0,"Static",c->cle->identifiant,WS_VISIBLE|WS_CHILD,25,5+((pos+1)*25),70,13,hWnd,NULL,NULL,NULL);
    CreateWindowEx(0,"Static",carburant,WS_VISIBLE|WS_CHILD,85,5+((pos+1)*25),50,13,hWnd,NULL,NULL,NULL);
    CreateWindowEx(0,"Static",consommation,WS_VISIBLE|WS_CHILD,140,5+((pos+1)*25),50,13,hWnd,NULL,NULL,NULL);
    CreateWindowEx(0,"Static",h1,WS_VISIBLE|WS_CHILD,195,5+((pos+1)*25),20,13,hWnd,NULL,NULL,NULL);
    CreateWindowEx(0,"Static",":",WS_VISIBLE|WS_CHILD,210,5+((pos+1)*25),5,13,hWnd,NULL,NULL,NULL);
    CreateWindowEx(0,"Static",h2,WS_VISIBLE|WS_CHILD,215,5+((pos+1)*25),20,13,hWnd,NULL,NULL,NULL);
    CreateWindowEx(0,"Static",av->compagnie->nom,WS_VISIBLE|WS_CHILD,255,5+((pos+1)*25),100,13,hWnd,NULL,NULL,NULL);
    CreateWindowEx(0,"Static",acronyme,WS_VISIBLE|WS_CHILD,360,5+((pos+1)*25),40,13,hWnd,NULL,NULL,NULL);
}

int windowHeight(HWND hWnd)
{
    RECT rect;
    int height = 0;
    if(GetWindowRect(hWnd, &rect))
      height = rect.bottom - rect.top;
    return height;
}

void refreshView()
{
    DestroyWindow(hView);
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_RANGE;
    GetScrollInfo(hMainWindow,SB_VERT,&si);
    si.nMax = listHeight(searchResult);
    SetScrollInfo(hMainWindow,SB_VERT,&si,TRUE);
    hView = CreateWindowEx(0,"ViewClass",NULL,WS_CHILDWINDOW|WS_VISIBLE,0,0,450,3000,hMainWindow,NULL,NULL,NULL);
}

LRESULT CALLBACK CopyrightProcedure(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
    char copyright[] = "Gestionnaire d'aeroport\nDroits d'auteur © 2021 Yassine Saidane\nTout droits réservées\nMini Projet Mme Malek Amel";
    switch(message)
    {
        case WM_CREATE:
            CreateWindowEx(0,"Static",copyright,WS_VISIBLE|WS_CHILDWINDOW|WS_OVERLAPPED,150,50,300,100,hWnd,(HMENU)BUTTON_DIALOG_OK,NULL,NULL);
            EnumChildWindows(hWnd, (WNDENUMPROC)SetFont, (LPARAM)GetStockObject(DEFAULT_GUI_FONT));
            EnableWindow(hMainWindow,false);
            break;
        case WM_DESTROY:
            EnableWindow(hMainWindow,true);
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc (hWnd, message, wParam, lParam);
    }
}

void registerCopyrightClass(HINSTANCE hInst)
{
    WNDCLASSEX copyright;

    copyright.lpszClassName = "CopyrightClass";
    copyright.hInstance = hInst;
    copyright.lpfnWndProc = CopyrightProcedure;
    copyright.style = CS_DBLCLKS;
    copyright.cbSize = sizeof (WNDCLASSEX);

    copyright.hIcon = LoadIcon (hInst, MAKEINTRESOURCE(IDI_ICON));
    copyright.hIconSm = LoadIcon (hInst, MAKEINTRESOURCE(IDI_ICON));
    copyright.hCursor = LoadCursor (NULL, IDC_ARROW);
    copyright.lpszMenuName = NULL;
    copyright.cbClsExtra = 0;
    copyright.cbWndExtra = 0;
    copyright.hbrBackground = (HBRUSH) COLOR_WINDOW;

    RegisterClassEx(&copyright);
}

void displayCopyright(HWND hWnd)
{
    CreateWindowEx(0,"CopyrightClass","Copyright ©",WS_VISIBLE | WS_OVERLAPPED | WS_SYSMENU | DS_CENTER, CW_USEDEFAULT, CW_USEDEFAULT, 400, 200, hWnd,NULL,NULL,NULL);
}

void CustomHandleVScroll(HWND hWnd, int iAction)
{
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS;
    GetScrollInfo(hWnd,SB_VERT,&si);
    switch(iAction)
    {
        case SB_TOP:
            si.nPos = si.nMin;
            break;
        case SB_BOTTOM:
            si.nPos = si.nMax;
            break;
        case SB_LINEUP:
            si.nPos -= 10;
            break;
        case SB_LINEDOWN:
            si.nPos += 10;
            break;
        case SB_PAGEUP:
            si.nPos -= si.nPage;
            break;
        case SB_PAGEDOWN:
            si.nPos += si.nPage;
            break;
        case SB_THUMBTRACK:
            si.nPos = HIWORD(iAction);
            break;
        default:
        case SB_THUMBPOSITION:
            si.nPos = HIWORD(iAction);
            break;
    };
    SetScrollInfo(hWnd,SB_VERT,&si,TRUE);
    posY = -si.nPos;
    MoveWindow(hView,0,posY,450,3000,TRUE);
}

void CustomHandleKeyDown(HWND hWnd, UINT vkCode)
{
    switch (vkCode) {
    case VK_HOME:   CustomHandleVScroll(hWnd, SB_TOP); break;
    case VK_END:    CustomHandleVScroll(hWnd, SB_BOTTOM); break;
    case VK_UP:     CustomHandleVScroll(hWnd, SB_LINEUP); break;
    case VK_DOWN:   CustomHandleVScroll(hWnd, SB_LINEDOWN); break;
    case VK_PRIOR:  CustomHandleVScroll(hWnd, SB_PAGEUP); break;
    case VK_NEXT:   CustomHandleVScroll(hWnd, SB_PAGEDOWN); break;
    }
}
