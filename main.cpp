/*

TO DO LIST

1. glowny algorytm przeszukujacy
2. funkcja ktora nada ogranicznik by algorytm zatrzymal sie przed okreslonym czasem
3. funkcja tworzaca plik z wynikami
4. sprawozdanie


ostatnie poprawki:

dodalem funkcje ktora okresla czy da sie dojechac do wszystkich klientow oraz mnimalna ilosc marszrut
*/



#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

///////zmienne globalne///////////
int i,j,marszruty,pojemnosc,customers=0;
int ilosc_w_polu=0;
unsigned long long int masa=0;
vector<int> k(0);
vector<vector <int> > wektor(0,k);
vector<int> l(0);
vector<vector <int> > wyniki(0,l);
clock_t finish=clock()+180*CLOCKS_PER_SEC;
int przeszukiwane[100];


//////////////////////////////////
/*
Wektor[0][j] - zawiera informacje o depocie
j=0 podaje ID customera
j=1 podaje wspolrzedne x
j=2 podaje wspolrzedne y
j=3 podaje ilosc zapotrzebowania na dany towar
j=4 podaje czas gotowosci
j=5 podaje "deadline"
j=6 czas obslugi
*/
/////////////////////////////////


int wczytaj_plik(string nazwa_pliku)
{
int indeks=0;
string nazwa_instancji,smieci;
fstream plik;

    nazwa_pliku+=".txt";
    plik.open(nazwa_pliku.c_str(), ios::in);
//  cout << (plik.good()) << endl;


////////////////////////////////////// czesc naglowkowa ////////////////////////////////////////

    plik >> nazwa_instancji;
    plik >> smieci >> smieci >> smieci; //wczytuje tytuly zmiennych w pliku
    plik >> marszruty >> pojemnosc;
    for(i=0;i<12; i++)
        plik >> smieci; // wczytuje nazwy kolumn

////////////////////////////////////// czesc naglowkowa ////////////////////////////////////////


i=0;
while( true )
    {
        k.push_back(1);
        wektor.push_back(vector<int>(8));
        for(j=0;j<7; j++)
          plik >> wektor[i][j];
        i++;
        if( !(plik.good()) ) // EOF?
             break;
    }


plik.close();
    return (i-2); //zwroc ilosc indeksow
}

double odleglosc_miedzy(int i_1, int i_2)
{
    //sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1))
    return sqrt((wektor[i_1][1]-wektor[i_2][1])*(wektor[i_1][1]-wektor[i_2][1])+(wektor[i_1][2]-wektor[i_2][2])*(wektor[i_1][2]-wektor[i_2][2]));

}

double max(double x, double y)
{
    if(x>y)
        return x;
    else
        return y;
}

int najblizszyf(int i_1, int *aktywne,double **tab)
{
    double MIN=2147483646;
    int x=0;


    for(i=1; i<=customers+1; i++)
    {
        if(MIN>tab[j][aktywne[i]])
        {
            MIN=tab[j][aktywne[i]];
            x=aktywne[i];
        }
    }
    return x;

}

int deadline(int *tab)
{
   int MIN=wektor[tab[1]][5];
   j=tab[1];
    for(i=2; i<=customers+1; i++)
        if(MIN>wektor[i][5])
        {
        MIN=wektor[i][5];
        j=i;
        }

    return j;
}

void wypisz_wczytany_plik()
{
                for(i=0; i<=customers+1; i++)
            {
                for(j=0; j<=7; j++)
                    cout << wektor[i][j] << " ";
                cout << endl;
            }

}

int obiekty_w_polu(int i, int *tab, int *przeszukiwane)
{
int x=wektor[i][1];
int y=wektor[i][2];
int xdepo=wektor[0][1];
int ydepo=wektor[0][2];
j=1;
       if(abs(x-xdepo)>abs(y-ydepo))
        {
        for(i=1; i<=customers+1; i++)
            if((wektor[i][1]>=xdepo && wektor[i][1]<=x) || (wektor[i][1]<=xdepo && wektor[i][1]>=x))
            {
                przeszukiwane[j]=i;
                j++;
            }
        }
    else
        {
        for(i=1; i<=customers+1; i++)
            if((wektor[i][2]>=ydepo && wektor[i][2]<=y) || (wektor[i][2]<=ydepo && wektor[i][2]>=y))
            {
                przeszukiwane[j]=i;
                j++;
            }
        }
        return j;
}

int najblizszy_w_polu(int j,int *przeszukiwane, double **tab)
{
    double MIN=2147483646;
    int x=0;


    for(i=1; i<ilosc_w_polu; i++)
    {
        if(przeszukiwane[i]!=0 && MIN>tab[j][przeszukiwane[i]])
        {
            MIN=tab[j][przeszukiwane[i]];
            x=przeszukiwane[i];
        }
    }
    return x;
}



int main()
{

    customers=wczytaj_plik("C101");
    //cout << (customers+1) << endl; // czy plik zostal otwarty poprawnia oraz zwraca ilosc customerow
                               // (jezeli customers==-1 to znaczy ze plik nie zostal poprawnie otwarty)

    double **tab = new double *[customers+2]; // tablica dwuwymiarowa zawierajaca odleglosci miedzy wszystkimi obiektami
    for(i=0; i<=customers+1; i++)
        tab[i] = new double[customers+2];

    for(i=0; i<=customers+1; i++)
        for(j=0; j<=customers+1; j++)
            tab[i][j]=odleglosc_miedzy(wektor[i][0],wektor[j][0]);
             // by sprawdzic odleglosc miedy miastem x a y nalezy wpisac tab[x][y];

    bool straznik=1;
    while(straznik)
        {

////////////////////// funkcja sprawdzajaca czy da sie dojechac do wszystkich klientow ///////////////////

        for(i=1; i<=customers+1; i++)
        {
            if((max(tab[0][i],wektor[i][4])+wektor[i][6]+tab[0][i])<=wektor[0][5] && wektor[i][3]<=pojemnosc);
            // max(odleglosc miedzy punktami, czas gotowosci) + odleglosc miedzy punktami + czas zaladowywania <= deadline depozytu
            // && czy zapotrzebowanie nie jest wieksze od pojemnosci wozu
            else
            {
                straznik=0;
                break;
            }
        }

        if (straznik==0) break;

///////////////////////////////wylicza minimalna ilosc wozow ////////////////////////////////////////////

        for(i=1; i<=customers+1; i++)
        {
            masa+=wektor[i][3];
        }
        double minimalna=(double)masa/pojemnosc;
        if (round(minimalna)!=minimalna)
            minimalna=floor(minimalna)+1;

////////////////////////////////////////////////////////////////////////////////////////////////////////

        while(finish>clock())
        {
            int * wykonani=new int[customers+1];
            int * aktywni=new int[customers+1];
            int * przeszukiwane=new int[customers+1];

            for(i=1; i<=customers+1; i++)
                aktywni[i]=i;

            int ilosc_aktywnych=i-1;
            int ilosc_wykonanych=0;

          while(ilosc_aktywnych>0)
            {

               int cel=deadline(aktywni); // wylicza cel
                cout << cel << endl;
                ilosc_w_polu=obiekty_w_polu(cel,aktywni,przeszukiwane); //wpisuje do tablicy przeszukiwane obiekty ktore beda brane pod uwage
                /*for(i=1; i<ilosc_w_polu; i++)
                    cout << przeszukiwane[i] << " ";
                cout << najblizszy_w_polu(0,przeszukiwane,tab) << endl;*/
                int najblizszy=najblizszy_w_polu(0,przeszukiwane,tab);
                int poprzedni=0;
                int aktualna_pojemnosc=pojemnosc-wektor[cel][3];
                l.push_back(2);
                wyniki.push_back(vector<int>(2));
                int indeks=0,marszruta=0;




            while(poprzedni!=cel)
                if (((max(tab[najblizszy][poprzedni],wektor[najblizszy][4])+wektor[najblizszy][6])<=wektor[najblizszy][5]) || tab[najblizszy][cel]==0)
                    if(((((max(tab[najblizszy][poprzedni],wektor[najblizszy][4])+wektor[najblizszy][6])+
                        max(tab[najblizszy][cel],wektor[cel][4])+wektor[cel][6])<=wektor[cel][5]) && aktualna_pojemnosc>=wektor[najblizszy][3])
                         || tab[najblizszy][cel]==0)
                {
                    cout << najblizszy << " ";
                    aktualna_pojemnosc-=wektor[najblizszy][3];
                    wyniki.push_back(vector<int>(2));
                    wyniki[marszruta][indeks]=najblizszy;
                    indeks++;
                    aktywni[najblizszy]=0;
                    for(j=1; j<=customers; j++)
                        if(przeszukiwane[j]==najblizszy)
                        przeszukiwane[j]=0;
                    poprzedni=najblizszy;
                    najblizszy=najblizszy_w_polu(poprzedni,przeszukiwane,tab);

                }
                else
                {
                    for(j=1; j<=customers; j++)
                    if(przeszukiwane[j]==najblizszy)
                    przeszukiwane[j]=0;
                    najblizszy=najblizszy_w_polu(poprzedni,przeszukiwane,tab);
                }

                cel=0;

                najblizszy=najblizszyf(poprzedni,aktywni,tab);

                while(poprzedni!=cel)
/*
Wektor[0][j] - zawiera informacje o depocie
j=0 podaje ID customera
j=1 podaje wspolrzedne x
j=2 podaje wspolrzedne y
j=3 podaje ilosc zapotrzebowania na dany towar
j=4 podaje czas gotowosci
j=5 podaje "deadline"
j=6 czas obslugi
*/

                if (((max(tab[najblizszy][poprzedni],wektor[najblizszy][4])+wektor[najblizszy][6])<=wektor[najblizszy][5]) || tab[najblizszy][0]==0)
                //    if(((((max(tab[najblizszy][poprzedni],wektor[najblizszy][4])+wektor[najblizszy][6])+
                 //       max(tab[najblizszy][0],wektor[0][4])+wektor[0][6])<=wektor[0][5]) && aktualna_pojemnosc>=wektor[najblizszy][3])
                  //       || tab[najblizszy][0]==0)
                {
                    cout << najblizszy << " ";
                    aktualna_pojemnosc-=wektor[najblizszy][3];
                    wyniki.push_back(vector<int>(2));
                    wyniki[marszruta][indeks]=najblizszy;
                    indeks++;
                    aktywni[najblizszy]=0;
                    poprzedni=najblizszy;
                    najblizszy=najblizszyf(poprzedni,aktywni,tab);

                }
                else
                {


                    najblizszy=najblizszyf(poprzedni,aktywni,tab);
                    cout << najblizszy << endl;
                }








              break;
            }



            break;
        }



        break;
       }//WHILE STRAZNIK




        if (straznik==0)
            cout << "Brak poprawnych odpowiedzi" << endl;
            else
            cout << "jest ok" << endl;





    return 0;
}
