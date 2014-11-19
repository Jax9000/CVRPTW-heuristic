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
int marszruty,pojemnosc=0;


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



int main()
{

    int clients=wczytaj_plik("C101"); // clients - liczba klientow

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







return 0;
}
