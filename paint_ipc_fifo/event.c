#include "header.h"

// main window
Display		*dpy;
Window		w;
XEvent		xe;
// other ui stuff (context)
GC			gc;
int			lineWidth;
int			color;
// drawn line
int			indexPath;
XPoint		path[MAX_INDEX_PATH];
long		pathColor[MAX_INDEX_PATH];
int	        pathWidth[MAX_INDEX_PATH];

extern void InitDisplay() {
	dpy = XOpenDisplay(NULL);
	w = XCreateSimpleWindow(dpy, RootWindow(dpy, 0), 50, 50, 1050, 500,
							5, BlackPixel(dpy, 0), WhitePixel(dpy, 0));
	XSelectInput(dpy, w, ExposureMask | 
		ButtonPressMask | ButtonReleaseMask | 
	//	EnterWindowMask | LeaveWindowMask | 
		PointerMotionMask  
	);
	XMapWindow(dpy, w);
	XFlush(dpy);

	gc = XCreateGC(dpy, w, 0L, (XGCValues *)NULL);
	color = BlackPixel(dpy, 0);
	lineWidth = 5;
	XSetFont(dpy, gc, XLoadFont(dpy, "fixed"));
	XSetLineAttributes(dpy, gc, lineWidth, LineSolid, CapRound, JoinRound);
	XFlush(dpy);
} // func

// (XEvent xe)
extern void onDraw() {
	int i;
	long preColor;
	int preWidth;
	preColor =pathColor[0];	
	preWidth = pathWidth[0];
	
	SetForegroundToColor(preColor);
	SetLineWidth(preWidth);
	DrawPallete();
	
	for (i=1; i<indexPath; i++){
		if ( path[i-1].x != -1 && path[i-1]. y != -1 &&
			 path[i].x != -1 && path [i].y != -1){
			XDrawLine(dpy, w, gc,
				path[i-1].x, path[i-1].y,
				path[i].x, path[i].y);
		}
		else{
			if(path[i-1].x == -1 && path[i].x != -1){
				preColor = pathColor[i];
				preWidth = pathWidth[i];
				SetForegroundToColor(preColor);
				SetLineWidth(preWidth);
			}
		}
	}
	// repaint(xe);
} // func

static void onMouseMoved(XEvent xe) {

	if (EventCursorIsWithinCanvas(xe) && EventCursorIsBeingClicked(xe) && indexPath < MAX_INDEX_PATH) {

		path[indexPath].x = xe.xmotion.x;
		path[indexPath].y = xe.xmotion.y;

		if (indexPath == 0) { return; }
		XDrawLine(dpy, w, gc,
			path[indexPath - 1].x, path[indexPath - 1].y,
			path[indexPath].x, path[indexPath].y);

		++indexPath;
	} // if
} // func

static void onButtonPress(XEvent xe) {
	if (EventCursorIsWithinCanvas(xe) && indexPath < MAX_INDEX_PATH) {
		path[indexPath].x = xe.xmotion.x;
		path[indexPath].y = xe.xmotion.y;
		pathColor[indexPath] = color;
		pathWidth[indexPath] = lineWidth;
		++indexPath;
	}
} // func

static void onButtonRelease(XEvent xe) {
	int result;
	XEvent local_xe = xe;
	if (EventCursorIsWithinColorPick(local_xe)) { 
		// printf("EventCursorIsWithinColorPick\n");
		if ((result = GetColorPick(local_xe)) != -1) {
			SetForegroundToColorIndex(result);
		} else if (GetClearPick(local_xe)) { 
			Clear();
		}
	} // if
	else if (EventCursorIsWithinWidthPick(local_xe)) {
		// printf("EventCursorIsWithinWidthPick\n");
		if ((result = GetWidthPick(local_xe)) != -1) { 
			SetLineWidth(result * 5 + 5); 
		}
	}else if (EventCursorIsWithinCanvas( local_xe)){
		path[indexPath].x = -1;
		path[indexPath].y = -1;
		pathColor[indexPath] = color;
		pathWidth[indexPath] = lineWidth;
		++indexPath;
	} // elif
} // func

// input processing routine for main thread
static void UserInputMessageLoop() {
	for (;;) {
		XNextEvent(dpy, &xe);
		if(isDrawer){
			switch (xe.type) {
			case Expose:  
				onDraw(xe);
				break;
			case MotionNotify:  
				onMouseMoved(xe);
				break;
			case ButtonPress: 
				onButtonPress(xe);
				break;
			case ButtonRelease:
				onButtonRelease(xe);
				break;
			case EnterNotify: 
				break;
			case LeaveNotify:  
				break;
			default:  // Neither of above
				printf("xe.type unknown: check either XSelectInput \n");
				break;
			}
		}else{
			switch (xe.type) {
			case Expose:  
				onDraw(xe);
				break;
			case MotionNotify:  				
				break;
			case ButtonPress: 
				break;
			case ButtonRelease:
				break;
			case EnterNotify: 
				break;
			case LeaveNotify:  
				break;
			default:  // Neither of above
				printf("xe.type unknown: check either XSelectInput() \n");
				break;
			}
		}
	} // for(;;)
} // func

extern void *Thread1() {
	UserInputMessageLoop();
	pthread_exit(NULL);
} // func
