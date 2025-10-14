#include <iostream>
#include <vector>
#include "perm.h"
///omg ce librarie poznasa!? perm.h?

/*
    de facut:
    -metoda urmciclu
    -metoda numar ordine 
    -operator citire?
    -ceva exemplu pentru clasa mea
    -idk \_('_')_/
*/

int main(){
    std::cout<<"teste set n si set n si perm: \n";
    ///test constructor + set(n)
    permutare p1;
    p1.setn(4);
    std::cout<<"permutare identica de 4 elemente: \n";
    p1.afisare();

    ///test set(n,permutare)
    unsigned int v2[5]={0,2,3,1,4};
    ///am pus 0 pentru ca indexez de la 1 
    permutare p2;
    p2.setnperm(4,v2);
    std::cout<<"permutare setat n=4 si elem 2,3,1,4: \n";
    p2.afisare();
    
    //test constructor de copiere
    permutare p3(p2);
    std::cout<<"permutare p3(p2): \n";
    p3.afisare();

    //test operator *
    permutare produs = p1*p2;
    std::cout<<"compunerea p1*p2= \n";
    produs.afisare();

    //test operator =
    permutare p4;
    p4 = p2;
    std::cout << "permutarea p4 dupa atribuire p2: \n";
    p4.afisare();

    //test operatori == si !=
    std::cout << "p2 == p4 ? " << (p2 == p4 ? "DA" : "NU") << "\n";
    std::cout << "p2 != p1 ? " << (p2 != p1 ? "DA" : "NU") << "\n\n";

    //test operator ! (semnatura)
    std::cout << "Semnatura permutarii p2: " << (!p2) << "\n\n";

    //test descompunerea in cicluri
    std::cout << "Descompunerea in cicluri pentru p2: \n";
    std::vector<std::vector<unsigned int>> cicluri = p2.descompunere();

    for (size_t i = 0; i < cicluri.size(); i++) {
        std::cout << "(";
        for (size_t j = 0; j < cicluri[i].size(); j++) {
            std::cout << cicluri[i][j];
            if (j + 1 < cicluri[i].size()) std::cout << " ";
        }
        std::cout << ") ";
    }
    std::cout << "\n";


    //test urmatoarea permutare
    std::cout << "\nUrmatoarea permutare dupa p2: \n";
    permutare urm = p2.urmlexic();
    urm.afisare();

    //test numar de ordine
    std::cout << "Numarul de ordine al lui p2: \n" << p2.numarordine() << "\n";

    //test inversa
    std::cout << "Inversa permutarii p2: \n";
    permutare inv = p2.inversa();
    inv.afisare();

    std::cout << "\n \n Sfarsit demonstratie \n";
    
    return 0;
}
