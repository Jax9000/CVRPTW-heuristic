#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "customer.h"

using namespace std;

vector<Customer*> client;
int marszruty=0,pojemnosc=0,clients=0;



int wczytaj_plik(string file)
{
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
int T[10];
while( true )
    {



       for(j=0;j<7; j++)
          plik >> T[j];
      /*  for(j=0; j<7; j++)
           cout << T[j] << " ";
       cout << endl;*/
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

bool avalible(int start,double aktualny_czas,int pojemnosc_ciezarowki,double **tab, int koniec)
{
    //max(tab[0][i],wektor[i][4])+wektor[i][6]+tab[0][i])<=wektor[0][5] && wektor[i][3]<=pojemnosc)
    int zamkniecie_depotu=client[0]->DUE_DATE;
    double czas_dojazdu=tab[start][koniec]+aktualny_czas;
    int gotowosc_klienta=client[koniec]->READY_TIME;
    //max{(tab[start][koniec]+aktualny_czas,client[koniec]->READY_TIME);
    double dotarcie_do_celu;
    if(czas_dojazdu>gotowosc_klienta)
        dotarcie_do_celu=czas_dojazdu;
    else
        dotarcie_do_celu=gotowosc_klienta;
 //   cout << "dotarcie: " << dotarcie_do_celu+client[koniec]->SERVICE_TIME+tab[0][koniec] << " ";
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

int najblizszy(int start, double aktualny_czas, int pojemnosc_ciezarowki, double **tab)
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
if(straznik)
{
    int wykonani=0;
    int aktualny=0,pojemnosc_ciezarowki,cel,ciezarowki=0;
    double aktualny_czas=0,czas=0;
    double droga=0;
    vector<int> wyniki;


    int i=0,k=0;


while(wykonani!=(clients-1))
{
    if(aktualny==0)
    {
    czas+=aktualny_czas;
    aktualny_czas=0;
    pojemnosc_ciezarowki=pojemnosc;
    ciezarowki+=1;
    droga+=tab[aktualny][0];
    }

    cel=najblizszy(aktualny,aktualny_czas,pojemnosc_ciezarowki,tab);
    if(cel!=0)
        {
           wyniki.push_back(1);
           wyniki[i]=cel;
           i++;
        }
        else
        {
            wyniki.push_back(1);
            wyniki[i]=-1;
            i++;
        }

//   cout << "pojemnosc ciezarowki dla " << aktualny << " wynosi " << pojemnosc_ciezarowki <<endl;
//    max{(tab[aktualny][cel]+aktualny_czas),client[cel]->READY_TIME});

    if(tab[aktualny][cel]+aktualny_czas>client[cel]->READY_TIME)
    {
        aktualny_czas+=tab[aktualny][cel]+client[cel]->SERVICE_TIME;
    }
    else
    {
        aktualny_czas=client[cel]->READY_TIME+client[cel]->SERVICE_TIME;
    }
    droga+=tab[aktualny][cel];
    pojemnosc_ciezarowki-=client[cel]->DEMAND;
    client[cel]->WYKONANY=1;
    if (cel!=0)wykonani++;
    aktualny=cel;

}
droga+=tab[cel][0];
aktualny_czas+=tab[cel][0];
czas+=aktualny_czas;
aktualny_czas>czas ? czas=aktualny_czas : 0;
cout.setf( ios::fixed );
cout.precision(5);

double suma=0;
for(int i=1; i<clients; i++)
    suma+=client[i]->DEMAND;


    fstream plik2;
    plik2.setf(cout.fixed);
    plik2.open( "Results.txt", std::ios::out);
    plik2 << ciezarowki << " ";
    plik2 << czas << fixed;
    plik2 << endl;
int ile=i;
cout << ile << endl;

    for(int j=0; j<ile; j++)
    {
    //    plik2 << j << ": ";
        if (wyniki[j]!=-1)
        plik2 << wyniki[j] << " ";
    else
        plik2 << endl;
    }


    plik2.close();
    for(i=0; i<clients; i++)
        free(client[i]);




} //IF STRAZNIK
else
    cout << -1 << endl;

return 0;
}
