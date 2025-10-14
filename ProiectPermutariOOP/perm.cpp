#include "perm.h"
#include <vector>
#include <iostream>

///constructori

permutare::permutare(){
  n=0;
  perm=nullptr;
};

permutare::permutare(unsigned int n){
  (*this).n=n;
  perm= new unsigned int [n+1];
  for(int i = 1; i <= n; i++) 
    perm[i]=i;
}

permutare::permutare(unsigned int v[],unsigned int n){
  (*this).n=n;
  perm = new unsigned int [n+1];
  for(int i = 1; i <= n; i++)
    perm[i] = v[i];
};

permutare::permutare(permutare &p) {
    n = p.n;
    perm = new unsigned int[n + 1];
    for(int i = 1; i <= n; i++)
        perm[i] = p.perm[i];
};

permutare::~permutare(){
  delete []perm;
};


/// metode

void permutare::afisare(){
  for(int i=1; i<=n; i++)
    std::cout<<i<<" ";
  std::cout<<"\n";
  for(int i=1; i<=n; i++)
    std::cout<<perm[i]<<" ";
  std::cout<<"\n";
}

void permutare::setn(unsigned int n){
  if(perm) delete[] perm;
  (*this).n=n;

  perm = new unsigned int[n + 1];

  for(int i = 1; i <= n; i++)
    perm[i] = i;  // permutarea identica
}

void permutare::setnperm(unsigned int n, unsigned int v[]){
  if(perm) delete[] perm;
  (*this).n=n;

  perm = new unsigned int[n + 1];

  for(int i = 1; i <= n; i++)
    perm[i] = v[i];  // copiez permutarea
}

permutare permutare::inversa(){
  permutare r(n);
  for(int i=1; i<=n; i++)
    r.perm[perm[i]]=i;
  return r;
}
int permutare::factorial(int x){
  int f=1;
  for(int i=2; i<=x; i++)f*=i;
  return f;
}

int permutare::numarordine(){
/*
  X   PENTRU NUMAR DE ORDINE FACI CMMMC DE LUNGIMI
  X   DE CICLURI :DDDD
  Y   aparent asta merge doar ca ordinu permutarii in sens matematic ://
*/
  int ordin=1; ///aici monitorizez nr de ordine
  int folosit[100]={0}; ///vector frecventa 
  for(int i=1; i<=n; i++){
      int maimicinefolosite=0; ///destul de clar ce e cred

      for(int j=1; j<perm[i]; j++)
        if(folosit[j]==0)
          maimicinefolosite++;

      ordin += maimicinefolosite*factorial(n-i);
      ///basically cum ne ai invatat sa calculam ordinul la clasa :DD
      folosit[perm[i]]=1;
  }
      return ordin;
}

permutare permutare::urmlexic(){
  /*
    munca de chinez batran </3
  */ 
  permutare next(*this);
  int i=n-1;
  
  while(i>=1 && next.perm[i] > next.perm[i+1])i--;
  if(i<1){
    std::cout<<"aceasta permutare este ultima! \n nu exista urmatoarea permutare! \n";
    std::cout<<"AM RETURNAT PERMUTAREA INTRODUSA! \n";
    return next;
  }

  int j=n;
  while(next.perm[j]<next.perm[i])j--;

  std::swap(next.perm[i],next.perm[j]);

  ///
  int s=i+1; 
  int d=n;
  while(s<d){
    std::swap(next.perm[s],next.perm[d]);
    s++; 
    d--;
  }
  return next;
}

/// ok, asta a fost painful.... 
std::vector<std::vector<unsigned int>> permutare::descompunere() {
    std::vector<std::vector<unsigned int>> cicluri;
    std::vector<bool> vizitat(n + 1, false); // elemente vizitate

    for (unsigned int i = 1; i <= n; i++) {
        if (!vizitat[i]) {
            std::vector<unsigned int> ciclu;
            unsigned int curent = i;

            while (!vizitat[curent]) {
                ciclu.push_back(curent);
                vizitat[curent] = true;
                curent = perm[curent];
            }

            cicluri.push_back(ciclu);
        }
    }

    return cicluri;
}


/// operatori

permutare permutare::operator * (permutare p){
  permutare r(n);
  for(int i=1; i<=n; i++)
    r.perm[i]=perm[p.perm[i]];
  return r;
}

permutare permutare::operator = (permutare p){
  n=p.n;
  perm=new unsigned int[n+1];
  for(int i=1; i<=n; i++)
    perm[i] = p.perm[i];
  return *this;
}

bool permutare::operator==(permutare p) {
    if(n != p.n) return false; ///in caz ca nu au n egal
    for(int i = 1; i <= n; i++)
        if(perm[i] != p.perm[i]) return false;
    return true;
}

bool permutare::operator!=(permutare p) {
    return !(*this == p); ///sper ca vei aprecia asta :DDDD
}

int permutare::operator!() {
    int inv = 0;
    for(int i = 1; i <= n; i++) {
        for(int j = i+1; j <= n; j++) {
            if(perm[i] > perm[j]) inv++;
        }
    }
    return (inv % 2 == 0) ? 1 : -1; // 1 = pară, -1 = impară
}

