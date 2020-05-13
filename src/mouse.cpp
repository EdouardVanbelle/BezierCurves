
/*  d‚claration des fonctions de gestion de la souris

				 par VANBELLE Edouard    */

#define byte unsigned char
#define RIEN {0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF}

typedef struct {unsigned int x;
                unsigned int y;
                unsigned int masque_ecran[16];
                unsigned int masque_souris[16];
               } forme;

// typedef enum {normal, font,blanc} modes;


forme cros={3,3,RIEN,
      {0x1000,0x1000,0x1000,0xEE00,0x1000,0x1000,0x1000,0x0000,
       0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}};


forme sablier={8,8,RIEN,
      {0x0000,0x7ffe,0x2814,0x2814,0x27e4,0x23C4,0x21c4,0x21c4,
       0x21c4,0x21c4,0x25a4,0x2994,0x2bd4,0x2ff4,0x7ffe,0x0000}};


forme carre={3,3,RIEN,
      {0xE000,0xE000,0xE000,0x0000,0x0000,0x0000,0x0000,0x0000,
       0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}};

forme montre={8,8,RIEN,
      {0x1A98,0x1958,0x1A98,0x1818,0x310C,0x6106,0xC103,0xC103,
       0xC3F1,0xC103,0xC003,0x6006,0x381C,0x1818,0x1A98,0x1958}};

forme curs_texte={7,14,RIEN,
      {0x0000,0x0000,0x1C70,0x0480,0x0100,0x0100,0x0100,0x0100,
       0x0100,0x0100,0x0100,0x0100,0x0480,0x1C70,0x0000,0x0000}};

forme interdiction={8,8,RIEN,
      {0x0000,0x0000,0x03E0,0x0FF8,0x1C1C,0x1C3C,0x3076,0x30E6,
       0x31C6,0x3386,0x3706,0x1A0C,0x1C1C,0x0FF8,0x03E0,0x0000}};

forme loupe={8,6,RIEN,
      {0x0000,0x03C0,0x0C30,0x181C,0x324C,0x300C,0x324C,0x1818,
       0x03C0,0x03C0,0x0180,0x0180,0x0180,0x0180,0x0180,0x0000}};

forme crayon={2,15,RIEN,
      {0x0000,0x0008,0x0014,0x0022,0x0044,0x0088,0x0110,0x0220,
       0x0440,0x0880,0x1100,0x1200,0x2800,0x2000,0x4000,0x0000}};

forme ciseaux={11,5,RIEN,
      {0x0040,0x0024,0x0044,0x00CC,0x00D8,0x00F0,0x00E0,0x01C0,
       0x03C0,0x06C0,0x1CC0,0x64C0,0x6960,0x3160,0x0060,0x0000}};

forme deplacer={9,1,RIEN,
      {0x0180,0x0E70,0x1248,0x124E,0x1249,0x1249,0x1249,0x1249,
       0x7249,0x9001,0x9001,0x9001,0x8001,0x8001,0x4002,0x3FFC}};

forme normal_curs={0,0,RIEN,
      {0xC000,0xE000,0xF000,0xF800,0xFC00,0xFE00,0xFF00,0xFF80,
       0xFFC0,0xFFE0,0xFE00,0xEF00,0xCF00,0x0780,0x0780,0x03C0}};

void mouse_affiche(void)
{
  asm {
  mov ax,01
  int 0x33
  }
}

void mouse_cache(void)
{
  asm {
  mov ax,02
  int 0x33
  }
}

int mouse_click(char btn,int &x,int &y)
{
 int tmp,xx,yy;
 asm {
      mov ax,05
      mov bx,btn
      int 0x33
      mov tmp,bx
      mov xx,cx
      mov yy,dx
     }
 x=xx;
 y=yy;
 return (tmp);
}

int mouse_release(char btn,int &x,int &y)
{
 int tmp,xx,yy;
 asm {
      mov ax,06
      mov bx,btn
      int 0x33
      mov tmp,bx
      mov xx,cx
      mov yy,dx
     }
 x=xx;
 y=yy;
 return (tmp);
}



unsigned char mouse_init(void) // cette fonction renvoie le nombre de boutons
{                           // de la souris.
  char tmp,ok;              // si le retour est 0 alors le driver n'est pas
  asm {                     // install‚
  mov ax,0
  int 0x33                   // de plus cette fonction r‚initialise le
  and ax,bx                 // driver souris (si celui-ci est pr‚sent)
  mov word ptr tmp,ax
  }
  return (tmp);
}

void mouse_pointer (forme *pntr)
{
 asm {
      push es
      push ds
      pop es

      cld
      mov cx,16
      mov di,pntr
      add di,4
      mov si,di
      add si,32
     }
 boucle:
 asm {
      lodsw
      not ax
      stosw
      loop boucle

      mov ax,09
      mov si,pntr
      mov bx,word ptr [ds:si]
      inc si
      inc si
      mov cx,word ptr [ds:si]
      inc si
      inc si

      mov dx,si
      int 0x33

      pop es

      mov ax,2
      int 0x33
      dec ax
      int 0x33
     }
}

void mouse_window(int x1,int y1,int x2,int y2)
{
 asm {
      mov ax,7
      mov cx,x1
      mov dx,x2
      int 0x33
      mov ax,8
      mov cx,y1
      mov dx,y2
      int 0x33
     }
}

void mouse_move (int x,int y)
{
 asm {
      mov ax,04
      mov cx,x
      mov dx,y
      int 0x33
     }
}

void mouse_event (int &X,int &Y,int &M)
{
 int x,y,m;
 asm {
      mov ax,03
      int 0x33
      mov x,cx
      mov y,dx
      mov m,bx
     }
 X=x;
 Y=y;
 M=m;
}

//                                                      fin de fichier