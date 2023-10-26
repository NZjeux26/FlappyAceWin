#include "game.h"
#include "states.h"
#include <ace/managers/key.h>
#include <ace/managers/game.h>
#include <ace/managers/system.h>
#include <ace/managers/viewport/simplebuffer.h>
#include <ace/managers/blit.h> // Blitting fns
#include <ace/managers/state.h>
#include <ace/utils/file.h>
#include <ace/utils/font.h>
#include <ace/utils/string.h>

#define false 0
#define true 1
#define SCORE_COLOR 1
#define WALL_HEIGHT 1
#define WALL_COLOR 1
#define PADDLE_SPEED 4
//-------------------------------------------------------------- NEW STUFF START
//AMiga Pal 320x256
#define PLAYFIELD_HEIGHT (256-32) //32 for the top viewport height
#define PLAYFIELD_WIDTH (320)

static tView *s_pView; // View containing all the viewports
static tVPort *s_pVpScore; // Viewport for score
static tSimpleBufferManager *s_pScoreBuffer;
static tVPort *s_pVpMain; // Viewport for playfield
static tSimpleBufferManager *s_pMainBuffer;

g_obj player; //player object declaration
tFont *fallfontsmall; //global for font
tTextBitMap *scoretextbitmap;//global for score text
char scorebuffer[20];
int gSCORE = 0;
int g_highScore = 0; //needs to be assigned prior to initialization

ULONG startTime;
UBYTE g_scored = false;


void gameGsCreate(void) {
  s_pView = viewCreate(0,
    TAG_VIEW_GLOBAL_PALETTE, 1,
  TAG_END);
  
  // Viewport for score bar - on top of screen
  s_pVpScore = vPortCreate(0,
    TAG_VPORT_VIEW, s_pView,
    TAG_VPORT_BPP, 4,
    TAG_VPORT_HEIGHT, 32,
  TAG_END);
  s_pScoreBuffer = simpleBufferCreate(0,
    TAG_SIMPLEBUFFER_VPORT, s_pVpScore,
    TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR,
  TAG_END);

  // Now let's do the same for main playfield
  s_pVpMain = vPortCreate(0,
    TAG_VPORT_VIEW, s_pView,
    TAG_VPORT_BPP, 4,
  TAG_END);
  s_pMainBuffer = simpleBufferCreate(0,
    TAG_SIMPLEBUFFER_VPORT, s_pVpMain,
    TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR,
  TAG_END);
  //palette with VBB set to four i can get 16-colours
  s_pVpScore->pPalette[0] = 0x0000; // First color is also border color 
  s_pVpScore->pPalette[1] = 0x0888; // Gray
  s_pVpScore->pPalette[2] = 0x0800; // Red - not max, a bit dark
  s_pVpScore->pPalette[3] = 0x0008; // Blue - same brightness as red 
  s_pVpScore->pPalette[4] = 0x0080; // Green - same brightness as blue
  s_pVpScore->pPalette[5] = 0xFFC0; // Yellow
  s_pVpScore->pPalette[6] = 0xFFFF; // white

  // Draw line separating score VPort and main VPort, leave one line blank after it
  blitLine(
    s_pScoreBuffer->pBack,
    0, s_pVpScore->uwHeight - 2,
    s_pVpScore->uwWidth - 1, s_pVpScore->uwHeight - 2,
    SCORE_COLOR, 0xFFFF, 0 // Try patterns 0xAAAA, 0xEEEE, etc.
  );
  gSCORE = 99999999; 
  g_highScore = 0;//getHighScore();  //get the highscore from the file, returning zero if no file
  char i_highScore[20]; //buffer string to hold the highscore
  stringDecimalFromULong(g_highScore, i_highScore); //convert to short

  fallfontsmall = fontCreate("myacefont.fnt");//create font

  tTextBitMap *highscorebitmap = fontCreateTextBitMapFromStr(fallfontsmall, "High Score: ");
  fontDrawTextBitMap(s_pScoreBuffer->pBack, highscorebitmap, 0,10, 6, FONT_COOKIE);
  highscorebitmap = fontCreateTextBitMapFromStr(fallfontsmall, i_highScore);  //reuse the bitmap from writing the highscore text  
  fontDrawTextBitMap(s_pScoreBuffer->pBack, highscorebitmap, 73,10, 6, FONT_COOKIE); //write out the highscore

  tTextBitMap *textbitmap = fontCreateTextBitMapFromStr(fallfontsmall, "Score: ");
  fontDrawTextBitMap(s_pScoreBuffer->pBack, textbitmap, 0,20, 6, FONT_COOKIE);
  
  stringDecimalFromULong(gSCORE, scorebuffer);
  scoretextbitmap = fontCreateTextBitMapFromStr(fallfontsmall, scorebuffer); //redo bitmap
  
  startTime = timerGet();
  player.x = 10;
  player.y = (s_pVpMain->uwHeight - player.h) / 2;
  player.w = 10;
  player.h = 10;
  player.colour = 5;

  systemUnuse();
  // Load the view
  viewLoad(s_pView);
}

/*The following is a breakdown of the logic of the collides() function:
The first condition checks if the x-coordinate of the first block is less than the x-coordinate of the second block plus the width of the second block. This ensures that the first block is not to the right of the second block.
The second condition checks if the x-coordinate of the first block plus the width of the first block is greater than the x-coordinate of the second block. This ensures that the first block is not to the left of the second block.
The third condition checks if the y-coordinate of the first block is less than the y-coordinate of the second block plus the height of the second block. This ensures that the first block is not above the second block.
The fourth condition checks if the y-coordinate of the first block plus the height of the first block is greater than the y-coordinate of the second block. This ensures that the first block is not below the second block.
If all of these conditions are met, then the two blocks collide.*/
//collision detection function
UBYTE Collision(g_obj *a, g_obj *b)
{
  return (a->x < b->x + b->w && a->x + a->w > b->x && a->y < b->y + b->h && a->y + a->h > b->y);
}

//need a 2nd block counter first one is the max and the second is the current amount
short BLOCKS = 2;
void gameGsLoop(void) {
  // This will loop every frame
  if(keyCheck(KEY_ESCAPE)) {
    gameExit();
  }
  else {
  
  //undraw player
  blitRect( 
    s_pMainBuffer->pBack,
    player.x, player.y,
    player.w, player.h, 0
    );

  //**Move things down**

  if(keyCheck(KEY_D)){  //move player right
    player.x = MIN(player.x + PADDLE_SPEED, 275);
  }
  if(keyCheck(KEY_A)){  //move player left
    player.x = MAX(player.x - PADDLE_SPEED, 0);
  }

 //**Draw things**
  //time_t currentTime = time(&currentTime);  //gets the current time and if 15 has past add another block
   ULONG  currentTime = timerGet();
  if(currentTime - startTime >= 300){
    BLOCKS++;
    startTime = currentTime;
  }
 // Redraw the player at new position
  blitRect(
    s_pMainBuffer->pBack, player.x, player.y,
      player.w, player.h, player.colour
  );
  
  if (g_scored){  //if the player ahs scored, set to false and update the score
    g_scored = false;
    updateScore(); //moved into function for code clarity
  }
  vPortWaitForEnd(s_pVpMain);
  }
 
}

void gameGsDestroy(void) {
  logBlockBegin("gameGsDestroy");
  systemUse();
  // This will also destroy all associated viewports and viewport managers
  viewDestroy(s_pView);
  logBlockEnd("gameGsDestroy");
}

void updateScore(void) {  //bug seems to appear where text for 10000 + seems to be erroring with: ERR: Text '10000' doesn't fit in text bitmap, text needs: 33,8, bitmap size: 32,8
    gSCORE = gSCORE + 100;  //add score
    stringDecimalFromULong(gSCORE, scorebuffer);
    blitRect(s_pScoreBuffer->pBack, 40, 20, scoretextbitmap->uwActualWidth, scoretextbitmap->uwActualHeight, 0); //erase scorebuffer
    fontFillTextBitMap(fallfontsmall, scoretextbitmap, scorebuffer);//refill
    fontDrawTextBitMap(s_pScoreBuffer->pBack, scoretextbitmap, 40,20, 6, FONT_COOKIE);  //draw
}

// void highScoreCheck(void) {
//   int score = gSCORE;
//   char charScore[30];
//   systemUse();
//   char filename[20] = "scoresheet.txt";
    
//   if(!fileExists(filename)){  //check if the file exists, if not create and add the score
//       tFile *file = fileOpen(filename, "w");
//       stringDecimalFromULong(score,charScore);
//       fileWriteStr(file, charScore);//add the score to the file 
//       fileClose(file); 
//   }
//   else{//if file exsits, read the score chec against the player score, if greater write to file else do nothing
//     tFile *file = fileOpen(filename, "a");
//     char tempscore[10];
//     short tScore;
//     fileRead(file,tempscore,10);//read the score
//     //function to return highest score in the file.

//     //tScore = strtol(tempscore, NULL, 10);//convert to short.
//     tScore = getHighScore();
//     logWrite("TScore Is: %d\n", tScore);
//     if(score > tScore){//if score is greater than the current HS then add it to the end.
//       stringDecimalFromULong(score,charScore);
//       fileWriteStr(file, "\n");
//       fileWriteStr(file,charScore);
//       fileClose(file);
//     }
//     else fileClose(file);//else do nothing
//   }
//   systemUnuse();
// }
// //reads through the scoresheet to find the highest score and returns that to compare it with the current score by the player
// short getHighScore(void){
//   char filename[20] = "scoresheet.txt";
//   int highScore = 0;
//   if((!fileExists(filename))) return 0;

//   tFile *file = fileOpen(filename, "r");
//   if(!file) return 101; //set to 1 so i know it was a cope out

//   char tline[512];
//   while(fgets(tline, 512, file)){//issues getting the tokens into the array of lines
//     char *token = strtok(tline, "\n");
//     while(token){
//       logWrite("File Reading: %s\n", token); 
//       int tScore = strtol(token, NULL, 10);  //convert to short
//       if(tScore > highScore) highScore = tScore;  //if the read score is > than the HS then overwirte it.
//       token = strtok(NULL, "\n");
//       if(token == NULL) break;
//     }
//   }
//   fileClose(file);
 
//   return highScore; //return the Highest found score.
//   /*In Theory the last int he file should be the highest if this works correctly. This could get resource intensive if a person has hundreds of High scores*/
// }

