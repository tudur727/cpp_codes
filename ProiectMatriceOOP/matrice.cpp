#include "matrice.h"
#include <iostream>


// -------------------METODE PRIVATE ALE CLASEI :DDDD-----------------

//metodele mele de alocare :DDD + metode de rotire + metoda submatrice

//metode alocare / delete pentru matrice

void matrice::alocdinamic(int n){
    (*this).n=n;
    
    a = new int*[n+1];

    for(int i=0; i<=n; i++)
        a[i] = new int[n+1];
}

void matrice::dealocdinamic(){
    if(a){
        for(int i=0; i<=n; i++)
            delete[] a[i];
        delete[] a;
        a = nullptr;
    }
    n=0;
}

//metode rotire

void matrice::rot90(){
    for(int i = 1; i <= n/2; i++) {
        for(int j = i; j <= n-i; j++) {
            std::swap(a[i][j], a[n-j+1][i]);
            std::swap(a[i][j], a[n-i+1][n-j+1]);
            std::swap(a[i][j], a[j][n-i+1]);
        }
    }
}

void matrice::rot180(){
    for(int i = 1; i <= n/2; i++) {
        for(int j = 1; j <= n; j++) {
            std::swap(a[i][j], a[n-i+1][n-j+1]);
        }
    }
    if(n % 2 == 1) {
        int mid = (n+1)/2;
        for(int j = 1; j <= n/2; j++)
            std::swap(a[mid][j], a[mid][n-j+1]);
    }
}

void matrice::rot270(){
    for(int i = 1; i <= n/2; i++) {
        for(int j = i; j <= n-i; j++) {
            std::swap(a[i][j], a[j][n-i+1]);
            std::swap(a[i][j], a[n-i+1][n-j+1]);
            std::swap(a[i][j], a[n-j+1][i]);
        }
    }
}

// metoda submatrice fara linia x si coloana y (det si !)

matrice matrice::submat(int x,int y){
    matrice m(n-1);
    int mi = 1; // i in noua mat
    for(int i = 1; i <= n; i++) {
        int mj = 1; // j in noua mat
        for(int j = 1; j <= n; j++) {
            // doar copiez ce aveam in matricea originala (nu sunt pe lin x sau col y)
            if(i != x && j != y) {
                m.a[mi][mj] = a[i][j];
                mj++;
            }
        }
        if(i != x) mi++;
    }
    return m;
}

//---------------------------constructori----------------------

matrice::matrice(){ // <---- implicit
    n=0;
    a=nullptr;
};

matrice::matrice(int n){ // <---- cu parametru (toate elem devin 0)
    alocdinamic(n);
    for(int i=0; i<=n; i++)
        for(int j=0; j<=n; j++)
            a[i][j]=0;
};

matrice::matrice(matrice &m){ // <---- de copiere
    alocdinamic(m.n);
    for(int i=0; i<=n; i++)
        for(int j=0; j<=n; j++)
            a[i][j]=m.a[i][j];
};

matrice::~matrice(){
    dealocdinamic();
};

// ----------------------opeatori ~ citire / afisare---------------

std::istream& operator >> (std::istream &in, matrice &m){
    //mai intai sterg matricea veche (daca exista, evident)
    m.dealocdinamic();
    /*
        dupa citesc dimensiunea si aloc dinamic iarasi matricea m;
        scriu asta pentru ca am dat mess up inainte </3
        (uimitor, stiu)
    
        aveam asa:
        in>>m.n;
        m.dealocdinamic();
        m.alocdinamic();

        si nu mergea pentru ca in "dealocdinamic()"
        am o linie care zice ca "n=0;" 
        am stat 10 minute pentru prostia asta </3
    
    */
    in>>m.n;
    m.alocdinamic(m.n);

    for(int i=1; i<=m.n; i++)
        for(int j=1; j<=m.n; j++)
            in>>m.a[i][j];
    
    return in;
}

std::ostream& operator << (std::ostream &out, matrice &m){
    for(int i=1; i<=m.n; i++){
        for(int j=1; j<=m.n; j++)
            out<<m.a[i][j]<<" ";
        out<<"\n";
    }
    return out;
}

// --------------------------operatori------------------------


matrice matrice::operator = (matrice &m){
    if (this == &m)return *this;

    dealocdinamic();
    alocdinamic(m.n);
    
    for(int i=1; i<=n; i++)
        for(int j=1; j<=n; j++)
            a[i][j]=m.a[i][j];
    return *this;
}

matrice matrice::operator + (matrice &m){
    if(n!=m.n){
        std::cout<<"Dimensiunile matricilor sunt diferite, nu se pot aduna! \n";
        return *this;
    }

    matrice s(n);

    for(int i=1; i<=n; i++)
        for(int j=1; j<=n; j++)
            s.a[i][j]=a[i][j]+m.a[i][j];

    return s;
}

matrice matrice::operator - (matrice &m){
    if(n!=m.n){
        std::cout<<"Dimensiunile matricilor sunt diferite, nu se pot aduna! \n";
        return *this;
    }

    matrice d(n);

    for(int i=1; i<=n; i++)
        for(int j=1; j<=n; j++)
            d.a[i][j]=a[i][j]-m.a[i][j];

    return d;
}

matrice matrice::operator * (matrice &m){
    matrice p(n);
    for(int i=1; i<=n; i++){
        for(int j=1; j<=n; j++){
            int x=0; // in x tin suma produselor

            for(int c=1; c<=n; c++)
                x=x+a[i][c]*m.a[c][j];

            p.a[i][j]=x;
        }
    }
    return p;
}

matrice matrice::operator * (int k){ // inmultire cu scalar
    matrice pscalar(n);

    for(int i=1; i<=n; i++)
        for(int j=1; j<=n; j++)
            pscalar.a[i][j]=a[i][j]*k;

    return pscalar;
}
matrice matrice::operator ! (){
    double determinant = det();
    if(determinant == 0) {
        std::cout<< "Matricea nu are inversa! \n";
        return matrice(n); // returnează matrice zero sau goală
    }

    matrice inv(n);

    if(n == 2) {
        inv.a[1][1] =  a[2][2] / determinant;
        inv.a[1][2] = -a[1][2] / determinant;
        inv.a[2][1] = -a[2][1] / determinant;
        inv.a[2][2] =  a[1][1] / determinant;
    }
    else if(n == 3) {
        // calculează cofactorii
        inv.a[1][1] =  (a[2][2]*a[3][3] - a[2][3]*a[3][2]) / determinant;
        inv.a[1][2] = -(a[2][1]*a[3][3] - a[2][3]*a[3][1]) / determinant;
        inv.a[1][3] =  (a[2][1]*a[3][2] - a[2][2]*a[3][1]) / determinant;

        inv.a[2][1] = -(a[1][2]*a[3][3] - a[1][3]*a[3][2]) / determinant;
        inv.a[2][2] =  (a[1][1]*a[3][3] - a[1][3]*a[3][1]) / determinant;
        inv.a[2][3] = -(a[1][1]*a[3][2] - a[1][2]*a[3][1]) / determinant;

        inv.a[3][1] =  (a[1][2]*a[2][3] - a[1][3]*a[2][2]) / determinant;
        inv.a[3][2] = -(a[1][1]*a[2][3] - a[1][3]*a[2][1]) / determinant;
        inv.a[3][3] =  (a[1][1]*a[2][2] - a[1][2]*a[2][1]) / determinant;
    }

    return inv;
}
// de facut operator !

// ----------------------metode (2 la numar)-------------------
    // A.det()
    // A.rotire(r) r-grade 90 180 270 si multiplii

void matrice::roteste(int r){
    r = ((r%360)+360)%360;
    // ok deci il aduc pe r in rangeu 0 359 
    // stiu ca mergea direct din r%360
    // dar asa merge si pentru valori negative
    switch (r){
        case 0:
            std::cout<<"matricea ramane la fel \n";
            break;

        case 90:
            rot90();
            break;

        case 180:
            rot180();
            break;

        case 270:
            rot270();
            break;

        default:
            std::cout<<"rotatia aleasa este invalida \n";
            break;
    }
}

double matrice::det(){
    if(n == 1) return a[1][1];

    double determinant = 0;
    for(int j = 1; j <= n; j++) {
        int semn = (j % 2 == 0) ? -1 : 1; // o schema ca sa nu mai scriu un if aditional
        determinant += semn * a[1][j] * submat(1,j).det();
    }
    return determinant;
    //puteam sa iau si particular pentru n==2 si n==3 dar arata urat codul
    //si oricum nu trebuie sa calculeze multe sub matrici pentru 2,3 anyways...
}