/*==============================================================================
  
   Purpose:    simple graphic functions for X11
   Author:     Rudolf Berrendorf
               Computer Science Department
               Bonn-Rhein-Sieg University
	       53754 Sankt Augustin, Germany
               rudolf.berrendorf@h-brs.de
  
==============================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "libFHBRS.h"

// should be defined in unistd.h, but it isn't :-(
extern int usleep (int);


/*==============================================================================*/
// globale Makros

// time in usec that an event handling thread should sleep
#define SLEEP_TIME 100000

#define PI 3.14159265358979323846

// mark public functions
#define public

#define min(x,y) ((x < y) ? x : y)
#define max(x,y) ((x > y) ? x : y)


/*==============================================================================*/
// X11 variables

// X11 display
static Display *display;
// X11 screen
static int screen;
// track number of user windows
static int n_windows = -1;             // subsequent window numbers
static int window_counter = 0;         // number of active windows

static struct
{
  int width, height;		       // window size
  Window window;		       // X11 window descriptor
  GC gc;			       // X11 graphic context
  Pixmap output_device;		       // backing pixmap
  int thread_id;                       // thread ID associated with window
  pthread_t event_handler_thread;      // thread associated with window
} window_descr[GRAPHIC_WINDOW_MAX];


// default colormap
static Colormap defaultcolormap;

// RGB colors in each plane we actually use
#define RGB_MAX     16u
// 65536 color values in each RGB plane
#define RGB_FAKTOR   (65536u / RGB_MAX)

// color pixel values
static unsigned long default_pixels[GRAPHIC_MAX_COLOR];
static bool rgb_initialized = false;
static unsigned long RGB_pixels[RGB_MAX * RGB_MAX * RGB_MAX];
static bool gray_initialized = false;
static unsigned long gray_pixels[GRAPHIC_MAX_GRAY];
static bool rainbow_initialized = false;
static unsigned long rainbow_pixels[GRAPHIC_RAINBOW_MAX];

// Fonts
#define NFONTS 7
static bool font_initialized = false;
static XFontStruct *font_info[NFONTS];
static char *fontname[NFONTS] = {
  "5x8",
  "6x10",
  "7x13",
  "8x13",
  "9x15",
  "10x20",
  "12x24"
};

// available font sizes (to be used by user)
static int fontsizes[] = { 5, 6, 7, 8, 9, 10, 12 };

// lock for X11 functions
static pthread_mutex_t x11_mutex = PTHREAD_MUTEX_INITIALIZER;
#define lock()   pthread_mutex_lock (&x11_mutex)
#define unlock() pthread_mutex_unlock (&x11_mutex);



/*==============================================================================*/
// thread for X11 event handling
// thread gets cancelled externally by main thread

static void *
handler_thread (void *arg)
{
  XEvent an_event;
  int window_number = *(int *) arg;


  // enter an "endless" loop of handling events
  while (1)
    {
      // lock X11 mutex
      lock ();

      // check for exposure event
      if (XCheckWindowEvent (display, window_descr[window_number].window, ExposureMask, &an_event) == 0)
	{
	  // no such event available: unlock mutex
	  unlock ();

	  // nothing to do: sleep 1 second
	  usleep (SLEEP_TIME);
	}

      else
	{
	  // unlock mutex
	  unlock ();

	  // handle event
	  switch (an_event.type)
	    {
	      case Expose:
		// are there more exposure events in event queue?
		if (an_event.xexpose.count > 0)
		  // the last one will handle this
		  break;
		// ok, we are the last one: draw backing pixmap
		graphic_flush (window_number);
		break;

	      default:
		// should never happen
		break;
	    }

	  // here too: sleep a small period of time as other events may be queued
	  usleep (SLEEP_TIME/1000);
	}
    }

  return NULL;
}


/*==============================================================================*/
// X11 error handler

static int
error_handler(Display *display, XErrorEvent *error)
{
  int length = 1024;
  char buffer[length];

  // get error text
  XGetErrorText(display, error->error_code, buffer, length);

  // print it to stderr
  fprintf(stderr, "X11 error: %s\n", buffer);

  return 0;
}


/*==============================================================================*/
// define user coordinate system

// user coordinate system
struct {
  double x_min;
  double y_min;
  double x_max;
  double y_max;
  double height;
  double width;
} coord_system[GRAPHIC_WINDOW_MAX];

// mapping function for x coordinate to pixel
static int
mapX (int window_number, double x)
{
  return (int) round ((window_descr[window_number].width * (x - coord_system[window_number].x_min) / (double) (coord_system[window_number].x_max - coord_system[window_number].x_min)));
}

// mapping function for y coordinate to pixel
static int
mapY (int window_number, double y)
{
  return window_descr[window_number].height -
	  (int) round ((window_descr[window_number].height * (coord_system[window_number].y_max - y) / (double) (coord_system[window_number].y_max - coord_system[window_number].y_min)));
}

// auxiliary function to compute distance from user coordinate system to pixel distance
static int
distanzX (int window_number, double x)
{
  return (int) round (x * window_descr[window_number].width / abs (coord_system[window_number].x_max - coord_system[window_number].x_min));
}

// auxiliary function to compute distance from user coordinate system to pixel distance
static int
distanzY (int window_number, double y)
{
  return (int) round (y * window_descr[window_number].height / abs (coord_system[window_number].y_max - coord_system[window_number].y_min));
}


/* map user coordinates to X11 coordinates
   user coordinates: origin lower left, x to the right, y to the top
   X11 coordinates: origin upper left corner, x to the bootom, y to the right
*/
static void
mapCoordinates (int window_number, double x, double y, int *sx, int *sy)
{
  *sx = mapX (window_number, x);
  *sy = window_descr[window_number].height - mapY (window_number, y);
}


/*==============================================================================*/
// convert HSL to RGB

static int
HSLtoRGB (float h,		       // I: h-value in [0,1]
	  float s,		       // I: s-value in [0,1]
	  float l		       // I: l-value in [0,1]
	)
{
  float r, g, b;		       // this function works with floats between 0 and 1
  float temp1, temp2, tempr, tempg, tempb;


  // If saturation is 0, the color is a shade of gray
  if (s == 0)
    r = g = b = l;

  // If saturation > 0, more complex calculations are needed
  else
    {
      // Set the temporary values 
      if (l < 0.5)
	temp2 = l * (1 + s);
      else
	temp2 = (l + s) - (l * s);
      temp1 = 2 * l - temp2;
      tempr = h + 1.0 / 3.0;
      if (tempr > 1)
	tempr--;
      tempg = h;
      tempb = h - 1.0 / 3.0;
      if (tempb < 0)
	tempb++;

      // Red 
      if (tempr < 1.0 / 6.0)
	r = temp1 + (temp2 - temp1) * 6.0 * tempr;
      else if (tempr < 0.5)
	r = temp2;
      else if (tempr < 2.0 / 3.0)
	r = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempr) * 6.0;
      else
	r = temp1;

      // Green 
      if (tempg < 1.0 / 6.0)
	g = temp1 + (temp2 - temp1) * 6.0 * tempg;
      else if (tempg < 0.5)
	g = temp2;
      else if (tempg < 2.0 / 3.0)
	g = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempg) * 6.0;
      else
	g = temp1;

      // Blue 
      if (tempb < 1.0 / 6.0)
	b = temp1 + (temp2 - temp1) * 6.0 * tempb;
      else if (tempb < 0.5)
	b = temp2;
      else if (tempb < 2.0 / 3.0)
	b = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempb) * 6.0;
      else
	b = temp1;
    }


  int r1 = (int) (r * 255.0);
  int g1 = (int) (g * 255.0);
  int b1 = (int) (b * 255.0);

  return RGB_VALUE (r1, g1, b1);
}


/*==============================================================================*/
// initialize fonts

static void
initialize_font(void)
{
  // lock X11 mutex
  lock();

  font_initialized = true;

  // Font
  for (int fnr = 0; fnr < NFONTS; fnr++)
    {
      if ((font_info[fnr] = XLoadQueryFont (display, fontname[fnr])) == NULL)
	{
	  fprintf (stderr, "can not load font\n");
	  return;
	}
    }
  
  // unlock X11 mutex
  unlock();
}

/*==============================================================================*/
// initialize RGB colors

static void
initialize_rgb(void)
{
  int RGBncolours;
  XColor col;

  // lock X11 mutex
  lock();

  rgb_initialized = true;

  // allocate RGB colours
  RGBncolours = 0;
  for (int r = 0; r < RGB_MAX; r++)
    for (int g = 0; g < RGB_MAX; g++)
      for (int b = 0; b < RGB_MAX; b++)
	{
	  col.red = r * RGB_FAKTOR;
	  col.green = g * RGB_FAKTOR;
	  col.blue = b * RGB_FAKTOR;
	  int rc = XAllocColor (display, defaultcolormap, &col);
	  assert(rc != 0);
	  if(rc != 0)
	    RGB_pixels[RGBncolours++] = col.pixel;
	}
  
  // unlock X11 mutex
  unlock();
}

/*==============================================================================*/
// initialize gray colors

static void
initialize_gray(void)
{
  int grayncolours;
  XColor col;

  // lock X11 mutex
  lock();

  gray_initialized = true;

  // allocate gray colors
  grayncolours = 0;
  for (int i = 0; i < GRAPHIC_MAX_GRAY; i++)
    {
      col.red = 65536 - (i * 65536 / (GRAPHIC_MAX_GRAY - 1));
      col.green = 65536 - (i * 65536 / (GRAPHIC_MAX_GRAY - 1));
      col.blue = 65536 - (i * 65536 / (GRAPHIC_MAX_GRAY - 1));
      int rc = XAllocColor (display, defaultcolormap, &col);
      assert(rc != 0);
      if(rc != 0)
	gray_pixels[grayncolours++] = col.pixel;
    }
  
  // unlock X11 mutex
  unlock();
}

/*==============================================================================*/
// initialize rainbow colors

static void
initialize_rainbow(void)
{
  int rainbow_ncolours;
  XColor col;

  // lock X11 mutex
  lock();

  rainbow_initialized = true;

  // allocate rainbow colours
  rainbow_ncolours = 0;
  for (int i = 0; i < GRAPHIC_RAINBOW_MAX; i++)
    {
      float h = 0.77 - (i / (1.3 * GRAPHIC_RAINBOW_MAX));
      float s = 1.0;
      float l = 0.5;
      int x = HSLtoRGB (h, s, l);
      int r, g, b;
      r = RGB_RED (x);
      g = RGB_GREEN (x);
      b = RGB_BLUE (x);
      col.red = r * 255;
      col.green = g * 255;
      col.blue = b * 255;

      int rc = XAllocColor (display, defaultcolormap, &col);
      assert(rc != 0);
      if(rc != 0)
	rainbow_pixels[rainbow_ncolours++] = col.pixel;
    }
  
  // unlock X11 mutex
  unlock();
}


/*==============================================================================*/
// Start graphics and returns a window number

public int
graphic_start (int size_x,	       // I: number of pixels in x-dimension
	       int size_y,	       // I: number of pixels in y-dimension
	       char *windowname	       // I: window title
	)
{
  static int initialized = 0;
  int rc;
  int ncolours;
  XColor col;
  Window window;
  int width, height;
  Pixmap pixmap;
  GC gc;
  int rgb[GRAPHIC_MAX_COLOR][3] = {
    {65535, 65535, 65535},	       /* white */
    {0, 0, 0},			       /* black */
    {0, 0, 65535},		       /* blue */
    {65535, 0, 0},		       /* red */
    {0, 65535, 0},		       /* green */
    {65535, 65535, 0},		       /* yellow */
    {65535, 35000, 0},		       /* orange */
    {12000, 60000, 50000}	       /* turquoise */
  };


  /*--------------------------------------------------------------------------*/
  /* startup */

  if (!initialized || (window_counter == 0))
    {
      initialized = 1;

      for (int i = 0; i < GRAPHIC_WINDOW_MAX; i++)
	window_descr[i].thread_id = i;

      // connect to X-Server
      char *display_name = getenv ("DISPLAY");
      if ((display = XOpenDisplay (display_name)) == NULL)
	{
	  fprintf (stderr, "%s: No connection to XServer %s\n", windowname, XDisplayName (display_name));
	  return -1;
	}

      // get default screen
      screen = DefaultScreen (display);
      assert(screen >= 0);

      // ------------------------------------------------------------------------

      // allocate colors
      defaultcolormap = DefaultColormap (display, screen);
      assert(defaultcolormap != (Colormap)0);

      // default colors
      ncolours = 0;
      for (int i = 0; i < GRAPHIC_MAX_COLOR; i++)
	{
	  col.red = rgb[i][0];
	  col.green = rgb[i][1];
	  col.blue = rgb[i][2];
	  int rc = XAllocColor (display, defaultcolormap, &col);
	  assert(rc != 0);
	  if( rc != 0)
	    default_pixels[ncolours++] = col.pixel;
	}

      // set error handler
      XSetErrorHandler(error_handler);
#if 0 // only for debug purposes (slows down)
      XSynchronize(display, 1);
#endif
    }


  // ----------------------------------------------------------------------------
  // a new window

  ++n_windows;
  window_counter++;

  if(n_windows > GRAPHIC_WINDOW_MAX)
    {
      fprintf (stderr, "error: too many windows used\n");
      return -1;
    }

  // create window of given size
  window = window_descr[n_windows].window =
	  XCreateSimpleWindow (display, RootWindow (display, screen), 0, 0, size_x, size_y, 4,
			       BlackPixel (display, screen), WhitePixel (display, screen));
  assert(window != (Window)0);

  // store window name
  rc = XStoreName (display, window, windowname);
  assert(rc != 0);

  // create a new backing pixmap which is our default output device
  width = window_descr[n_windows].width = size_x;
  height = window_descr[n_windows].height = size_y;
  pixmap = window_descr[n_windows].output_device = XCreatePixmap (display, window, size_x, size_y,
								  DefaultDepth (display, DefaultScreen (display)));
  assert(pixmap != (Pixmap)NULL);


  // ----------------------------------------------------------------------------
  // color management

  // allocate colors
  defaultcolormap = DefaultColormap (display, screen);
  assert(defaultcolormap != (Colormap)0);
  gc = window_descr[n_windows].gc = XCreateGC (display, window_descr[n_windows].output_device, 0, 0);
  assert(gc > 0);

  // clean pixmap
  rc = XSetForeground (display, gc, WhitePixel (display, screen));
  assert(rc != 0);
  rc = XFillRectangle (display, pixmap, gc, 0, 0, width, height);
  assert(rc != 0);

  // default background color
  rc = XSetBackground (display, gc, WhitePixel (display, screen));
  assert(rc != 0);
  rc = XSetForeground (display, gc, BlackPixel (display, screen));
  assert(rc != 0);

  // ----------------------------------------------------------------------------
  // finish up

  // map window
  rc = XMapWindow (display, window);
  assert(rc != 0);

#if 0 // we have problems with this when opening a new window after closing all
  rc = XSync (display, 0);
  assert(rc != 0);
#endif

  rc = XFlush (display);
  assert(rc != 0);


  // ----------------------------------------------------------------------------
  // start event handler

  // subscribe to the given set of event types
  rc = XSelectInput (display, window, ExposureMask);
  assert(rc != 0);
  
  // create a thread to handle events in the background
  if ((rc = pthread_create (&window_descr[n_windows].event_handler_thread, NULL,
			    handler_thread, &window_descr[n_windows].thread_id)) != 0)
    {
      fprintf (stderr, "can not start event handler thread (%s)\n", strerror (rc));
      return -1;
    }

  // define default user coordinate system
  graphic_userCoordinateSystem (n_windows, 0.0, 0.0, size_x, size_y);


  return n_windows;
}


/*==============================================================================*/
// close graphic

public void
graphic_end (int window_number	       // I: window number
	)
{
  assert ((window_number >= 0) && (window_number <= n_windows));
  int rc;

  // flush everything
  rc = XFlush (display);
  assert(rc != 0);

  // cancel thread
  lock();
  pthread_cancel (window_descr[window_number].event_handler_thread);
  unlock();

  // close the window
  rc = XDestroyWindow (display, window_descr[window_number].window);
  assert(rc != 0);

  // free the pixmap with the given ID
  rc = XFreePixmap (display, window_descr[window_number].output_device);
  assert(rc != 0);

#if 0 // do we need that really?
  // all windows closed?
  if (--window_counter == 0)
    {
      // close the display
      rc = XCloseDisplay (display);
      assert(rc != 0);
    }
#endif
}


/*============================================================================*/
// flush everything to display

public int
graphic_flush (int window_number       // I: window number
	)
{
  assert ((window_number >= 0) && (window_number <= n_windows));
  int rc;

  // lock X11 mutex
  lock ();

  rc = XCopyArea (display, window_descr[window_number].output_device,
		  window_descr[window_number].window,
		  window_descr[window_number].gc, 0, 0,
		  window_descr[window_number].width, window_descr[window_number].height, 0, 0);
  assert(rc != 0);

  rc = XFlush (display);
  assert(rc != 0);

  // unlock X11 mutex
  unlock ();

  return 0;
}

/*==============================================================================*/
// define user coordinate system

public void
graphic_userCoordinateSystem (int window_number,	// I: window number
			      double x_min1,	// I: lower left x-coordinate
			      double y_min1,	// I: lower left y-coordinate
			      double x_max1,	// I: upper right x-coordinate
			      double y_max1	// I: upper right y-coordinate
	)
{
  coord_system[window_number].x_min = x_min1;
  coord_system[window_number].y_min = y_min1;
  coord_system[window_number].x_max = x_max1;
  coord_system[window_number].y_max = y_max1;
  coord_system[window_number].width = x_max1 - x_min1;
  coord_system[window_number].height = y_max1 - y_min1;
}


/*==============================================================================*/
// set foreground color with a default color

public void
graphic_setColor (int window_number,   // I: window number
		  int color	       // I: predefined color to set
	)
{
  assert ((window_number >= 0) && (window_number <= n_windows));

  // map to allowed values
  if (color < 0)
    color = 0;
  if (color >= GRAPHIC_MAX_COLOR)
    color = GRAPHIC_MAX_COLOR - 1;

  // lock X11 mutex
  lock ();

  int rc = XSetForeground (display, window_descr[window_number].gc, default_pixels[color]);
  assert(rc != 0);

  // unlock X11 mutex
  unlock ();
}

/*==============================================================================*/
// set foreground color with an RGB color

public void
graphic_setRGBColor (int window_number,	// I: window number
		     int red,	       // I: red component (0<=red<=255)
		     int green,	       // I: green component (0<=green<=255)
		     int blue	       // I: blue component (0<=blue<=255)
	)
{
  assert ((window_number >= 0) && (window_number <= n_windows));

  if(!rgb_initialized)
    initialize_rgb();

  // map to allowed values
  unsigned int r = red & 0xff;
  unsigned int g = green & 0xff;
  unsigned int b = blue & 0xff;

  // convert a RGB value into an index into the color map
  unsigned int color = ((r / RGB_MAX) * RGB_MAX * RGB_MAX) + ((g / RGB_MAX) * RGB_MAX) + (b / RGB_MAX);

  // lock X11 mutex
  lock ();

  int rc = XSetForeground (display, window_descr[window_number].gc, RGB_pixels[color]);
  assert(rc != 0);

  // unlock X11 mutex
  unlock ();
}


/*==============================================================================*/
// set foreground color with a rainbow color

public void
graphic_setRainbowColor (int window_number,	// I: window number
			 double rainbow_color	// I: rainbow value (0<=rainbow<=1.0)
	)
{
  assert ((window_number >= 0) && (window_number <= n_windows));

  if(!rainbow_initialized)
    initialize_rainbow();

  // map to allowed values
  if (rainbow_color > 1.0)
    rainbow_color = 1.0;
  if (rainbow_color < 0.0)
    rainbow_color = 0.0;

  int color = (int) (rainbow_color * (GRAPHIC_RAINBOW_MAX - 1));

  // lock X11 mutex
  lock ();

  int rc = XSetForeground (display, window_descr[window_number].gc, rainbow_pixels[color]);
  assert(rc != 0);

  // unlock X11 mutex
  unlock ();
}


/*==============================================================================*/
// set foreground color with a gray color

public void
graphic_setGrayColor (int window_number,	// I: window number
		      int gray_color   // I: gray value (0<=gray<=255)
	)
{
  assert ((window_number >= 0) && (window_number <= n_windows));

  if(!gray_initialized)
    initialize_gray();

  // map to allowed values
  if (gray_color < 0)
    gray_color = 0;
  if (gray_color > 255)
    gray_color = 255;

  // lock X11 mutex
  lock ();

  int rc = XSetForeground (display, window_descr[window_number].gc, gray_pixels[gray_color]);
  assert(rc != 0);

  // unlock X11 mutex
  unlock ();
}


/*==============================================================================*/
// draw point

public void
graphic_drawPoint (int window_number,  // I: window number
		   double x,	       // I: x coordinate
		   double y	       // I: y coordinate
	)
{
  assert ((window_number >= 0) && (window_number <= n_windows));

  int rc;
  int sx, sy;
  mapCoordinates (window_number, x, y, &sx, &sy);

  if(window_descr[window_number].width < coord_system[window_number].width)
    {
      // lock X11 mutex
      lock ();

      // at least one pixel
      rc = XDrawPoint (display, window_descr[window_number].output_device, window_descr[window_number].gc, sx, sy);
      assert(rc != 0);
    }
  else
    {
      // fill corresponding rectangle
      int sx1, sy1, sx2, sy2;
      mapCoordinates (window_number, x, y, &sx1, &sy1);
      mapCoordinates (window_number, x+1, y+1, &sx2, &sy2);

      int h, w, sh, sw;
      w = abs(sx1 - sx2);
      h = abs(sy1 - sy2);
      sw = distanzX (window_number, w);
      sh = distanzY (window_number, h);
      
      // lock X11 mutex
      lock ();

      rc = XFillRectangle (display, window_descr[window_number].output_device, window_descr[window_number].gc, sx, sy, sw, sh);
      assert(rc != 0);
    }

  // unlock X11 mutex
  unlock ();
}


/*==============================================================================*/
// draw line

public void
graphic_drawLine (int window_number,   // I: window number
		  double x1,	       // I: lower left corner x
		  double y1,	       // I: lower left corner y
		  double x2,	       // I: upper right corner x
		  double y2	       // I: upper right corner y
	)
{
  assert ((window_number >= 0) && (window_number <= n_windows));

  int sx1, sy1, sx2, sy2;
  mapCoordinates (window_number, x1, y1, &sx1, &sy1);
  mapCoordinates (window_number, x2, y2, &sx2, &sy2);

  // lock X11 mutex
  lock ();

  int rc = XDrawLine (display, window_descr[window_number].output_device, window_descr[window_number].gc, sx1, sy1, sx2, sy2);
  assert(rc != 0);

  // unlock X11 mutex
  unlock ();
}


/*==============================================================================*/
// draw circle

public void
graphic_drawCircle (int window_number, // I: window number
		    double x,	       // I: center x
		    double y,	       // I: center y
		    double radius      // I: radius
	)
{
  assert ((window_number >= 0) && (window_number <= n_windows));

  int sx, sy, sradius;
  mapCoordinates (window_number, x - radius, y + radius, &sx, &sy);
  sradius = distanzX (window_number, radius);

  // lock X11 mutex
  lock ();

  int rc = XDrawArc (display, window_descr[window_number].output_device, window_descr[window_number].gc, sx, sy, 2 * sradius, 2 * sradius, 0, 64 * 360);
  assert(rc != 0);


  // unlock X11 mutex
  unlock ();
}


/*==============================================================================*/
// draw filled circle

public void
graphic_drawCircleFilled (int window_number,	// I: window number
			  double x,    // I: center x
			  double y,    // I: center y
			  double radius	// I: radius
	)
{
  assert ((window_number >= 0) && (window_number <= n_windows));

  int sx, sy, sradius;
  mapCoordinates (window_number, x - radius, y + radius, &sx, &sy);
  sradius = distanzX (window_number, radius);

  // lock X11 mutex
  lock ();

  int rc = XFillArc (display, window_descr[window_number].output_device, window_descr[window_number].gc, sx, sy, 2 * sradius, 2 * sradius, 0, 64 * 360);
  assert(rc != 0);
      

  // unlock X11 mutex
  unlock ();
}


/*==============================================================================*/
// draw square

public void
graphic_drawSquare (int window_number, // I: window number
		    double x1,	       // I: lower left corner x
		    double y1,	       // I: lower left corner y
		    double x2,	       // I: upper ight corner x
		    double y2	       // I: upper ight corner y
	)
{
  assert ((window_number >= 0) && (window_number <= n_windows));

  double x, y, h, w;

  x = min (x1, x2);
  w = max (x1, x2) - min (x1, x2);
  y = max (y1, y2);
  h = max (y1, y2) - min (y1, y2);

  int sx, sy, sw, sh;
  mapCoordinates (window_number, x, y, &sx, &sy);
  sw = distanzX (window_number, w);
  sh = distanzY (window_number, h);

  // lock X11 mutex
  lock ();

  int rc = XDrawRectangle (display, window_descr[window_number].output_device, window_descr[window_number].gc, sx, sy, sw, sh);
  assert(rc != 0);


  // unlock X11 mutex
  unlock ();
}


/*==============================================================================*/
// draw filled square

public void
graphic_drawSquareFilled (int window_number,	// I: window number
			  double x1,   // I: lower left corner x
			  double y1,   // I: lower left corner y
			  double x2,   // I: upper ight corner x
			  double y2    // I: upper ight corner y
	)
{
  assert ((window_number >= 0) && (window_number <= n_windows));

  double x, y, h, w;

  x = min (x1, x2);
  w = max (x1, x2) - min (x1, x2);
  y = max (y1, y2);
  h = max (y1, y2) - min (y1, y2);

  int sx, sy, sw, sh;
  mapCoordinates (window_number, x, y, &sx, &sy);
  sw = distanzX (window_number, w);
  sh = distanzY (window_number, h);

  // lock X11 mutex
  lock ();

int rc =   XFillRectangle (display, window_descr[window_number].output_device, window_descr[window_number].gc, sx, sy, sw, sh);
 assert(rc != 0);


  // unlock X11 mutex
  unlock ();
}


/*==============================================================================*/
// draw polygon

public void
graphic_drawPolygon (int window_number,	// I: window number
		     double *x,	       // I: vector of x coordinates
		     double *y,	       // I: vector y coordinates
		     int npoints       // I: vector length
	)
{
  assert ((window_number >= 0) && (window_number <= n_windows));

  XPoint points[npoints];

  for (int i = 0; i < npoints; i++)
    {
      int sx, sy;
      mapCoordinates (window_number, x[i], y[i], &sx, &sy);
      points[i].x = sx;
      points[i].y = sy;
    }

  // lock X11 mutex
  lock ();

  int rc = XDrawLines (display, window_descr[window_number].output_device, window_descr[window_number].gc, points, npoints, CoordModeOrigin);
  assert(rc != 0);


  // unlock X11 mutex
  unlock ();
}


/*==============================================================================*/
// draw filled polygon

public void
graphic_drawPolygonFilled (int window_number,	// I: window number
			   double *x,  // I: vector of x coordinates
			   double *y,  // I: vector y coordinates
			   int npoints // I: vector length
	)
{
  assert ((window_number >= 0) && (window_number <= n_windows));

  XPoint points[npoints];

  for (int i = 0; i < npoints; i++)
    {
      int sx, sy;
      mapCoordinates (window_number, x[i], y[i], &sx, &sy);
      points[i].x = sx;
      points[i].y = sy;
    }

  // lock X11 mutex
  lock ();

  int rc = XFillPolygon (display, window_descr[window_number].output_device, window_descr[window_number].gc, points, npoints, Complex, CoordModeOrigin);
  assert(rc != 0);


  // unlock X11 mutex
  unlock ();
}


/*==============================================================================*/
// draw string

public void
graphic_drawString (int window_number, // I: window number
		    double x,	       // I: x coordinate
		    double y,	       // I: y coordinate
		    char *string,      // I: string
		    int fontsize       // I: font size (from predefined fonts)
	)
{
  assert ((window_number >= 0) && (window_number <= n_windows));
  int rc;

  if(!font_initialized)
    initialize_font();

  int font = -1;
  for (int fnr = 0; fnr < NFONTS; fnr++)
    if (fontsizes[fnr] == fontsize)
      {
	font = fnr;
	break;
      }
  if (font == -1)
    // error case: use largest font
    font = NFONTS - 1;

  int sx, sy;
  mapCoordinates (window_number, x, y, &sx, &sy);


  // lock X11 mutex
  lock ();

  rc = XSetFont (display, window_descr[window_number].gc, font_info[font]->fid);
  assert(rc != 0);
  rc = XDrawString (display, window_descr[window_number].output_device,
		    window_descr[window_number].gc, sx, sy, string, strlen (string));
#if 0 // here we have sometimes problems with rc == 0. Why?
  assert(rc != 0);
#endif

  // unlock X11 mutex
  unlock ();
}


/*==============================================================================*/
// turtle graphics

static double pos_x, pos_y, direction_angle;

// move position without drawing
public void
graphic_goto (int window_number,       // I: window number
	      double x,		       // I: x coordinate
	      double y		       // I: y coordinate
	)
{
  assert ((window_number >= 0) && (window_number <= n_windows));

  pos_x = x;
  pos_y = y;
}


// goto position with drawing
public void
graphic_moveto (int window_number,     // I: window number
		double x,	       // I: x coordinate
		double y	       // I: y coordinate
	)
{
  assert ((window_number >= 0) && (window_number <= n_windows));

  graphic_drawLine (window_number, pos_x, pos_y, x, y);

  pos_x = x;
  pos_y = y;
}


// move on n steps in current direction
public void
graphic_moveon (int window_number,     // I: window number
		double steps	       // I: steps in current direction
	)
{
  assert ((window_number >= 0) && (window_number <= n_windows));

  double xold = pos_x;
  double yold = pos_y;
  double toRadians = PI / 180.0;
  double xnew = xold + steps * cos (toRadians * direction_angle);
  double ynew = yold + steps * sin (toRadians * direction_angle);

  graphic_drawLine (window_number, xold, yold, xnew, ynew);

  pos_x = xnew;
  pos_y = ynew;
}


// rotate counterclockwise direction
public void
graphic_rotate (int window_number,     // I: window number
		double angle	       // I: angle
	)
{
  assert ((window_number >= 0) && (window_number <= n_windows));

  direction_angle += angle;
}


// get current x position
public double
graphic_getX (void)
{
  return pos_x;
}


// get current y position
public double
graphic_getY (void)
{
  return pos_y;
}


/*==============================================================================*/
/* main program (for test purposes) */

#if defined(TEST)

#define X 800
#define Y 800

int
main (int argc, char **argv)
{
  int rc;
  int window_number;
  double x[] = { 0, 200, 800, 900 };
  double y[] = { 12, 100, 500, 700 };
  int n_windows = 2;


  for (int w = 0; w < n_windows; w++)
    {
      // start graphic
      if ((window_number = graphic_start (X, Y, "My Test")) < 0)
	{
	  printf ("unable to open display\n");
	  exit (EXIT_FAILURE);
	}

      if (w == 0)
	graphic_userCoordinateSystem (window_number, -X, -Y, +X, +Y);


      // draw line with points (in standard colors)
      for (int i = 0; i < X / 2; i++)
	for (int j = 0; j < Y / 2; j++)
	  {
	    graphic_setColor (window_number, (int) ((i / (double) X * 2) * GRAPHIC_MAX_COLOR));
	    graphic_drawPoint (window_number, i, j);
	  }
      rc = graphic_flush (window_number);
      assert (rc == 0);

      // draw line with points (in rainbow colors)
      for (int i = 0; i < X / 2; i++)
	for (int j = Y / 2; j < Y; j++)
	  {
	    graphic_setRainbowColor (window_number, (double) i / (X / 2));
	    graphic_drawPoint (window_number, i, j);
	  }
      rc = graphic_flush (window_number);
      assert (rc == 0);

      // draw line with points (in RGB colors)
      for (int i = X / 2; i < X; i++)
	for (int j = 0; j < Y / 2; j++)
	  {
	    int r = min (255, X / 2 - i);
	    int g = min (255, j);
	    int b = min (255, (X / 2) - i + j);
	    graphic_setRGBColor (window_number, r, g, b);
	    graphic_drawPoint (window_number, i, j);
	  }
      rc = graphic_flush (window_number);
      assert (rc == 0);

      // draw line with points (in gray)
      for (int i = X / 2; i < X; i++)
	for (int j = Y / 2; j < Y; j++)
	  {
	    graphic_setGrayColor (window_number, (i + j) % 256);
	    graphic_drawPoint (window_number, i, j);
	  }
      rc = graphic_flush (window_number);
      assert (rc == 0);

      graphic_setColor (window_number, GRAPHIC_BLACK);
      graphic_drawLine (window_number, 0, 0, X, Y);
      graphic_setColor (window_number, GRAPHIC_WHITE);
      graphic_drawPolygon (window_number, x, y, 3);
      graphic_setColor (window_number, GRAPHIC_BLUE);
      graphic_drawPolygonFilled (window_number, x, y, 3);
      graphic_setColor (window_number, GRAPHIC_RED);
      graphic_drawSquare (window_number, 0, 0, X / 2, Y / 2);
      graphic_setColor (window_number, GRAPHIC_GREEN);
      graphic_drawSquareFilled (window_number, 0, 0, X / 4, Y / 4);
      graphic_setColor (window_number, GRAPHIC_YELLOW);
      graphic_drawCircle (window_number, X / 2, Y / 2, X / 2);
      graphic_setColor (window_number, GRAPHIC_ORANGE);
      graphic_drawCircleFilled (window_number, X / 2, Y / 2, X / 4);
      graphic_setColor (window_number, GRAPHIC_WHITE);
      graphic_drawString (window_number, X / 2, Y / 2, "test", 12);
      rc = graphic_flush (window_number);
      assert (rc == 0);
    }

  // close windows
  for (int w = 0; w < n_windows; w++)
    graphic_end (w);


  // turtle graphics
  if ((window_number = graphic_start (X, Y, "Turtle Test")) < 0)
    {
      printf ("unable to open display\n");
      exit (EXIT_FAILURE);
    }
  graphic_goto (window_number, X / 2, Y / 2);
  rc = graphic_flush (window_number);
  graphic_moveon (window_number, 100.0);
  rc = graphic_flush (window_number);
  for (int i = 0; i < 4; i++)
    {
      graphic_rotate (window_number, 90.0);
      graphic_moveon (window_number, 100.0);
      rc = graphic_flush (window_number);
    }


  return EXIT_SUCCESS;
}

#endif

/*============================================================================*
 *                             that's all folks                               *
 *============================================================================*/
