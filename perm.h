#ifndef PERM_H
#define PERM_H
#endif


#include <vector>
#include <iostream>

class permutare{
  private:
    unsigned int n;
    unsigned int *perm; //alocare dinamica vector de permutare

    int factorial(int x);///am nevoie de asta la nr ordine
    ///am facut privat pentru ca am nevoie de ea doar in clasa
    ///si nu am vrut sa fac functie globala...
  public:

    ///constructori

    permutare();
    permutare(unsigned int n);
    permutare(unsigned int v[],unsigned int n);
    permutare(permutare &p);
    ~permutare();
    
    /// metode 
    void afisare();
    void setn(unsigned int n); ///set n 
    void setnperm(unsigned int n, unsigned int v[]); ///set n si permutare
    std::vector<std::vector<unsigned int>> descompunere();
    //arata urat (zic eu) dar fac o functie care returneaza 
    //un vector de vectori care tin variabile de tip unsigned int
    //nu am facut cu array ca sa nu aloc multa memorie degeaba
    int numarordine();
    permutare urmlexic();
    permutare inversa();

    ///operatori

    permutare operator *(permutare p);
    permutare operator =(permutare p);

    int operator !();
    bool operator ==(permutare p);
    bool operator !=(permutare p);

};