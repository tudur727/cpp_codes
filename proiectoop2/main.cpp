#include "matrice.h"
#include <iostream>

int main() {
    matrice A, B;

    std::cout << "Introduceti prima matrice:\n";
    std::cin >> A;

    std::cout << "Introduceti a doua matrice:\n";
    std::cin >> B;

    std::cout << "\nA:\n" << A;
    std::cout << "\nB:\n" << B;

    matrice C = A + B;
    std::cout << "\nA + B:\n" << C;

    matrice D = A - B;
    std::cout << "\nA - B:\n" << D;

    matrice E = A * B;
    std::cout << "\nA * B:\n" << E;

    matrice F = A * 2;
    std::cout << "\nA * 2:\n" << F;

    std::cout << "\nDeterminant A: " << A.det() << "\n";
    std::cout << "Determinant B: " << B.det() << "\n";

    if(A.det() != 0) {
        matrice Ainv = !A;
        std::cout << "\nInversa lui A:\n" << Ainv;
    } else {
        std::cout << "\nA nu are inversa\n";
    }

    std::cout << "\nRotire A cu 90 grade:\n";
    A.roteste(90);
    std::cout << A;

    std::cout << "\nRotire A cu 180 grade:\n";
    A.roteste(180);
    std::cout << A;

    std::cout << "\nRotire A cu 270 grade:\n";
    A.roteste(270);
    std::cout << A;

    std::cout << "\nExemplu exceptie rotatie matrice \n";
    std::cout << "Rotire A cu 60 grade:\n";
    A.roteste(60);
    std::cout << A;
    std::cout<<"\n(codul nu modifica matricea deloc daca nu o poate roti asa ca ramane ce era inainte)";

    return 0;
}
