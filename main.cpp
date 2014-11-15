#include <iostream>
#include <vector>
#include <fstream>
#include "customer.h"

using namespace std;

vector<Customer*> wektor(0);
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
        wektor.push_back(customer);


        i++;
        if( !(plik.good()) ) // EOF?
             break;
    }


plik.close();
    return (i); //zwroc ilosc indeksow
}




int main()
{

    int x=wczytaj_plik("C101"); // x zawira ilosc klientow
    for(int i=0; i<x; i++)
    cout << wektor[i]->Y << endl;

}
