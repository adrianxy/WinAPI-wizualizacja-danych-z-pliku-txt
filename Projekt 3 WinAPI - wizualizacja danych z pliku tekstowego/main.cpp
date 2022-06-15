#include <windows.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////// ZMIENNE GLOBALNE #USUNIECIE

LPSTR NazwaKlasy = "Klasa Okienka";
MSG Komunikat;
HWND g_przycisk1, g_przycisk2, g_przycisk3, g_text_ktora_probka, g_przycisk4,
g_przycisk5, g_przycisk6_pion, g_przycisk7_pion, g_przycisk8_poziom, g_przycisk9_poziom;

int ktora_probka;
int czy_zaczac=0;
int klj_pkt;
int n=5;
int gestoscX=2, wysY=1;
int iks;
int up, pls;
float temp_tab[7];
float tab[10000], smieci; int id=0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////// DEKLAROWANIE FUNKCJI
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
void odczyt_danych();
void rysowanie_czesci_stalych(PAINTSTRUCT ps, HDC hdc, HWND hwnd);
void rysowanie_kursora(PAINTSTRUCT ps, HDC hdc, HWND hwnd);
void rysowanie_wykresu(PAINTSTRUCT ps, HDC hdc, HWND hwnd);


////////////////////////////////////////////////////////////////////////////////////////////////////////////// FuNKCJA GLOWNA
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{ 
    // WYPE£NIANIE STRUKTURY
    WNDCLASSEX wc;
   
    wc.cbSize = sizeof( WNDCLASSEX );
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground =( HBRUSH )( COLOR_WINDOW + 1 );
    wc.lpszMenuName = NULL;
    wc.lpszClassName = NazwaKlasy;
    wc.hIconSm = LoadIcon( NULL, IDI_APPLICATION );
   
    // REJESTROWANIE KLASY OKNA
    if( !RegisterClassEx( & wc ) )
    {
        MessageBox( NULL, "B³¹d 404", "ERROR",
        MB_ICONEXCLAMATION | MB_OK );
        return -1;
    }
   HWND hwnd;
    // TWORZENIE OKNA
    hwnd = CreateWindowEx( WS_EX_CLIENTEDGE, NazwaKlasy, "projekt_3", WS_VISIBLE|WS_OVERLAPPEDWINDOW,
    					   CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, hInstance, NULL );
   
   	// TWORZENIE PRZYCISKOW
  	g_przycisk1 = CreateWindowEx( 0, "BUTTON", "Wczytaj dane", WS_CHILD | WS_VISIBLE, 1200, 100, 200, 50, hwnd, NULL, hInstance, NULL );
	g_przycisk2 = CreateWindowEx( 0, "BUTTON", "START", WS_CHILD | WS_VISIBLE, 1200, 150, 200, 50, hwnd,NULL, hInstance, NULL );
	g_przycisk3 = CreateWindowEx( 0, "BUTTON", "OK", WS_CHILD | WS_VISIBLE, 1302, 50, 100, 20, hwnd, NULL, hInstance, NULL );
	g_przycisk4 = CreateWindowEx( 0, "BUTTON", "-", WS_CHILD | WS_VISIBLE, 250, 720, 100, 50, hwnd, NULL, hInstance, NULL );
	g_przycisk5 = CreateWindowEx( 0, "BUTTON", "+", WS_CHILD | WS_VISIBLE, 350, 720, 100, 50, hwnd,NULL, hInstance, NULL );
	g_przycisk6_pion = CreateWindowEx( 0, "BUTTON", "-", WS_CHILD | WS_VISIBLE, 740, 500, 50, 50, hwnd,NULL, hInstance, NULL );
	g_przycisk7_pion = CreateWindowEx( 0, "BUTTON", "+", WS_CHILD | WS_VISIBLE, 740, 450, 50, 50, hwnd,NULL, hInstance, NULL );
	g_przycisk8_poziom = CreateWindowEx( 0, "BUTTON", "-", WS_CHILD | WS_VISIBLE, 1050, 720, 100, 50, hwnd,NULL, hInstance, NULL );
	g_przycisk9_poziom = CreateWindowEx( 0, "BUTTON", "+", WS_CHILD | WS_VISIBLE, 1150, 720, 100, 50, hwnd,NULL, hInstance, NULL );
	
	g_text_ktora_probka = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,1200, 50, 100, 20, hwnd, NULL, hInstance, NULL );
	HWND hStatic = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |SS_CENTER, 1200, 30, 200, 20, hwnd, NULL, hInstance, NULL );
	SetWindowText( hStatic, "Od której próbki zacz¹æ?" );
	
	HWND hStatic1 = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |SS_CENTER, 890, 30, 20, 20, hwnd, NULL, hInstance, NULL );
	SetWindowText( hStatic1, "N" );
	HWND hStatic2 = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |SS_CENTER, 1000, 140, 20, 20, hwnd, NULL, hInstance, NULL );
	SetWindowText( hStatic2, "E" );
	HWND hStatic3 = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |SS_CENTER, 890, 250, 20, 20, hwnd, NULL, hInstance, NULL );
	SetWindowText( hStatic3, "S" );
	HWND hStatic4 = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |SS_CENTER, 780, 140, 20, 20, hwnd, NULL, hInstance, NULL );
	SetWindowText( hStatic4, "W" );
	
	// CZY OKNO OTWORZYLO SIE POPRAWNIE	
    if( hwnd == NULL )
    {
        MessageBox( NULL, "B³¹d przy tworzeniu okna" , "ERROR", MB_ICONEXCLAMATION );
        return 1;
    }
    
    // POKAZUJE OKNO
    ShowWindow( hwnd, nCmdShow );
    UpdateWindow( hwnd );

 	// TIMER
	UINT TimerId = SetTimer(hwnd, 1, 40, (TIMERPROC) NULL); //tworzenie timera

    // PETLA KOMUNIKATOW
    while( GetMessage( & Komunikat, NULL, 0, 0 ) )
    {
        TranslateMessage( & Komunikat );
        DispatchMessage( & Komunikat );
    }

    return Komunikat.wParam;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////// OBS£UGA ZDARZEÑ
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ){	
	
	PAINTSTRUCT ps;
	HDC hdc;
	
    switch( msg ){
    	
		case WM_TIMER:
			if(czy_zaczac==1){
				rysowanie_wykresu(ps, hdc, hwnd);
				rysowanie_kursora(ps, hdc, hwnd);
			}
			break;
			
	    case WM_COMMAND:
	    	if(( HWND ) lParam == g_przycisk1 ){
	    		odczyt_danych();	
			}
			else if(( HWND ) lParam == g_przycisk2 ){
				InvalidateRect(NULL, NULL, TRUE);	//CZYSCZENIE OKNA (na nowy start)
				czy_zaczac=1;
				klj_pkt=ktora_probka*3;	//USTAWIANIE PIERWSZEJ PROBKI
				
				iks=800;					// DO WYROSOWANIA WYKRESU
				up=0; pls=0;				// DO WYROSOWANIA WYKRESU
				temp_tab[1]=tab[klj_pkt];	// DO WYROSOWANIA WYKRESU
				temp_tab[3]=tab[klj_pkt+1];	// DO WYROSOWANIA WYKRESU
				temp_tab[5]=tab[klj_pkt+2];	// DO WYROSOWANIA WYKRESU
			}	
			else if(( HWND ) lParam == g_przycisk3 ){
				DWORD dlugosc = GetWindowTextLength( g_text_ktora_probka );
				LPSTR Bufor =( LPSTR ) GlobalAlloc( GPTR, dlugosc + 1 );
				GetWindowText( g_text_ktora_probka, Bufor, dlugosc + 1 );
				ktora_probka = atoi(Bufor);	
				GlobalFree( Bufor );
			}
			else if(( HWND ) lParam == g_przycisk4 ){
				if(n>1)
				n=n-1;
			}
			else if(( HWND ) lParam == g_przycisk5 ){
				if(n<9)
				n=n+1;
			}
			else if(( HWND ) lParam == g_przycisk6_pion ){
				if(wysY>1)
				wysY=wysY-1;
			}
			else if(( HWND ) lParam == g_przycisk7_pion ){
				if(wysY<9)
				wysY=wysY+1;
			}
			else if(( HWND ) lParam == g_przycisk8_poziom ){
				if(gestoscX<9)
				gestoscX=gestoscX+1;
			}
			else if(( HWND ) lParam == g_przycisk9_poziom ){
				if(gestoscX>1)
				gestoscX=gestoscX-1;
			}
			break;
			
		case WM_PAINT:{
				rysowanie_czesci_stalych(ps, hdc, hwnd);}
				break;
				
	    case WM_CLOSE:
	        DestroyWindow( hwnd );
	        break;
	       
	    case WM_DESTROY:
	        PostQuitMessage( 0 );
	        break;
	       
	    default:
	        return DefWindowProc( hwnd, msg, wParam, lParam );
	}
	    
    return 0;
}


void odczyt_danych(){		// ZAPISYWANIE DANYCH DO TABLICY
	ifstream plik;
	
	plik.open("outputPendulumOrt02.log", ios::in);
	if(!plik.good())  MessageBox( NULL, "Plik siê nie otwiera", "Niestety",MB_ICONEXCLAMATION | MB_OK );
	else		   	  MessageBox( NULL, "Plik otworzy³ siê pomyœlnie", "Super",MB_ICONINFORMATION| MB_OK );
	
	while( !plik.eof() ){		// WCZYTYWANIE PLIKU DO TABLICY
        plik >> tab[id] >> tab[id+1] >> tab[id+2];
        for(int j=0; j<9; j++) plik >> smieci;
        id=id+3;
    } 
	id=id-3;
}

void rysowanie_czesci_stalych(PAINTSTRUCT ps, HDC hdc, HWND hwnd){
	
	// RYSOWANIE OBWODU KOMPASU
	hdc = BeginPaint(hwnd, &ps);
	Ellipse(hdc, 800, 50, 1000, 250);
	EndPaint(hwnd, &ps);
	
	HDC hdcOkno;
	hdcOkno = GetDC( hwnd );
	POINT stary_punkt;
	
	// RYSOWANIE OSI MAPY
	MoveToEx( hdcOkno, 350, 700, & stary_punkt );
	LineTo( hdcOkno, 350, 0 );
	MoveToEx( hdcOkno, 0, 350, & stary_punkt );
	LineTo( hdcOkno, 700, 350 );
	MoveToEx( hdcOkno, 340, 20, & stary_punkt );
	LineTo( hdcOkno, 350, 0 );
	LineTo( hdcOkno, 360, 20);
	MoveToEx( hdcOkno, 680, 340, & stary_punkt );
	LineTo( hdcOkno, 700, 350 );
	LineTo( hdcOkno, 680, 360);

	// RYSOWANIE OSI WYKRESU
	MoveToEx( hdcOkno, 800, 500, & stary_punkt );
	LineTo( hdcOkno, 1500, 500);
	MoveToEx( hdcOkno, 800, 700, & stary_punkt );
	LineTo( hdcOkno, 800, 300);
	MoveToEx( hdcOkno, 795, 305, & stary_punkt );
	LineTo( hdcOkno, 800, 300);
	LineTo( hdcOkno, 805, 305);
	MoveToEx( hdcOkno, 1495, 495, & stary_punkt );
	LineTo( hdcOkno, 1500, 500);
	LineTo( hdcOkno, 1495, 505);
	
	ReleaseDC( hwnd, hdcOkno );
}


void rysowanie_kursora(PAINTSTRUCT ps, HDC hdc, HWND hwnd){
	if(klj_pkt<id){

	///////////////////////////////////////////////////////////////////////////////////////////////////////////// RYSOWANIE PUNKTU NA MAPIE
		rysowanie_czesci_stalych(ps, hdc, hwnd); // ODNAWIANIE OSI MAPY
		HDC hdcOkno;
		hdcOkno = GetDC( hwnd );
		POINT stary_punkt;
		HPEN PioroCzerw, PioroBiale, Pudelko;
		PioroCzerw = CreatePen( PS_SOLID, 1, 0x0000FF );		
		PioroBiale = CreatePen( PS_SOLID, 1, 0xFFFFFF );
		
		Pudelko =( HPEN ) SelectObject( hdcOkno, PioroBiale );
		MoveToEx( hdcOkno, (n*tab[klj_pkt-3]+350), (350-n*tab[klj_pkt+1-3]), & stary_punkt );	//czysczenie poprzedniego punktu
		LineTo( hdcOkno, (n*tab[klj_pkt-3]+355), (350-n*tab[klj_pkt+1-3]) );
		MoveToEx( hdcOkno, (n*tab[klj_pkt-3]+350), (350-n*tab[klj_pkt+1-3]), & stary_punkt );
		LineTo( hdcOkno, (n*tab[klj_pkt-3]+350), (355-n*tab[klj_pkt+1-3]) );
		MoveToEx( hdcOkno, (n*tab[klj_pkt-3]+350), (350-n*tab[klj_pkt+1-3]), & stary_punkt );
		LineTo( hdcOkno, (n*tab[klj_pkt-3]+345), (350-n*tab[klj_pkt+1-3]) );
		MoveToEx( hdcOkno, (n*tab[klj_pkt-3]+350), (350-n*tab[klj_pkt+1-3]), & stary_punkt );
		LineTo( hdcOkno, (n*tab[klj_pkt-3]+350), (345-n*tab[klj_pkt+1-3]) );
		
		SelectObject( hdcOkno, PioroCzerw );
		MoveToEx( hdcOkno, (n*tab[klj_pkt]+350), (350-n*tab[klj_pkt+1]), & stary_punkt );	//rysowanie kolejnego punktu
		LineTo( hdcOkno, (n*tab[klj_pkt]+355), (350-n*tab[klj_pkt+1]) );
		MoveToEx( hdcOkno, (n*tab[klj_pkt]+350), (350-n*tab[klj_pkt+1]), & stary_punkt );
		LineTo( hdcOkno, (n*tab[klj_pkt]+350), (355-n*tab[klj_pkt+1]) );
		MoveToEx( hdcOkno, (n*tab[klj_pkt]+350), (350-n*tab[klj_pkt+1]), & stary_punkt );
		LineTo( hdcOkno, (n*tab[klj_pkt]+345), (350-n*tab[klj_pkt+1]) );
		MoveToEx( hdcOkno, (n*tab[klj_pkt]+350), (350-n*tab[klj_pkt+1]), & stary_punkt );
		LineTo( hdcOkno, (n*tab[klj_pkt]+350), (345-n*tab[klj_pkt+1]) );
		
	///////////////////////////////////////////////////////////////////////////////////////////////////////////// RYSOWANIE STRZALKI KOMPASU
		SelectObject( hdcOkno, PioroBiale ); 		// czysczenie poprzedniego punktu
		MoveToEx( hdcOkno, 900, 150, & stary_punkt );
		float x=tab[klj_pkt-3]*1000-tab[klj_pkt-3-3]*1000, y=tab[klj_pkt+1-3]*1000-tab[klj_pkt+1-3-3]*1000;
		float cosalfa = x/sqrt(pow(x,2)+pow(y,2));
		float sinalfa = y/sqrt(pow(x,2)+pow(y,2));
		LineTo( hdcOkno, 900+99*cosalfa, 150-99*sinalfa);
		
		SelectObject( hdcOkno, PioroCzerw );		//rysowanie kolejnego punktu
		MoveToEx( hdcOkno, 900, 150, & stary_punkt );
		x=tab[klj_pkt]*1000-tab[klj_pkt-3]*1000, y=tab[klj_pkt+1]*1000-tab[klj_pkt+1-3]*1000;
		cosalfa = x/sqrt(pow(x,2)+pow(y,2));
		sinalfa = y/sqrt(pow(x,2)+pow(y,2));
		LineTo( hdcOkno, 900+99*cosalfa, 150-99*sinalfa);
		
		SelectObject( hdcOkno, Pudelko );
		DeleteObject( PioroCzerw );
		DeleteObject( PioroBiale );
		ReleaseDC( hwnd, hdcOkno );
		
		klj_pkt=klj_pkt+3;	// WZIECIE NASTEPNEGO PUNKTU (dla calego programu)
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////// RYSOWANIE WYKRESU
void rysowanie_wykresu(PAINTSTRUCT ps, HDC hdc, HWND hwnd){
	if((klj_pkt+gestoscX+1)<id){
		POINT stary_punkt2;
		HDC hdcOkno;
		hdcOkno = GetDC( hwnd );
		HPEN PioroCzerw, PioroZiel, PioroNieb, Pudelko;
		PioroCzerw = CreatePen( PS_SOLID, 1, 0x0000FF );		
		PioroZiel = CreatePen( PS_SOLID, 1, 0x00FF00 );
		PioroNieb = CreatePen( PS_SOLID, 1, 0xFF0000 );
	
		temp_tab[0]=0;		// CZYSZCZENIE KOMOREK NA NOWE DANE
		temp_tab[2]=0;		//
		temp_tab[4]=0;		//
		
		if(!(up%gestoscX))	// TAKIE WARUNKI ODPALAJA SIE RAZ NA gestoscX RAZY
		for(int p=0; p<gestoscX; p++){
			temp_tab[0]=temp_tab[0]+tab[ktora_probka*3+pls*3];		// SUMOWANIE gestoscX SASIEDNICH KOMUREK
			temp_tab[2]=temp_tab[2]+tab[ktora_probka*3+pls*3+1];	//
			temp_tab[4]=temp_tab[4]+tab[ktora_probka*3+pls*3+2];	//
			pls++;
		}
		
		temp_tab[0]=temp_tab[0]/gestoscX;	// SREDNIA WARTOSC gestoscX KOMOREK SASIEDNICH
		temp_tab[2]=temp_tab[2]/gestoscX;	//
		temp_tab[4]=temp_tab[4]/gestoscX;	//
		
		
		// RYSOWANIE WYKRESU
		if(!(up%gestoscX)){		
			Pudelko =( HPEN ) SelectObject( hdcOkno, PioroCzerw );
			MoveToEx( hdcOkno, iks, 500-temp_tab[1]*wysY, & stary_punkt2 );
			LineTo(hdcOkno, iks+1, 500-temp_tab[0]*wysY);
		
			SelectObject( hdcOkno, PioroZiel );
			MoveToEx( hdcOkno, iks, 500-temp_tab[3]*wysY, & stary_punkt2 );
			LineTo(hdcOkno, iks+1, 500-temp_tab[2]*wysY);
			
			SelectObject( hdcOkno, PioroNieb );
			MoveToEx( hdcOkno, iks, 500-temp_tab[5]*wysY, & stary_punkt2 );
			LineTo(hdcOkno, iks+1, 500-temp_tab[4]*wysY);
			iks++;
		}
		
		if(!(up%gestoscX)){				// USTAWIANIE WARTOSCI PUNKTU POPRZEDNIEGO
			temp_tab[1]=temp_tab[0];	//
			temp_tab[3]=temp_tab[2];	//
			temp_tab[5]=temp_tab[4];	//
		}
		
		up++;	// ZLICZA ILOSC WEJSC W FUNKCJE (POMAGA W DZIALANIU OPERACJI RAZ NA gestoscX RAZY)
		
		DeleteObject( PioroCzerw );
		DeleteObject( PioroNieb );
		DeleteObject( PioroZiel );
		ReleaseDC( hwnd, hdcOkno );
	
	}
}

