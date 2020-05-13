
/*  d‚claration des fonctions de gestion de l'‚cran
                                 par VANBELLE Edouard   */

// #define __COLORS

#if !defined(__COLORS)
#define __COLORS


 #define BLACK          0x0
 #define BLUE           0x1
 #define GREEN          0x2
 #define CYAN           0x3
 #define RED            0x4
 #define MAGENTA        0x5
 #define BROWN          0x6
 #define LIGHTGRAY      0x7
 #define DARKGRAY       0x8
 #define LIGHTBLUE      0x9
 #define LIGHTGREEN     0xa
 #define LIGHTCYAN      0xb
 #define LIGHTRED       0xc
 #define LIGHTMAGENTA   0xd
 #define YELLOW         0xe
 #define WHITE          0xf

#endif

#define VGA            0x12

#define CRLF gotoxy (0,wherey()+1)    // simule un retour chariot

#define BYTE unsigned char


BYTE type_ecran,num_page=0;
BYTE getcolor;

int abs (int nbr)
{
 if (nbr<0) return (-nbr);
 else       return (nbr);
}

void init_graph(BYTE ecr)
{
  asm {
       mov ah,0x0F
       int 0x10
       mov type_ecran,al
       mov al,ecr
       xor ah,ah
       int 0x10
      }
}


int getch (void)
{
 BYTE tmp;
 asm {
      mov ah,07
      int 0x21
      mov tmp,al
     }
 return (tmp);
}

BYTE keypressed(void)
{
 BYTE tmp;
 asm {
     mov ah,01
     int 0x16
     lahf
     and ah,0x40
     mov cl,6
     shr ah,cl
     not ah
     and ah,1
     mov tmp,ah
     }
 return (tmp);
}

void restore_graph(void)
{
  asm {
       mov ah,00
       mov al,type_ecran
       int 0x10
      }
}

void setcolor(BYTE C)
{
 getcolor=C;
}


void background (BYTE C=getcolor)
{
 asm {
      mov ah,0x0b
      xor bh,bh
      mov bl,C
      int 0x10
     }
}

void chose_page (char page)
{
 asm {
      mov ah,05
      mov al,page
      mov num_page,al
      int 0x10
     }
}

void gotoxy (char x,char y)
{
 asm {
      mov ah,02
      mov bh,num_page
      mov dh,y
      mov dl,x
      int 0x10
     }
}

char wherex ()
{
 char tmp;
 asm {
      mov ah,03
      mov bh,num_page
      int 0x10
      mov tmp,dl
     }
 return (tmp);
}

char wherey ()
{
 char tmp;
 asm {
      mov ah,03
      mov bh,num_page
      int 0x10
      mov tmp,dh
     }
 return (tmp);
}


void outtext (char* text,BYTE C=getcolor)
{
 asm {
      cld
      mov si,text
      mov bh,num_page
      mov bl,C
     }
 retour:
 asm {
      lodsb                     // caratŠre[DS:SI]->al puis SI++
      or  al,al
      jz  fin:
      push si
      mov ah,09
      mov cx,1
      int 0x10                  // affichage du caractŠre
      mov ah,03
      int 0x10                  // lecture du curseur
      inc dl                    // curseur vers la droite
      mov ah,02                 // place le curseur
      int 0x10
      pop si
      jmp retour:
     }
 fin: ;
}

void outtextxy (char x,char y,char* text,BYTE C=getcolor)
{
 gotoxy (x,y);
 outtext (text,C);
}

void putpixel (int X,int Y,BYTE C=getcolor)
{
 asm {
      mov ax,Y
      mov dx,(640/8)
      mul dx
      mov bx,X
      mov cl,bl
      shr bx,1
      shr bx,1
      shr bx,1
      add bx,ax
      and cl,7
      xor cl,7
      mov ah,1
      shl ah,cl
      mov dx,0x3ce
      mov al,8
      out dx,ax
      mov ax,(02 shl 8)+5
      out dx,ax
      push es
      mov ax,0xA000
      mov es,ax
      mov al,es:[bx]
      mov al,byte ptr C
      mov es:[bx],al
      pop es
      mov ax,(0xFF shl 8)+8
      out dx,ax
      mov ax,(0x00 shl 8)+5
      out dx,ax

     }
}

void bez_putpixel (int x,int y,BYTE C=getcolor)
{
 if ((0<x) && (0<y) && (x<CARRE) && (y<CARRE))
  putpixel (x,y,C);
}

char getpixel (int X,int Y)
{
 char tmp;
 asm {
      mov ah,0x0D
      mov dx,Y
      mov cx,X
      int 0x10
      mov tmp,al
     }
 return (tmp);
}

void line (int X1,int Y1,int X2,int Y2,BYTE C=getcolor)
{
 int tmp,xincr,yincr,aincr,bincr,d,dX,dY,X,Y;
 if (abs(X2-X1)<abs(Y2-Y1))
 {
  if (Y1>Y2)
  {
   tmp=X1;X1=X2;X2=tmp;
   tmp=Y1;Y1=Y2;Y2=tmp;
  }

  xincr= (X2>X1)?1:-1;
  dY=Y2-Y1;
  dX=abs(X2-X1);
  d=2*dX-dY;
  aincr=2*(dX-dY);
  bincr=2*dX;
  X=X1;
  Y=Y1;
  putpixel (X,Y,C);
  for (Y=Y1+1;Y<=Y2;++Y)
  {
   if (d>=0) {
              X+=xincr;
              d+=aincr;
             }
   else d+=bincr;
   putpixel (X,Y,C);
  }
 }
 else
 {
  if (X1>X2)
  {
   tmp=X1;X1=X2;X2=tmp;
   tmp=Y1;Y1=Y2;Y2=tmp;
  }
  yincr=(Y2>Y1) ? 1:-1;
  dX=X2-X1;
  dY=abs(Y2-Y1);
  d=2*dY-dX;
  aincr=2*(dY-dX);
  bincr=2*dY;
  X=X1;
  Y=Y1;

  putpixel (X,Y,C);
  for (X=X1+1;X<=X2;++X)
  {
   if (d>=0) {
              Y+=yincr;
              d+=aincr;
             }
   else d+=bincr;
   putpixel (X,Y,C);
  }
 }
}

void bez_line (int X1,int Y1,int X2,int Y2,BYTE C=getcolor)
{
 int tmp,xincr,yincr,aincr,bincr,d,dX,dY,X,Y;
 if (abs(X2-X1)<abs(Y2-Y1))
 {
  if (Y1>Y2)
  {
   tmp=X1;X1=X2;X2=tmp;
   tmp=Y1;Y1=Y2;Y2=tmp;
  }

  xincr= (X2>X1)?1:-1;
  dY=Y2-Y1;
  dX=abs(X2-X1);
  d=2*dX-dY;
  aincr=2*(dX-dY);
  bincr=2*dX;
  X=X1;
  Y=Y1;
  bez_putpixel (X,Y,C);
  for (Y=Y1+1;Y<=Y2;++Y)
  {
   if (d>=0) {
              X+=xincr;
              d+=aincr;
             }
   else d+=bincr;
   bez_putpixel (X,Y,C);
  }
 }
 else
 {
  if (X1>X2)
  {
   tmp=X1;X1=X2;X2=tmp;
   tmp=Y1;Y1=Y2;Y2=tmp;
  }
  yincr=(Y2>Y1) ? 1:-1;
  dX=X2-X1;
  dY=abs(Y2-Y1);
  d=2*dY-dX;
  aincr=2*(dY-dX);
  bincr=2*dY;
  X=X1;
  Y=Y1;

  bez_putpixel (X,Y,C);
  for (X=X1+1;X<=X2;++X)
  {
   if (d>=0) {
              Y+=yincr;
              d+=aincr;
             }
   else d+=bincr;
   bez_putpixel (X,Y,C);
  }
 }
}

void bar (int X1,int Y1,int X2,int Y2,BYTE C=getcolor)
{
 int tmp,X,Y=Y1;
 if (X2<X1) {tmp=X1;X1=X2;X2=tmp;}
 if (Y2<Y1) {tmp=Y1;Y1=Y2;Y2=tmp;}
 for (Y=Y1;Y<=Y2;++Y)
  for (X=X1;X<=X2;++X)
    putpixel (X,Y,C);
}




//                                              fin de fichier