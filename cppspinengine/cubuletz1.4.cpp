// ROTATIN shape tudur ver 1.4
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
#include <cmath>
#include <iostream>
#include <chrono>
#include <thread>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>
#include <cstring>

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
        
        std::vector<Vec3> puncte;
        std::vector<Vec3> temp;
        std::vector<Vec2> projected;
        std::vector<std::pair<int,int>> edges;

        float cameradistance = 10.0f;
        float scale = 35.0f;
        float thetax,thetay,thetaz; // propria rotatie (future feature)
    public: 
        float offsetx, offsety, offsetz;
        float speedxaxis =12;
        float speedyaxis =15;
        float speedzaxis =18;
        ///constructori
            /// default cube
        shape(){
            l = 3;
            puncte.push_back({-l,-l,-l});
            puncte.push_back({ l,-l,-l});
            puncte.push_back({ l, l,-l});
            puncte.push_back({-l, l,-l});
            puncte.push_back({-l,-l, l});
            puncte.push_back({ l,-l, l});
            puncte.push_back({ l, l, l});
            puncte.push_back({-l, l, l});

            edges = {
            {0,1},{1,2},{2,3},{3,0},
            {4,5},{5,6},{6,7},{7,4},
            {0,4},{1,5},{2,6},{3,7}
            };

            temp.resize(puncte.size());
            projected.resize(puncte.size());
        };
        shape(const std::string& type){
            offsetx = 0;
            offsety = 0;
            offsetz = 0;

            if(type == "cub"){
                puncte.push_back({ l,-l,-l});
                puncte.push_back({ l, l,-l});
                puncte.push_back({-l, l,-l});
                puncte.push_back({-l,-l, l});
                puncte.push_back({ l,-l, l});
                puncte.push_back({ l, l, l});
                puncte.push_back({-l, l, l});

                edges = {
                {0,1},{1,2},{2,3},{3,0},
                {4,5},{5,6},{6,7},{7,4},
                {0,4},{1,5},{2,6},{3,7}
                }; 
            }

            if(type == "tetraedru"){
                l = 3;
                puncte = {{ l, l, l}, {-l,-l, l}, {-l, l,-l}, { l,-l,-l}};
                edges = {{0,1},{1,2},{2,0},{0,3},{1,3},{2,3}};
            }

            if(type=="piramida"){
                l = 4;
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
            if(type=="mishu"){
                l = 3;
                offsety -=0.5;
                puncte = {
                    {-2, -2, 0}, { 2, -2, 0}, { 2, 1.5, 0}, {-2,  1.5, 0}, // face (cube)
                    {-1, -3, 0}, { 0, -2, 0}, { 1, -3, 0}, // ears
                    {-0.75, -1.5, 0}, {-0.75, -1, 0}, // eyes (stanga)
                    { 0.75, -1.5, 0}, { 0.75, -1, 0}, // eyes (dreapta)
                    {-0.75, 0, 0}, {-3.5, -1, 0}, {-3.5, 1.25, 0}, //whiskers stanga
                    {0.75, 0, 0}, {3.5, -1, 0}, {3.5, 1.25, 0}, //whiskers stanga
                    {0.5, 0, 0},{-0.5, 0, 0},{0, 0.5, 0} //botic :D
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
            temp.resize(puncte.size());
            projected.resize(puncte.size());
        }
        ///setPuncte()
        ///setMuchii()

        Vec3 getPoint(int i){
            return puncte[i];
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
            int size =  puncte.size();

            clearScreen();

            // desenez colutirile varului...
            for(int i=0; i<size; i++){
                int x = projected[i].x2 + W/2;
                int y = projected[i].y2 + H/2;
                putPixel(x, y, '#');
            }

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
    shape cub("mishu"); // de aici alegi ce forma da render
    int n = cub.getSize();
    Vec3 t; // auxiliare
    Vec2 p; // auxiliare
    /// setez de aici numarul de colturi ale formei si dau resize la vectori
    while(true){
        H = getTermH(); // functiile astea             la dimensiunile 
        W = getTermW(); //                  dau fetch                   terminalului
        /// parcurg toti indicii si ii rotesc in 3d (rotation matrices)
        /// dupa ii 'proiectez' in plan 2d
        for(int i=0; i<n; i++){
            t = cub.transformpoint(cub.getPoint(i));
            t.y -= cub.offsety;
            t.y += cub.offsetx;
            t.z += cub.offsetz;
            p = cub.projectedpoint(t);
            cub.setTemp(i,t);
            cub.setProjected(i,p);
        }
        cub.render();       // -> iau punctele si trasez linii intre ele
        
        // delta este (alfa') functiilor trigonometrice cu care det punctele 3d
        
        // asa facean inainte da e greu de schimbat viteza asa :D
        // deltax +=0.012;
        //deltay +=0.015;
        // deltaz +=0.018;

        cub.speedxaxis = 0;
        cub.speedzaxis = 0;
        cub.speedyaxis = 15;

        deltax += cub.speedxaxis/1000;
        deltay += cub.speedyaxis/1000;
        deltaz += cub.speedzaxis/1000;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        //linia asta doar modifica cat de des dai refresh la display, 
        //nu modifica viteza de spin!
    }
    return 0;
}