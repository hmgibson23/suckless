#include "layouts.c"
#include "X11/XF86keysym.h"

/* See LICENSE file for copyright and license details. */

/* appearance */
#define MODKEY          Mod4Mask
#define NUM_WORKSPACES  9

static const unsigned int BAR_HEIGHT = 30;

static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int dwm_bar   = 1;        /* 1 is use the dwm_bar */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappx     = 5;
static const unsigned int hpad      = 2;
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int bartitle           = 0;

static const char *fonts[]          = {
  "Wuncon Siji=12:antialias=true:autohint=true",
  "SauceCodePro Nerd Font:size=12:antialias=true:autohint=true",
  "Hack:pixelsize=12:antialias=true:autohint=true" };
static const char dmenufont[]       = "Hack:pixelsize=12:antialias=true:autohint=true";

static const char col_gray1[]       = "#4F4847";
static const char col_gray2[]       = "#4F4847";
static const char col_gray3[]       = "#fdf6e3";
static const char col_gray4[]       = "#fdf6e3";
static const char col_cyan[]        = "#073642";
static const char col_black[]       = "#000000";
static const char col_red[]         = "#A54242";
static const char col_yellow[]      = "#ffff00";
static const char col_white[]       = "#ffffff";
static const char *colors[][3]      = {
  /*               fg         bg         border   */
  [SchemeNorm] =         { col_gray3, col_gray1, col_gray2 },
  [SchemeSel]  =         { col_gray4, col_cyan,  col_cyan  },
  [SchemeWarn] =	 { col_black, col_yellow, col_red },
  [SchemeUrgent]=	 { col_white, col_red,    col_red },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "華", "", "", "" };
static const char *tagsalt[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
  /* xprop(1):
   *	WM_CLASS(STRING) = instance, class
   *	WM_NAME(STRING) = title
   */
  /* class      instance    title       tags mask     isfloating   monitor */
  { "Gimp",     NULL,       NULL,       0,            1,           -1 },
  { "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
  { "Chromium",  NULL,       NULL,       1 << 8,       0,           -1 },
  { "libreoffice-writer",   NULL,       NULL,     1 << 6,     False,        1,      0,          -1 },
  { "VirtualBox",     NULL,       NULL,           1 << 3,     False,      -1 },
  { "Transmission",   NULL,       NULL,           1 << 3,     False,      -1 },
  { "mpv",            "gl",       NULL,           1 << 4,     False,      -1 },
  { "Slack",           "slack",   NULL,           1 << 7,     False,      -1 },
  { "Keybase",           "keybase",   NULL,           1 << 7,     False,      -1 },
  { "Vlc",            "vlc",      NULL,           1 << 4,     False,      -1 },
  { "Emacs",            "emacs",      NULL,           1 << 3,     False,      -1 },

};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#define NUM_LAYOUTS     5
static const Layout layouts[] = {
  /* symbol     arrange function */
  { "◰",		          tile },    /* first entry is default */
  { "🀱",		          monocle },
  { "",	              gaplessgrid },
  { "",                  centeredmaster },
  { "﩯",                  grid },
  { "ﱷ",                  centeredfloatingmaster},
  { "★",		          NULL },    /* no layout function means floating behavior */
};

#define TAGKEYS(KEY,TAG) \
{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static char *dmenucmd[] = { "dmenu_run", "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4,
  "-o", "0.8",
  "-w", "500",
  "-y", "400",
  "-x", "650",
  "-l", "10",
  "-h", "50",
  NULL };

static const char *altcmd[] = {"rofi", "-sidebar-mode",
                               "-modi", "run,drun,window",
                               "-lines",  "12",
                               "-padding", "18",
                               "-width",  "60",
                               "-location",  "0",
                               "-show", "drun",
                               "-columns", "3",
                               "-font", "Meslo LG L Nerd Font 10",
                               NULL};

static const char *termcmd[]  = { "st", "-e", "/bin/zsh", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };
static const char *lockcmd[]  = { "slock", NULL };
static const char *brightnessUp[]  = { "lux", "-a", "5%", NULL };
static const char *brightnessDown[]  = { "lux", "-s", "5%", NULL };
static const char *volincr[] = { "amixer", "-q", "set", "pulse", "Master", "5%-", "unmute", NULL };
static const char *voldecr[] = { "amixer", "-q", "set", "pulse", "Master", "5%+", "unmute", NULL };
static const char *voltogg[] = { "amixer", "-D", "pulse", "set", "Master", "1+", "toggle", NULL };
static const char *playpause[] = { "playerctl", "play-pause", NULL };
static const char *playnext[] = { "playerctl", "next", NULL };
static const char *playprevious[] = { "playerctl", "previous", NULL };

 /* XF86AudioPlay */
 /* XF86AudioPause */
 /* XF86AudioNext */
 /* XF86AudioPrev */


static Key keys[] = {
  /* modifier                     key        function        argument */
  { 0,                  XF86XK_AudioMute,        spawn,                             {.v = voltogg} },
  { 0,                  XF86XK_AudioRaiseVolume, spawn,                             {.v = volincr} },
  { 0,                  XF86XK_AudioLowerVolume, spawn,                             {.v = voldecr} },
  { 0,                  XF86XK_AudioPlay, spawn,                             {.v = playpause} },
  { 0,                  XF86XK_AudioPause, spawn,                             {.v = playpause} },
  { 0,                  XF86XK_AudioNext, spawn,                             {.v = playnext} },
  { 0,                  XF86XK_AudioPrev, spawn,                             {.v = playprevious} },
  { 0,                 XF86XK_MonBrightnessUp,     spawn,          {.v = brightnessUp } },
  { 0,                 XF86XK_MonBrightnessDown,   spawn,          {.v = brightnessDown } },
  { MODKEY,                       XK_p,      spawn,          {.v = altcmd } },
  { MODKEY,                       XK_space,  spawn,          {.v = dmenucmd
                                                                        } },
  { MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
  { MODKEY,                       XK_grave,  togglescratch,  {.v = scratchpadcmd } },
  { MODKEY,                       XK_b,      togglebar,      {0} },
  { MODKEY,                       XK_n,      focusstack,     {.i = +1 } },
  { MODKEY,                       XK_e,      focusstack,     {.i = -1 } },
  { MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = +1 } },
  { MODKEY|ShiftMask,             XK_y,      incnmaster,     {.i = -1 } },
  { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
  { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
  { MODKEY,                       XK_w,      zoom,           {0} },
  { MODKEY,                       XK_Tab,    view,           {0} },
  { MODKEY,                       XK_q,      killclient,     {0} },
  { MODKEY|ShiftMask,             XK_l,      spawn,          {.v = lockcmd} },
  { MODKEY|ControlMask,		XK_comma,  cyclelayout,    {.i = -1 } },
  { MODKEY|ControlMask,           XK_period, cyclelayout,    {.i = +1 } },
  { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
  { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[1]} },
  { MODKEY|ShiftMask,             XK_m,      setlayout,      {.v = &layouts[2]} },
  { MODKEY|ShiftMask,             XK_g,      setlayout,      {.v = &layouts[3]} },
  { MODKEY|ShiftMask,             XK_c,      setlayout,      {.v = &layouts[4]} },
  { MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[6]} },
  { MODKEY|ShiftMask,             XK_p,      togglefloating, {0} },
  { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
  { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
  { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
  { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
  { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
  { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
  { MODKEY|ShiftMask,             XK_n,      togglealttag,         {.i = +1 } },
  TAGKEYS(                          XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_5,                      4)
    TAGKEYS(                        XK_6,                      5)
    TAGKEYS(                        XK_7,                      6)
    TAGKEYS(                        XK_8,                      7)
    TAGKEYS(                        XK_9,                      8)
    { MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
  /* click                event mask      button          function        argument */
  { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
  { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
  { ClkWinTitle,          0,              Button2,        zoom,           {0} },
  { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
  { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
  { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
  { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
  { ClkTagBar,            0,              Button1,        view,           {0} },
  { ClkTagBar,            0,              Button3,        toggleview,     {0} },
  { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
  { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
