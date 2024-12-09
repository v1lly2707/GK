/******************************************************************
 Grafika komputerowa, środowisko MS Windows - program  przykładowy
 *****************************************************************/

#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;


//deklaracja funkcji obslugi okna
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//funkcja Main - dla Windows
 int WINAPI WinMain(HINSTANCE hInstance,
               HINSTANCE hPrevInstance,
               LPSTR     lpCmdLine,
               int       nCmdShow)
{
	MSG meldunek;		  //innymi slowy "komunikat"
	WNDCLASS nasza_klasa; //klasa głównego okna aplikacji
	HWND okno;
	static char nazwa_klasy[] = "Podstawowa";
	
	GdiplusStartupInput gdiplusParametry;// parametry GDI+; domyślny konstruktor wypełnia strukturę odpowiednimi wartościami
	ULONG_PTR	gdiplusToken;			// tzw. token GDI+; wartość uzyskiwana przy inicjowaniu i przekazywana do funkcji GdiplusShutdown
   
	// Inicjujemy GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusParametry, NULL);

	//Definiujemy klase głównego okna aplikacji
	//Okreslamy tu wlasciwosci okna, szczegoly wygladu oraz
	//adres funkcji przetwarzajacej komunikaty
	nasza_klasa.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	nasza_klasa.lpfnWndProc   = WndProc; //adres funkcji realizującej przetwarzanie meldunków 
 	nasza_klasa.cbClsExtra    = 0 ;
	nasza_klasa.cbWndExtra    = 0 ;
	nasza_klasa.hInstance     = hInstance; //identyfikator procesu przekazany przez MS Windows podczas uruchamiania programu
	nasza_klasa.hIcon         = 0;
	nasza_klasa.hCursor       = LoadCursor(0, IDC_ARROW);
	nasza_klasa.hbrBackground = (HBRUSH) GetStockObject(GRAY_BRUSH);
	nasza_klasa.lpszMenuName  = "Menu" ;
	nasza_klasa.lpszClassName = nazwa_klasy;

    //teraz rejestrujemy klasę okna głównego
    RegisterClass (&nasza_klasa);
	
	/*tworzymy okno główne
	okno będzie miało zmienne rozmiary, listwę z tytułem, menu systemowym
	i przyciskami do zwijania do ikony i rozwijania na cały ekran, po utworzeniu
	będzie widoczne na ekranie */
 	okno = CreateWindow(nazwa_klasy, "Grafika komputerowa", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
						CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	
	
	/* wybór rozmiaru i usytuowania okna pozostawiamy systemowi MS Windows */
   	ShowWindow (okno, nCmdShow) ;
    
	//odswiezamy zawartosc okna
	UpdateWindow (okno) ;

	// GŁÓWNA PĘTLA PROGRAMU
	while (GetMessage(&meldunek, NULL, 0, 0))
     /* pobranie komunikatu z kolejki; funkcja GetMessage zwraca FALSE tylko dla
	 komunikatu WM_QUIT; dla wszystkich pozostałych komunikatów zwraca wartość TRUE */
	{
		TranslateMessage(&meldunek); // wstępna obróbka komunikatu
		DispatchMessage(&meldunek);  // przekazanie komunikatu właściwemu adresatowi (czyli funkcji obslugujacej odpowiednie okno)
	}

	GdiplusShutdown(gdiplusToken);
	
	return (int)meldunek.wParam;
}

/********************************************************************
FUNKCJA OKNA realizujaca przetwarzanie meldunków kierowanych do okna aplikacji*/
LRESULT CALLBACK WndProc (HWND okno, UINT kod_meldunku, WPARAM wParam, LPARAM lParam)
{
	HMENU mPlik, mInfo, mGlowne;
    	
/* PONIŻSZA INSTRUKCJA DEFINIUJE REAKCJE APLIKACJI NA POSZCZEGÓLNE MELDUNKI */
	switch (kod_meldunku) 
	{
	case WM_CREATE:  //meldunek wysyłany w momencie tworzenia okna
		mPlik = CreateMenu();
		AppendMenu(mPlik, MF_STRING, 100, "&Zapiszcz...");
		AppendMenu(mPlik, MF_SEPARATOR, 0, "");
		AppendMenu(mPlik, MF_STRING, 101, "&Koniec");
		mInfo = CreateMenu();
		AppendMenu(mInfo, MF_STRING, 200, "&Autor...");
		mGlowne = CreateMenu();
		AppendMenu(mGlowne, MF_POPUP, (UINT_PTR) mPlik, "&Plik");
		AppendMenu(mGlowne, MF_POPUP, (UINT_PTR) mInfo, "&Informacja");
		SetMenu(okno, mGlowne);
		DrawMenuBar(okno);

	case WM_COMMAND: //reakcje na wybór opcji z menu
		switch (wParam)
		{
		case 100: if(MessageBox(okno, "Zapiszczeć?", "Pisk", MB_YESNO) == IDYES)
					MessageBeep(0);
                  break;
		case 101: DestroyWindow(okno); //wysylamy meldunek WM_DESTROY
        		  break;
		case 200: MessageBox(okno, "Imię i nazwisko:\nNumer indeksu: ", "Autor", MB_OK);
		}
		return 0;
	
	case WM_LBUTTONDOWN: //reakcja na lewy przycisk myszki
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			return 0;
		}

	case WM_PAINT:
		{
			PAINTSTRUCT paint;
			HDC kontekst;

			kontekst = BeginPaint(okno, &paint);
			POINT punkt[] =
			{
				{150,500},
				{250,500},
				{225,600},
				{175,600}
			};

			HPEN pioro = CreatePen(PS_SOLID, 1, RGB(255,255,255));
			SelectObject(kontekst, pioro);

			MoveToEx(kontekst, 99, 300,NULL);
			LineTo(kontekst, 150, 500);
			MoveToEx(kontekst, 300, 300, NULL);
			LineTo(kontekst, 250, 500);

			MoveToEx(kontekst, 100, 100, NULL);
			Ellipse(kontekst, 100, 200, 300, 400);
			DeleteObject(pioro);
			HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
			SelectObject(kontekst, brush);
			Pie(kontekst, 100, 200, 300, 400, 200, 0, 200, 100000);
			MoveToEx(kontekst, 200, 400, NULL);
			LineTo(kontekst, 200, 500);

			

			Polygon(kontekst, punkt, 4);
			
			DeleteObject(brush);
			

			

			
			
			
			
			// utworzenie obiektu umożliwiającego rysowanie przy użyciu GDI+
			// (od tego momentu nie można używać funkcji GDI
			Graphics grafika(kontekst);
			
			// MIEJSCE NA KOD GDI+
			


			
			// utworzenie czcionki i wypisanie tekstu na ekranie
			FontFamily  fontFamily(L"Times New Roman");
			Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
			PointF      pointF(100.0f, 400.0f);
			SolidBrush  solidBrush(Color(255, 0, 0, 255));

		//	grafika.DrawString(L"To jest tekst napisany za pomocą GDI+.", -1, &font, pointF, &solidBrush);

			EndPaint(okno, &paint);

			return 0;
		}
  	
	case WM_DESTROY: //obowiązkowa obsługa meldunku o zamknięciu okna
		PostQuitMessage (0) ;
		return 0;
    
	default: //standardowa obsługa pozostałych meldunków
		return DefWindowProc(okno, kod_meldunku, wParam, lParam);
	}
}
