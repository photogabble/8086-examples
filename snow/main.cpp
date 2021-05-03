#include <stdlib.h>
#include <conio.h>

#define SCREEN_WIDTH        320       /* width in pixels of mode 4 */
#define SCREEN_HEIGHT       200       /* height in pixels of mode 4 */
#define NUM_COLORS          16         /* number of colors in mode 4 */

typedef unsigned char  byte;
typedef unsigned short word;

byte *VGA=(byte *)0xA0000000L;        /* this points to video memory. */
word *my_clock=(word *)0x0000046C;    /* this points to the 18.2hz system
                                         clock. */

// See Q29. How do #pragma's work?  How do I use inline assembly language?
// in http://www.azillionmonkeys.com/qed/watfaq.shtml#29

// Sets Video mode to CGA (Mode 13) 320x200 with 16 colour palette
extern void SetVideoMode(void);

#pragma aux SetVideoMode = \
    "    mov ax,0013h    " \
    "    int 10h         " \
    modify [ax];

extern void SetTextMode(void);

#pragma aux SetTextMode =  \
    "    mov ax,0003h    " \
    "    int 10h         " \
    modify [ax];

// Pixel snow from http://www.brackeen.com/vga/source/bc31/pixel.c.html
void plot_pixel_fast(int x,int y,byte color)
{
  VGA[y*SCREEN_WIDTH+x]=color;
}
    
int main(void)
{
    SetVideoMode();

    int x,y,color;
    word i;

    /* seed the number generator. */
    srand(*my_clock);

    for(i=0;i<50000L;i++)               /* randomly plot 50000 pixels. */
    {
        x=rand()%SCREEN_WIDTH;
        y=rand()%SCREEN_HEIGHT;
        color=rand()%NUM_COLORS;
        plot_pixel_fast(x,y,color);
    }
    
    getch();
    SetTextMode();
    return 0;
}
