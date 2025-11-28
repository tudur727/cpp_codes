// ROTATIN shape tudur ver 1.6
// just the basics!

/// what's new? (1.1)
/// array -> vector de puncte (mai usor de gestionat punctele)
/// foarte usor de introdus orice forma geometrica doresti sa invarti
/// acum cube[] nu mai exista, acum este shape[]
/// am modificat functiile transform si project
/// (acuma ele functioneaza pe cate un punct individual nu pe toate pe rand)

/// what's new? (1.2)
/// - rescriu totul sub forma unei clase "shape"
///     -> se pot crea usor mai multe "shapes"
///     -> coduld devine mai usor de citit / folosit
/// - am adaugat offset pentru shape-uri doar ca trebuie sa dezactivezi centrarea ca sa poate functiona....

/// what's new? (1.3)
/// - constructori - cu parametri
/// - moduri diferite de display (doar modifici cum se modifica delta x,y,z)
/// - new shapes!

/// what's new? (1.4)
/// - mishu shape! (cam patratos...)
/// (una din versiunile urmatorare va da revamp la mishu! o sa devina mai frumos XDD)
/// - in loop, rotatiile tin de o viteza pe care o poti atribuii individual unui obiect
/// (variabile publice ce tin de viteza invartiri obiectului) 
/// no functionality update

///what's new? (1.5)
/// - non blocking input (putem sa facem chestii funny acuma XDD)
/// - quit key! (q)
/// - autospin on / off si manual spining
/// - ability to change the shape whilerunning! (   cycle momentan :(    )
/// - functie noua (privata a clasei) de set type! acuma nu mai e asa urat codul :D

///what's new? (1.6)
/// - fixed minor issues with the shapes from previous versions...
/// - added camera distance modifier feature (distanta fata de camera)
/// - MANUAL / apasa m / -> pentru ca codu deja are cam multe binduri....

#include <cmath>
#include <iostream>
#include <chrono>
#include <thread>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>
#include <cstring>
#include <termios.h>
#include <fcntl.h>

void enableNonBlockingInput() {
    termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);

    // disable canonical mode & echo
    ttystate.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

    // make stdin non-blocking
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

int getKey() {
    int ch = getchar();
    if (ch != EOF) return ch;
    return -1; // no input
}

void manual(){
    system("clear");
    std::cout<<"\n \n \n";
    std::cout<<"      Welcome to the Manual    /    Bine ai venit in sectiunea Manualului \n \n \n";
    std::cout<<"  - Q - quit / pentru a inchide aplicatia \n";
    std::cout<<"  - R - cycle through the shapes / schimbi forma \n";
    std::cout<<"  - P - autospin toggle (into manual mode) / rotire automata, sau rotire manuala \n";
    std::cout<<"  - W A S D - free rotation keys / taste pentru rotatie libera \n";
    std::cout<<"  - [ & ] - change the camera distance / modifici distanta camerei \n";
    std::cout<<"       (keep camera distance between 7 and 12 for best experience) ";
    std::cout<<"\n \n \n  - M - return / paraseste aceasta pagina";
    while(true){
        if(getKey() == 'm')
            return;
    }
}

char screen[500][500]; //matricea in care stochezi chestiile de se afiseaza later on

float deltax = 0.0f;
float deltay = 0.0f;
float deltaz = 0.0f;

// latura cubului si dimensiunile terminalului 
//(ca sa dau center la cubuletz sa fie frumos)
int H;
int W;
int n; //nr de colturi -> size(shape)


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

/// definesc punctele 3,2d
struct Vec3{
    float x,y,z;
};
/// punct 2d
struct Vec2{
    int x2,y2;
};

class shape{
    private:
        float l; ///lungimea cubului / shape

        std::vector<std::string> types = {"cub","piramida","tetraedru","mishu"};
        std::string type;
        int currentType;

        std::vector<Vec3> puncte;
        std::vector<Vec3> temp;
        std::vector<Vec2> projected;
        std::vector<std::pair<int,int>> edges;

        float cameradistance = 10.0f;
        float scale = 35.0f;
        float thetax,thetay,thetaz; // propria rotatie (future feature)
                                    // aparent nu am folosit unghi diferit pentru
                                    // autospin si manual spin deci maybe for next feature :D

        void makeCub(){
            currentType =0;
            l = 3;
            offsety -= 0.2;
            puncte = {
                {-l,-l/2,-l},{ l, -l/2, -l},{ l, l/2,-l},{-l, l/2,-l},
                {-l,-l/2, l},{ l,-l/2, l},{ l, l/2, l},{-l, l/2, l}
            };

            edges = {
            {0,1},{1,2},{2,3},{3,0},
            {4,5},{5,6},{6,7},{7,4},
            {0,4},{1,5},{2,6},{3,7}
            };
        }

        void makeTetra(){
            l = 3.5;
            currentType = 2;
            offsety -=2;
            float h = (sqrt(3)/2)*l;
            puncte = {{ 0, 0, h}, {-l, 0,-h}, {l, 0,-h}, { 0,-l,0}};
            edges = {{0,1},{1,2},{2,0},{0,3},{1,3},{2,3}};   
        }

        void makePiramida(){
            l = 3;
            currentType = 1;
            offsety -= 1.5;
            puncte = {
                {-l, 0, -l}, { l, 0, -l}, { l, 0, l}, {-l, 0, l}, // baza
                {0, -l, 0} // vârful
            };
            edges = {
                {0,1},{1,2},{2,3},{3,0}, // muchii baza
                {0,4},{1,4},{2,4},{3,4}  // muchii spre vârf
            };            
        }

        void makeMishu(){
            l = 0; // l doesnt matter ca mishu nu scaleaza cu l \/
            currentType = 3;
            offsety -=1;
            puncte = {
                {-2, -2, 0}, { 2, -2, 0}, { 2, 1.5, 0}, {-2,  1.5, 0}, // face (cube)
                {-1, -3, 0}, { 0, -2, 0}, { 1, -3, 0}, // ears
                {-0.75, -1.5, 0}, {-0.75, -1, 0}, // eyes (stanga)
                { 0.75, -1.5, 0}, { 0.75, -1, 0}, // eyes (dreapta)
                {-0.75, 0, 0}, {-3.5, -1, 0}, {-3.5, 1.25, 0}, //whiskers stanga
                {0.75, 0, 0}, {3.5, -1, 0}, {3.5, 1.25, 0}, //whiskers stanga
                {0.5, 0, 0},{-0.5, 0, 0},{0, 0.5, 0} //botic :d
            };

            edges = {
                /*{0,1},*/ {1,2}, {2,3}, {3,0}, // cube face
                {0,4}, {4,5},{5,6},{6,1}, //ears
                {7,8},{9,10}, // eyes (stanga + dreapta)
                {11,12},{11,13}, //whiskers stanga
                {14,15},{14,16}, //whiskers dreapta
                {17,18},{17,19},{18,19}
            };
        }

        void setType(std::string type){
            puncte.clear();
            edges.clear(); //curat vectorii ca sa nu am puncte/edges vechi...
            
            offsetx = offsety = offsetz = 0; //restez starea de rotatie si offsetul
            deltax = deltay = deltaz =0 ;

            (*this).type = type; 

            if(type == "cub") makeCub();
            if(type == "tetraedru") makeTetra();
            if(type == "piramida") makePiramida();
            if(type == "mishu") makeMishu();

            temp.resize(puncte.size());      //ma asigur ca toate chestiile au ce size trebuie
            projected.resize(puncte.size());
            //std::cout<<"AAAAAAAGH \n";  //<-- debug antic... era 'n' sau acuma VERTEXES din main

        }
    public: 
        float offsetx, offsety, offsetz;
        float speedxaxis =12;
        float speedyaxis =15;
        float speedzaxis =18;
        ///constructori
            /// fara parametru -> cub  // cu parametru -> forma selectata
        shape(){
            setType("cub");
            temp.resize(puncte.size());
            projected.resize(puncte.size());
        };

        shape(const std::string type){
            (*this).type = type;
            setType(type);
            temp.resize(puncte.size());
            projected.resize(puncte.size()); 
        };
        ///setPuncte()
        ///setMuchii()     <-- astea sunt useless acuma...

        void CameraDistanceModifier(float x){
            cameradistance += x;
        }
        
        float GetCameraDistance(){
            return cameradistance;
        }

        Vec3 getPoint(int i){
            return puncte[i];
        }

        std::string getType(){
            return type;
        }

        void setTemp(int i, const Vec3& t){
            temp[i] = t;
        }

        void setProjected(int i, const Vec2& p){
            projected[i] = p;
        }

        int getSize(){
            return puncte.size();
        }

        void cycleType(){
            currentType = ( currentType  + 1) % types.size();
            setType(types[currentType]);
            //aici am avut prima data problema ca mi crapa din vector
            //mai bine ca abia am facut codul frumos... aveam scris aici
            // 50 de linii numa cu shape - uri....
        }

        Vec3 transformpoint(const Vec3& p){
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

        Vec2 projectedpoint(const Vec3& p){
            float factor = 1.0f / (p.z + cameradistance);
            Vec2 colt;
            colt.x2 = static_cast<int>(p.x * factor * 35);
            colt.y2 = static_cast<int>(p.y * factor * 35);
            return colt;
        }
        
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

        void render(){
            //debuguri antice, merita de tinut aici... nici macar nu le vedeam ca si output
            //std::cout<<"AM INTRAT IN RENDER \n";
            int size =  puncte.size();
            //std::cout<<"AM TRECUT DE PUNCTE.SIZE()  \n";
            clearScreen();
            //std::cout<<"AM TRECUT DE CLEAR SCREEN \n";
            // desenez colutirile varului...
            for(int i=0; i<size; i++){
                int x = projected[i].x2 + W/2;
                int y = projected[i].y2 + H/2;
                putPixel(x, y, '#');
            }
            //std::cout<<"MARIANMARIANMAIRANASDADAD \n";
            // desenez muchiile lui cubuletzz

            for(const auto& edge : edges){
                int a = edge.first;
                int b = edge.second;
                
                if (a>= size || b>=size) continue;

                int x1 = projected[a].x2 + W/2;
                int y1 = projected[a].y2 + H/2;
                int x2 = projected[b].x2 + W/2;
                int y2 = projected[b].y2 + H/2;

                /// fara linia asta apar doar colturile cubului :DDD
                drawLine(x1, y1, x2, y2);
            }
            display();    
        }

};


 /// din ver 1.2 functiile sunt doar in clasa.... 
 /// (rip functie old de rotatie)

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

int main(){
    shape cub("cub"); // de aici alegi ce forma da render
    int VERTEXES = cub.getSize();
    bool autospin = true;
    Vec3 t; // auxiliare
    Vec2 p; // auxiliare
    enableNonBlockingInput();
    cub.speedxaxis = 0;
    cub.speedzaxis = 0;
    cub.speedyaxis = 15;
    /// setez de aici numarul de colturi ale formei si dau resize la vectori

    while(true){
        H = getTermH(); // functiile astea             la dimensiunile 
        W = getTermW();///                  dau fetch                   terminalului
        //cu functia asta caut/astept input 
        int key = getKey();

        if(key=='q'){
            // momentan e bugged!
            std::cout << "\033[0m";       // reset colors
            std::cout << "\033[?25h";     // show cursor
            std::cout.flush();
            system("clear");
            system("reset");
            return 0;
        }

        if(key == 'p')
            autospin = (autospin - 1)*(-1);
        if (autospin){
                deltax += cub.speedxaxis/1000;
                deltay += cub.speedyaxis/1000;
                deltaz += cub.speedzaxis/1000;
        }else{
            if(key == 'd')
                deltay -= 0.15;
            if (key == 'a')
                deltay += 0.15;
            if (key == 'w')
                deltax -= 0.15;
            if (key == 's')
                deltax += 0.15;
            
            
        }
        if(key=='r'){
            cub.cycleType();
            VERTEXES = cub.getSize();
            /// MEREU SA DAI UPDATE LA SIZE CAND SCHIMBI CEVA...
        }
        if (key == ']')
            cub.CameraDistanceModifier(0.5);
        if (key == '[')
            cub.CameraDistanceModifier(-0.5);
        if (key == 'm')
            manual();

        /// parcurg toti indicii si ii rotesc in 3d (rotation matrices)
        /// dupa ii 'proiectez' in plan 2d
        for(int i=0; i<VERTEXES; i++){
            t = cub.transformpoint(cub.getPoint(i));
            t.y -= cub.offsety;
            t.y += cub.offsetx;
            t.z += cub.offsetz;
            p = cub.projectedpoint(t);
            cub.setTemp(i,t);
            cub.setProjected(i,p);
            //std::cout<<"MARIAN \n";
        }
        cub.render();       // -> iau punctele si trasez linii intre ele
        std::cout<<"shape: "<<cub.getType()<<"\n";
        std::cout<<"autospin: "<<autospin<<"\n";
        std::cout<<"distance: "<<cub.GetCameraDistance()<<"\n";
        // delta este (alfa') functiilor trigonometrice cu care det punctele 3d 
        // asa facean inainte da e greu de schimbat viteza asa :D
        // deltax +=0.012;
        //deltay +=0.015;
        // deltaz +=0.018;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        //linia asta doar modifica cat de des dai refresh la display, 
        //nu modifica viteza de spin!
    }
    return 0;
}