//Се есть отличный образчик говнокода, который я написал тогда, когда учился программировать

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SCALE 8
#define SCALEm (SCALE-1)
#define LEFT_POINT (100)
#define DOWN_POINT (100)
#define LEFT_BORDER (LEFT_POINT*SCALE)
#define DOWN_BORDER (DOWN_POINT*SCALE)
#define x0 (50*SCALE)
#define y0 (50*SCALE)

// Отмечу характерные особенности этих дефайнов
#define UP 111
#define DOWN 116  
#define LEFT 113 
#define RIGHT 114
// Они обозначают кейкоды, могут отличаться.
// Если не работает, надо запустить xev и понажимать стрелочки
// Вверх, вниз, влево, вправо
// Выставить соответствующие цифры сюда.
// Это можно пофиксить, используя кейсуммы вместо кейкодов, такие дела.

#define GROW 10
#define random(a, b) ((rand() % (b - a + 1)) + a)

inline void rebuildchain (unsigned int *, unsigned int *,
         unsigned int *, unsigned int *);



inline char selfeat (unsigned int *x, unsigned int  *y,
                      unsigned int xn, unsigned int  yn,
                      unsigned int LENGTH)
{
    unsigned int count=1;
    while (count != LENGTH) 
    {
        if ((x[count] == xn) && (y[count] == yn)) return (0);
        count++;
    }
    return (1);
}


////////////////////////////////////////////////////////////////////////
///////////////////////// КОПИПАСТ - ГОВНО /////////////////////////////

inline void blacktohead (Display *d, Window w, unsigned int s,
         unsigned int *x, unsigned int *y)
{
    XSetForeground(d, DefaultGC(d, s), 0x000000);
    XDrawRectangle(d, w, DefaultGC(d, s), x[0], y[0], SCALEm, SCALEm);
    //закрашиваем черным то место, где была голова
}

inline void whitetoass (Display *d, Window w, unsigned int s,
         unsigned int *x, unsigned int *y, unsigned int *mv)
{
    XSetForeground(d, DefaultGC(d, s), 0xFFFFFF);
    XDrawRectangle(d, w, DefaultGC(d, s), x[*mv], y[*mv], SCALEm, SCALEm);
    //закрашиваем белым то место, где был зад
}

inline void redhead (Display *d, Window w, unsigned int s,
         unsigned int *x, unsigned int *y)
{
    XSetForeground(d, DefaultGC(d, s), 0xFF0000);
    XDrawRectangle(d, w, DefaultGC(d, s), x[0], y[0], SCALEm, SCALEm);
    //рисуем красную головку
}

////////////////////////////////////////////////////////////////////////


inline void rebuildchain (unsigned int *x, unsigned int *y,
         unsigned int *LENGTH, unsigned int *mv)
{
    if (*mv == 1)
    {
        x[*LENGTH]=x[1];
        y[*LENGTH]=y[1];
        x[1]=LEFT_BORDER;
        y[1]=DOWN_BORDER;
        return;
    }
    unsigned int count=1, Nmv = *mv;
    unsigned int xn[*LENGTH], yn[*LENGTH];
    xn[*LENGTH]=x[*mv];
    yn[*LENGTH]=y[*mv];
    do
    {
        xn[count]=x[*mv];
        yn[count]=y[*mv];
        count++; (*mv)++;
    }
    while (*mv != *LENGTH);
    *mv=1;
    do
    {
        xn[count]=x[*mv];
        yn[count]=y[*mv];
        count++; (*mv)++;
    }
    while (*mv != Nmv);
    count=1;
    *mv=1;
    do
    {
        x[count]=xn[count];
        y[count]=yn[count];
        count++;
    }
    while (count != (*LENGTH+1));
    x[1]=LEFT_BORDER;
    y[1]=DOWN_BORDER;
}

inline void step (unsigned int *x, unsigned int *y, unsigned int *mv)
{
    y[*mv]=*y;
    x[*mv]=*x;
}

inline void draweatpoint (Display *d, Window w, unsigned int s,
         unsigned int *xrand, unsigned int *yrand)
{
    XSetForeground(d, DefaultGC(d, s), 0x00FF00);
    XDrawRectangle(d, w, DefaultGC(d, s), *xrand, *yrand, SCALEm, SCALEm); //рисуем точку для пожирания
}


// Обработка нажатия кнопки
inline void read_KeyPress 
                  (XEvent *eventp, unsigned int *x,
                   unsigned int  *y, unsigned int *mv,
                   char *plusminus, Display *d, Window w,
                   unsigned int s, unsigned int *LENGTH,
                   unsigned int *xrand, unsigned int *yrand)
{
    unsigned int Xnw=0, Ynw=0;
    XKeyEvent *e = (XKeyEvent *) eventp;
    switch (e->keycode)
    {
        case UP:
            Ynw = y[0]-SCALE;
            Xnw = x[0];
            break;
        case DOWN:
            Ynw = y[0]+SCALE;
            Xnw = x[0];
            break;
        case LEFT:
            Ynw = y[0];
            Xnw = x[0]-SCALE;
            break;
        case RIGHT:
            Ynw = y[0];
            Xnw = x[0]+SCALE;
            break;
        default:
            return;
    }
    if (((Xnw!=x[0]) ^ (Ynw!=y[0])) && ((Xnw) != (LEFT_BORDER-SCALE)) \
        && ((Ynw) != (DOWN_BORDER-SCALE)) && (selfeat (x, y, Xnw, Ynw, *LENGTH)) \
        && ((Xnw) != 0) && ((Ynw) != 0) )
    {
        blacktohead (d, w, s, x, y);
        whitetoass (d, w, s, x, y, mv);
        step (x, y, mv);
        x[0]=Xnw;
        y[0]=Ynw;

        if ((*mv) == (*LENGTH)) {*mv=1; *plusminus=1;}//заменил его этим
        else if ((*mv) == 1) {*mv=*LENGTH; *plusminus=-1;}
        *mv = *mv+*plusminus;
        if ((*x == *xrand) & (*y == *yrand)) //пожирание точки
        {
            rebuildchain(x, y, LENGTH, mv);
            *LENGTH = *LENGTH+GROW;
            *yrand=random(1, DOWN_POINT-2)*SCALE;
            *xrand=random(1, LEFT_POINT-2)*SCALE;
            draweatpoint (d, w, s, xrand, yrand);
        }
    }

    redhead (d, w, s, x, y);

}


 
int main(void) 
{
    srand((unsigned)time(NULL));
    unsigned int yrand=random(1, DOWN_POINT-2)*SCALE;
    unsigned int xrand=random(1, LEFT_POINT-2)*SCALE;
    unsigned int LENGTH = 3;
    unsigned int x[1000], y[1000];
    Display *d;
    Window w;
    Atom delWindow;
    unsigned int s;
    unsigned int mv; //переброс пикселей из зада вперед
    unsigned int count;
    char plusminus=0;
    XEvent e;
    int exit_game = 0;
    
                        /* open connection with the server */
    d = XOpenDisplay(NULL);
    if (d == NULL)
    {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }
 
    s = DefaultScreen(d);
                        /* create window */
    w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, LEFT_BORDER,
                           DOWN_BORDER, 1, BlackPixel(d, s),
                           WhitePixel(d, s));
    delWindow = XInternAtom(d, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(d, w, &delWindow, 1);
                        /* select kind of events we are interested in */
    XSelectInput(d, w, ExposureMask | KeyPressMask);
                        /* map (show) the window */
    XMapWindow(d, w);
    count = 2;
    do
    {
        x[count]=SCALE+x0-(count*SCALE);
        y[count]=y0;
        count++;
    }
    while (count != (LENGTH)); //неактивные куски за пределами экрана
    do
    {
        x[count]=LEFT_BORDER;
        y[count]=DOWN_BORDER;
        count++;
    }
    while (count != 500);
    count = 2;
    mv=1;
    x[0]=x0;
    y[0]=y0;
    x[1]=x0-(LENGTH*SCALE)+SCALE;
    y[1]=y0;
    draweatpoint (d, w, s, &xrand, &yrand);
                           /* event loop */  
    while (!exit_game)
    {
        XNextEvent(d, &e);
        if (e.type == Expose)
        {
            redhead (d, w, s, x, y);
            count=1;
            XSetForeground(d, DefaultGC(d, s), 0x000000);
            //рисуем змею
            do
            {
                XDrawRectangle(d, w, DefaultGC(d, s), x[count], y[count], SCALEm, SCALEm);
                count++;
            }
            while (count != (LENGTH));
            draweatpoint (d, w, s, &xrand, &yrand);
        }    
        if (e.type == KeyPress)
        {
            read_KeyPress (&e, x, y, &mv, &plusminus, d, w, s, &LENGTH,\
            &xrand, &yrand );
        }
        if (e.type == ClientMessage)
        {
            exit_game = 1;
        }
    }

    return 0;
}
