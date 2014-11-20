#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "customer.h"

using namespace std;

vector<Customer*> client; //globalny wektor zawierajacy wskaznik na obiekt customer
int marszruty=0,pojemnosc=0,clients=0;



int wczytaj_plik(string file)
{
Customer *customer;
int i,j=0;
string nazwa_instancji,smieci;
fstream plik;

    file+=".txt";
    plik.open(file.c_str(), ios::in);

////////////////////////////////////// czesc naglowkowa ////////////////////////////////////////

    plik >> nazwa_instancji;
    plik >> smieci >> smieci >> smieci; //wczytuje tytuly zmiennych w pliku
    plik >> marszruty >> pojemnosc;
    for(i=0;i<12; i++)
        plik >> smieci; // wczytuje nazwy kolumn

////////////////////////////////////// czesc naglowkowa ////////////////////////////////////////


    i=0;
    int T[10];
    while( true )
    {

       for(j=0;j<7; j++)
          plik >> T[j];
              customer = new Customer(T[0],T[1],T[2],T[3],T[4],T[5],T[6]);
        client.push_back(customer);
        i++;
        if( !(plik.good()) ) // odczytuj dopoki nie ma EOF
             break;
    }
plik.close();
    return (i); //zwroc ilosc klientow
}

double odleglosc(int i_1, int i_2) // pomocnicza funkcja mierzaca odleglosc miedzy dwoma punktami
{
    int X1=(client[i_1]->X),Y1=client[i_1]->Y,X2=client[i_2]->X,Y2=client[i_2]->Y;

    //sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1))
    return sqrt((X2-X1)*(X2-X1)+(Y2-Y1)*(Y2-Y1));
}

bool avalible(int start,double aktualny_czas,int pojemnosc_ciezarowki,double **tab, int koniec)
// pomocnicza funkcja sprawdzajaca czy jest mozliwosc dojazdu
{
    // funkcja sprawdza czy klient nie zakonczyl przyjmowania towaru,
    // czy ciezarowka ma odpowiednia ilosc towaru, oraz czy zdazy dojechac przed zamknieciem depotu
    int zamkniecie_depotu=client[0]->DUE_DATE;
    double czas_dojazdu=tab[start][koniec]+aktualny_czas;
    int gotowosc_klienta=client[koniec]->READY_TIME;
    double dotarcie_do_celu;

    if(czas_dojazdu>gotowosc_klienta)
        dotarcie_do_celu=czas_dojazdu;
    else
        dotarcie_do_celu=gotowosc_klienta;

    if(dotarcie_do_celu<=client[koniec]->DUE_DATE)
    {
        if(dotarcie_do_celu+client[koniec]->SERVICE_TIME+tab[0][koniec]<=zamkniecie_depotu && (pojemnosc_ciezarowki-client[koniec]->DEMAND>=0))
            return 1;
        else
            return 0;
    }
    else
        return 0;

}

int najblizszy(int start, double aktualny_czas, int pojemnosc_ciezarowki, double **tab) // glowna funkcja wyszukajaca najblizszego dostepnego klienta
{
    int MIN=2147483647;
    int x=0;
    for(int i=1; i<clients; i++)
    {
        if (start!=i && tab[start][i]<MIN && !(client[i]->WYKONANY) && avalible(start,aktualny_czas,pojemnosc_ciezarowki,tab,i))
        {
            MIN=tab[start][i];
            x=i;
        }
    }
    return x;
}

int main()
{

    clients=wczytaj_plik("C101"); // clients - liczba klientow

//////////////////////TABLICA ODLEGLOSCI///////////////////////////
//by sprawdzic odleglosc miedzy miastem i a j wystarczy wywolac tab[i][j] lub tab[j][i]
    double **tab = new double *[clients]; // tablica dwuwymiarowa zawierajaca odleglosci miedzy wszystkimi obiektami
    for(int i=0; i<clients; i++)
        tab[i] = new double[clients];

    for(int i=0; i<clients; i++)
        for(int j=0; j<clients; j++)
            tab[i][j]=odleglosc(i,j);

///////////////////////////////////////////////////////////////////
bool straznik=1; //  straznik sprawdza czy da sie dojechac do wszystkich
                 // klientow wysylajac po jednej ciezarowce na klienta

    for(int i=1; i<clients; i++)
    {
        if (!(avalible(0,0,pojemnosc,tab,i)))
        {
            straznik=0;
            break;
        }
    }

////////////////////////////  GLOWNA FUNKCJA  ////////////////////////////////////
if(straznik)
{
    int wykonani=0;
    int aktualny=0,pojemnosc_ciezarowki,cel,ciezarowki=0;
    double aktualny_czas=0,czas=0;
    double droga=0;
    vector<int> wyniki;
    int i=0;


while(wykonani!=(clients-1)) // dopoki liczba odwiedzonych nie bedzie rowna ilosc klientow
{
    if(aktualny==0) //jezeli jestes w DEPO
    {
    czas+=aktualny_czas; //podlicz ogolny czas
    aktualny_czas=0; //wyzeruj czas kolejnej marszruty
    pojemnosc_ciezarowki=pojemnosc; //zaladuj ciezarowke
    ciezarowki+=1; // wyslij kolejna ciezarowke
    droga+=tab[aktualny][0]; // licz droge
    }

    cel=najblizszy(aktualny,aktualny_czas,pojemnosc_ciezarowki,tab); // ustaw nastepny cel
    if(cel!=0) //nie dodajemy DEPOTU do wynikow
        {
           wyniki.push_back(1);
           wyniki[i]=cel;
           i++;
        }
        else
        {
            wyniki.push_back(1);
            wyniki[i]=-1; // do oddzielenia wynikow (problem z wektorem dwuwymiarowym)
            i++;
        }

    if(tab[aktualny][cel]+aktualny_czas>client[cel]->READY_TIME) // czy ciezarowka dojedzie po READY_TIME
    {
        aktualny_czas+=tab[aktualny][cel]+client[cel]->SERVICE_TIME;
    }
    else
    {
        aktualny_czas=client[cel]->READY_TIME+client[cel]->SERVICE_TIME;
    }
    droga+=tab[aktualny][cel];
    pojemnosc_ciezarowki-=client[cel]->DEMAND;
    client[cel]->WYKONANY=1; // flaga
    if (cel!=0)wykonani++;
    aktualny=cel;

} // WHILE - koniec wyszukiwania marszrut

    droga+=tab[cel][0];         //
    aktualny_czas+=tab[cel][0]; // zliczanie ostatniego dojazdu
    czas+=aktualny_czas;        //

////////////////////////////////  ZAPISYWANIE WYNIKU  //////////////////////////////////////


    fstream plik2;
    plik2.setf(cout.fixed);  //notacja zwykla
    plik2.open( "Results.txt", std::ios::out);
    plik2 << ciezarowki << " ";
    plik2 << czas << fixed;
    plik2 << endl;

    int ile=i;
    for(int j=0; j<ile; j++)
    {
        if (wyniki[j]!=-1) // separator marszrut
            plik2 << wyniki[j] << " ";
        else
            plik2 << endl;
    }


    plik2.close();
    for(i=0; i<clients; i++)
        delete client[i]; //zwalnianie dynamicznych struktur

} //IF STRAZNIK
else
{
    fstream plik2;
    plik2.open("Results.txt",std::ios::out);
    plik2 << -1;
    plik2.close();
}

return 0;
}
