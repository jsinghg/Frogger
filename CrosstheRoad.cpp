#include <iostream>
#include <deque>
#include <stdlib.h>
#include <stdbool.h>
#include <vector>
#include <ncurses.h>
using namespace std;

// Initiates the player and sets the initial position of the player

class player {
public:
    int pos_x;
    int pos_y;

    //Constructor for player

    player(int width){
        pos_x = width/2;
        pos_y = 0;
    }
};

//Initiates all the lanes (produces all the "rows")

class lanes{
private:
    deque<bool> cars;
    bool right;
public:
    // Constructor for lanes,
    lanes(int width)
    {
        for(int i = 0; i < width; i++)
        {
            if(rand()%10 == 2){
                cars.push_front(true);
            }
            else
            cars.push_front(false);
        }
    }

    void change_state()
    {
        if(rand() % 10 == 1)
        {
            cars.push_front(true);
        }
        else
            cars.push_front(false);
        cars.pop_back();
    }

    void Move()
    {
        if (right)
        {
            cars.push_front(rand() % 10 == 1);
            cars.pop_back();
        }
        else
        {
            cars.push_back(rand() % 10 == 1 ? true : false);
            cars.pop_front();
        }

    }

    bool check_pos(int pos)
    {
        return cars[pos];
    }

    void ChangeDirection() { right = !right; }
};


class game{

private:
    bool quit;
    int no_of_lanes;
    int width;
    player *pl1;
    vector <lanes*> map;

public:
    // Setter:

    game(int w = 20, int h= 20){
        no_of_lanes = h;
        width = w;
        quit = false;
        for(int i = 0; i <= no_of_lanes; i++)
        {
            map.push_back(new lanes(width));
        }
        pl1 = new player(width);
    }


//     Functions:
    void draw(){
        clear();
        for(int i = 0; i < no_of_lanes; i++){
            for(int j = 0; j < width; j++){
                if (i == 0 && (j == 0 || j == width - 1)) printw("S");
                if (i == no_of_lanes - 1 && (j == 0 || j == width - 1)) printw("F") ;
                if(map[i]->check_pos(j) && i != 0 && i != no_of_lanes-1 )
                    printw("#") ;
                else if(pl1->pos_x == j && pl1->pos_y == i) {
                    printw("A") ;
                }
                else printw(" ");

            }
            printw("\n") ;
        }

    }


    void input(){
        char ch;
        nodelay(stdscr, TRUE);
        if ((ch = getch()) != ERR) {
        if(ch == 'a')
            pl1->pos_x--;
        if(ch == 'd')
            pl1->pos_x++;
        if(ch == 'w')
            pl1->pos_y--;
        if(ch == 's')
            pl1->pos_y++;
        if(ch == 'q' )
            quit = true;
        }

    }


    void Logic(int &level){
        int diff;
        if(level == 1) diff = 4000;
        if(level == 2) diff = 3000;
        if(level == 3) diff = 2000;
        if(level == 4) diff = 1500;
        if(level == 5) diff = 1000;
        if(level > 5) diff -= 200;

        printw("Level : %d",level);
        printw("Level : %d",level);
        getch();
        for (int i = 1; i < no_of_lanes - 1; i++){
            if (rand() % diff == 1)
                map[i]->Move();
            if (map[i]->check_pos(pl1->pos_x) && pl1->pos_y == i)
                quit = true;
        }
        if (pl1->pos_y == no_of_lanes - 1){
            level++;
            pl1->pos_y = 0;
            map[rand() % no_of_lanes]->ChangeDirection();
        }
    }


    void run(int & level){
        while(!quit){
            draw();

            input();

            Logic(level);
            if(quit){
                nodelay(stdscr, false);
                printw("Game Over");
                char garbage =getch();
            }
        }

    }
};

using namespace std;
int main(int argc, char* argv[]){
    int level = 0;
    if(argc == 1 ){
        level = 1;
    } else {
      level = atoi(argv[1]);
    }
    level = level > 5? 5: level;
    level = level <= 0? 1: level;
    initscr();
    refresh();
    noecho();
    cbreak();
    game g(30,5);
    g.run(level);
    endwin();
}

