#include "gsdl.hpp"
#include "notes.hpp"
#include <climits>
#include <iostream>
#include <queue>
#include <SDL_ttf.h>
#ifdef EMCXX
#include <emscripten.h>
#endif // EMCXX

SDL sdl(SDL_INIT_VIDEO | SDL_INIT_TIMER);

enum tiles {
  nil,
  ltcb,
  rtcb,
  lbcb,
  rbcb,
  hb,
  vb,
  p,
  ldob,
  rdob,
  luob,
  ruob,
  hw,
  vw,
  ltcw,
  rtcw,
  lbcw,
  rbcw,
  drob,
  urob,
  dlob,
  ulob,
  door,
  bp,
  gblp

};

int init_maze[31][28] = {
    {ltcb, hb, hb, hb, hb, hb, hb, hb, hb, hb, hb, hb, hb, ldob,
     rdob, hb, hb, hb, hb, hb, hb, hb, hb, hb, hb, hb, hb, rtcb},
    {vb, p, p, p, p, p, p, p, p, p, p, p, p, vw,
     vw, p, p, p, p, p, p, p, p, p, p, p, p, vb},
    {vb, p, ltcw, hw, hw, rtcw, p,    ltcw, hw,   hw, hw, rtcw, p, vw,
     vw, p, ltcw, hw, hw, hw,   rtcw, p,    ltcw, hw, hw, rtcw, p, vb},
    {vb, bp, vw, 0, 0, vw, p,  vw, 0,  0, 0, vw, p,  vw,
     vw, p,  vw, 0, 0, 0,  vw, p,  vw, 0, 0, vw, bp, vb},
    {vb,   p, lbcw, hw, hw, rbcw, p,    lbcw, hw,   hw, hw, rbcw, p, lbcw,
     rbcw, p, lbcw, hw, hw, hw,   rbcw, p,    lbcw, hw, hw, rbcw, p, vb},
    {vb, p, p, p, p, p, p, p, p, p, p, p, p, p,
     p,  p, p, p, p, p, p, p, p, p, p, p, p, vb},
    {vb, p,  ltcw, hw,   hw, rtcw, p,    ltcw, rtcw, p,  ltcw, hw,   hw, hw,
     hw, hw, hw,   rtcw, p,  ltcw, rtcw, p,    ltcw, hw, hw,   rtcw, p,  vb},
    {vb,   p,  lbcw, hw,   hw, rbcw, p,  vw, vw,   p,  lbcw, hw,   hw, rtcw,
     ltcw, hw, hw,   rbcw, p,  vw,   vw, p,  lbcw, hw, hw,   rbcw, p,  vb},
    {vb, p, p, p, p, p,  p,  vw, vw, p, p, p, p, vw,
     vw, p, p, p, p, vw, vw, p,  p,  p, p, p, p, vb},
    {lbcb, hb, hb,   hb, hb, rtcb, p,  vw, lbcw, hw, hw, rtcw, 0,  vw,
     vw,   0,  ltcw, hw, hw, rbcw, vw, p,  ltcb, hb, hb, hb,   hb, rbcb},
    {0,    0, 0,    0,  0,  vb,   p,  vw, ltcw, hw, hw, rbcw, 0, lbcw,
     rbcw, 0, lbcw, hw, hw, rtcw, vw, p,  vb,   0,  0,  0,    0, 0},
    {0, 0, 0, 0, 0, vb, p,  vw, vw, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, vw, vw, p,  vb, 0, 0, 0, 0, 0},
    {0,    0,  0,  0,    0, vb, p,  vw, vw, 0, ltcb, hb, hb, door,
     door, hb, hb, rtcb, 0, vw, vw, p,  vb, 0, 0,    0,  0,  0},
    {hb, hb, hb, hb, hb, rbcb, p,    lbcw, rbcw, 0,  vb, 0,  0,  0,
     0,  0,  0,  vb, 0,  lbcw, rbcw, p,    lbcb, hb, hb, hb, hb, hb},
    {0, 0, 0, 0,  0, 0, p, 0, 0, 0, vb, 0, 0, 0,
     0, 0, 0, vb, 0, 0, 0, p, 0, 0, 0,  0, 0, 0},
    {hb, hb, hb, hb, hb, rtcb, p,    ltcw, rtcw, 0,  vb, 0,  0,  0,
     0,  0,  0,  vb, 0,  ltcw, rtcw, p,    ltcb, hb, hb, hb, hb, hb},
    {0,  0,  0,  0,    0, vb, p,  vw, vw, 0, lbcb, hb, hb, hb,
     hb, hb, hb, rbcb, 0, vw, vw, p,  vb, 0, 0,    0,  0,  0},
    {0, 0, 0, 0, 0, vb, p,  vw, vw, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, vw, vw, p,  vb, 0, 0, 0, 0, 0},
    {0,  0,  0,  0,    0, vb, p,  vw, vw, 0, ltcw, hw, hw, hw,
     hw, hw, hw, rtcw, 0, vw, vw, p,  vb, 0, 0,    0,  0,  0},
    {ltcb, hb, hb, hb,   hb, rbcb, p,    lbcw, rbcw, 0,  lbcw, hw, hw, rtcw,
     ltcw, hw, hw, rbcw, 0,  lbcw, rbcw, p,    lbcb, hb, hb,   hb, hb, rtcb},
    {vb, p, p, p, p, p, p, p, p, p, p, p, p, vw,
     vw, p, p, p, p, p, p, p, p, p, p, p, p, vb},
    {vb, p, ltcw, hw, hw, rtcw, p,    ltcw, hw,   hw, hw, rtcw, p, vw,
     vw, p, ltcw, hw, hw, hw,   rtcw, p,    ltcw, hw, hw, rtcw, p, vb},
    {vb,   p, lbcw, hw, rtcw, vw, p,    lbcw, hw, hw,   hw, rbcw, p, lbcw,
     rbcw, p, lbcw, hw, hw,   hw, rbcw, p,    vw, ltcw, hw, rbcw, p, vb},
    {vb, bp, p, p, vw, vw, p, p, p,  p,  p, p, p,  0,0,  p,  p, p, p,  p,  p, p, vw, vw, p, p, bp, vb},
    {urob, hw, rtcw, p, vw,   vw,   p, ltcw, rtcw, p, ltcw, hw, hw,  hw, hw,
     hw,   hw, rtcw, p, ltcw, rtcw, p, vw,   vw,   p, ltcw, hw, ulob},
    {drob, hw, rbcw, p, lbcw, rbcw, p, vw,   vw,   p, lbcw, hw, hw,  rtcw, ltcw,
     hw,   hw, rbcw, p, vw,   vw,   p, lbcw, rbcw, p, lbcw, hw, dlob},
    {vb, p, p, p, p, p,  p,  vw, vw, p, p, p, p, vw,
     vw, p, p, p, p, vw, vw, p,  p,  p, p, p, p, vb},
    {vb, p, ltcw, hw, hw, hw,   hw,   rbcw, lbcw, hw, hw, rtcw, p, vw,
     vw, p, ltcw, hw, hw, rbcw, lbcw, hw,   hw,   hw, hw, rtcw, p, vb},
    {vb,   p, lbcw, hw, hw, hw, hw, hw, hw, hw, hw, rbcw, p, lbcw,
     rbcw, p, lbcw, hw, hw, hw, hw, hw, hw, hw, hw, rbcw, p, vb},
    {vb, p, p, p, p, p, p, p, p, p, p, p, p, p,
     p,  p, p, p, p, p, p, p, p, p, p, p, p, vb},
    {lbcb, hb, hb, hb, hb, hb, hb, hb, hb, hb, hb, hb, hb, hb,
     hb,   hb, hb, hb, hb, hb, hb, hb, hb, hb, hb, hb, hb, rbcb}

};

unsigned int redghost_s[14][14] = {
    {0, 0, 0, 0, 0, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0, 0, 0, 0,
     0},
    {0, 0, 0, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff, 0xff0000ff, 0, 0, 0},
    {0, 0, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0, 0},
    {0, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0},
    {0, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0},
    {0, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0},
    {0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff},
    {0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff},
    {0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff},
    {0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff},
    {0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff},
    {0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff},
    {0xff0000ff, 0xff0000ff, 0x00000000, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0x00000000, 0x00000000, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0x00000000,
     0xff0000ff, 0xff0000ff},
    {0xff0000ff, 0x00000000, 0x00000000, 0x00000000, 0xff0000ff, 0xff0000ff,
     0x00000000, 0x00000000, 0xff0000ff, 0xff0000ff, 0x00000000, 0x00000000,
     0x00000000, 0xff0000ff},

};
unsigned int redghost2_s[14][14] = {
    {0, 0, 0, 0, 0, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0, 0, 0, 0,
     0},
    {0, 0, 0, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff, 0xff0000ff, 0, 0, 0},
    {0, 0, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0, 0},
    {0, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0},
    {0, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0},
    {0, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0},
    {0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff},
    {0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff},
    {0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff},
    {0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff},
    {0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff},
    {0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff},
    {0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0, 0xff0000ff, 0xff0000ff,
     0xff0000ff, 0xff0000ff, 0, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff},
    {0, 0xff0000ff, 0xff0000ff, 0x00000000, 0, 0, 0xff0000ff, 0xff0000ff, 0x0,
     0x00000, 0x00000000, 0xff0000ff, 0xff0000ff, 0x0000000},

};

unsigned int eye_s[14][14] = {
    {0},
    {0},
    {0},
    {0, 0, 0, 0, 0xffffffff, 0xffffffff, 0, 0, 0, 0, 0xffffffff, 0xffffffff, 0,
     0},
    {0, 0, 0, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0, 0, 0xffffffff,
     0xffffffff, 0xffffffff, 0xffffffff, 0},
    {0, 0, 0, 0xffffffff, 0xffffffff, 0x0000ffff, 0x0000ffff, 0, 0, 0xffffffff,
     0xffffffff, 0x0000ffff, 0x0000ffff, 0},
    {0, 0, 0, 0xffffffff, 0xffffffff, 0x0000ffff, 0x0000ffff, 0, 0, 0xffffffff,
     0xffffffff, 0x0000ffff, 0x0000ffff, 0},
    {0, 0, 0, 0, 0xffffffff, 0xffffffff, 0, 0, 0, 0, 0xffffffff, 0xffffffff, 0,
     0},

};

unsigned int eye_u[14][14] = {
    {0, 0, 0, 0x0000ffff, 0x0000ffff, 0, 0, 0, 0, 0x0000ffff, 0x0000ffff, 0, 0},
    {0, 0, 0xffffffff, 0x0000ffff, 0x0000ffff, 0xffffffff, 0, 0, 0xffffffff,
     0x0000ffff, 0x0000ffff, 0xffffffff, 0},
    {0, 0, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0, 0, 0xffffffff,
     0xffffffff, 0xffffffff, 0xffffffff, 0},
    {0, 0, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0, 0, 0xffffffff,
     0xffffffff, 0xffffffff, 0xffffffff, 0},
    {0, 0, 0, 0xffffffff, 0xffffffff, 0, 0, 0, 0, 0xffffffff, 0xffffffff, 0, 0},

};

unsigned int eye_f[14][14] = {
    {0},
    {0},
    {0},
    {0},
    {0, 0, 0, 0, 0xffa500ff, 0xffa500ff, 0, 0, 0xffa500ff, 0xffa500ff, 0, 0},
    {0, 0, 0, 0, 0xffa500ff, 0xffa500ff, 0, 0, 0xffa500ff, 0xffa500ff, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0xffa500ff, 0xffa500ff, 0, 0, 0xffa500ff, 0xffa500ff, 0, 0,
     0xffa500ff, 0xffa500ff, 0, 0},
    {0, 0xffa500ff, 0, 0, 0xffa500ff, 0xffa500ff, 0, 0, 0xffa500ff, 0xffa500ff,
     0, 0, 0xffa500ff, 0},

};

int maze[31][28];

enum g_modes {
  nilmode,
  scatter,
  chase,
  fright

};

int mode = scatter;

int SDL_RenderFillCircle(SDL_Renderer *renderer, int x, int y, int radius) {
  int offsetx, offsety, d;
  int status;

  offsetx = 0;
  offsety = radius;
  d = radius - 1;
  status = 0;

  while (offsety >= offsetx) {

    status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
                                 x + offsety, y + offsetx);
    status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                 x + offsetx, y + offsety);
    status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                 x + offsetx, y - offsety);
    status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
                                 x + offsety, y - offsetx);

    if (status < 0) {
      status = -1;
      break;
    }

    if (d >= 2 * offsetx) {
      d -= 2 * offsetx + 1;
      offsetx += 1;
    } else if (d < 2 * (radius - offsety)) {
      d += 2 * offsety - 1;
      offsety -= 1;
    } else {
      d += 2 * (offsety - offsetx - 1);
      offsety -= 1;
      offsetx += 1;
    }
  }

  return status;
}

int px = 18 * 14 - 9, py = 23 * 18;

enum ors {
  nilor,
  up = -1,
  down = 1,
  left = -2,
  right = 2

};

int por = left;

void SDL::drawmouth(int x, int y, double radius, int ang1, int ang2) {
  double ang = ang1;
  SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0xff);
  while (ang <= ang2) {

    SDL_RenderDrawLineF(sdl.m_renderer, x, y,
                        (x + radius * sin(ang / 180 * 3.1415926)),
                        (y + radius * cos(ang / 180 * 3.1415926)));
    ang += 0.2;
  }
}

int pxd, pyd;
int rgx = 14 * 18 - 9, rgy = 11 * 18, rgdx = 0, rgdy = 0, pgx = 14 * 18 - 9,
    pgy = 14 * 18, pgdx, pgdy, bgx = 13 * 18 - 9, bgy = 14 * 18, bgdx, bgdy;
int ogx = 15 * 18 - 9, ogy = 14 * 18, ogdx, ogdy;
int roldor, poldor, boldor, ooldor;
int eatingpc = 0, reaten = 0, beaten = 0, oeaten = 0, peaten = 0;

SDL_Texture *get;

std::queue<struct songnote> s_eff_queue[4];

void s_gliss(unsigned int frames, unsigned int *freq, unsigned int channel = 0,
             double vol = 0.5) {
  int freqlen;
  for (freqlen = 0; freq[freqlen] != 0; ++freqlen)
    ;
  ++freqlen;
  unsigned int *freq2 = (unsigned int *)malloc(freqlen * sizeof(unsigned int));
  memcpy(freq2, freq, freqlen * sizeof(unsigned int));
  s_eff_queue[channel].push(DSN(GLISS, 50, frames, (unsigned long)freq2, vol));
}
int frames = 0;

int pellets = 0;

int frighttimes[] = {6, 5, 4, 3, 2, 5, 2, 2, 1, 5, 2, 1, 1, 3, 1, 1, 0, 1};
int rres =0 , bres = 0, pres = 0, ores = 0;

TTF_Font *font;
int initcounter;

void drawText(SDL_Renderer *screen, char *str, int x, int y, int sz, SDL_Color fgC, SDL_Color bgC, int leftanchor) {

	TTF_Init();

	font = TTF_OpenFont("./font.ttf", 30);
	if (!font) {
		printf("[ERROR] TTF_OpenFont() Failed with: %s\n", TTF_GetError());
		exit(2);
	}

	// SDL_Surface* textSurface = TTF_RenderText_Solid(font, string, fgC);
	// // aliased glyphs
	SDL_Surface *textSurface = TTF_RenderText_Solid(font, str, fgC); // anti-aliased glyphs
	SDL_Rect textLocation = {(int)(x - sz * leftanchor * (strlen(str) - 1)), y, (int)(sz * (strlen(str))), sz};
	SDL_Texture *t = SDL_CreateTextureFromSurface(screen, textSurface);

	SDL_RenderCopy(screen, t, NULL, &textLocation);

	SDL_DestroyTexture(t);
	SDL_FreeSurface(textSurface);
	TTF_CloseFont(font);

	TTF_Quit();
	// printf("[ERROR] Unknown error in drawText(): %s\n", TTF_GetError());
	// return 1;
}


void draw_ghost(SDL_Renderer *m_renderer, unsigned int colour, int x, int y,
                SDL_Texture *rgt, int oldor, int eaten, int res) {
  SDL_Rect rrect = {x + 9 - 14, y + 9 - 14, 28, 28};

  if (!get)
    get = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888,
                            SDL_TEXTUREACCESS_STREAMING, 14, 14);
    if(colour == 0)
    colour |= 0xff;
  for (int x = 0; x < 14; ++x) {
    for (int y = 0; y < 14; ++y) {

      if (redghost_s[x][y])
        redghost_s[x][y] = colour;
      if (redghost2_s[x][y])
        redghost2_s[x][y] = colour;
    }
  }
  int pitch;
  unsigned int *pixels;
  if (((oldor == left || oldor == right) ? fabs(fmod(x, 18)) : 0) +
          ((oldor == up || oldor == down) ? fabs(fmod(y, 18)) : 0) ==
      0) {
    SDL_UpdateTexture(rgt, NULL, redghost_s, 14 * 4);

  } else if (((oldor == left || oldor == right) ? fabs(fmod(x, 9)) : 0) +
                 ((oldor == up || oldor == down) ? fabs(fmod(y, 9)) : 0) ==
             0) {
    SDL_UpdateTexture(rgt, NULL, redghost2_s, 14 * 4);
  }

  SDL_SetTextureBlendMode(rgt, SDL_BLENDMODE_BLEND);
  SDL_Point rc;
  SDL_RendererFlip flip;

  //  SDL_SetTextureAlphaMod(rgt, 1);
  if(!eaten || res)
    SDL_RenderCopy(m_renderer, rgt, NULL, &rrect);
  if ((mode == fright && !res)&& !eaten) {
    SDL_UpdateTexture(get, NULL, eye_f, 14 * 4);
    rc = {14 / 2, 14 / 2};
    flip = SDL_FLIP_NONE;

  } else {

    if (oldor == left || oldor == right) {
      SDL_UpdateTexture(get, NULL, eye_s, 14 * 4);

    } else {

      SDL_UpdateTexture(get, NULL, eye_u, 14 * 4);
    }

    SDL_SetTextureBlendMode(get, SDL_BLENDMODE_BLEND);

    if (oldor == left) {
      flip = SDL_FLIP_HORIZONTAL;
    } else
      flip = SDL_FLIP_NONE;

    if (oldor == down) {
      rc = {14, 9};
    } else {
      rc = {0, 0};
    }
  }
  SDL_RenderCopyEx(m_renderer, get, NULL, &rrect,
                   (oldor == down && !(mode == fright)) ? 180 : 0, &rc, flip);
}
SDL_Texture *rgt, *pgt, *bgt, *ogt;

std::queue<unsigned int> tex_buff_queue;
int drawbp = 1;
int dying = 0;

int diecounter=0;
int winning;

void SDL::draw() {
  // Clear the window with a black background
  SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
  SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
  SDL_RenderClear(m_renderer);

  int b_space = 4;

  if (frames % 10 == 0) {
    drawbp = !drawbp;
  }
  for (int y = 0; y < 31; ++y) {
    for (int x = 0; x < 28; ++x) {
      SDL_Rect tile = {x * 18, y * 18, 18, 18};
      if(initcounter && winning && frames%20 <= 10&&initcounter>=150){
        SDL_SetRenderDrawColor(m_renderer,255,255,255,255);
      }
      else
      SDL_SetRenderDrawColor(m_renderer, 0, 0, 0xff, 0xff);
      switch (maze[y][x]) {
      case hb:
        tile = {x * 18, y * 18 + 9 - b_space / 2, 18, 1};
        SDL_RenderFillRect(m_renderer, &tile);
        tile.y += b_space - 1;
        SDL_RenderFillRect(m_renderer, &tile);
        break;
      case ltcb:
        tile = {x * 18 + 9 - b_space / 2, y * 18 + 9 - b_space / 2,
                9 + b_space / 2, 1};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9 + b_space / 2 - 1, y * 18 + 9 + b_space / 2 - 1,
                9 - b_space / 2 + 1, 1};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9 - b_space / 2, y * 18 + 9 - b_space / 2, 1,
                9 + b_space / 2};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9 + b_space / 2 - 1, y * 18 + 9 + b_space / 2, 1,
                9 - b_space / 2};
        SDL_RenderFillRect(m_renderer, &tile);
        break;
      case rtcb:
        tile = {x * 18 + 9 + b_space / 2, y * 18 + 9 - b_space / 2,
                -(9 + b_space / 2), 1};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9 - b_space / 2 + 1, y * 18 + 9 + b_space / 2 - 1,
                -(9 - b_space / 2 + 2), 1};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9 + b_space / 2 - 1, y * 18 + 9 - b_space / 2, 1,
                9 + b_space / 2};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9 - b_space / 2, y * 18 + 9 + b_space / 2, 1,
                9 - b_space / 2};
        SDL_RenderFillRect(m_renderer, &tile);
        break;
      case lbcb:
        tile = {x * 18 + 9 - b_space / 2, y * 18 + 9 + b_space / 2 - 1,
                9 + b_space / 2, 1};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9 + b_space / 2 - 1, y * 18 + 9 - b_space / 2,
                9 - b_space / 2 + 1, 1};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9 - b_space / 2, y * 18 + 9 + b_space / 2, 1,
                -(9 + b_space / 2)};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9 + b_space / 2 - 1, y * 18 + 9 - b_space / 2, 1,
                -(9 - b_space / 2)};
        SDL_RenderFillRect(m_renderer, &tile);
        break;
      case rbcb:
        tile = {x * 18 + 9 + b_space / 2, y * 18 + 9 + b_space / 2 - 1,
                -(9 + b_space / 2), 1};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9 - b_space / 2 + 1, y * 18 + 9 - b_space / 2,
                -(9 - b_space / 2) - 1, 1};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9 + b_space / 2 - 1, y * 18 + 9 + b_space / 2, 1,
                -(9 + b_space / 2)};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9 - b_space / 2, y * 18 + 9 - b_space / 2, 1,
                -(9 - b_space / 2)};
        SDL_RenderFillRect(m_renderer, &tile);
        break;
      case vb:
        tile = {x * 18 + 9 - b_space / 2, y * 18, 1, 18};
        SDL_RenderFillRect(m_renderer, &tile);
        tile.x += b_space - 1;
        SDL_RenderFillRect(m_renderer, &tile);
        break;
      case rdob:
        tile = {x * 18, y * 18 + 9 - b_space / 2, 18, 1};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9, y * 18 + 9 + b_space / 2 - 1, 9, 1};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9, y * 18 + 9 + b_space / 2, 1, 9 - b_space / 2};
        SDL_RenderFillRect(m_renderer, &tile);
        break;
      case ldob:
        tile = {x * 18, y * 18 + 9 - b_space / 2, 18, 1};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9 + 1, y * 18 + 9 + b_space / 2 - 1, -9 - 1, 1};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9, y * 18 + 9 + b_space / 2, 1, 9 - b_space / 2};
        SDL_RenderFillRect(m_renderer, &tile);
        break;
      case ruob:
        tile = {x * 18, y * 18 + 9 + b_space / 2, 18, 1};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9, y * 18 + 9 - b_space / 2 - 1, 9, 1};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9, y * 18 + 9 - b_space / 2, 1, -(9 - b_space / 2)};
        SDL_RenderFillRect(m_renderer, &tile);
        break;
      case luob:
        tile = {x * 18, y * 18 + 9 + b_space / 2, 18, 1};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9 + 1, y * 18 + 9 - b_space / 2 - 1, -9 - 1, 1};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9, y * 18 + 9 - b_space / 2, 1, -(9 - b_space / 2)};
        SDL_RenderFillRect(m_renderer, &tile);
        break;
      case drob:
        tile = {x * 18 + 9 - b_space / 2, y * 18, 1, 18};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9 + b_space / 2 - 1, y * 18 + 9, 1, -(-9)};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9 + b_space / 2, y * 18 + 9, 9 - b_space / 2, 1};
        SDL_RenderFillRect(m_renderer, &tile);
        break;
      case urob:
        tile = {x * 18 + 9 - b_space / 2, y * 18, 1, 18};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9 + b_space / 2 - 1, y * 18 + 9 + 1, 1, -9 - 1};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9 + b_space / 2, y * 18 + 9, 9 - b_space / 2, 1};
        SDL_RenderFillRect(m_renderer, &tile);
        break;
      case ulob:
        tile = {x * 18 + 9 + b_space / 2 - 1, y * 18, 1, 18};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9 - b_space / 2, y * 18 + 9 + 1, 1, -9 - 1};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9 - b_space / 2, y * 18 + 9, -(9 - b_space / 2), 1};
        SDL_RenderFillRect(m_renderer, &tile);
        break;
      case dlob:
        tile = {x * 18 + 9 + b_space / 2 - 1, y * 18, 1, 18};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9 - b_space / 2, y * 18 + 9, 1, 9};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9 - b_space / 2, y * 18 + 9, -(9 - b_space / 2), 1};
        SDL_RenderFillRect(m_renderer, &tile);
        break;
      case door:
        SDL_SetRenderDrawColor(m_renderer, 222, 161, 133, 0xff);
        tile = {x * 18, y * 18 + 9 - (b_space - 2) / 2, 18, b_space - 2};
        SDL_RenderFillRect(m_renderer, &tile);
        break;
      case hw:
        tile = {x * 18, y * 18 + 9, 18, 1};
        SDL_RenderFillRect(m_renderer, &tile);
        break;
      case vw:
        tile = {x * 18 + 9, y * 18, 1, 18};
        SDL_RenderFillRect(m_renderer, &tile);
        break;
      case p:
        SDL_SetRenderDrawColor(m_renderer, 222, 161, 133, 0xff);
        SDL_RenderFillCircle(m_renderer, x * 18 + 9, y * 18 + 9, 2);
        break;
      case bp:

        if (drawbp) {
          SDL_SetRenderDrawColor(m_renderer, 222, 161, 133, 0xff);
          SDL_RenderFillCircle(m_renderer, x * 18 + 9, y * 18 + 9, 8);
        }
        break;
      case ltcw:
        tile = {x * 18 + 9, y * 18 + 9, 9, 1};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9, y * 18 + 9, 1, 9};
        SDL_RenderFillRect(m_renderer, &tile);
        break;
      case rtcw:
        tile = {x * 18 + 9, y * 18 + 9, -9, 1};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9, y * 18 + 9, 1, 9};
        SDL_RenderFillRect(m_renderer, &tile);
        break;
      case lbcw:
        tile = {x * 18 + 9, y * 18 + 9, 9, 1};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9, y * 18 + 9, 1, -9};
        SDL_RenderFillRect(m_renderer, &tile);
        break;
      case rbcw:
        tile = {x * 18 + 9 + 1, y * 18 + 9, -9 - 1, 1};
        SDL_RenderFillRect(m_renderer, &tile);
        tile = {x * 18 + 9, y * 18 + 9, 1, -9};
        SDL_RenderFillRect(m_renderer, &tile);
        break;
      }
    }
  }
  if(!(dying == 1)){

  if (!rgt)
    rgt = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888,
                            SDL_TEXTUREACCESS_STREAMING, 14, 14);


  draw_ghost(m_renderer, mode != fright||rres ? 0xff0000ff : (0x0000ffff*!reaten), rgx, rgy,
             rgt, roldor,reaten,rres);
  if (!pgt)
    pgt = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888,
                            SDL_TEXTUREACCESS_STREAMING, 14, 14);

  draw_ghost(m_renderer, mode != fright||pres ? 0xff80ffff : (0x0000ffff*!peaten), pgx, pgy,
             pgt, poldor,peaten,pres);
  if (!bgt)
    bgt = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888,
                            SDL_TEXTUREACCESS_STREAMING, 14, 14);

  draw_ghost(m_renderer, mode != fright||bres ? 0x00ffffff: (0x0000ffff*!beaten) , bgx, bgy,
             bgt, boldor,beaten,bres);
  if (!ogt)
    ogt = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888,
                            SDL_TEXTUREACCESS_STREAMING, 14, 14);
  draw_ghost(m_renderer, (mode != fright||ores) ? 0xffa500ff : (0x0000ffff*!oeaten), ogx, ogy,
             ogt, ooldor,oeaten,ores);
  }
  SDL_SetRenderDrawColor(m_renderer, 0xff, 0xff, 0, 0xff);
  SDL_RenderFillCircle(m_renderer, px + 9, py + 9, 14);

  if (pyd < 0 && pxd == 0)
    por = up;
  if (pyd > 0 && pxd == 0)
    por = down;
  if (pxd < 0 && pyd == 0)
    por = left;
  if (pxd > 0 && pyd == 0)
    por = right;

  double openangle = 45;

  openangle *=
      ((por == left || por == right) ? fabs(9 - fmod(px, 18)) / 9 : 0) +
      ((por == up || por == down) ? fabs(9 - fmod(py, 18)) / 9 : 0);
    if(!diecounter){
  switch (por) {
  case up:
    drawmouth(px + 9, py + 9, 15, 180 - openangle, 180 + openangle);

    break;
  case down:
    drawmouth(px + 9, py + 9, 15, 360 - openangle, 360 + openangle);

    break;
  case left:
    drawmouth(px + 9, py + 9, 15, 270 - openangle, 270 + openangle);
    break;

  case right:
    drawmouth(px + 9, py + 9, 15, 90 - openangle, 90 + openangle);

    break;
  }
    }
    else{
        drawmouth(px+9,py+9, 15, (diecounter-120)*180/60, 360 - (diecounter-120)*180/60);

    }
  if((initcounter && winning == 0) || (initcounter<=119 && winning == 1)){
    drawText(m_renderer,"READY!",11*18-14,17*18,24,{0xff,0xff,0x00,0xff},{0},0);
  }
  // Show the window


  draw_gui();
  SDL_RenderPresent(m_renderer);
}

int arr_maze() { memcpy(maze, init_maze, sizeof(init_maze)); return 0;}

int running = 1;

int chkcoll(int x, int y, int doorcoll = 0) {

  x = x / 18;

  if (!(maze[y / 18][x] == p || maze[y / 18][x] == bp || maze[y / 18][x] == 0 ||
        (maze[y / 18][x] == door) && !doorcoll)) {
    return 1;
  }
  return 0;
}

int speed_count = 0;

void s_beep(unsigned int frames, unsigned int freq, unsigned int channel = 0,
            double vol = 0.9) {
  s_eff_queue[channel].push(DSN(TRI, freq * 99 / 98, frames, 50, vol));
}
void s_beep2(unsigned int frames, unsigned int freq, unsigned int channel = 0,
            double vol = 0.4) {
  s_eff_queue[channel].push(DSN(SQR, freq * 99 / 98, frames, 50, vol));
}

void audio_tick() {
  int inc = 0;

  for (int i = 0; i < 4; ++i) {
    if (s_eff_queue[i].size() != 0) {
      if (s_eff_queue[i].front().playin == 0 &&
          s_eff_queue[i].front().durrcount == 0) {
        s_eff_queue[i].front().durrcount = s_eff_queue[i].front().durr;
        s_eff_queue[i].front().playin = 1;
        unsigned int len;
        switch (s_eff_queue[i].front().type) {
        case SQR:
          s_eff_queue[i].front().buf = sqrtonegen(
              s_eff_queue[i].front().pitch, s_eff_queue[i].front().specval,
              s_eff_queue[i].front().vol, &len);
          break;
        case TRI:
          s_eff_queue[i].front().buf = tritonegen(
              s_eff_queue[i].front().pitch, s_eff_queue[i].front().vol, &len);
          break;
        case NOISE:
          s_eff_queue[i].front().buf = randtonegen(
              s_eff_queue[i].front().pitch, s_eff_queue[i].front().vol, &len,
              s_eff_queue[i].front().specval);
          break;
        case GLISS:
          s_eff_queue[i].front().buf = glisstonegen(
              (unsigned int *)s_eff_queue[i].front().specval, 50,
              s_eff_queue[i].front().vol, &len, s_eff_queue[i].front().durr);
          s_eff_queue[i].front().durrcount -= 1;
          break;
        }
        s_eff_queue[i].front().c =
            Mix_QuickLoad_RAW((Uint8 *)s_eff_queue[i].front().buf, len);
        Mix_PlayChannel(-1, s_eff_queue[i].front().c,
                        s_eff_queue[i].front().type == GLISS ? 0 : -1);
      }
      --s_eff_queue[i].front().durrcount;
      if (s_eff_queue[i].front().durrcount <= 0) {
        Mix_FreeChunk(s_eff_queue[i].front().c);
        free(s_eff_queue[i].front().buf);
        if (s_eff_queue[i].front().type == GLISS) {
          free((void *)s_eff_queue[i].front().specval);
        }
        s_eff_queue[i].pop();
      }
    }
  }
}
int eat_snd_count = 0;

int level = 1;

int init = 0;

void ghost_decide(int tx, int ty, int x, int y, int *dx, int *dy, int *oldorp,
                  int *gin, int &eaten, int &res) {

  if (x % 18) {
    if (init == 0) {
      *dx = -3;
    }
    return;
  }

  if (y % 18) {
    return;
  }

  int dist = INT_MAX, olddist = dist, dist2;

  if (!*gin && !eaten) {
    tx = 18 * 14;
    ty = 11 * 18;
  }
  if (x == 18 * 14 && y == 14 * 18 && eaten) {
    eaten = 0;
    res = 1;
  }
  if (x == 18 * 14 && y == 11 * 18 && !eaten) {
    *gin = 1;
  }

  int opt_or;

  int oldor = *oldorp;

  *dx = 0;
  *dy = 0;

  switch (oldor) {
  case right:
    *dx = 3;
    break;
  case left:
    *dx = -3;
    break;
  case up:
    *dy = -3;
    break;
  case down:
    *dy = 3;
    break;
  }

  x += *dx * 6;

  y += *dy * 6;

  if (oldor != right && !chkcoll(x - 3, y, *gin)) {
    olddist = dist;
    dist = std::min(abs(dist2 = ((((x - 18) - tx) * ((x - 18) - tx)) +
                                 ((y - ty) * (y - ty)))),
                    dist);
    if (dist != olddist) {
      opt_or = left;
    }
  }
  if (oldor != left && !chkcoll(x + 18, y, *gin)) {
    olddist = dist;
    dist = std::min(abs(dist2 = ((((x + 18) - tx) * ((x + 18) - tx)) +
                                 ((y - ty) * (y - ty)))),
                    dist);
    if (dist != olddist) {
      opt_or = right;
    }
  }
  if (oldor != down && !chkcoll(x, y - 3, *gin)) {
    olddist = dist;
    dist = std::min(abs(dist2 = ((((x)-tx) * ((x)-tx)) +
                                 (((y - 18) - ty) * ((y - 18) - ty)))),
                    dist);
    if (dist != olddist) {
      opt_or = up;
    }
  }
  if (oldor != up && !chkcoll(x, y + 18, *gin)) {
    olddist = dist;
    dist = std::min(abs(dist2 = ((((x)-tx) * ((x)-tx)) +
                                 (((y + 18) - ty) * ((y + 18) - ty)))),
                    dist);
    if (dist != olddist) {
      opt_or = down;
    }
  }
  x -= *dx * 6;
  y -= *dy * 6;

  *oldorp = opt_or;


}

int pgdc = 0, bgdc = 0, ogdc = 0;
int frightcount = 0;

int ginit;
int pinit = 0, binit = 0, rinit = 0, oinit = 0;

  int lifes = 0;

  int score = 0, hiscore = 0;


void ghost_tick() {
  if (level == 1) {
    if (frames <= 7 * 60) {
      mode = scatter;
    } else if (frames <= 27 * 60) {
      mode = chase;
    } else if (frames <= 34 * 60) {
      mode = scatter;
    } else if (frames <= 54 * 60) {
      mode = chase;
    } else if (frames <= 59 * 60) {
      mode = scatter;
    } else if (frames <= 79 * 60) {
      mode = chase;
    } else if (frames <= 84 * 60) {
      mode = scatter;
    } else
      mode = chase;
  } else if (level <= 4) {
    if (frames <= 7 * 60) {
      mode = scatter;
    } else if (frames <= 27 * 60) {
      mode = chase;
    } else if (frames <= 34 * 60) {
      mode = scatter;
    } else if (frames <= 54 * 60) {
      mode = chase;
    } else if (frames <= 59 * 60) {
      mode = scatter;
    } else
      mode = chase;
  } else {
    if (frames <= 5 * 60) {
      mode = scatter;
    } else if (frames <= 25 * 60) {
      mode = chase;
    } else if (frames <= 30 * 60) {
      mode = scatter;
    } else if (frames <= 50 * 60) {
      mode = chase;
    } else if (frames <= 55 * 60) {
      mode = scatter;
    } else
      mode = chase;
  }

  if (frightcount) {
    --frightcount;
    mode = fright;
    if(frightcount == 0)
        rres = pres = bres = ores = 0;
  }

  int rgtx, rgty;
  if (mode == chase) {
    rgtx = px;
    rgty = py;
  } else {
    rgtx = 25 * 18;
    rgty = -36;
  }

  int pgtx, pgty;
  if (mode == chase) {
    pgtx = px + ((por == left || por == right) ? 18 * por : 0);
    pgty = py + ((por == up || por == down) ? 18 * por / 2 : 0);
  } else {
    pgtx = 3 * 18;
    pgty = -36;
  }

  int bgtx, bgty;
  if (mode == chase) {
    bgtx = px + ((por == up || por == down) ? 18 * 4 * por / 2 : 0) +
           ((por == left || por == right) ? 18 * 2 * por / 2 : 0);
    bgty = py + ((por == up || por == down) ? 18 * 4 * por / 2 : 0) +
           ((por == left || por == right) ? -18 * 2 * por / 2 : 0);

    int dfbx = bgtx - rgx;
    int dfby = bgty - rgy;
    bgtx += dfbx;
    bgty += dfby;
  } else {
    bgtx = 28 * 18;
    bgty = 32 * 18;
  }

  int ogtx, ogty;


  if (mode == chase &&
      abs(sqrt((px - ogx) * (px - ogx) + (py - ogy) * (py - ogy))) >= 18 * 8) {
    ogtx = px;
    ogty = py;
  } else {
    ogtx = 0;
    ogty = 32 * 18;
  }

  if (mode == fright && !rres) {
    rgtx = rand() % 28 * 18;
    rgty = rand() % 31 * 18;

  }
  if (mode == fright && !pres) {
    pgtx = rand() % 28 * 18;
    pgty = rand() % 31 * 18;

  }  if (mode == fright && !ores) {
    ogtx = rand() % 28 * 18;
    ogty = rand() % 31 * 18;

  }  if (mode == fright && !bres) {
    bgtx = rand() % 28 * 18;
    bgty = rand() % 31 * 18;

  }
  if(reaten){
    rgtx = 14 * 18;
    rgty = 14 * 18;
    rinit = 0;
  }
  if(peaten){
    pgtx = 14 * 18;
    pgty = 14 * 18;
    pinit = 0;
  }
  if(oeaten){
    ogtx = 14 * 18;
    ogty = 14 * 18;
    oinit = 0;
  }
  if(beaten){
    bgtx = 14 * 18;
    bgty = 14 * 18;
    binit = 0;
  }
  ghost_decide(pgtx, pgty, pgx, pgy, &pgdx, &pgdy, &poldor, &pinit,peaten,pres);

  ghost_decide(rgtx, rgty, rgx, rgy, &rgdx, &rgdy, &roldor, &rinit,reaten,rres);

  ghost_decide(bgtx, bgty, bgx, bgy, &bgdx, &bgdy, &boldor, &binit,beaten,bres);

  ghost_decide(ogtx, ogty, ogx, ogy, &ogdx, &ogdy, &ooldor, &oinit,oeaten,ores);

  if (init == 0)
    init = 1;

  int speed, speed2, pspeed, pspeed2, ospeed, ospeed2, rspeed, rspeed2, bspeed, bspeed2;

  if (level == 1) {
    speed = 8;
    speed2 = 4;
  }
  if (level >= 2 && level <= 4) {
    speed = 16;
    speed2 = 12;
  }
  if (level >= 5) {
    speed = 8;
    speed2 = 6;
  }
  rspeed = pspeed = bspeed = ospeed = speed;
rspeed2 = pspeed2  = bspeed2 = ospeed2 = speed2;


  if (mode == fright && level == 1) {
    speed = 4;
    speed2 = 1;
  } else if (mode == fright) {
    speed = 8;
    speed2 = 3;
  }
  if(!rres){
    rspeed = speed;
    rspeed2 = speed2;
  }
  if(!ores){
    ospeed = speed;
    ospeed2 = speed2;
  }
  if(!pres){
    pspeed = speed;
    pspeed2 = speed2;
  }
  if(!bres){
    bspeed = speed;
    bspeed2 = speed2;
  }



  if (rgx + rgdx < -8) {
    rgx = 480;
    rgdy = 0;
    roldor = left;
  }
  if (rgx + rgdx >= 482) {
    rgx = 3;    rgdy = 0;
    roldor = right;
  }

  if (pgx + pgdx < -8) {
    pgx = 480;    pgdy = 0;
    poldor = left;
  }
  if (pgx + pgdx >= 482) {
    pgx = 3;    pgdy = 0;
    poldor = right;
  }

  if (bgx + bgdx < -8) {
    bgx = 480;    bgdy = 0;
    boldor = left;
  }
  if (bgx + bgdx >= 482) {
    bgx = 3;    bgdy = 0;
    boldor = right;
  }

  if (ogx + ogdx < -8) {
    ogx = 480;    ogdy = 0;
    ooldor = left;
  }
  if (ogx + ogdx >= 482) {
    ogx = 3;    ogdy = 0;
    ooldor = right;
  }
  if(reaten){rspeed = 1; rspeed2 = 0;}
  if(oeaten){ospeed = 1; ospeed2 = 0;}
  if(peaten){pspeed = 1; pspeed2 = 0;}
  if(beaten){bspeed = 1; bspeed2 = 0;}
  if ((((speed_count) % (pspeed)) <= pspeed2)) {
    pgx += pgdx;
    pgy += pgdy;
  }
  if ((((speed_count) % (rspeed)) <= rspeed2)) {
    rgx += rgdx;
    rgy += rgdy;
  }
  if ((((speed_count) % (bspeed)) <= bspeed2)) {
    bgx += bgdx;
    bgy += bgdy;
  }
  if ((((speed_count) % (ospeed)) <= ospeed2)) {
    ogx += ogdx;
    ogy += ogdy;
  }

  if (mode != fright) {
    if (frames % 22 == 0) {
      unsigned int gfreqs[] = {G4, A4, B4, C5, D5, 0};
      s_gliss(11, gfreqs, 1, 0.2);
    } else if (frames % 11 == 0) {
      unsigned int gfreqs[] = {D5, C5, B4, A4, G4, 0};
      s_gliss(11, gfreqs, 1, 0.2);
    }
  } else if (!(peaten || beaten || reaten || oeaten)){

    if (frames % 8 == 0) {
      unsigned int gfreqs[] = {G4S, C5S, E5, G5S, B5, C6S, 0};
      s_gliss(8, gfreqs, 3, 0.4);
    }
  }
  else{
        if(frames%18 == 0){
       unsigned int gfreqs[] = {E6,D6F,D6,C6S,C6,B5,A5S,A5,E5,C5,0};
       s_gliss(18,gfreqs,3,0.4);
        }

  }

}

void SDL::draw_gui(){
    char msg[100];
    sprintf(msg, "%02d", lifes);
    drawText(m_renderer, "LIFES", 26*20, 0, 26, {0xff, 0xff, 0xff, 0xff},{0}, 0);
    drawText(m_renderer, msg, 26*20, 26, 26, {0xff, 0xff, 0xff, 0xff},{0}, 0);
    drawText(m_renderer, "SCORE", 26*20, 52, 26, {0xff, 0xff, 0xff, 0xff},{0}, 0);
    sprintf(msg, "%07d", score);
    drawText(m_renderer, msg, 26*20, 78, 26, {0xff, 0xff, 0xff, 0xff},{0}, 0);
    drawText(m_renderer, "HISCORE", 26*20, 104, 26, {0xff, 0xff, 0xff, 0xff},{0}, 0);
    sprintf(msg, "%07d", hiscore);
    drawText(m_renderer, msg, 26*20, 130, 26, {0xff, 0xff, 0xff, 0xff},{0}, 0);
    drawText(m_renderer, "LEVEL", 26*20, 156, 26, {0xff, 0xff, 0xff, 0xff},{0}, 0);
    sprintf(msg, "%03d", level);
    drawText(m_renderer, msg, 26*20, 182, 26, {0xff, 0xff, 0xff, 0xff},{0}, 0);

    drawText(m_renderer, "MADE BY", 26*20, 576-26*3, 26, {0xff, 0xff, 0xff, 0xff},{0}, 0);
    drawText(m_renderer, "CLEME8T", 26*20, 576-26*2, 26, {0xff, 0xff, 0xff, 0xff},{0}, 0);

}

int eatencount = 0;

void pacman_beeps() {
  s_beep(EIGHTH, G4, 3, 0);
  s_beep(EIGHTH / 3 * 2, C4, 3);
  s_beep(EIGHTH / 3 * 2, C5, 3);
  s_beep(EIGHTH / 3 * 2, G4, 3);
  s_beep(EIGHTH / 3 * 2, E4, 3);
  s_beep(SIXTEENTH / 3 * 2, C5, 3);
  s_beep(SIXTEENTH / 3 * 2, G4, 3);
  s_beep(EIGHTH / 3 * 2, G4, 3, 0);
  s_beep(EIGHTH / 3 * 2, E4, 3);
  s_beep(EIGHTH / 3 * 2, G4, 3, 0);

  s_beep2(EIGHTH,C2,2,0);
  s_beep2(EIGHTH/ 3 * 2,C2,2);
  s_beep2(QUARTER/ 3 * 2, C2,2,0);
  s_beep2(EIGHTH/ 3 * 2, G2, 2);
  s_beep2(EIGHTH/ 3 * 2,C2,2);
  s_beep2(QUARTER/ 3 * 2, C2,2,0);
  s_beep2(EIGHTH/ 3 * 2, G2, 2);

  s_beep(EIGHTH / 3 * 2, C4S, 3);
  s_beep(EIGHTH / 3 * 2, C5S, 3);
  s_beep(EIGHTH / 3 * 2, A4F, 3);
  s_beep(EIGHTH / 3 * 2, F4, 3);
  s_beep(SIXTEENTH / 3 * 2, C5S, 3);
  s_beep(SIXTEENTH / 3 * 2, A4F, 3);
  s_beep(EIGHTH / 3 * 2, G4, 3, 0);
  s_beep(EIGHTH / 3 * 2, F4, 3);
  s_beep(EIGHTH / 3 * 2, G4, 3, 0);


  s_beep2(EIGHTH/ 3 * 2,C2S,2);
  s_beep2(QUARTER/ 3 * 2, C2,2,0);
  s_beep2(EIGHTH/ 3 * 2, G2S, 2);
  s_beep2(EIGHTH/ 3 * 2,C2S,2);
  s_beep2(QUARTER/ 3 * 2, C2,2,0);
  s_beep2(EIGHTH/ 3 * 2, G2S, 2);

  s_beep(EIGHTH / 3 * 2, C4, 3);
  s_beep(EIGHTH / 3 * 2, C5, 3);
  s_beep(EIGHTH / 3 * 2, G4, 3);
  s_beep(EIGHTH / 3 * 2, E4, 3);
  s_beep(SIXTEENTH / 3 * 2, C5, 3);
  s_beep(SIXTEENTH / 3 * 2, G4, 3);
  s_beep(EIGHTH / 3 * 2, G4, 3, 0);
  s_beep(EIGHTH / 3 * 2, E4, 3);
  s_beep(EIGHTH / 3 * 2, G4, 3, 0);

  s_beep2(EIGHTH/ 3 * 2,C2,2);
  s_beep2(QUARTER/ 3 * 2, C2,2,0);
  s_beep2(EIGHTH/ 3 * 2, G2, 2);
  s_beep2(EIGHTH/ 3 * 2,C2,2);
  s_beep2(QUARTER/ 3 * 2, C2,2,0);
  s_beep2(EIGHTH/ 3 * 2, G2, 2);

  s_beep(SIXTEENTH / 3 * 2, E4, 3);
  s_beep(SIXTEENTH / 3 * 2, F4, 3);
  s_beep(SIXTEENTH / 3 * 2, F4S, 3);
  s_beep(SIXTEENTH / 3 * 2, A3, 3, 0);

  s_beep(SIXTEENTH / 3 * 2, G4, 3);
  s_beep(SIXTEENTH / 3 * 2, G4S, 3);
  s_beep(SIXTEENTH / 3 * 2, A4, 3);
  s_beep(SIXTEENTH / 3 * 2, A4, 3, 0);

  s_beep(SIXTEENTH / 3 * 2, A4, 3);
  s_beep(SIXTEENTH / 3 * 2, A4S, 3);
  s_beep(SIXTEENTH / 3 * 2, B4, 3);
  s_beep(SIXTEENTH / 3 * 2, A4, 3, 0);

  s_beep(EIGHTH / 3 * 2, C5, 3);
  s_beep(SIXTEENTH / 3 * 2, A4, 3, 0);


  s_beep2(SIXTEENTH / 3 * 2*4,G2,2);
  s_beep2(SIXTEENTH / 3 * 2*4,A2,2);
  s_beep2(SIXTEENTH / 3 * 2*4,B2,2);
  s_beep2(SIXTEENTH / 3 * 2*4,C3,2);

}

bool intersects(SDL_Rect r1, SDL_Rect r2) {
  return !((r2.x > (r1.x + r1.w)) || ((r2.x + r2.w) < r1.x) ||
           ((r2.y + r2.h) < (r1.y)) || (r2.y) > (r1.y + r1.h));
}

int chkgcoll(int x,int y, int gx, int gy){
    SDL_Rect pr = {x-10,y-10,20,20};
    SDL_Rect gr = {gx-10,gy-10,20,20};
    return intersects(pr,gr);
}


void new_game(int lost = 1){
    dying = 0;
    diecounter = 0;
    init=0;
    reaten = beaten = peaten = oeaten = 0;
    eatingpc = 0;
    roldor = boldor = poldor = ooldor = 0;
    mode = scatter;
    frightcount = 0;
    winning = 0;
    frames = 0;


    pinit = 0; binit = 0; rinit = 0; oinit = 0;
    rgx = 14 * 18 - 9; rgy = 11 * 18; rgdx = 0; rgdy = 0; pgx = 14 * 18 - 9;
    pgy = 14 * 18; bgx = 12 * 18 - 9; bgy = 14 * 18;
    ogx = 16 * 18 - 9; ogy = 14 * 18;
    px = 18 * 14 - 9; py = 23 * 18;

    rgdx = rgdy = pgdx = pgdy = bgdx = bgdy = ogdx = ogdy = pxd = pyd = 0;

    if(lost){
    if(lifes == 0){
        level = 1;
        lifes = 3;
        pacman_beeps();
        arr_maze();
        initcounter = 270;
        pellets = 0;
        score = 0;
    }
    else
    initcounter = 60;
    --lifes;}
    else{
            initcounter = 270;
            winning = 1;
            arr_maze();
            pellets = 0;
            ++level;

    }
}



void game_tick() {
  ++frames;
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_QUIT:
      running = 0;
      break;
    }
  }

            if(!eatingpc&&diecounter) --diecounter;

            if(initcounter == 0){
                winning = 0;
            }

            if( diecounter == 1){
                new_game();
            }
            if(!eatingpc && diecounter == 179){
                unsigned int freqs[] = {E4,D4S,D4,D4S,E4,D4,D4F,C4,C4S,D4,C4,B3,A3S,B3,C4,B3,A3S,A3,A3S,B3,A3,G3S,G3,G3S,A3, A2,A3,C4,G4,B4,E5, A2,A3,C4,G4,B4,E5,0};
                s_gliss(180,freqs, 2);

            }
  if(eatingpc){
    --eatingpc;
  }

  if(score > hiscore){
        hiscore = score;
  }

    if(initcounter) --initcounter;
  if (!eatingpc && !dying && !initcounter) {
    const Uint8 *ks = SDL_GetKeyboardState(NULL);
    int nomove = 0;
    if (maze[(size_t)round(py / 18.0)][(size_t)round(px / 18.0)] == p) {
      maze[(size_t)round(py / 18.0)][(size_t)round(px / 18.0)] = 0;
      if (++eat_snd_count % 2 == 0) {
        unsigned int freqs[] = {B4, F4, D4, A3F, 0};
        s_gliss(6, freqs);
      } else {
        unsigned int freqs[] = {B3, D4, F4, A4, 0};
        s_gliss(6, freqs);
      }
      nomove = 1;
      ++pellets;
      score += 10;
    }
    if (maze[(size_t)round(py / 18.0)][(size_t)round(px / 18.0)] == bp) {
      maze[(size_t)round(py / 18.0)][(size_t)round(px / 18.0)] = 0;
      if (++eat_snd_count % 2 == 0) {
        unsigned int freqs[] = {B4, E4, C4, G3S, 0};
        s_gliss(6, freqs);
      } else {
        unsigned int freqs[] = {G3, D4, F4, A4, 0};
        s_gliss(6, freqs);
      }
      score += 50;

      eatencount = 0;

      nomove = 1;
      if(!chkcoll(rgx-rgdx*6,rgy-rgdy*6))
        roldor *= -1;
      if(!chkcoll(bgx-bgdx*6,bgy-bgdy*6))
        boldor *= -1;
      if(!chkcoll(pgx-pgdx*6,pgy-pgdy*6))
        ooldor *= -1;
      if(!chkcoll(ogx-ogdx*6,ogy-ogdy*6))
        poldor *= -1;
        rres = pres = bres = ores = 0;

      if ((level - 1) < sizeof(frighttimes) / 4) {
        frightcount = frighttimes[level - 1] * 60;
      }
            ++pellets;

    }

    if (pellets == 244) {
            new_game(0);
            winning = 1;
    }

    if (ks[SDL_SCANCODE_A]) {
      pxd = -3;
      por = left;
    }

    if (ks[SDL_SCANCODE_D]) {
      pxd = 3;
      por = right;
    }

    if (ks[SDL_SCANCODE_W]) {
      pyd = -3;
      por = up;
    }

    if (ks[SDL_SCANCODE_S]) {
      pyd = 3;
      por = down;
    }

    if (px + pxd < -16) {
      px = 480;
    } else {
      if (px + pxd >= 482) {
        px = 3;
      } else {
        ++speed_count;
        int speed;
        int speed2;

        if (level == 1) {
          speed = 8;
          speed2 = 6;
        }
        if ((level >= 2 && level <= 4) || level >= 21) {
          speed = 8;
          speed2 = 6;
        }
        if ((level >= 5 && level <= 20)) {
          speed = 16;
          speed2 = 14;
        }

        if (mode == fright) {
          speed2 += 1;
        }

        if(mode == fright){
            int oec = eatencount;
            unsigned int freqs[] = {E3,F3,F3S,G3,G3S,A3,A3S,B3,C4,C4S,D4,D4S,E4,F4,F4S,G4,G4S,A4,A4S,B4,C5,C5S,D5,D5S,E5,F5,0};
            if(chkgcoll(px,py,rgx,rgy)){
                if(!reaten && !rres){
                    eatingpc = 90;

                    s_gliss(90,freqs);
                    reaten = 1;
                    ++eatencount;
                }
            }
            if(chkgcoll(px,py,bgx,bgy)){
                if(!beaten && !bres){

                eatingpc = 90;
                s_gliss(90,freqs);
                beaten = 1;
                ++eatencount;
                }
            }
            if(chkgcoll(px,py,ogx,ogy)){
                if(!oeaten && !ores){

                    s_gliss(90,freqs);

                eatingpc = 90;
                oeaten = 1;
                ++eatencount;
                }
            }
            if(chkgcoll(px,py,pgx,pgy)){
                if(!peaten&&!pres){

                    s_gliss(90,freqs);

                eatingpc = 90;
                peaten = 1;
                ++eatencount;
                }
            }
            if(oec != eatencount)
            score += 200*pow(2,eatencount-1);

        }
        else{

            if((chkgcoll(px,py,bgx,bgy)&&(!reaten||rres))||(chkgcoll(px,py,ogx,ogy)&&(ores||!oeaten))||(chkgcoll(px,py,pgx,pgy)&&(pres||!peaten))||(chkgcoll(px,py,rgx,rgy)&&(rres || !reaten))){
                eatingpc = 90;
                dying = 1;
                diecounter = 180;
            }
        }
        cheat:

        if (!chkcoll(px + (pxd > 0 ? 18 : -3), py)) {
          if (py % 18 == 0 && (((speed_count) % (speed)) <= speed2) &&
              !nomove) {
            px += pxd;
          }
        } else {
          pxd = 0;
        }
        if (!chkcoll(px, py + (pyd > 0 ? 18 : -3))) {
          if (px % 18 == 0 && (((speed_count) % (speed)) <= speed2) && !nomove)
            py += pyd;
        } else
          pyd = 0;
      }
    }
    ghost_tick();
  }

  audio_tick();

  sdl.draw();
}

int main(int argc, char *argv[]) {
    lifes = 0;
  new_game();
#ifdef EMCXX
  emscripten_set_main_loop(game_tick, 60, 1);
#else

  while (running) {
    game_tick();
  }
#endif
}
