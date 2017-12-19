#include "header.h"

//////////////////// color ////////////////////

// color related
#define MAX_COLOR 18  // number of basic colors available to pick
#define WHITE 17
static char *color_name[MAX_COLOR] = {
	"Black", "Red", "Green", "Blue", "Navy",
	"Orange", "Yellow", "Pink", "Magenta", "Cyan",
	"Brown", "Grey", "LimeGreen", "Turquoise", "Violet",
	"Wheat", "Purple" , "White"
};

// "Black", "Red", "Green", "Blue", "Navy", 
// "Orange", "Yellow", "Pink", "Magenta", "Cyan",
// "Brown", "Grey", "LimeGreen", "Turquoise", "Violet",
// "Wheat", "Purple", "White"
// ex) XSetForeground(dpy, gc, UsrColorPixel("blue"));
static unsigned long UsrColorPixel(char *name) {  // get color
	XColor c0, c1;
	Colormap cmap;
	cmap = DefaultColormap(dpy, 0);
	XAllocNamedColor(dpy, cmap, name, &c0, &c1);
	return (c0.pixel);
}

extern void SetForegroundToColorIndex(int i) {
	color = UsrColorPixel(color_name[i]);
	XSetForeground(dpy, gc, color);
}

/*
static void SetForegroundToColorName(char* name) {
	color = UsrColorPixel(name);
	XSetForeground(dpy, gc, color);
}
*/

extern void SetForegroundToColor(unsigned long _color) {
	color = _color;
	XSetForeground(dpy, gc, color);
}

//////////////////// ////////////////////


extern void SetLineWidth(int width) {
	XGCValues gv;
	lineWidth = width;
	gv.line_width = width;
	XChangeGC(dpy, gc, GCLineWidth, &gv);
}

#define MAX_NPICK_WIDTH 6


extern void DrawPallete() {
	int i;
	XGCValues gv;

	//canvas
	XSetForeground(dpy, gc, BlackPixel(dpy, 0));
	gv.line_width = 4;
	XChangeGC(dpy, gc, GCLineWidth, &gv);
	XDrawRectangle(dpy, w, gc, CANVAS_X, CANVAS_Y, CANVAS_WIDTH, CANVAS_HEIGHT);

	//answer
	XSetForeground(dpy, gc, UsrColorPixel("blue"));
	gv.line_width = 4;
	XChangeGC(dpy, gc, GCLineWidth, &gv);
	XDrawRectangle(dpy, w, gc, PLAYER_X, PLAYER_Y, PLAYER_WIDTH, PLAYER_HEIGHT);

	XSetForeground(dpy, gc, BlackPixel(dpy, 0));
	if(isDrawer) {
		XDrawString(dpy, w, gc,
			PLAYER_X + (PLAYER_WIDTH / 5 ),
			PLAYER_Y + (PLAYER_HEIGHT / 2),
			strAnswerCorrect, strlen(strAnswerCorrect));
	} else {
		XDrawString(dpy, w, gc,
			PLAYER_X + (PLAYER_WIDTH / 5),
			PLAYER_Y + (PLAYER_HEIGHT / 2),
			"?????????? : guess what!", strlen("?????????? : guess what!"));
	}
	// palets draw
	for (i = 0; i< WHITE; i++) {
		XSetForeground(dpy, gc, UsrColorPixel(color_name[i]));
		XFillRectangle(dpy, w, gc,
			MARGIN + (PALET_AREA_X * i), MARGIN + PALET_AREA_Y,
			PALET_ITEM_WIDTH, PALET_ITEM_HEIGHT);
	}
	

	// draw white palet
	i = WHITE;
	XSetForeground(dpy, gc, BlackPixel(dpy, 0));
	gv.line_width = 1;
	XChangeGC(dpy, gc, GCLineWidth, &gv);
	XDrawRectangle(dpy, w, gc, 
		MARGIN + (PALET_AREA_X * i), MARGIN + PALET_AREA_Y,
		PALET_ITEM_WIDTH, PALET_ITEM_HEIGHT);
	//clear palet
	XSetFont(dpy, gc, XLoadFont(dpy, "fixed"));
	XDrawString(dpy, w, gc, 
		MARGIN + (PALET_AREA_X * (i + 1)), MARGIN + PALET_AREA_Y + 15, 
		"CLEAR", 5);


///////// brush width draw////////////
	for(i=0; i< MAX_NPICK_WIDTH; i++){
		XSetForeground(dpy, gc, BlackPixel(dpy, 0));
		gv.line_width = i * 5 + 5;
		XChangeGC(dpy, gc, GCLineWidth, &gv);
		XDrawLine(dpy, w, gc, 
			MARGIN + (BRUSH_AREA_X * i) + (BRUSH_ITEM_WIDTH / 2) - 1,
			BRUSH_AREA_Y + (BRUSH_ITEM_HEIGHT / 2) - 1,
			MARGIN + (BRUSH_AREA_X * i) + (BRUSH_ITEM_WIDTH / 2) + 1,
			BRUSH_AREA_Y + (BRUSH_ITEM_HEIGHT / 2) + 1);
	}


	SetForegroundToColor(color);
	gv.line_width = lineWidth;
	XChangeGC(dpy, gc, GCLineWidth, &gv);
}


extern void Clear() {
	XClearArea(dpy, w, CANVAS_X, CANVAS_Y, CANVAS_WIDTH, CANVAS_HEIGHT, 0);
	indexPath = 0;
}




extern bool EventCursorIsWithinCanvas(XEvent xe) {
	return (xe.xmotion.x > CANVAS_X && xe.xmotion.x < CANVAS_WIDTH + CANVAS_X)
		&& (xe.xmotion.y > CANVAS_Y && xe.xmotion.y < CANVAS_HEIGHT + CANVAS_Y);
}

extern bool EventCursorIsWithinColorPick(XEvent xe) {
	return xe.xmotion.y > MARGIN + PALET_AREA_Y && xe.xmotion.y < MARGIN + PALET_AREA_Y + PALET_ITEM_HEIGHT;
}

extern bool EventCursorIsWithinWidthPick(XEvent xe) {
	return xe.xmotion.y > BRUSH_AREA_Y && xe.xmotion.y < BRUSH_AREA_Y + BRUSH_ITEM_HEIGHT;
}

extern bool EventCursorIsBeingClicked(XEvent xe) {
	return xe.xmotion.state & Button1Mask;
}


extern bool GetClearPick(XEvent xe) {
	int i = MAX_COLOR;
	return xe.xmotion.x > MARGIN + (BRUSH_AREA_X * i) && xe.xmotion.x < MARGIN + (BRUSH_AREA_X * i) + 
BRUSH_ITEM_WIDTH;
}


extern int GetColorPick(XEvent xe) {
	int i;
	for (i = 0; i < MAX_COLOR + 1; ++i) {
		if (xe.xmotion.x > MARGIN + (BRUSH_AREA_X * i)
			&& xe.xmotion.x < MARGIN + (BRUSH_AREA_X * i) + BRUSH_ITEM_WIDTH) {
			break;
		} // if
	} // for
	if (i >= MAX_COLOR) { return -1; }
	// printf("color %d \n", i);
	return i;
}

extern int GetWidthPick(XEvent xe) {
	int i;
	for (i = 0; i < MAX_COLOR + 1; ++i) {
		if (xe.xmotion.x > MARGIN + (PALET_AREA_X * i)
			&& xe.xmotion.x < MARGIN + (PALET_AREA_X * i) + PALET_ITEM_WIDTH) {
			break;
		} // if
	} // for
	if (i >= MAX_NPICK_WIDTH) { return -1; }
	// printf("width arr n %d \n", i);
	return i;
}




extern void RepaintPath(){
	int i; // for variable
	int contextColor = color; // ex color
	int contextWidth = lineWidth; // ex width
	SetForegroundToColor(pathColor[0]);
	SetLineWidth(pathWidth[0]);
	for (i = 1; i<indexPath; i++) {
		if (path[i].x != -1) { // 
			if (path[i - 1].x != -1) { // 
				XDrawLine(dpy, w, gc, path[i - 1].x, path[i - 1].y, path[i].x, path[i].y); //
			} else if (path[i - 1].x == -1) { //
				SetForegroundToColor(pathColor[i]); // 
				SetLineWidth(pathWidth[i]);
			}
		} else {}
	}
	SetForegroundToColor(contextColor);
	SetLineWidth(contextWidth);
} // func
