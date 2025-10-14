#ifndef MATRICE_H
#define MATRICE_H
#endif

#include <iostream>


class matrice{
    private:
        //variabilele clasei

        int n;
        int **a;
        
        //metoda de alocare ca sa incalc DRY 

        void alocdinamic(int n);
        void dealocdinamic();

        //metode de rotire, o sa vezi de ce le am facut asa...

        void rot90();
        void rot180();
        void rot270();

        //metoda pentru aflarea unei submatrici fara linia i si coloana j

        matrice submat(int x, int y);

    public:

        //constructori

        matrice();
        matrice(int n);
        matrice(matrice &other);
        ~matrice();

        //operatori

        matrice operator = (matrice &m);
        matrice operator + (matrice &m);
        matrice operator - (matrice &m);
        matrice operator * (matrice &m); // inmultire matrice
        matrice operator * (int k); // inmultire scalar
        matrice operator ! ();

        // operatori citire / afisare 
        
        friend std::istream & operator >> (std::istream &in, matrice &m);
        friend std::ostream & operator << (std::ostream &out, matrice &m);

        //metode 

        double det(); //determinant notat ca la matematica
        void roteste(int r); //self explanatory



};