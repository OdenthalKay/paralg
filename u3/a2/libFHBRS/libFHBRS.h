/*==============================================================================
  
   Purpose:    functions of FH library
   Author:     Rudolf Berrendorf
               Computer Science Department
               Bonn-Rhein-Sieg University
	       53754 Sankt Augustin, Germany
               rudolf.berrendorf@h-brs.de

   Version:    3.1

==============================================================================*/

#if !defined(LIBFHBRS_INCLUDED)
#define LIBFHBRS_INCLUDED

#include <stdint.h>


/*----------------------------------------------------------------------------*/
// time functions

// elapsed time
extern double gettime (void);
extern double gettime_ (void);
// cycle counter (sychronized on all cores of one processor)
extern unsigned long long getcycles (void);


/*----------------------------------------------------------------------------*/
// sequential random numbers

// initializes random number generator
extern void rand_init (unsigned long seed);

// returns a random number in the intervall [0,1)
extern double rand_standard (void);

// returns an integer random number in the intervall [0,n)
extern int rand_limit (int n);


/*----------------------------------------------------------------------------*/
// parallel random numbers

// initializes the random number generator
extern void * randp_init (unsigned long seed);

// returns a double random number in the intervall [0,1)
extern double randp_standard (void *generator);

// vector of random numbers
extern void randp_vector (void * generator, int n, double a[n]);

// returns an integer random number in the intervall [0,n)
extern unsigned long randp_limit (void * generator, unsigned long n);


/*----------------------------------------------------------------------------*/
// some graphic definitions

// window coordinates:
// origin is in the lower left corner
// - x-dimension goes to the right
// - y-dimension goes to the top

// maximum number of windows
#define GRAPHIC_WINDOW_MAX 10

/* possible colors specifications
 * 1) default colors (8) given by predefined names (see below)
 * 2) grey "colors" given by 0-255
 * 3) RGB colors given by (r << 0) | (g << 8) | (b << 16)
 * 4) 1024 Rainbow colors given in the range 0.0 - 1.0 and mapped to the rainbow spectrum
 */
// number of predefined colors
#define GRAPHIC_MAX_COLOR   8
// predefined color names
#define GRAPHIC_WHITE     0u
#define GRAPHIC_BLACK     1u
#define GRAPHIC_BLUE      2u
#define GRAPHIC_RED       3u
#define GRAPHIC_GREEN     4u
#define GRAPHIC_YELLOW    5u
#define GRAPHIC_ORANGE    6u
#define GRAPHIC_TURQUOISE 7u

// number of gray colors
#define GRAPHIC_MAX_GRAY    256u

// number of Rainbow colors
#define GRAPHIC_RAINBOW_MAX 1024u

// RGB colors
typedef unsigned int rgb_t;
// RGB values are packed into one int as three 8 Bit values
#define RGB_VALUE(red, green, blue) (  (((red)    & 0xff)) \
                                     | ((((green) & 0xff) << 8)) \
				     | ((((blue)  & 0xff) << 16)) )
// RGB values are packed into one int as three 8 Bit values
#define RGB_RED(rgb_value)     (((rgb_value) & 0xff))
#define RGB_GREEN(rgb_value)   ((((rgb_value) >> 8) & 0xff))
#define RGB_BLUE(rgb_value)    ((((rgb_value) >> 16) & 0xff))


/*----------------------------------------------------------------------------*/
// graphic functions

// Start graphics and returns a window number
extern int graphic_start (int size_x,  // I: number of pixels in x-dimension
			  int size_y,  // I: number of pixels in y-dimension
			  char *windowname	// I: window title
	);

// close graphic
extern void graphic_end (int window_number	// I: window number
	);

// flush everything to display
extern int graphic_flush (int window_number	// I: window number
	);

// define user coordinate system
extern void graphic_userCoordinateSystem (int window_number,	// I: window number
					  double x_min1,	// I: lower left x-coordinate
					  double y_min1,	// I: lower left y-coordinate
					  double x_max1,	// I: upper right x-coordinate
					  double y_max1	// I: upper right y-coordinate
	);

// set foreground color with a default color
extern void graphic_setColor (int window_number,	// I: window number
			      int color	// I: predefined color to set
	);

// set foreground color with an RGB color
extern void graphic_setRGBColor (int window_number,	// I: window number
				 int red,	// I: red component (0<=red<=255)
				 int green,	// I: green component (0<=green<=255)
				 int blue	// I: blue component (0<=blue<=255)
	);

// set foreground color with a rainbow color
extern void graphic_setRainbowColor (int window_number,	// I: window number
				     double rainbow_color	// I: rainbow value
				     // (0<=rainbow<=1.0)
	);

// set foreground color with a gray color
extern void graphic_setGrayColor (int window_number,	// I: window number
				  int gray_color	// I: gray value (0<=gray<=255)
	);


// draw point
extern void graphic_drawPoint (int window_number,	// I: window number
			       double x,	// I: x coordinate
			       double y	// I: y coordinate
	);

// draw line
extern void graphic_drawLine (int window_number,	// I: window number
			      double x1,	// I: lower left corner x
			      double y1,	// I: lower left corner y
			      double x2,	// I: upper right corner x
			      double y2	// I: upper right corner y
	);

// draw circle
extern void graphic_drawCircle (int window_number,	// I: window number
				double x,	// I: center x
				double y,	// I: center y
				double radius	// I: radius
	);

// draw filled circle
extern void graphic_drawCircleFilled (int window_number,	// I: window number
				      double x,	// I: center x
				      double y,	// I: center y
				      double radius	// I: radius
	);

// draw square
extern void graphic_drawSquare (int window_number,	// I: window number
				double x1,	// I: lower left corner x
				double y1,	// I: lower left corner y
				double x2,	// I: upper ight corner x
				double y2	// I: upper ight corner y
	);

// draw filled square
extern void graphic_drawSquareFilled (int window_number,	// I: window number
				      double x1,	// I: lower left corner x
				      double y1,	// I: lower left corner y
				      double x2,	// I: upper ight corner x
				      double y2	// I: upper ight corner y
	);

// draw polygon
extern void graphic_drawPolygon (int window_number,	// I: window number
				 double *x,	// I: vector of x coordinates
				 double *y,	// I: vector y coordinates
				 int npoints	// I: vector length
	);

// draw filled polygon
extern void graphic_drawPolygonFilled (int window_number,	// I: window number
				       double *x,	// I: vector of x coordinates
				       double *y,	// I: vector y coordinates
				       int npoints	// I: vector length
	);

// draw string
extern void graphic_drawString (int window_number,	// I: window number
				double x,	// I: x coordinate
				double y,	// I: y coordinate
				char *string,	// I: string
				int fontsize	// I: font size (from predefined fonts)
	);

// turtle graphics

// move position without drawing
extern void graphic_goto (int window_number,	// I: window number
			  double x,    // I: x coordinate
			  double y     // I: y coordinate
	);

// goto position with drawing
extern void graphic_moveto (int window_number,	// I: window number
			    double x,  // I: x coordinate
			    double y   // I: y coordinate
	);

// move on n steps in current direction
extern void graphic_moveon (int window_number,	// I: window number
			    double steps	// I: steps in current direction
	);

// rotate counterclockwise direction
extern void graphic_rotate (int window_number,	// I: window number
			    double angle	// I: angle
	);

// get current x position
extern double graphic_getX (void);

// get current y position
extern double graphic_getY (void);


#endif

/*============================================================================*
 *                             that's all folks                               *
 *============================================================================*/
