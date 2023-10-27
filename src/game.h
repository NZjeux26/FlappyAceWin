#ifndef _GAME_H_
#define _GAME_H_

void gameGsCreate(void);
void gameGsLoop(void);
void gameGsDestroy(void);
void updateScore(void);
void highScoreCheck(void);
short getHighScore(void);

typedef struct g_obj {//struct for player parameters
    short x;        //position X
    short y;        //position Y
    short w;        //Rectangle width
    short h;        //Rectangle height
    short xvel;     //x velocity
    short yvel;     //y velocity
    short health;   //health
    short colour;    
} g_obj;

#endif // _GAME_H_