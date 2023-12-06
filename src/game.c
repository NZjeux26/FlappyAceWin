#include "game.h"
#include "states.h"
#include "mystdlib.h"
#include <ace/managers/key.h>
#include <ace/managers/game.h>
#include <ace/managers/system.h>
#include <ace/managers/viewport/simplebuffer.h>
#include <ace/managers/blit.h> // Blitting fns
#include <ace/managers/state.h>
#include <ace/managers/rand.h>
#include <ace/managers/sprite.h>
#include <ace/utils/file.h>
#include <ace/utils/font.h>
#include <ace/utils/string.h>
#include <ace/utils/palette.h>
#include <mini_std/stdio.h>

#define false 0
#define true 1
#define SCORE_COLOR 1
#define WALL_HEIGHT 1
#define WALL_COLOR 1
#define PADDLE_SPEED 3
#define MAXPIPES 4
#define MAXBIRDS 5
//-------------------------------------------------------------- NEW STUFF START
//AMiga Pal 320x256
#define PLAYFIELD_HEIGHT (256-32) //32 for the top viewport height
#define PLAYFIELD_WIDTH (320)

static tView *s_pView; // View containing all the viewports
static tVPort *s_pVpScore; // Viewport for score
static tSimpleBufferManager *s_pScoreBuffer;
static tVPort *s_pVpMain; // Viewport for playfield
static tSimpleBufferManager *s_pMainBuffer;
static tRandManager *s_pRandManager;

static tBitMap *s_pBmBirds[MAXBIRDS];
static tBitMap *s_pBmBirdMask[MAXBIRDS];
static tBitMap *s_pBmTopPipe[MAXPIPES];
static tBitMap *s_pBmBottomPipe[MAXPIPES];
static tBitMap *s_pBmTopPipeMask[MAXPIPES];
static tBitMap *s_pBmBottomPipeMask[MAXPIPES];
static tBitMap *s_pSprite0Data;
static tBitMap *s_pSpriteSrc;
static tBitMap *s_pSprite3Data;
static tSprite *s_pSprite3;
static tSprite *s_pSprite0;

g_obj player; //player object declaration
g_obj toppipe;
g_obj bottompipe; 

g_pipes pipes[MAXPIPES];

tFont *fallfontsmall; //global for font
tTextBitMap *scoretextbitmap;//global for score text

char scorebuffer[20];
int gSCORE = 0;
int g_highScore = 0; //needs to be assigned prior to initialization
short pipesdisplay = 1;
short birdplay = 0;
short oddframe = 0;
short pipestart = 304; //set to 304 as the width of the pipes are 16 which is 320 the total width of the screen.

ULONG startTime;
UBYTE g_scored = false;
//arrays holding previous positions of the player and pipes
tUwRect s_pTopPipePrevPos[MAXPIPES][2]; //2d array the size of max pipes, with the max pipes being the index of the pipe and the 2nd part the double buffer
tUwRect s_pBottomPipePrevPos[MAXPIPES][2];

tUwCoordYX s_pPlayerPrevPos[2];

UBYTE s_ubBufferIndex = 0;

void gameGsCreate(void) {
  tRayPos sRayPos = getRayPos();

  s_pRandManager = randCreate(1+(sRayPos.bfPosY << 8), 1 + sRayPos.bfPosX);

  s_pView = viewCreate(0,TAG_VIEW_GLOBAL_PALETTE, 1,TAG_END);
  
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
    TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_INTERLEAVED,
    TAG_SIMPLEBUFFER_IS_DBLBUF, 1, //add this line in for double buffering
    TAG_END);

  paletteLoad("data/flappypal6.plt", s_pVpScore->pPalette, 20); //replaces palette
  makebirds();//make the bitmaps for the frames
  
  spriteManagerCreate(s_pView, 0);
  systemSetDmaBit(DMAB_SPRITE, 1);

  s_pSpriteSrc = bitmapCreateFromFile("data/fullpipe.bm",0);//could make these sprites and sprite datas an array.
  s_pSprite0Data = bitmapCreate(16,112,2,BMF_INTERLEAVED);
  blitCopy(s_pSpriteSrc,0,112,
  s_pSprite0Data,0,0,
  16,112,0);
  s_pSprite0 = spriteAdd(0,s_pSprite0Data);//top
  
  
  s_pSprite3Data = bitmapCreateFromFile("data/fullpipe.bm",0);
  s_pSprite3 = spriteAdd(1,s_pSprite3Data);//bottom
 
  // Draw line separating score VPort and main VPort, leave one line blank after it
  blitLine(
    s_pScoreBuffer->pBack,
    0, s_pVpScore->uwHeight - 2,
    s_pVpScore->uwWidth - 1, s_pVpScore->uwHeight - 2,
    SCORE_COLOR, 0xFFFF, 0 // Try patterns 0xAAAA, 0xEEEE, etc.
  );

  gSCORE = 99999999;  //set to 99999999 so the bitmap is big enough to handle larger numbers
  g_highScore = getHighScore();  //get the highscore from the file, returning zero if no file
  char i_highScore[20]; //buffer string to hold the highscore
  stringDecimalFromULong(g_highScore, i_highScore); //convert to short

  fallfontsmall = fontCreate("myacefont.fnt");//create font

  tTextBitMap *highscorebitmap = fontCreateTextBitMapFromStr(fallfontsmall, "High Score ");//create the bitmap with HIGHSCORE
  fontDrawTextBitMap(s_pScoreBuffer->pBack, highscorebitmap, 0,10, 5, FONT_COOKIE); //draw the text
  highscorebitmap = fontCreateTextBitMapFromStr(fallfontsmall, i_highScore);  //reuse the bitmap from writing the highscore text  
  fontDrawTextBitMap(s_pScoreBuffer->pBack, highscorebitmap, 73,10, 5, FONT_COOKIE); //write out the highscore

  tTextBitMap *textbitmap = fontCreateTextBitMapFromStr(fallfontsmall, "Score ");
  fontDrawTextBitMap(s_pScoreBuffer->pBack, textbitmap, 0,20, 5, FONT_COOKIE);
  
  //convert the score from int to string for drawing
  stringDecimalFromULong(gSCORE, scorebuffer);
  scoretextbitmap = fontCreateTextBitMapFromStr(fallfontsmall, scorebuffer); //redo bitmap
  gSCORE = 0;
  
  startTime = timerGet();
  
  //placeholder for player
  player.x = 35;
  player.y = (s_pVpMain->uwHeight - player.h) / 2;
  player.w = 16;
  player.h = 12;
  player.yvel = 2;
  player.colour = 5;
  short pos = randUwMinMax(s_pRandManager, 62, 156); //the position of the 'centre' of the desired gap between pipes atted 32 to previous values since the sprites don't care about viewports
  short range = randUwMinMax(s_pRandManager, 45, 95); //the range/distance between the pipes, centred on the pos above.
  short pipecolour = randUwMinMax(s_pRandManager,2, 16);

    s_pSprite0->wX = pipestart;
    s_pSprite0->wY = 32;
    s_pSprite0->uwHeight = 30;//(s_pSprite0->wY - 32) + pos - (range / 2); // need to subtract 32 (scoreboardviewport) back off the Y pos to align  correctly in the main viewport.
    
 
 
    s_pSprite3->wX = pipestart;
    s_pSprite3->wY = pos + (range / 2);
    s_pSprite3->uwHeight = 256 - s_pSprite3->wY;
  //create first batch of pipes to fill the array
  for (short i = 0; i < MAXPIPES; i++) {
    //makePipes();
    // short pos = randUwMinMax(s_pRandManager, 62, 156); //the position of the 'centre' of the desired gap between pipes atted 32 to previous values since the sprites don't care about viewports
    // short range = randUwMinMax(s_pRandManager, 45, 95); //the range/distance between the pipes, centred on the pos above.
    // short pipecolour = randUwMinMax(s_pRandManager,2, 16);

    // s_pSprite0->wX = pipestart;
    // s_pSprite0->wY = 32;
    // s_pSprite0->uwHeight = (s_pSprite0->wY - 32) + pos - (range / 2); // need to subtract 32 (scoreboardviewport) back off the Y pos to align  correctly in the main viewport.
    // //s_pSprite0->wY = 32 + s_pSprite0->uwHeight;

    // s_pSprite3->wX = pipestart;
    // s_pSprite3->wY = pos + (range / 2);
    // s_pSprite3->uwHeight = 256 - s_pSprite3->wY;

    pipes[i].toppipe.x = pipestart;
    pipes[i].toppipe.y = 0;
    pipes[i].toppipe.h = pipes[i].toppipe.y + pos - (range / 2); //the height of the pipe is the Y pos - half of the gap(range) / 2;
    pipes[i].toppipe.w = 16;
    pipes[i].toppipe.colour = pipecolour;
    //manually setting the prepos values so they are not null values when the first undraw occurs
    s_pTopPipePrevPos[i][0].uwX = pipes[i].toppipe.x;
    s_pTopPipePrevPos[i][0].uwY = pipes[i].toppipe.y;
    s_pTopPipePrevPos[i][0].uwWidth = pipes[i].toppipe.w;
    s_pTopPipePrevPos[i][0].uwHeight = pipes[i].toppipe.h;
    s_pTopPipePrevPos[i][1].uwX = pipes[i].toppipe.x;
    s_pTopPipePrevPos[i][1].uwY = pipes[i].toppipe.y;
    s_pTopPipePrevPos[i][1].uwWidth = pipes[i].toppipe.w;
    s_pTopPipePrevPos[i][1].uwHeight = pipes[i].toppipe.h;;

    pipes[i].bottompipe.x = pipestart;
    pipes[i].bottompipe.y = pos + (range / 2); //the y pos of the bottom pipe is the Y pos of the gap + 1/2 it's width
    pipes[i].bottompipe.h = 223 - pipes[i].bottompipe.y; //the height is simply the height of the screen 224pc - the Y of the bottom pipe.
    if (pipes[i].bottompipe.h > 223) pipes[i].bottompipe.h = 223;
    pipes[i].bottompipe.w = 16;
    pipes[i].bottompipe.colour = pipecolour;

    s_pBottomPipePrevPos[i][0].uwX = pipes[i].bottompipe.x;
    s_pBottomPipePrevPos[i][0].uwY = pipes[i].bottompipe.y;
    s_pBottomPipePrevPos[i][0].uwWidth = pipes[i].bottompipe.w;
    s_pBottomPipePrevPos[i][0].uwHeight = pipes[i].bottompipe.h;
    s_pBottomPipePrevPos[i][1].uwX = pipes[i].bottompipe.x;
    s_pBottomPipePrevPos[i][1].uwY = pipes[i].bottompipe.y;
    s_pBottomPipePrevPos[i][1].uwWidth = pipes[i].bottompipe.w;
    s_pBottomPipePrevPos[i][1].uwHeight = pipes[i].bottompipe.h;
  }

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

void gameGsLoop(void) {
  // This will loop every frame
  if(keyCheck(KEY_ESCAPE)) {
    gameExit();
  }
  
  //update x 
  spriteProcess(s_pSprite0);
  spriteProcessChannel(0);

  spriteProcess(s_pSprite3);
  spriteProcessChannel(1);
  //undraw player
  //blitRect(s_pMainBuffer->pBack, s_pPlayerPrevPos[s_ubBufferIndex].uwX,s_pPlayerPrevPos[s_ubBufferIndex].uwY, player.w, player.h, 0);
  blitCopy(s_pBmBirds[birdplay],0,0,
  s_pMainBuffer->pBack, s_pPlayerPrevPos[s_ubBufferIndex].uwX,s_pPlayerPrevPos[s_ubBufferIndex].uwY,
  player.w,player.h,0);//16w,12h, 0 colour from the palette
 
  //undraw each pipe pair in the array upto pipe display #
  //undraws the pipe X/Y coordinates from the array which are the previous frames' pipe X/Y positions
  for (short i = 0; i < pipesdisplay; i++) {

       //update x 
      // spriteProcess(s_pSprite0);
      // spriteProcessChannel(0);
      // blitCopy(s_pBmTopPipe[i],0,0,
      // s_pMainBuffer->pBack,s_pTopPipePrevPos[i][s_ubBufferIndex].uwX,s_pTopPipePrevPos[i][s_ubBufferIndex].uwY,
      // s_pTopPipePrevPos[i][s_ubBufferIndex].uwWidth,s_pTopPipePrevPos[i][s_ubBufferIndex].uwHeight,0
      // );

      // blitCopy(s_pBmBottomPipe[i],0,0,
      // s_pMainBuffer->pBack,s_pBottomPipePrevPos[i][s_ubBufferIndex].uwX,s_pBottomPipePrevPos[i][s_ubBufferIndex].uwY,
      // s_pBottomPipePrevPos[i][s_ubBufferIndex].uwWidth,s_pBottomPipePrevPos[i][s_ubBufferIndex].uwHeight,0
      // );
  }
 
  //**Move things accross**

  for (short i = 0; i < pipesdisplay; i++) {
      if(pipes[i].toppipe.x <=0 || pipes[i].bottompipe.x <=0){//probably only need one pipe side but jsut in case something weird happens
        short pos = randUwMinMax(s_pRandManager, 30, 150); //recalculate position and the gap between pipes(range)
        short range = randUwMinMax(s_pRandManager, 20, 70);
      
        //move the pipe back to the start and redraw with new calculations.
        pipes[i].toppipe.x = pipestart;
        pipes[i].toppipe.y = 0;
        pipes[i].toppipe.h = pipes[i].toppipe.y + pos - (range / 2);
        
        pipes[i].bottompipe.x = pipestart;
        pipes[i].bottompipe.y = pos + (range / 2);
        pipes[i].bottompipe.h = 223 - pipes[i].bottompipe.y;  //the hight of the bottom pipe is the screen length - the y pos of the gap.
        if (pipes[i].bottompipe.h > 223) pipes[i].bottompipe.h = 223;//this will never work since it's the Y + the H that is the issue
        //add to scoreboard
        g_scored = true;
      }

      else {//move the pipes across the playfield
        pipes[i].toppipe.x -= 1;
        pipes[i].bottompipe.x -= 1;
      }
      //if the player touches a pipe
      // if(Collision(&player, &pipes[i].toppipe) || Collision(&player, &pipes[i].bottompipe)){
      //   stateChange(g_pStateManager, g_pMenuState); //switch to the menu state to ask to replay
      //   highScoreCheck(); //check the HS and write if required
      //   pipesdisplay = 1; //reset the pipedisplay to 1 so on reply not all pipes are spawned
      //   return;
      // }
  }

  //controls
  if(keyCheck(KEY_SPACE)){  //move player up
    player.y = MAX(player.y - PADDLE_SPEED, 0);
    birdplay++; //run the animation
  }
  else{
    //player.y = MIN(player.y + player.yvel , 210);
    birdplay = 4; //set the bird to wings down for falling
  }
     if (keyCheck(KEY_D))
    { // move player right
      player.x = MIN(player.x + PADDLE_SPEED, 275);
    }
    if (keyCheck(KEY_A))
    { // move player left
      player.x = MAX(player.x - PADDLE_SPEED, 0);
    }
  //this is done here, since the player has been undrawn, so we want to swap the player frame while it's undrawn
  if(birdplay == MAXBIRDS) birdplay = 0; //reset animation loop back to 0
  
   //**Draw things**

  //update the previous position array
  s_pPlayerPrevPos[s_ubBufferIndex].uwX = player.x;
  s_pPlayerPrevPos[s_ubBufferIndex].uwY = player.y;
  // Redraw the player at new position
  blitCopyMask(
  s_pBmBirds[birdplay],0,0,
  s_pMainBuffer->pBack,player.x,player.y, 
  player.w, player.h,s_pBmBirdMask[birdplay]->Planes[0]);
  

  // //redraw each pipe pair in the array upto pipe display #
  // for (short i = 0; i < pipesdisplay; i++) {
  //   //update toppipe previous position
  //   s_pTopPipePrevPos[i][s_ubBufferIndex].uwX = pipes[i].toppipe.x;
  //   s_pTopPipePrevPos[i][s_ubBufferIndex].uwY = pipes[i].toppipe.y;
  //   s_pTopPipePrevPos[i][s_ubBufferIndex].uwWidth = pipes[i].toppipe.w;
  //   s_pTopPipePrevPos[i][s_ubBufferIndex].uwHeight = pipes[i].toppipe.h;;
  //   //redraw
  //   blitCopyMask(s_pBmTopPipe[i],0,0,
  //   s_pMainBuffer->pBack,pipes[i].toppipe.x,pipes[i].toppipe.y,
  //   pipes[i].toppipe.w, pipes[i].toppipe.h,s_pBmTopPipeMask[i]->Planes[0]
  //   );

  //   //repeat for bottom pipe
  //   s_pBottomPipePrevPos[i][s_ubBufferIndex].uwX = pipes[i].bottompipe.x;
  //   s_pBottomPipePrevPos[i][s_ubBufferIndex].uwY = pipes[i].bottompipe.y;
  //   s_pBottomPipePrevPos[i][s_ubBufferIndex].uwWidth = pipes[i].bottompipe.w;
  //   s_pBottomPipePrevPos[i][s_ubBufferIndex].uwHeight = pipes[i].bottompipe.h;

  //   blitCopyMask(s_pBmBottomPipe[i],0,0,
  //   s_pMainBuffer->pBack,pipes[i].bottompipe.x,pipes[i].bottompipe.y,
  //   pipes[i].bottompipe.w, pipes[i].bottompipe.h,s_pBmBottomPipeMask[i]->Planes[0]
  //   );

  // }
  //timer handling the pipes being spawned, reset to 1 at game reset
  // ULONG currentTime = timerGet();
  //   if(currentTime - startTime > 120){
  //     pipesdisplay++;
  //   if(pipesdisplay >= MAXPIPES){
  //     pipesdisplay = MAXPIPES -1;
  //   }
  //   startTime = currentTime;
  // }

  if(g_scored){//if the player scores, update the score board. 
    g_scored = false;
    updateScore();
  }
  //update the index
  s_ubBufferIndex = !s_ubBufferIndex;
  viewProcessManagers(s_pView);//might be wrong
  copProcessBlocks();
  systemIdleBegin();
  vPortWaitUntilEnd(s_pVpMain);
  //vPortWaitForEnd(s_pVpMain);
  systemIdleEnd();
  }

void gameGsDestroy(void) {
  logBlockBegin("gameGsDestroy");
  systemUse();
  for(short int i = 0; i < MAXBIRDS; i++){
    bitmapDestroy(s_pBmBirdMask[i]);
    bitmapDestroy(s_pBmBirds[i]);
  }
  for(short int i = 0; i < MAXPIPES; i++){
    bitmapDestroy(s_pBmBottomPipe[i]);
    bitmapDestroy(s_pBmBottomPipeMask[i]);
    bitmapDestroy(s_pBmTopPipe[i]);
    bitmapDestroy(s_pBmTopPipeMask[i]);
  }
  spriteRemove(s_pSprite0);
  spriteRemove(s_pSprite3);
  systemSetDmaBit(DMAB_SPRITE, 0);
  spriteManagerDestroy();
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

void highScoreCheck(void) {
  int score = gSCORE;
  char charScore[30];
  char filename[20] = "scoresheet.txt";
  systemUse();

  if(!fileExists(filename)){  //check if the file exists, if not create and add the score
      tFile *file = fileOpen(filename, "w");
      stringDecimalFromULong(score,charScore);
      fileWriteStr(file, charScore);//add the score to the file 
      fileClose(file); 
  }
  else{//if file exsits, read the score chec against the player score, if greater write to file else do nothing
    short tScore = g_highScore; //since the highscore should be pulled in at the start, no need to call it again
    //function to return highest score in the file.
   // logWrite("TScore Is: %d\n", tScore);

    if(score > tScore){//if score is greater than the current HS then add it to the end.
      tFile *file = fileOpen(filename, "wb");
      stringDecimalFromULong(score,charScore);
      fileWriteStr(file,charScore);
      fileWriteStr(file, ","); //using comma to seperate the scores
      fileClose(file);
    }
  }
  systemUnuse();
}
// //reads through the scoresheet to find the highest score and returns that to compare it with the current score by the player
short getHighScore(void){
  int highScore = 0;
  char filename[20] = "scoresheet.txt";
  systemUse();
  
  if((!fileExists(filename))){
    systemUnuse();
    return 0; //return 0 if the file does not exist
  } 

  tFile *file = fileOpen(filename, "r");
  if(!file) {
    systemUnuse();
    return 102; //set to 2 so i know it was a cope out
  }

  char tline[512];
  while(fgets(tline, 512, file)){//issues getting the tokens into the array of lines
    char *token = strtok(tline, ",");
    while(token){
      //logWrite("File Reading: %s\n", token); 
      int tScore = strtol(token, &token, 10);  //convert to short //why is this returning zero?
      if(tScore > highScore) highScore = tScore;  //if the read score is > than the HS then overwirte it.
      token = strtok(NULL, "\n");
      if(token == NULL) break;
    }
  }
  
  fileClose(file);
  systemUnuse();
  return highScore; //return the Highest found score.
  /*In Theory the last int he file should be the highest if this works correctly. This could get resource intensive if a person has hundreds of High scores*/
}

void makebirds(void){
  //s_pBmBird = bitmapCreate(16,12,4,0); //16px wide, 12 high
  s_pBmBirds[0] = bitmapCreateFromFile("data/birdt1.bm",0);
  s_pBmBirdMask[0] = bitmapCreateFromFile("data/birdt1_mask.bm", 0);
  s_pBmBirds[1] = bitmapCreateFromFile("data/birdt2.bm",0);
  s_pBmBirdMask[1] = bitmapCreateFromFile("data/birdt2_mask.bm", 0);
  s_pBmBirds[2] = bitmapCreateFromFile("data/birdt3.bm",0);
  s_pBmBirdMask[2] = bitmapCreateFromFile("data/birdt3_mask.bm", 0);
  s_pBmBirds[3] = bitmapCreateFromFile("data/birdt4.bm",0);
  s_pBmBirdMask[3] = bitmapCreateFromFile("data/birdt4_mask.bm", 0);
  s_pBmBirds[4] = bitmapCreateFromFile("data/birdt5.bm",0);
  s_pBmBirdMask[4] = bitmapCreateFromFile("data/birdt5_mask.bm", 0);
}

void makePipes(){
  for(short i = 0; i < MAXPIPES; i++) {
    s_pBmTopPipe[i] = bitmapCreateFromFile("data/pipetop.bm",0);
    s_pBmTopPipeMask[i] = bitmapCreateFromFile("data/pipetop_mask.bm",0);
    s_pBmBottomPipe[i] = bitmapCreateFromFile("data/pipebottom.bm",0);
    s_pBmBottomPipeMask[i] = bitmapCreateFromFile("data/pipebottom_mask.bm",0);
  }
}