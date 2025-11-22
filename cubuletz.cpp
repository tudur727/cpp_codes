// ROTATIN CUBE tudur ver 1.0
#include <cmath>
#include <iostream>
#include <chrono>
#include <thread>

#include <sys/ioctl.h>
#include <unistd.h>

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


int W;
int H;



char screen[500][500];

float thetax,thetay,thetaz;
float cameradistance = 10;
float deltax = 0.0f;
float deltay = 0.0f;
float deltaz = 0.0f;

struct Vec3{
    float x,y,z;
};
struct Vec2{
    int x2,y2;
};



// latura cubului
int l = 3;




Vec3 cube[]={ {-l,-l,-l}, { l,-l,-l},
              { l, l,-l}, {-l, l,-l},
              {-l,-l, l}, { l,-l, l},
              { l, l, l}, {-l,l, l}
            };
Vec3 temp[8];
Vec2 rend[8];

void clearScreen(){
    for(int y=0; y<H; y++)
        for(int x=0; x<W; x++)
            screen[y][x] = ' ';
}

void putPixel(int x, int y, char c){
    if(x >= 0 && x < W && y >= 0 && y < H)
        screen[y][x] = c;
}

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

/*void transform(){
    /// in jurul axei Y
    for(int i=0; i<8; i++){
        float auxx,auxy,auxz;
        auxx=cube[i].x*cos(delta) + cube[i].z*sin(delta);
        auxz=-cube[i].x*sin(delta) + cube[i].z*cos(delta);
        cube[i].x=auxx;
        cube[i].z=auxz;
    }
}*/


void transform() {
    for (int i = 0; i < 8; i++) {
        float x = cube[i].x;
        float y = cube[i].y;
        float z = cube[i].z;

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

        // Save back
        temp[i].x = x;
        temp[i].y = y;
        temp[i].z = z;
    }
}

void project(){
    float factor;
    for(int i=0; i<8; i++){
        factor = 1.0f / (temp[i].z + cameradistance);
        rend[i].x2 = temp[i].x * factor * 35;
        rend[i].y2 = temp[i].y * factor * 35;
    }
}

void render(){
    clearScreen();

    // draw vertices
    for(int i=0; i<8; i++){
        int x = rend[i].x2 + W/2;
        int y = rend[i].y2 + H/2;
        putPixel(x, y, '#');
    }

    // draw edges
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

        drawLine(x1, y1, x2, y2);
    }

    display();    
}

int main(){
    while(true){
        //mai intati calculez noile puncte
        //dupa le 'aplatizez' (le transform in 2d)
        transform();
        project();
        render();
        H = getTermH();
        W = getTermW();
        deltax +=0.012;
        deltay +=0.015;
        deltaz +=0.018;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return 0;
}

