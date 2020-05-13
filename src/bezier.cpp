
/*                      D‚mo sur les courbes de Bezier


	Corps principal                 VANBELLE Edouard (c) 1998       */


#define TRUE   1
#define FALSE  0

#define MENU   1
#define SAISIE 2

#define XMAX   639
#define YMAX   479
#define CARRE  YMAX-17
#define DEP    59
#define ST     6
#define ITEMS  13

#define TINIT  200
#define TMAX   1000
#define TPAS   1

#define PMAX   100

#include <math.h>
#include <complex.h>
#include <stdio.h>
#include <time.h>
#include <graph.cpp> // gestion de l'‚cran (fonctions BIOS: INT 10h)
#include <mouse.cpp> // gestion de la souris (INT 33h)

void echo (char* text,BYTE C=GREEN)
{
 mouse_cache();
 outtextxy (1,29,text,C);
 mouse_affiche();
}

int btn,X,Y,XX=-1,
            YY=-1;
char fin      =FALSE,
     nbr_pts  =0,
     aff_poly =TRUE,
     intact   =TRUE;
int  courbe_t =TINIT,
     contexte =0;

typedef struct spoint
        {
         float x,y;
        } point;

point p[2][PMAX];

void reset_tableau(void)
{
 char i=0,j;
 for (;i<2;i++)
  for (j=0;j<PMAX;j++)
   p[i][j].x=p[i][j].y=0;
}

void CastelJau (int k,float t,int *newx,int *newy)
{
 int i,j;
 for (i=0;i<k-1;i++)
  {
   p[1][i].x=t*p[0][i].x+(1-t)*p[0][i+1].x;
   p[1][i].y=t*p[0][i].y+(1-t)*p[0][i+1].y;
  }
 for (j=k-1;i>1;j--)
  for (i=0;i<j-1;i++)
   {
    p[1][i].x=t*p[1][i].x+(1-t)*p[1][i+1].x;
    p[1][i].y=t*p[1][i].y+(1-t)*p[1][i+1].y;
   }
 *newx=(int)(p[1][0].x);
 *newy=(int)(p[1][0].y);
}

void aff_courbe(char C=WHITE)
{
 int x,y,xx,yy;
 float t,inc;
 point last;
 int k=nbr_pts;

 inc=(float)(1/(float)courbe_t);
 CastelJau (k,0,&xx,&yy);

 mouse_cache();

 for (t=inc;t<1;t+=inc)
 {
  CastelJau (k,t,&x,&y);
  bez_line  (xx+1,CARRE-yy-1,x+1,CARRE-y-1,C);
  if (keypressed()) getch(); // on vide le buffer
  xx=x;
  yy=y;
 }
 CastelJau (k,1,&x,&y);
 bez_line (xx+1,CARRE-yy-1,x+1,CARRE-y-1,C);

 mouse_affiche();
}

void aff_controle(char C=MAGENTA)
{
 mouse_cache();
 for(int i=0;i<nbr_pts-1;i++)
 {
  bez_line (p[0][  i].x+1,CARRE-p[0][  i].y-1,
            p[0][i+1].x+1,CARRE-p[0][i+1].y-1,C);
  if (C==MAGENTA) bez_putpixel (p[0][i].x+1,CARRE-p[0][i].y-1,YELLOW);
 }
 if (C==MAGENTA) bez_putpixel (p[0][nbr_pts-1].x+1,CARRE-p[0][nbr_pts-1].y-1,YELLOW);
 mouse_affiche();
}

void aff_general (void)
{
 if (aff_poly) aff_controle ();
 aff_courbe   ();
}

#include <menu.cpp>
#include <courbe.cpp>

int main (void)
{

 if (!mouse_init() )
  {
   printf("le driver de la souris n' est pas install‚.\n");
   return(1);
  }

 fond();
 mouse_affiche();
 reset_tableau();
 mouse_click (0,X,Y);

 do
 {
  mouse_event (X,Y,btn);
  if ((X!=XX) || (Y!=YY))
   {
    if ((X<CARRE) && (Y<CARRE))
     {
      outtextxy (70,29,"ÛÛÛÛÛÛÛ",BLACK);
      sprintf (buffer,"%.3d,%.3d",X-1,(CARRE-Y)-1);
      outtextxy (70,29,buffer);
      if (contexte!=SAISIE)
       {
        contexte=SAISIE;
        mouse_pointer(&cros);
       }
     }
    else
     {
      if (contexte!=MENU)
       {
        mouse_cache();
        outtextxy (70,29,"ÛÛÛÛÛÛÛ",BLACK);
        mouse_affiche();
        contexte=MENU;
        mouse_pointer(&normal_curs);
       }
     }
     XX=X;
     YY=Y;
   }
  switch (contexte)
  {
   case MENU:   gere_menu();   break;
   case SAISIE: gere_saisie(); break;
  }
 }
 while (!fin);

 mouse_cache();
 restore_graph();

 return (0);
}

//                                              fin de fichier