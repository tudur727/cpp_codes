// ROTATIN shape tudur ver 1.1
// just the basics!

/// what's new?
/// array -> vector de puncte (mai usor de gestionat punctele)
/// foarte usor de introdus orice forma geometrica doresti sa invarti
/// acum cube[] nu mai exista, acum este shape[]
/// am modificat functiile transform si project
/// (acuma ele functioneaza pe cate un punct individual nu pe toate pe rand)

#include <cmath>
#include <iostream>
#include <chrono>
#include <thread>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>

// functiile folosite ca sa dau center la cub
// mereu dau fetch la dimensiunea actuala a terminalului
// si daca dai resize cand ruleaza, se reactualizeaza pozitia!
int getTermW() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}

int getTermH() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;
}


char screen[500][500]; //matricea in care stochezi chestiile de se afiseaza later on

float cameradistance = 10;
float deltax = 0.0f;
float deltay = 0.0f;
float deltaz = 0.0f;

/// punct 3d
struct Vec3{
    float x,y,z;
};
/// punct 2d
struct Vec2{
    int x2,y2;
};



// latura cubului si dimensiunile terminalului 
//(ca sa dau center la cubuletz sa fie frumos)
int l = 3;
int H;
int W;
int n; //nr de colturi -> size(shape)


/// cubu reference (pe asta il inmultesti cu delta (alfa) si stochezi in temp \/ )
std::vector <Vec3> shape= {   {-l,-l,-l}, { l,-l,-l},
                             { l, l,-l}, {-l, l,-l},
                             {-l,-l, l}, { l,-l, l},
                             { l, l, l}, {-l,l, l}
                         };
std::vector<Vec3> temp; // aici stochez vertexurile cubului care apare pe ecran
std::vector<Vec2> rend;

void clearScreen(){
    for(int y=0; y<H; y++)
        for(int x=0; x<W; x++)
            screen[y][x] = ' ';
}

void putPixel(int x, int y, char c){
    if(x >= 0 && x < W && y >= 0 && y < H)
        screen[y][x] = c;
}
// pretty obvious deseneaza linie de la a la b
void drawLine(int x1, int y1, int x2, int y2){
    int dx = x2 - x1;
    int dy = y2 - y1;

    int steps = std::max(abs(dx), abs(dy));
    float sx = dx / (float)steps;
    float sy = dy / (float)steps;

    float x = x1;
    float y = y1;

    for(int i=0; i<=steps; i++){
        putPixel((int)x, (int)y, '#');
        x += sx;
        y += sy;
    }
}


void display(){
    // ANSI escape codes:
    // \x1b[2J  -> clear screen
    // \x1b[H   -> move cursor to top-left
    std::cout << "\x1b[2J\x1b[H";

    for(int y = 0; y < H; y++){
        for(int x = 0; x < W; x++){
            std::cout << screen[y][x];
        }
        std::cout << '\n';
    }
    std::cout.flush();
}

/*  FUNCTIE VECHE DE ROTATIE DOAR PE Y AXIS
    MERGE, DOAR CA NU ARE SENS DOAR PE Y
void TransformPoint(){
    /// in jurul axei Y
    for(int i=0; i<8; i++){
        float auxx,auxy,auxz;
        auxx=shape[i].x*cos(delta) + cube[i].z*sin(delta);
        auxz=-shape[i].x*sin(delta) + cube[i].z*cos(delta);
        shape[i].x=auxx;
        shape[i].z=auxz;
    }
}*/


Vec3 TransformPoint(Vec3 p) {
    Vec3 r;
        float x = p.x;
        float y = p.y;
        float z = p.z;

        // --- Rotirea în jurul axei X ---
        float y1 = y * cos(deltax) - z * sin(deltax);
        float z1 = y * sin(deltax) + z * cos(deltax);

        y = y1;
        z = z1;

        // --- Rotirea în jurul axei Y ---
        float x2 = x * cos(deltay) + z * sin(deltay);
        float z2 = -x * sin(deltay) + z * cos(deltay);

        x = x2;
        z = z2;

        // --- Rotirea în jurul axei Z ---
        float x3 = x * cos(deltaz) - y * sin(deltaz);
        float y3 = x * sin(deltaz) + y * cos(deltaz);

        x = x3;
        y = y3;

        // dupa modific temp (nu modific cubul default niciodata)
        // initial modificam shape (inconsistenta in viteza + prostie)
        r.x = x;
        r.y = y;
        r.z = z;
        return r;
}

Vec2 ProjectPoint(int nrcolt){
    float factor; Vec2 colt;
    factor = 1.0f / (temp[nrcolt].z + cameradistance);
    colt.x2 = temp[nrcolt].x * factor * 35;
    colt.y2 = temp[nrcolt].y * factor * 35;
    return colt;
}

void render(){
    clearScreen();

    // desenez colutirile varului...
    for(int i=0; i<n; i++){
        int x = rend[i].x2 + W/2;
        int y = rend[i].y2 + H/2;
        putPixel(x, y, '#');
    }

    // desenez muchiile lui cubuletzz
    int edges[][2] = {
        {0,1},{1,2},{2,3},{3,0},
        {4,5},{5,6},{6,7},{7,4},
        {0,4},{1,5},{2,6},{3,7}
    };

    for(int e=0; e<12; e++){
        int a = edges[e][0];
        int b = edges[e][1];

        int x1 = rend[a].x2 + W/2;
        int y1 = rend[a].y2 + H/2;
        int x2 = rend[b].x2 + W/2;
        int y2 = rend[b].y2 + H/2;

        /// fara linia asta apar doar colturile cubului :DDD
        drawLine(x1, y1, x2, y2);
    }

    display();    
}

int main(){
    n = size(shape);
    temp.resize(n);
    rend.resize(n);
    /// setez de aici numarul de colturi ale formei si dau resize la vectori
    while(true){
        H = getTermH(); // functiile astea             la dimensiunile 
        W = getTermW(); //                  dau fetch                   terminalului
        /// parcurg toti indicii si ii rotesc in 3d (rotation matrices)
        /// dupa ii 'proiectez' in plan 2d
        for(int i=0; i<n; i++){
            temp[i] = TransformPoint(shape[i]);
            rend[i] = ProjectPoint(i);
        }
        render();       // -> iau punctele si trasez linii intre ele
        
        // delta este (alfa') functiilor trigonometrice cu care det punctele 3d
        deltax +=0.012;
        deltay +=0.015;
        deltaz +=0.018;
        

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        //linia asta doar modifica cat de des dai refresh la display, 
        //nu modifica viteza de spin!
    }
    return 0;
}

