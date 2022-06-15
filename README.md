# WinAPI wizualizacja danych z pliku tekstowego - semestr 2 - projekt 3

Program pisany w środowisku Dev C++

Program WinAPI wizualizujący sygnały automatyki. 

Program zawiera podstawowe funkcjonalności takie jak wczytanie danych z pliku, pokazywanie położenia robota na mapie 2D,
pokazywanie aktualnego kierunku robota na kompasie, rysowanie wykresów: x(t), y(t), z(t), oraz zmiana skali wykresów.
Możliwy jest wybór od której próbki program ma zacząć wizualizację.

Program wczytuje dane z pliku tekstowego, w którym zawarte są informacje o położeniu robota,
dane w pliku są oddzielone spacjami. Po wczytaniu danych można wybrać próbkę startową lub zacząć od domyślnej (1).
Po wykonaniu powyższych kroków - program jest gotowy do działania. Wizualizacja rozpoczyna się w momencie wciśnięcia „START”.
W przypadku zmiany skali podczas działania wizualizacji należy uruchomić program ponownie (wciskając START).

![p1](https://user-images.githubusercontent.com/84225546/123486908-3fe7b200-d60d-11eb-8499-86f76f419b7c.png)

1:                                                                                                         
--"OK" - zatwierdza wpisany w pole tekstowe numer pierwszej próbki;                                                                                         
--"Wczytaj dane" - wczytuje dane z pliku "outputPendulumOrt02.log";                                                               
--"START" - uruchamia program, służy też do resetu w przypadku zmiany skali;                                                      
2: Kompas wskazujący aktualny kierunek robota;                                                                                   
3: Mapa 2D pokazująca aktualne położenie robota wxględem osi XY z możliwością regulacji skali                                                              
4: Wykresy x(t), y(t), z(t) z możliwością regulacji skali
                                                                                                                                                

----------------------------------------------------------PRZYKŁADOWE DZIAŁANIE PROGRAMU--------------------------------------------------------
1) Wpisać numer startowej próbki;
2) Pobrać dane "Wczytaj dane";
3) Wcisnąć "START";
4) Po zmianie skali należy ponownie kliknąć "START";
![p2](https://user-images.githubusercontent.com/84225546/123488108-8fc77880-d60f-11eb-8f26-19188e383825.png)

------------------------------------------------------------------ WYGLĄD PLIKU -----------------------------------------------------------------
![dane](https://user-images.githubusercontent.com/84225546/123489321-fcdc0d80-d611-11eb-848d-d0c55ea9e58a.png)
Pierwsze 3 kolumny to dane położenia: (x,y,z), kolejne 3: przyspieszenie (a_x, a_y, a_z), kolejne 3: wskazania magnetometru  (m_x, m_y, m_z),   
kolejne 3: wskazania żyroskopu (omega_x, omega_y, omega_z). Dla działania programu ważne są tylko pierwsze 3 kolumny.                                                          
---------------------------------------------------------------------- KOD ----------------------------------------------------------------------
![k1](https://user-images.githubusercontent.com/84225546/123488890-23e60f80-d611-11eb-92e8-e72e62d9d2d5.png)

Pobieranie danych z pliku "outputPendulumOrt02.log", pętla w.203 pobiera dane z pliku do tablicy tab[], reszte zapisuje w zmiennej smieci      
x = tab[1+3*k],   y = tab[2+3*k],   z = tab[3+3*k];   k - liczba naturalna                                                              
id - ilość wczytanych próbek;

--------------------------------------------------------------------------------------------------------------------------------------------
![k2](https://user-images.githubusercontent.com/84225546/123490453-53e2e200-d614-11eb-921f-8472e2256afa.png)                                 
Rysowanie elementów stałych, niezmiennych (osie, rama kompasu)

--------------------------------------------------------------------------------------------------------------------------------------------
![k3](https://user-images.githubusercontent.com/84225546/123490608-b936d300-d614-11eb-9639-a063e8845fa4.png)                                  
Rysowanie położenia robota na mapie 2D:                                                                                                  
1) Czyszczenie poprzedniedo punktu poprzez naniesienie na niego koloru tła w.262-270;
2) Nanoszenie bieżącego punktu w.272-280;
3) Każda wartości jest mnożona przez "n" czyli skalę zmienianą w w.150-157;

--------------------------------------------------------------------------------------------------------------------------------------------
![k4](https://user-images.githubusercontent.com/84225546/123491012-f3ed3b00-d615-11eb-81ff-43fb3c520c75.png)
Rysowanie strzałki kompasu:
1) Zamiana x,y na współżędne biegunowe w.285-287 x=r*cos(a), y=r*sin(a), a - kąt, r - promień tarczy kompasu;
2) Czyszczenie poprzedniej igły poprzez naniesienie na niego koloru tła w.284,288;
3) Nanoszenie bieżącej igły w.291,295;

--------------------------------------------------------------------------------------------------------------------------------------------
![k5](https://user-images.githubusercontent.com/84225546/123512948-90e9bb80-d68a-11eb-82c6-b2dc071e05a8.png)
![k6](https://user-images.githubusercontent.com/84225546/123512985-ca222b80-d68a-11eb-987f-e3d97862150e.png)

1) temp_tab[0], temp_tab[2], temp_tab[2] - zmienne dla uśrednionych wartosci gestoscX próbek, np.: gdy gestoscX=3 to usredniona wartość 3 próbek;
2) "up" - zlicza ktora probka z rzedu, inkrementuje się o jeden przy kazdym wybiciu timera (25 razy na sekunde);
3) "gestoscX" - sluży do regulacji skali czasu; 
4) if(!(up%gestoscX)) w.321 - dzieki temu funkcja rysowania zachodzi raz na gestoscX razy, np.: gdy gestoscX=3 to funnkcja wykona sie raz na 3 wybicia timera;
5) w.323-326 oraz w.329-331 zapisują w temp_tab[0,2,4] uśrenione wartości gestoscX próbek;
6) w.350-354 - zapisywanie w temp_tab[1,3,5] danych uśrednionych punktu, od którego za gestoscX próbek zacznie się rysowanie linii do kolejnego punktu;
7) w.337-338 - dla wykresu x(t) - ustawianie punktu początkowego rysowania na punkcie poprzednim (temp_tab[1]) i rysowanie lini do bieżącego punktu (temp_tab[0]), każda wielkość pomnożona jest przez zmienna wysY - czyli skalowanie osi Y (więcej w w.158-165);
8) dla wykresów y(t), z(t) - analogicznie;

