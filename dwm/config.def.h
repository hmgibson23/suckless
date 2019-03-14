#include "gaplessgrid.c"
#include "X11/XF86keysym.h"

/* See LICENSE file for copyright and license details. */

/* appearance */
#define NUMCOLORS 17
#define ALTKEY          Mod1Mask
#define WINKEY          Mod4Mask

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int focusonwheel       = 0;
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    { "Gimp",     NULL,       NULL,       0,            1,           -1 },
    { "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
    { "VirtualBox",     NULL,       NULL,           1 << 3,     False,      -1 },
    { "Transmission",   NULL,       NULL,           1 << 3,     False,      -1 },
    { "mpv",            "gl",       NULL,           1 << 5,     False,      -1 },
    { "Slack",           "slack",   NULL,           1 << 2,     False,      -1 },
    { "Vlc",            "vlc",      NULL,           1 << 5,     False,      -1 }
};
/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
};

#ifdef SOLARIZED_DARK
static const char colors[NUMCOLORS][ColLast][13] = {
    /* border    fg         bg */
    { "#286e75", "#286e75", "#002b36" },        /* [0]  01 - Client normal */
    { "#af8700", "#268bd2", "#002b36" },        /* [1]  02 - Client selected */
    { "#286e75", "#dc322f", "#002b36" },        /* [2]  03 - Client urgent */
    { "#286e75", "#286e75", "#002b36" },        /* [3]  04 - Client occupied */
    { "#002b36", "#dc322f", "#002b36" },        /* [4]  05 - Red */
    { "#002b36", "#af8700", "#002b36" },        /* [5]  06 - Yellow */
    { "#002b36", "#859900", "#002b36" },        /* [6]  07 - Green */
    { "#002b36", "#666666", "#002b36" },        /* [7]  08 - Dark grey */
    { "#002b36", "#DCDCDC", "#002b36" },        /* [8]  09 - Light grey */
    { "#286e75", "#286e75", "#002b36" },        /* [9]  0A - Bar normal*/
    { "#268bd2", "#268bd2", "#002b36" },        /* [10] 0B - Bar selected*/
    { "#286e75", "#dc322f", "#002b36" },        /* [11] 0C - Bar urgent*/
    { "#286e75", "#268bd2", "#002b36" },        /* [12] 0D - Bar occupied*/
};
#endif

#ifdef SOLARIZED_LIGHT
static const char colors[NUMCOLORS][ColLast][13] = {
    /* border    fg         bg */
    { "#93a1a1", "#93a1a1", "#fdf6e3" },        /* [0]  01 - Client normal */
    { "#268bd2", "#268bd2", "#fdf6e3" },        /* [1]  02 - Client selected */
    { "#93a1a1", "#dc322f", "#fdf6e3" },        /* [2]  03 - Client urgent */
    { "#93a1a1", "#93a1a1", "#fdf6e3" },        /* [3]  04 - Client occupied */
    { "#fdf6e3", "#dc322f", "#fdf6e3" },        /* [4]  05 - Red */
    { "#fdf6e3", "#af8700", "#fdf6e3" },        /* [5]  06 - Yellow */
    { "#fdf6e3", "#859900", "#fdf6e3" },        /* [6]  07 - Green */
    { "#fdf6e3", "#666666", "#fdf6e3" },        /* [7]  08 - Dark grey */
    { "#fdf6e3", "#DCDCDC", "#fdf6e3" },        /* [8]  09 - Light grey */
    { "#93a1a1", "#93a1a1", "#fdf6e3" },        /* [9]  0A - Bar normal*/
    { "#268bd2", "#268bd2", "#fdf6e3" },        /* [10] 0B - Bar selected*/
    { "#93a1a1", "#dc322f", "#fdf6e3" },        /* [11] 0C - Bar urgent*/
    { "#93a1a1", "#268bd2", "#fdf6e3" },        /* [12] 0D - Bar occupied*/
};
#endif

#ifdef GRUVBOX
static const char colors[NUMCOLORS][ColLast][17] = {
    /* border    fg         bg */
    { "#282828", "#928374", "#282828" },        /* [0]  01 - Client normal */
    { "#ebdbb2", "#458588", "#282828" },        /* [1]  02 - Client selected */
    { "#83a598", "#fb4934", "#282828" },        /* [2]  03 - Client urgent */
    { "#83a598", "#83a598", "#282828" },        /* [3]  04 - Client occupied */
    { "#282828", "#fb4934", "#282828" },        /* [4]  05 - Red */
    { "#282828", "#fabd2f", "#282828" },        /* [5]  06 - Yellow */
    { "#282828", "#b8bb26", "#282828" },        /* [6]  07 - Green */
    { "#282828", "#928374", "#282828" },        /* [7]  08 - Dark grey */
    { "#282828", "#d5c4a1", "#282828" },        /* [8]  09 - Light grey */
    { "#928374", "#928374", "#282828" },        /* [9]  0A - Bar normal*/
    { "#3c3836", "#a89985", "#282828" },        /* [10] 0B - Bar selected*/
    { "#fb4934", "#fb4934", "#282828" },        /* [11] 0C - Bar urgent*/
    { "#928374", "#458588", "#282828" },        /* [12] 0D - Bar occupied*/
    { "#3c3836", "#3c3836", "#282828" },        /* [13] 0E - Tag normal*/
    { "#83a598", "#83a598", "#282828" },        /* [14] 0F - Tag selected*/
    { "#fb4934", "#fb4934", "#282828" },        /* [15] 10 - Tag urgent*/
    { "#3c3836", "#928374", "#282828" },        /* [16] 11 - Tag occupied*/
};
#endif
/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */

#ifdef SOLARIZED_DARK
static const char *menu[] = { "dmenu_run", "-fn", dmenufont, "-nb", "#002b36", "-nf", "#568e75", "-sb", "#002b36", "-sf", "#268bd2", "-h", "22", NULL };
#endif

#ifdef SOLARIZED_LIGHT
static const char *menu[] = { "dmenu_run", "-fn", dmenufont, "-nb", "#fdf6e3", "-nf", "#93a1a1", "-sb", "#fdf6e3", "-sf", "#268bd2", "-h", "22", NULL };
#endif

#ifdef GRUVBOX
static const char *menu[] = { "dmenu_run", "-fn", dmenufont, "-nb", "#282828", "-nf", "#928374", "-sb", "#3c3836", "-sf", "#a89984", "-h", "22", NULL };
#endif

static const char *termcmd[]  = { "st", "-e", "fish" };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_space,      spawn,          {.v = dmenucmd } },
	{ MODKEY,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_p,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_p,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
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
