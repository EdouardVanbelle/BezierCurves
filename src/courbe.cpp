#define CRX    5
#define ECH    50
#define TOLER  2

char sauve (void)
{
 FILE *f;
 int i;

 erase_bar_bas ();
if (nbr_pts>1)
{
 if (input ("Entrez le nom de sauvegarde:",buffer))
 {
  if ((f=fopen(buffer,"wt"))==NULL)
   {
    erase_bar_bas();
    echo ("Erreur de cr‚ation de fichier...",RED);
    return(FALSE);
   }
  else
   {
    for (i=0;i<nbr_pts;i++)
     if (EOF==fprintf (f,"%d,%d\n",(int)(p[0][i].x),(int)(p[0][i].y)))
      {
       erase_bar_bas();
       echo ("Une erreure est survenue durant l'‚criture du fichier",RED);
       fclose (f);
       return (FALSE);
      }

    fclose (f);
    erase_bar_bas();
    echo ("Ecriture sur disque achev‚e.");
    return (intact=TRUE);
   }
 }
 erase_bar_bas();
 return (FALSE);
}
}

char accept(void)
{
 char c=FALSE;
 int  x,y;
 if (intact) return(TRUE);
 erase_bar_bas ();
 echo ("Voulez-vous Sauver, Continuer sans sauver, Annuler avant de poursuivre?");
 mouse_cache();
 outtextxy (13,29,"S",YELLOW);
 outtextxy (21,29,"C",YELLOW);
 outtextxy (44,29,"A",YELLOW);
 mouse_affiche();
 mouse_pointer(&montre);
 mouse_release (0,x,y);
 do
 {
  if (keypressed()) c=getch();
  else
  if ((mouse_release(0,x,y)) && ((y>>4)==29))
  switch (x>>3)
   {
    case 13: c='S';
             break;
    case 21: c='C';
             break;
    case 44: c='A';
             break;
    default: c=' ';
             break;
   }
 }
 while ((c!='s') && (c!='S') &&
        (c!='a') && (c!='A') &&
        (c!='c') && (c!='C') &&
                    (c!=ESC));
 erase_bar_bas();
 switch (contexte)
 {
  case SAISIE:  mouse_pointer (&cros);
  case MENU:    mouse_pointer (&normal_curs);
 }

 switch (c)
 {
  case 'S':
  case 's':
            return(sauve());
  case 'C':
  case 'c':
            return(TRUE);
  default :
            return(FALSE);
 }
}

void finit (void)
{
 fin=(accept());
}

void vide (void)
{
 mouse_cache();
 bar (1,1,CARRE-1,CARRE-1,BLACK);
 mouse_affiche();
}

void zero (void)
{
 vide();
 reset_tableau();
 nbr_pts=0;
 intact=TRUE;
}

void nouve (void)
{
 if (accept()) zero();
}

void rafra (void)
{
 vide ();
 aff_general();
}

void charg (void)
{
 FILE *f;
 int ligne=0,i,j,
     x,y,m,tmp;
 char txt[255],sortie=FALSE;

if (accept())
{
  erase_bar_bas();
  if (input ("Entrez le nom du fichier",buffer))
 {

  if ((f=fopen(buffer,"rt"))==NULL)
   {
    erase_bar_bas();
    echo ("Erreur de lecture du fichier...",RED);
    return;
   }

  erase_bar_bas();
  mouse_cache();
  echo ("Lecture du fichier...",BLUE);
  erase_bar_bas();
  nbr_pts=0;

  while ((fscanf(f,"%s",txt)!=EOF) && (!sortie))
   {
    ligne++;
    switch (sscanf(txt,"%d,%d",&i,&j))
    {
     case 0:
              sprintf (buffer,"Erreur … la ligne %d: %s\n",ligne,txt);
              echo (buffer,RED);
              outtextxy (wherex()-1,29,BLANC,BLACK);
              mouse_affiche();
              fclose (f);
              return;
     case EOF:
              sprintf(buffer,"Erreur … la ligne %d: %s\n",ligne,txt);
              echo (buffer,RED);
              outtextxy (wherex()-1,29,BLANC,BLACK);
              mouse_affiche();
              fclose (f);
              return;
     default:
              if (nbr_pts==PMAX)
              {
               sprintf (buffer,"Version ‚tudiante limit‚ … %d points.",PMAX);
               echo (buffer,RED);
               mouse_pointer (&interdiction);

               mouse_release (0,tmp,tmp);
               do ; while ( !keypressed() && !mouse_release(0,tmp,tmp) );

               if (contexte==SAISIE) mouse_pointer (&cros);
               else                  mouse_pointer (&normal_curs);
               sortie=TRUE;
               break;
              }
              p[1][nbr_pts  ].x=(float)i;
              p[1][nbr_pts++].y=(float)j;
              break;
    }
   }

  fclose (f);
  echo ("Cacul des points...",BLUE);
  mouse_affiche();

  for (tmp=0;tmp<nbr_pts;tmp++)
   {
    p[0][tmp].x=p[1][tmp].x;
    p[0][tmp].y=p[1][tmp].y;
   }
  rafra();
  intact=!(sortie);
 }
}
erase_bar_bas();
}

void croix (int x,int y,BYTE C=YELLOW)
{
 mouse_cache();
 for (int a=-CRX;a<=CRX;a++)
 {
  bez_putpixel (a+x,  y,C);
  bez_putpixel (  x,a+y,C);
 }
 mouse_affiche();
}

void add_point (int pos,int x,int y)
{
 int i;
 for (i=0;i<pos;i++)
 {
  p[1][i].x=p[0][i].x;
  p[1][i].y=p[0][i].y;
 }
 p[1][pos].x=float(x);
 p[1][pos].y=float(y);
 nbr_pts++;
 for (i=pos+1;i<nbr_pts;i++)
 {
  p[1][i].x=p[0][i-1].x;
  p[1][i].y=p[0][i-1].y;
 }
 for (i=0;i<nbr_pts;i++)
 {
  p[0][i].x=p[1][i].x;
  p[0][i].y=p[1][i].y;
 }
 intact=FALSE;
}

void delete_point (int pos)
{
 int i;
 for (i=0;i<pos;i++)
 {
  p[1][i].x=p[0][i].x;
  p[1][i].y=p[0][i].y;
 }
 for (i=pos+1;i<nbr_pts;i++)
 {
  p[1][i-1].x=p[0][i].x;
  p[1][i-1].y=p[0][i].y;
 }
 nbr_pts--;
 for (i=0;i<nbr_pts;i++)
 {
  p[0][i].x=p[1][i].x;
  p[0][i].y=p[1][i].y;
 }
 intact=FALSE;
}

void erase_help (void)
{
 int i,j;
 mouse_cache();
 for (j=24;j<28;j++)
  for (i=DEP;i<DEP+20;i++)
   outtextxy (i,j,BLANC,BLACK);
 mouse_affiche();
}

void ajout (void)
{
char sortie=FALSE,
     c,
     contexte2=0,
     update_coor,
     new_point=FALSE;
int x,y,m,xx=-1,yy=-1,
    x1,y1,tmp1,tmp2,
    p_sel=nbr_pts,
    old_x=-1,old_y=-1;

mouse_cache();
outtextxy (DEP,24,"Utilisez '+' et '-' ",MAGENTA);
outtextxy (DEP,25,"pour changer de pnt.",MAGENTA);
outtextxy (DEP,26,"Entrez coordon. avec",MAGENTA);
outtextxy (DEP,27," souris ou clavier  ",MAGENTA);
mouse_affiche();

mouse_release (0,x1,y1);
do
{
  mouse_cache();
  gotoxy (1,29);
  for (char tmp=0;tmp<DEP;tmp++) outtext (BLANC,BLACK);
  if (nbr_pts==PMAX-1)
   {
    sprintf (buffer,"Version ‚tudiante limit‚ … %d points.",PMAX);
    echo (buffer,RED);
    mouse_affiche();
    mouse_pointer (&interdiction);

    mouse_release (0,tmp1,tmp2);
    do ; while ( !keypressed() && !mouse_release(0,tmp1,tmp2) );
    if (contexte==SAISIE) mouse_pointer (&cros);
    else                  mouse_pointer (&normal_curs);
    erase_help();
    return;
   }
  else
   {
    aff_controle ();
    sprintf (buffer,"Indiquez les coordonn‚es du points nø%d",p_sel+1);
    echo (buffer);
    if (nbr_pts!=0)
    {
     if (p_sel==0)
       croix (old_x=(p[0][p_sel].x+1),old_y=(CARRE-p[0][p_sel].y-1),YELLOW);
     else if (p_sel==nbr_pts)
       croix (old_x=(p[0][p_sel-1].x+1),old_y=(CARRE-p[0][p_sel-1].y-1),YELLOW);
     else
       bez_line (p[0][p_sel-1].x+1,CARRE-p[0][p_sel-1].y-1,
                 p[0][p_sel  ].x+1,CARRE-p[0][p_sel  ].y-1,YELLOW);
    }
   }

  mouse_affiche();
  new_point=FALSE;

 do
 {
  m=0;
  update_coor=FALSE;
  if (keypressed())
  {
   update_coor=TRUE;
   switch (c=getch())
   {
   case 0:
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
    break;
   case ESC   :
    sortie=TRUE;
    break;
   case '-':    croix (old_x,old_y,BLACK);
                if ((--p_sel)<0) p_sel=nbr_pts;
                new_point=TRUE;
                break;
   case '+':    croix (old_x,old_y,BLACK);
                if ((++p_sel)==nbr_pts+1) p_sel=0;
                new_point=TRUE;
                break;

   default   :
    if (contexte2==MENU) break;
    update_coor=TRUE;
    erase_bar_bas();
    input ("Entrez les coordonn‚es au format ###,### :",buffer);
    if (sscanf(buffer,"%d,%d",&x,&y)!=2)
    {
     new_point=TRUE;
     break;
    }
    m=-1;
    x+=1;
    y=CARRE-y-1;
   case ENTER :
   case ' ':
    if (contexte2==MENU) break;
    update_coor=TRUE;
    if (nbr_pts>1)
    {
     aff_courbe(BLACK);
     aff_controle (BLACK);
    }
    croix (old_x,old_y,BLACK);
    if (m!=-1) mouse_event (x,y,m);
    add_point (p_sel,x-1,CARRE-y-1);
    p_sel=nbr_pts;
    if (nbr_pts>1) aff_courbe();
    new_point=TRUE;
    break;
   }
  }

  {
   mouse_event (x,y,m);
   if ((update_coor) || (x!=xx) || (y!=yy))
   {
    if ((x<CARRE) && (y<CARRE))
     {
      outtextxy (70,29,"ÛÛÛÛÛÛÛ",BLACK);
      sprintf (buffer,"%.3d,%.3d",x-1,(CARRE-y)-1);
      outtextxy (70,29,buffer);
      if (contexte2!=SAISIE)
       {
        contexte2=SAISIE;
        mouse_pointer(&cros);
       }
     }
    else
     {
      if (contexte2!=MENU)
       {
        mouse_cache();
        outtextxy (70,29,"ÛÛÛÛÛÛÛ",BLACK);
        mouse_affiche();
        contexte2=MENU;
        mouse_pointer(&normal_curs);
       }
     }
     xx=x;
     yy=y;
   }
  }

 if (mouse_release(0,x1,y1))
 {
  if ((x1<CARRE) && (y1<CARRE))
  {
   update_coor=TRUE;
   if (nbr_pts>1)
   {
    aff_courbe(BLACK);
    aff_controle (BLACK);
   }
   croix (old_x,old_y,BLACK);
   add_point (p_sel,x1-1,CARRE-y1-1);
   p_sel=nbr_pts;
   if (nbr_pts>1) aff_courbe();
   new_point=TRUE;
  }
  else sortie=TRUE;
 }

 }
 while ((!new_point) && (!sortie));
}
while (!sortie);
erase_bar_bas();
erase_help();

croix (old_x,old_y,BLACK);
if (aff_poly) aff_controle ();
else          aff_controle (BLACK);
aff_courbe();
}

void transforme (float k,float alpha,complex omega)
{
 int i;
 complex z,zz;
 for (i=0;i<nbr_pts;i++)
 {
  z=complex (p[1][i].x,p[1][i].y);
  zz=(complex(k,0)*exp(complex(0,alpha))*(z-omega))+omega;
  p[0][i].x=float(real (zz));
  p[0][i].y=float(imag (zz));
 }
}

void simil (void)
{
 int x,y,xx=-1,yy=-1,mbtn;
 char fin=FALSE,contexte2=0,
      contin=FALSE;
 point omega;
 float k,alpha;

if (!nbr_pts)
 {
  erase_bar_bas();
  echo ("Il n'y a pas de courbe charg‚e...",RED);
  return;
 }

 erase_bar_bas();
 echo ("Veuillez pointer le centre de la similitude");
 aff_controle();
 mouse_release (0,x,y);
 do
 {
  mouse_event (x,y,mbtn);
  if ((x!=xx) || (y!=yy))
   {
    if ((x<CARRE) && (y<CARRE))
     {
      outtextxy (70,29,"ÛÛÛÛÛÛÛ",BLACK);
      sprintf (buffer,"%.3d,%.3d",x-1,(CARRE-y)-1);
      outtextxy (70,29,buffer);
      if (contexte2!=SAISIE)
       {
        contexte2=SAISIE;
        mouse_pointer(&cros);
       }
     }
    else
     {
      if (contexte2!=MENU)
       {
        mouse_cache();
        outtextxy (70,29,"ÛÛÛÛÛÛÛ",BLACK);
        mouse_affiche();
        contexte2=MENU;
        mouse_pointer(&normal_curs);
       }
     }
     xx=x;
     yy=y;
   }

  if (keypressed()) if (getch()==ESC) contin=fin=TRUE;
  if (mouse_release(0,x,y))
  {
   if (contexte2==SAISIE)
   {
    omega.x=x;
    omega.y=y;
    mouse_cache();
    croix (omega.x,omega.y,YELLOW);
    mouse_affiche();
    contin=TRUE;
   }
   else contin=fin=TRUE;
  }
 } while (!contin);

 if (!fin)
 {
  mouse_cache();
  outtextxy (DEP,24,"Bouton  gauche  pour",MAGENTA);
  outtextxy (DEP,25,"valider la transfor-",MAGENTA);
  outtextxy (DEP,26,"mation, sinon bouton",MAGENTA);
  outtextxy (DEP,27,"droit annule tout.  ",MAGENTA);
  mouse_affiche();
  mouse_window (1,1,CARRE-2,CARRE-1);

  xx=yy=-1;
  aff_courbe(BLACK);
  for (int cnt=0;cnt<nbr_pts;cnt++)
  {
   p[1][cnt].x=p[0][cnt].x;
   p[1][cnt].y=p[0][cnt].y;
  }
  do
  {
   mouse_event (x,y,mbtn);
   if (keypressed()) if (getch()==ESC) mbtn=2;
   if ((x!=xx) || (y!=yy))
    {
     mouse_cache();
     if (yy!=-1) bez_line (omega.x,omega.y,xx,yy,BLACK);
     xx=x;
     yy=y;
     aff_controle(BLACK);
     k=abs(complex(x-omega.x,omega.y-y))/ECH;
     alpha=arg(complex(x-omega.x,omega.y-y));
     transforme (k,alpha,complex(omega.x-1,CARRE-omega.y-1));
     erase_bar_bas();
     sprintf (buffer,"Similitude de centre ê(%.3d,%.3d), de rapport k=%f et d'angle à=%+f",
              (int)(omega.x),(int)(omega.y),k,alpha);
     echo (buffer);
     bez_line (omega.x,omega.y,omega.x+ECH,omega.y,LIGHTGREEN);
     croix (omega.x,omega.y,YELLOW);
     bez_line (omega.x+ECH,omega.y,omega.x+ECH-CRX,omega.y-CRX,LIGHTGREEN);
     bez_line (omega.x+ECH,omega.y,omega.x+ECH-CRX,omega.y+CRX,LIGHTGREEN);
     bez_line (omega.x,omega.y,x,y,LIGHTBLUE);
     aff_controle();
     mouse_affiche();
    }
  } while (!mbtn);
  mouse_window (1,1,XMAX-2,YMAX-2);
  mouse_cache();
  erase_help();
  erase_bar_bas();
  bez_line (omega.x,omega.y,xx,yy,BLACK);
  bez_line (omega.x,omega.y,omega.x+ECH,omega.y,BLACK);
  croix (omega.x,omega.y,BLACK);
  bez_line (omega.x+ECH,omega.y,omega.x+ECH-CRX,omega.y-CRX,BLACK);
  bez_line (omega.x+ECH,omega.y,omega.x+ECH-CRX,omega.y+CRX,BLACK);
  mouse_affiche();
  aff_controle (BLACK);
  if (mbtn!=1)
   {
    echo ("Opp‚ration annul‚e");
    for (int cnt=0;cnt<nbr_pts;cnt++)
    {
     p[0][cnt].x=p[1][cnt].x;
     p[0][cnt].y=p[1][cnt].y;
    }
   }
  else intact=FALSE;
  aff_courbe ();
 }
 if (aff_poly) aff_controle ();
 else         { aff_controle (BLACK); aff_courbe(); }
}


void modif (void)
{
int X,Y,btn,XX=-1,YY=-1,
    x,y,xx=-1,yy=-1,
    find,find_pnt,old_pnt,
    cnt,
    a,b;
char contexte2=0,
     sortie=FALSE;

if (nbr_pts)
{
 mouse_cache();
 outtextxy (DEP,24,"Click  gauche  pour ",MAGENTA);
 outtextxy (DEP,25,"d‚placer  le point. ",MAGENTA);
 outtextxy (DEP,26,"Click droit pour le ",MAGENTA);
 outtextxy (DEP,27,"supprimer.          ",MAGENTA);
 mouse_affiche();
 mouse_release (0,x,y);
 mouse_release (1,x,y);
 if (!aff_poly) aff_controle ();
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
      if (contexte2!=SAISIE)
       {
        contexte2=SAISIE;
        mouse_pointer(&cros);
       }
     }
    else
     {
      if (contexte2!=MENU)
       {
        mouse_cache();
        outtextxy (70,29,"ÛÛÛÛÛÛÛ",BLACK);
        mouse_affiche();
        contexte2=MENU;
        mouse_pointer(&normal_curs);
       }
     }
     XX=X;
     YY=Y;
   }

 if (mouse_release (0,x,y) && ((x>=CARRE) || (y>=CARRE))) sortie=TRUE;
 if (keypressed()) if (getch()==ESC) sortie=TRUE;

 if (contexte2==SAISIE)
  {
   mouse_event (X,Y,btn);
   find=FALSE;
   a=X-1;
   b=CARRE-Y-1;
   for (cnt=0;((cnt<nbr_pts) && !find);cnt++)
    {
     find=((p[0][cnt].x>a-TOLER) && (p[0][cnt].y>b-TOLER) &&
           (p[0][cnt].x<a+TOLER) && (p[0][cnt].y<b+TOLER));
     if (find) find_pnt=cnt;
    }
   if (!find)
   {
    if (old_pnt!=-1)
    {
     old_pnt=-1;
     mouse_pointer(&cros);
     erase_bar_bas();
     mouse_release (1,x,y);
    }
   }
   else
   {
    if (old_pnt!=find_pnt)
    {
     mouse_release(1,x,y);
     old_pnt=find_pnt;
     mouse_move (a=(p[0][find_pnt].x+1),b=(CARRE-p[0][find_pnt].y-1));
     mouse_pointer (&deplacer);
     sprintf (buffer,"Point s‚l‚tionn‚: %d",find_pnt+1);
     erase_bar_bas();
     echo (buffer);
    } else
    {
     if (mouse_release (1,x,y))
     {
      aff_courbe (BLACK);
      aff_controle (BLACK);
      delete_point (find_pnt);
      aff_controle ();
      aff_courbe ();
     }
     mouse_event (x,y,btn);
     if (btn==1)
     {
      mouse_window (1,1,CARRE-1,CARRE-1);
     do
      {
       mouse_event (x,y,btn);
       if ((x!=xx) || (y!=yy))
       {
        aff_courbe (BLACK);
        aff_controle (BLACK);
        delete_point (find_pnt);
        add_point    (find_pnt,x-1,CARRE-y-1);
        aff_controle ();
        aff_courbe   ();
        xx=x;
        yy=y;
        outtextxy (70,29,"ÛÛÛÛÛÛÛ",BLACK);
        sprintf (buffer,"%.3d,%.3d",x-1,(CARRE-y)-1);
        outtextxy (70,29,buffer);
       }
      }
     while (btn==1);
     mouse_window (1,1,XMAX-2,YMAX-2);
     }
    }
   }
  }
 }
 while (!sortie);

erase_bar_bas();
erase_help();

if (!aff_poly)
{
 aff_controle (BLACK);
 aff_courbe();
}

}
}

void degre (void)
{
 int x,y,j;
 if (accept())
 {
  if ((nbr_pts<PMAX) && (nbr_pts))
  {
   erase_bar_bas();
   echo ("Calcul des nouveaux points pour une ‚l‚vation de un degr‚...");
   if (aff_poly) aff_controle(BLACK);
   aff_courbe (BLACK);

   p[0][nbr_pts].x=p[0][nbr_pts].y=0;
   for (j=1;j<=nbr_pts;j++)
   {
    p[1][j].x=((float)j)/((float)nbr_pts)*p[0][j-1].x
           +(1-((float)j)/((float)nbr_pts))*p[0][j].x;
    p[1][j].y=((float)j)/((float)nbr_pts)*p[0][j-1].y
           +(1-((float)j)/((float)nbr_pts))*p[0][j].y;
   }
   intact=FALSE;
   nbr_pts++;

   for (j=0;j<nbr_pts;j++)
   {
    p[0][j].x=p[1][j].x;
    p[0][j].y=p[1][j].y;
   }

   if (aff_poly) aff_controle();
   aff_courbe ();
  }
  else
  {
   if (nbr_pts)
    {
     erase_bar_bas();
     sprintf (buffer,"Version ‚tudiante limit‚ … %d points.",PMAX);
     echo (buffer,RED);
     mouse_pointer (&interdiction);
     mouse_release (0,x,y);
     do ; while ( !keypressed() && !mouse_release(0,x,y) );
     mouse_pointer (&normal_curs);
    }
   else
    {
     erase_bar_bas();
     echo ("Il n'y a pas de courbe charg‚e...",RED);
    }
  }
  }
}

void envel (void)
{

 time_t t,tt;

 if (nbr_pts)
 {
 mouse_pointer (&sablier);
 erase_bar_bas();
 mouse_cache();
 echo ("Calcul de l'enveloppe :");
 int pos=wherex();
 for (int i=0; i<(360/8); i++) outtext ("±",GREEN);
 mouse_affiche();
 gotoxy (pos,29);
 for (i=0; i<(360/8); i++)
  {
   t=time(NULL);
   while ((time(NULL)-t)<1)
   {
    if (keypressed())
     if (getch()==ESC)
      {
       erase_bar_bas();
       mouse_cache();
       echo ("Opp‚ration annul‚e");
       mouse_affiche();
       mouse_pointer (&normal_curs);
       return;
      }
   }
   mouse_cache();
   outtext (BLANC,LIGHTGREEN);
   mouse_affiche();
  }
 erase_bar_bas();
 mouse_cache();
 echo("Enveloppe d‚termin‚e.");
 mouse_affiche();
 mouse_pointer (&normal_curs);
 } else
 {
 erase_bar_bas();
 mouse_cache();
 outtextxy (1,29,"Il n'y a pas de courbe charg‚e...",RED);
 mouse_affiche();
 }
}

