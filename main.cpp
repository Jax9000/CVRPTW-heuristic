#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "customer.h"

using namespace std;

vector<Customer*> client(0);
int marszruty=0,pojemnosc=0,clients=0;


int wczytaj_plik(string file)
{
int indeks=0;
Customer *customer;
int i,j=0;
string nazwa_instancji,smieci;
fstream plik;

    file+=".txt";
    plik.open(file.c_str(), ios::in);
//  cout << (plik.good()) << endl;


////////////////////////////////////// czesc naglowkowa ////////////////////////////////////////

    plik >> nazwa_instancji;
    plik >> smieci >> smieci >> smieci; //wczytuje tytuly zmiennych w pliku
    plik >> marszruty >> pojemnosc;
    for(i=0;i<12; i++)
        plik >> smieci; // wczytuje nazwy kolumn

////////////////////////////////////// czesc naglowkowa ////////////////////////////////////////


i=0;
int a;
int T[10];
while( true )
    {



       for(j=0;j<7; j++)
          plik >> T[j];
        for(j=0; j<7; j++)
           cout << T[j] << " ";
       cout << endl;
        customer = new Customer(T[0],T[1],T[2],T[3],T[4],T[5],T[6]);
        client.push_back(customer);


        i++;
        if( !(plik.good()) ) // EOF?
             break;
    }


plik.close();
    return (i); //zwroc ilosc indeksow
}

double odleglosc(int i_1, int i_2)
{
    int X1=(client[i_1]->X),Y1=client[i_1]->Y,X2=client[i_2]->X,Y2=client[i_2]->Y;

    //sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1))
    return sqrt((X2-X1)*(X2-X1)+(Y2-Y1)*(Y2-Y1));
}

bool avalible(int start,int aktualny_czas,int pojemnosc_ciezarowki,double **tab, int koniec)
{
    //max(tab[0][i],wektor[i][4])+wektor[i][6]+tab[0][i])<=wektor[0][5] && wektor[i][3]<=pojemnosc)
    int zamkniecie_depotu=client[0]->DUE_DATE;
    double czas_dojazdu=tab[start][koniec]+aktualny_czas;
    int gotowosc_klienta=client[koniec]->READY_TIME;

    double dotarcie_do_celu;
    if(czas_dojazdu>gotowosc_klienta)
        dotarcie_do_celu=czas_dojazdu;
    else
        dotarcie_do_celu=gotowosc_klienta;
 //   cout << "dotarcie: " << dotarcie_do_celu+client[koniec]->SERVICE_TIME+tab[0][koniec] << " ";
    if(dotarcie_do_celu<=client[koniec]->DUE_DATE)
    {
    if(dotarcie_do_celu+client[koniec]->SERVICE_TIME+tab[0][koniec]<=zamkniecie_depotu)
        return 1;
    else
        return 0;
    }
    else
        return 0;

}

int najblizszy(int start, int aktualny_czas, int pojemnosc_ciezarowki, double **tab)
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
    {
        for(int j=0; j<clients; j++)
        {
            tab[i][j]=odleglosc(i,j);
//            cout << tab[i][j] << " ";
        }
//        cout << endl;
    }

///////////////////////////////////////////////////////////////////
bool straznik=1;
for(int i=1; i<clients; i++)
{
    if (!(avalible(0,0,pojemnosc,tab,i)))
        {
            straznik=0;
            break;
        }
}

 cout << "czy isntieje wynik: " <<straznik << endl;

 cout << najblizszy(3,0,pojemnosc,tab) << endl;





return 0;
}
