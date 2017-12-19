#include <stdio.h>
#include <pthread.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysymdef.h>
//

#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef enum bool { false, true } bool;

#define MARGIN				50

#define CANVAS_X			MARGIN
#define CANVAS_Y			MARGIN
#define CANVAS_WIDTH		600
#define CANVAS_HEIGHT		300

#define PLAYER_X			MARGIN + CANVAS_WIDTH + MARGIN
#define PLAYER_Y			MARGIN
#define PLAYER_WIDTH		300
#define PLAYER_HEIGHT		CANVAS_HEIGHT

#define PALET_AREA_X		MARGIN
#define PALET_AREA_Y		370
#define PALET_ITEM_WIDTH	30
#define PALET_ITEM_HEIGHT	30

#define BRUSH_ITEM_HEIGHT	30
#define BRUSH_ITEM_WIDTH	30
#define BRUSH_AREA_X		MARGIN
#define BRUSH_AREA_Y		MARGIN + CANVAS_HEIGHT + BRUSH_ITEM_HEIGHT

// main window
extern Display			*dpy;
extern Window			w;
extern XEvent			xe;

// other ui stuff (context)
extern GC				gc;
extern int				lineWidth;
extern int				color;

// ipc
extern char				strAnswerCorrect[100];
extern bool				isDrawer;
extern bool				bGameOver;


// drawn line
#define	MAX_INDEX_PATH	100000
extern int				indexPath;
extern long				pathColor[MAX_INDEX_PATH];
extern int				pathWidth[MAX_INDEX_PATH];
extern XPoint			path[MAX_INDEX_PATH];

// drawing
extern void DrawPallete();
extern void RepaintPath();
extern void Clear();
extern bool EventCursorIsWithinCanvas(XEvent);
extern bool EventCursorIsWithinColorPick(XEvent);
extern bool EventCursorIsWithinWidthPick(XEvent);
extern bool EventCursorIsBeingClicked(XEvent);
extern bool GetClearPick(XEvent);
extern int GetColorPick(XEvent);
extern int GetWidthPick(XEvent);
extern void SetForegroundToColorIndex(int i);
extern void SetForegroundToColor(unsigned long);
extern void SetLineWidth(int width);


//typo
extern void TypoSetInputAnswer();
extern void TypoInputLoopWriter();
extern void TypoInputLoopReader();


//ipc
extern void SndPath(int, int, int, int, int);
extern void SndAnswerCorrect(char *strAnswer);
extern void SndGameOver();

//init
extern void InitDisplay();
extern void IpcInit();
extern void IpcInitClear();

// user_input // ipc
extern void *Thread1();
extern void *Thread2Writer();
extern void *Thread2Reader();