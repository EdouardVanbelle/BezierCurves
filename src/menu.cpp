
#define ESC       27
#define ENTER     13
#define BACKSPACE 8
#define HAUT      72
#define BAS       80
#define DROITE    77
#define GAUCHE    75
#define PGUP      73
#define PGDOWN    81

#define TICK   "¹"
#define BLANC  "█"
#define UP     "\030"
#define DOWN   "\031"

char buffer[255];

char *menu_item[ITEMS];

void charg (void);
char sauve (void);
void nouve (void);
void rafra (void);
void ajout (void);
void modif (void);
void simil (void);
void degre (void);
void envel (void);

void finit (void);

void update_t(int new_t)
{
 if (new_t)
 {
  if (courbe_t!=new_t)
  {
   if (nbr_pts) {
                 if (aff_poly) aff_controle (BLACK);
                 aff_courbe (BLACK);
                }
   courbe_t=new_t;
   if      (courbe_t>TMAX) courbe_t=TPAS;
   else if (courbe_t<TPAS) courbe_t=TMAX;
   if (nbr_pts) {
                 if (aff_poly) aff_controle ();
                 aff_courbe ();
                }
  }
 }
 sprintf (buffer,"%.4d",courbe_t);
 mouse_cache();
 outtextxy (DEP+9,21,buffer,CYAN);
 mouse_affiche();
}

void fond (void)
{
 menu_item[ 0]="Fichier...";
 menu_item[ 1]="  Charger";
 menu_item[ 2]="  Sauver";
 menu_item[ 3]="  Nouveau";
 menu_item[ 4]="  Quitter";
 menu_item[ 5]="Saisie...";
 menu_item[ 6]="  Création/Ajout";
 menu_item[ 7]="  Suppresion/Modif";
 menu_item[ 8]="  Rafraîchir";
 menu_item[ 9]="Application...";
 menu_item[10]="  Similitudes";
 menu_item[11]="  Elévation degré";
 menu_item[12]="  Enveloppe";


 init_graph (VGA);
 mouse_window (1,1,XMAX-2,YMAX-2);
 line (  0,  0,XMAX,  0,BLUE);
 line (  0,  0,  0,YMAX,BLUE);
 line (XMAX,  0,XMAX,YMAX,BLUE);
 line (CARRE,  1,CARRE,CARRE,GREEN);
 line (  1,CARRE,CARRE,CARRE,GREEN);
 outtextxy (DEP,2,"Courbes de Bézier",RED);
 outtextxy (DEP,3,"(c)1998 VANBELLE E.",RED);

 setcolor (CYAN);
 for (int i=0;i<ITEMS;i++)
  outtextxy (DEP+2,ST+i,menu_item[i]);

 outtextxy (DEP,20,TICK); outtext (" Polygone contrôle");
 outtextxy (DEP,21,"Lissage: #### \030\031");
 update_t(0);
 echo ("Chargement... Ok");
 setcolor (GREEN);
}

void erase_bar_bas()
{
 gotoxy (1,29);
 mouse_cache();
 for (int i=1;i<(XMAX>>3);i++) outtext(BLANC,BLACK);
 mouse_affiche();
}

char input (char *text,char *buffer)
{
 char c,i=0;
 int pos;
 mouse_pointer (&montre);
 mouse_cache();
 echo (text);
 outtext   (" _",LIGHTGRAY);
 mouse_affiche();
 pos=wherex()-1;
 buffer[0]=0;
 while ( ((c=getch())!=ENTER) && (c!=ESC) )
 {
  if (c==BACKSPACE)
  {
   if (i>0)
   {
    mouse_cache();
    outtextxy (wherex()-2,29,BLANC,BLACK);
    outtext (BLANC,BLACK);
    mouse_affiche();
    buffer[--i]=0;
   }
  }
  else if (wherex()!=(XMAX>>3))
  {
   buffer[i++]=c;
   buffer[  i]=0;
  }
   mouse_cache();
   outtextxy (pos,29,buffer,LIGHTGRAY);
   outtext   ("_",LIGHTGRAY);
   mouse_affiche();
 }
 mouse_pointer (&normal_curs);
 if (c==ESC) return(FALSE);
 return (TRUE);
}

void test_up_down()
{
 int x,y,m,set1=FALSE,set2=FALSE,set3=FALSE;

 mouse_event (x,y,m);
 x=x>>3;
 y=y>>4;
 if ( (x>=DEP+9) && (x<=DEP+15) && (x!=DEP+13) && (y==21) && (m==1) )

 {
  do
   {
    mouse_event (x,y,m);
    x=x>>3;
    y=y>>4;
    if ( ((x>=DEP+9) && (x<=DEP+12)) && (y==21))
     {
      if (!set3)
      {
       mouse_cache();
       outtextxy  (DEP+9,21,"????",YELLOW);
       mouse_affiche();
       set3=TRUE;
      }
     }
    else if (set3)
     {
      mouse_cache();
      update_t(0);
      mouse_affiche();
      set3=FALSE;
     }
    if ((x==DEP+14) && (y==21))
     {
      if (!set1)
      {
       update_t(courbe_t+TPAS);
       mouse_cache();
       outtextxy  (DEP+14,21,UP,YELLOW);
       mouse_affiche();
       set1=TRUE;
      }
     }
    else if (set1)
     {
      mouse_cache();
      outtextxy  (DEP+14,21,UP,CYAN);
      mouse_affiche();
      set1=FALSE;
     }
    if ((x==DEP+15) && (y==21))
     {
      if (!set2)
      {
       update_t(courbe_t-TPAS);
       mouse_cache();
       outtextxy  (DEP+15,21,DOWN,YELLOW);
       mouse_affiche();
       set2=TRUE;
      }
     }
    else if (set2)
     {
      mouse_cache();
      outtextxy  (DEP+15,21,DOWN,CYAN);
      mouse_affiche();
      set2=FALSE;
     }
   }
  while ( (m) && (!set3) );
  mouse_cache();
  outtextxy (DEP+14,21,UP,CYAN); outtext (DOWN,CYAN);
  mouse_affiche();
  if (set3)
  {
   erase_bar_bas();
   if (input ("Entrez la valeur de t",buffer))
    sscanf (buffer,"%d",&m);
   else
    m=courbe_t;
   erase_bar_bas();
  }
  update_t(m);
 }
}

void test_aff_poly()
{
 int x,y,m,set=FALSE;
 mouse_event (x,y,m);
 x=x>>3;
 y=y>>4;
 if ((x>=DEP) && (y==20) && (m==1))
 {
  do
   {
    mouse_event (x,y,m);
    x=x>>3;
    y=y>>4;
    if ((x>=DEP) && (y==20))
     {
      if (!set)
      {
       mouse_cache();
       outtextxy  (DEP+2,20,"Polygone contrôle",YELLOW);
       mouse_affiche();
       set=TRUE;
      }
     }
    else if (set)
     {
      mouse_cache();
      outtextxy  (DEP+2,20,"Polygone contrôle",CYAN);
      mouse_affiche();
      set=FALSE;
     }
   }
  while (m);
  mouse_cache();
  outtextxy  (DEP+2,20,"Polygone contrôle",CYAN);
  if (set)
   {
    if (aff_poly)
     {
      aff_poly=FALSE;
      outtextxy (DEP,20,BLANC,BLACK);
      if (nbr_pts) aff_controle (BLACK);
     }
    else
     {
      aff_poly=TRUE;
      outtextxy (DEP,20,TICK,CYAN);
      if (nbr_pts) aff_controle ();
     }
    if (nbr_pts) aff_courbe ();
   }
  mouse_affiche();
 }
}

void gere_menu (void)
{
 int x,y,m,yy=FALSE,select=-1;
 char c;

 if (mouse_click (0,x,y))
 {
  x=x>>3;
  y=y>>4;
  if (x>=DEP) switch (y)
  {
   case 20: test_aff_poly();
            break;
   case 21: test_up_down();
            break;
  }
 }

 if (keypressed())
 {
  if ((c=getch())==0)
  {
   switch(c=getch())
   {
    case PGUP:
                 update_t(courbe_t+TPAS);
                 break;
    case PGDOWN:
                 update_t(courbe_t-TPAS);
                 break;
   }
  }
 else if (c==ESC)
      {
       finit();
       return;
      }
 }

 mouse_event (x,y,m);
 x=x >> 3;
 y=y >> 4;

 do
 {
  if ((x>=DEP) && (y>=ST) && (y<=(ST+ITEMS-1)) && (m==1))
   {
    if (y!=yy)
    {
     mouse_cache();
     if (yy) outtextxy (DEP+2,yy,menu_item[yy-ST],CYAN);
     outtextxy (DEP+2,y, menu_item[select=y-ST],YELLOW);
     mouse_affiche();
     yy=y;
    }
   }
  else
   {
    if (yy)
    {
     select=-1;
     mouse_cache();
     outtextxy (DEP+2,yy,menu_item[yy-ST],CYAN);
     mouse_affiche();
     yy=FALSE;
    }
   }

  mouse_event (x,y,m);
  x=x >> 3;
  y=y >> 4;

 }
 while (m!=0);

 if (select!=-1)
  {
   switch (select)
   {
    case  1: charg(); break;
    case  2: sauve(); break;
    case  3: nouve(); break;
    case  4: finit(); break;
    case  6: ajout(); break;
    case  7: modif(); break;
    case  8: rafra(); break;
    case 10: simil(); break;
    case 11: degre(); break;
    case 12: envel(); break;
    default: break;
   }
   mouse_cache();
   outtextxy (DEP+2,ST+select,menu_item[select],CYAN);
   mouse_affiche();
  }


}

void gere_saisie (void)
{
 char c;
 int x,y,m;
 if (keypressed())
 {
  if ((c=getch())==0)
  {
   mouse_event (x,y,m);
   switch(c=getch())
   {
    case HAUT:
                 mouse_move (x,--y);
                 break;
    case BAS:
                 if (++y==CARRE) --y;
                 mouse_move (x,y);
                 break;
    case DROITE:
                 if (++x==CARRE) --x;
                 mouse_move (x,y);
                 break;
    case GAUCHE:
                 mouse_move (--x,y);
                 break;
    case PGUP:
                 update_t(courbe_t+TPAS);
                 break;
    case PGDOWN:
                 update_t(courbe_t-TPAS);
                 break;
   }
  }
 else if (c==ESC)
      {
       finit();
       mouse_pointer(&cros);
      }
 }
}