#include <X11/XF86keysym.h>

static const unsigned int borderpx  = 6;        /* border pixel of windows */
static const unsigned int snap      = 20;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 15;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 15;       /* vert inner gap between windows */
static const unsigned int gappoh    = 15;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 15;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 : no bar
                                                 * 1 : yes bar */
static const int topbar             = 1;        /* 0 : bottom bar */
static const int horizpadbar        = -5;        /* horizontal padding for statusbar */
static const int vertpadbar         = 10;        /* vertical padding for statusbar */
static const char *fonts[]          = { "iosevka comfy:size=22:style=Bold:antialias=true:autohint=true" };
static const char *colors[][3]      = {
/*                fg        bg        border   */
[SchemeNorm] = { "#81a1c1","#2e3440","#2e3440" },
[SchemeSel]  = { "#88c0d0","#2e3440","#5e81ac" },
/* Color bar settings */
/* [SchemeNorm] = { "#eceff4","#2e3440","#2e3440" }, */
/* [SchemeSel]  = { "#eceff4","#434c5e","#5e81ac" }, */
};
static const char *tags[] = { "M", "o", "e", "c", "h", "a", "o", "!" };
/* Tags Collection */
/* Arrows     { "←", "↑", "→", "↓", "↕", "↖", "↗", "↘", "↙" }
 * Subscript  { "₁", "₂", "₃", "₄", "₅", "₆", "₇", "₈", "₉" }
 * Dots       { "●", "●", "●", "●", "●", "●", "●", "●", "●" }
 * Japanese   { "一", "二", "三", "四", "五", "六", "七", "八", "九" };*/
static const Rule rules[] = {
/* xprop(1):
 *	WM_CLASS(STRING) = instance, class
 *	WM_NAME(STRING) = title */
/* class      instance    title       tags mask     iscentered   isterminal   noswallow     isfloating  isfakefullscreen  monitor */
{ "Gimp",     NULL,       NULL,       0,            0,           0,           0,            0,          0,                 -1 },
{ "firefox",  NULL,       NULL,       1 << 9,       0,           0,          -1,            0,          0,                 -1 },
{ "st",       NULL,       NULL,       0,            0,           1,           0,            0,          0,                 -1 },
{ "tabbed",   NULL,       NULL,       0,            0,           1,           0,            0,          0,                 -1 },
{ "Zathura",  NULL,       NULL,       0,            0,           0,           0,            0,          0,                 -1 },
{ "Sxiv",     NULL,       NULL,       0,            0,           0,           0,            0,          0,                 -1 },
{ "mpv",      NULL,       NULL,       0,            1,           0,           1,            1,          0,                 -1 },
{ "Dragon",   NULL,       NULL,       0,            1,           0,           0,            1,          0,                 -1 },
{ "stalonetray",NULL,     NULL,       0,            1,           0,           0,            1,          0,                 -1 },
{ "termapp",  NULL,       NULL,       0,            1,           1,           0,            1,          0,                 -1 },
};

/* layout */
static const float mfact     = 0.5;             /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;               /* number of clients in master area */
static const int resizehints = 0;               /* 0 : don't respect size hints
                                                 * 1 : means respect size hints in tiled resizals */
static const int decorhints  = 1;               /* 1 means respect decoration hints */

#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[ ]",      tile },    /* first entry is default */
	{ "TTT",      bstack },    /* master on top, slaves on bottom */
	{ "[D]",      deck },    /* master on left, slaves in monocle-like mode on right */
	{ "[M]",      monocle },    /* all windows on top of eachother */
	{ "|M|",      centeredmaster },    /* master in middle, slaves on sides */
	{ "><>",      NULL },    /* no layout function means floating behavior */
};

/************/
/* Bindings */
/************/
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
  /*apps*/
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-c", "-i", "-l", "1", NULL };
static const char *termcmd[] = { "st", "-e", NULL };
static const char *firefox[] = { "firefox", "-e", NULL };
static const char *torbrowser[] = { "st", "-c", "termapp", "-g", "100x35", "-e", "tor-browser", NULL };
static const char *ncmpcpp[] = { "st", "-e", "ncmpcpp", NULL };
static const char *pulsemixer[] = { "st", "-c", "termapp", "-g", "100x35", "-e", "pulsemixer", NULL };
static const char *ranger[]  = { "st", "-e", "ranger", NULL };
static const char *htop[] = { "st", "-c", "termapp", "-g", "100x35", "-e", "htop", NULL };
static const char *rss[]  = { "st", "-c", "termapp", "-g", "101x35", "-e", "newsboat", "-r", "-u", "~/.config/newsboat/urls", "-C", "~/.config/newsboat/config", "-c", "~/.config/newsboat/cache", NULL };
static const char *cal[] = {"st", "-c", "termapp", "-g", "100x35", "-e", "calcurse", NULL };
static const char *trackma[]  = { "st", "-e", "trackma-curses", NULL };
static const char *anicli[] = { "st", "-c", "termapp", "-g", "100x35", "-e", "ani-cli", "-q best", NULL };
static const char *mangacli[] = { "st", "-c", "termapp", "-g", "100x35", "-e", "manga-cli", "-f", NULL };
static const char *nmtui[] = { "st", "-c", "termapp", "-g", "100x35", "-e", "nmtui", NULL };
static const char *tremc[] = { "st", "-c", "termapp", "-g", "100x35", "-e", "tremc", NULL };
static const char *passmenu[] = { "passmenu", "-c", "-i", "-l", "1", NULL };
static const char *neomutt[] = { "st", "-c", "termapp", "-g", "100x35", "-e", "neomutt", NULL };

#include "movestack.c" /*movestack*/
#include "shiftview.c" /*shiftview*/
static Key keys[] = {
	/* modifier                     key        function        argument */

        /* Applications */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask, 		XK_d,	   spawn,	   SHCMD("cat ~/.local/share/key | dmenu -c -i -l 39") },
	{ MODKEY,		   XK_Return,	   spawn,	   {.v = termcmd } },
	{ MODKEY|ShiftMask,        XK_Return,      spawn,          SHCMD("st -c termapp -g 100x35") },
	{ MODKEY,                       XK_w,      spawn,          {.v = firefox } },
	{ MODKEY|ShiftMask,		XK_w,	   spawn,	   {.v = nmtui } },
	{ MODKEY,                       XK_m,      spawn,          {.v = ncmpcpp } },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          {.v = pulsemixer } },
	{ MODKEY,			XK_r,	   spawn,	   {.v = ranger } },
	{ MODKEY|ShiftMask,		XK_r,	   spawn,	   {.v = htop } },
	{ MODKEY,                       XK_n,      spawn,          {.v = rss } },
	{ MODKEY|ShiftMask,		XK_n,	   spawn,	   SHCMD("st -e nvim -c VimwikiIndex") },
        { MODKEY,                       XK_a,      spawn,          {.v = anicli } },
	{ MODKEY|ShiftMask,		XK_a,	   spawn,	   {.v = trackma } },
        { MODKEY,                       XK_s,      spawn,          {.v = mangacli } },
	{ MODKEY,			XK_c,	   spawn,	   {.v = cal } },
	{ MODKEY|ShiftMask,             XK_c,      spawn,          {.v = tremc } },
	{ MODKEY,                       XK_g,      spawn,          {.v = neomutt } },
	{ MODKEY|ShiftMask,             XK_g,      spawn,          {.v = passmenu } },
	{ MODKEY|ShiftMask,             XK_v,      spawn,          {.v = torbrowser } },
	{ MODKEY,		XK_BackSpace,	   spawn,	   SHCMD("~/.local/bin/power") },
	{ MODKEY|ShiftMask,	XK_BackSpace,	   spawn,	   SHCMD("~/.local/bin/power") },
	{ MODKEY,                      XK_F1,      spawn,          SHCMD("~/.local/bin/dmenu_mount") },
	{ MODKEY|ShiftMask,            XK_F1,      spawn,          SHCMD("~/.local/bin/dmenu_umount") },
	/* Pulseaudio Control */
	{ MODKEY,		    XK_equal,	   spawn,	   SHCMD("pamixer --allow-boost -i 5; ~/.local/bin/killbar") },
	{ MODKEY|ShiftMask,	    XK_equal,      spawn, 	   SHCMD("pamixer --allow-boost -i 15; ~/.local/bin/killbar") },
	{ MODKEY,		    XK_minus,	   spawn,	   SHCMD("pamixer --allow-boost -d 5; ~/.local/bin/killbar") },
	{ MODKEY|ShiftMask,	    XK_minus,	   spawn,	   SHCMD("pamixer --allow-boost -d 15; ~/.local/bin/killbar") },
	{ MODKEY|ControlMask,	        XK_m,	   spawn,	   SHCMD("pamixer -t; ~/.local/bin/killbar") },
        /* MPD Control */
	{ MODKEY,                  XK_period,      spawn,          SHCMD("~/.local/bin/mpcnext") },
	{ MODKEY|ShiftMask,	   XK_period,	   spawn,	   SHCMD("mpc repeat") },
	{ MODKEY,                   XK_comma,      spawn,          SHCMD("~/.local/bin/mpcprev") },
	{ MODKEY|ShiftMask,	    XK_comma,	   spawn,	   SHCMD("mpc seek 0%") },
	{ MODKEY,                       XK_p,      spawn,          SHCMD("~/.local/bin/mpctoggle") },
	{ MODKEY|ShiftMask,             XK_p,      spawn,          SHCMD("mpc pause; pauseallmpv") },
	{ MODKEY,	      XK_bracketleft,	   spawn,	   SHCMD("mpc seek -10") },
	{ MODKEY|ShiftMask,   XK_bracketleft,	   spawn,	   SHCMD("mpc seek -60") },
	{ MODKEY,	     XK_bracketright,	   spawn,	   SHCMD("mpc seek +10") },
	{ MODKEY|ShiftMask,  XK_bracketright,	   spawn,	   SHCMD("mpc seek +60") },
        /* Utilities */
	{ MODKEY,                  XK_Delete,      spawn,          SHCMD("~/.local/bin/screencapturearea") },
	{ MODKEY|ShiftMask,        XK_Delete,      spawn,          SHCMD("~/.local/bin/screencaptureroot") },
//	{ MODKEY|ShiftMask,             XK_c,      spawn,          SHCMD("~/.local/bin/colorpicker") },
	/* dwm */
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_o,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,      incnmaster,     {.i = -1 } },
	{ MODKEY,			XK_v,	   togglesticky,   {0} },
	{ MODKEY,			XK_f,	   togglefullscr,  {0} },
        { MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
        { MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
        { MODKEY|ShiftMask,             XK_l,      shiftview,      {.i = +1 } },
        { MODKEY|ShiftMask,             XK_h,      shiftview,      {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                   XK_space,      zoom,           {0} },
        { MODKEY,	                XK_z,      incrgaps,       {.i = +5 } },
        { MODKEY,                       XK_x,      incrgaps,       {.i = -5 } },
        { MODKEY,                   XK_grave,      togglegaps,     {0} },
        { MODKEY|ShiftMask,	    XK_grave,      defaultgaps,    {0} },
	{ MODKEY,                     XK_Tab,      view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,          	        XK_t,      setlayout,      {.v = &layouts[0]} }, /* tile */
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[1]} }, /* bstack */
	{ MODKEY,           	        XK_u,      setlayout,      {.v = &layouts[2]} }, /* deck */
        { MODKEY|ShiftMask,             XK_u,      setlayout,      {.v = &layouts[3]} }, /* monocle */
        { MODKEY,     		        XK_i,      setlayout,      {.v = &layouts[4]} }, /* centered master */
	{ MODKEY|ShiftMask,             XK_i,      togglefloating, {0} },
        { MODKEY|ControlMask,           XK_j,      moveresize,     {.v = "0x 25y 0w 0h" } },
        { MODKEY|ControlMask,           XK_k,      moveresize,     {.v = "0x -25y 0w 0h" } },
        { MODKEY|ControlMask,           XK_l,      moveresize,     {.v = "25x 0y 0w 0h" } },
        { MODKEY|ControlMask,           XK_h,      moveresize,     {.v = "-25x 0y 0w 0h" } },
        { MODKEY|ControlMask|ShiftMask, XK_j,      moveresize,     {.v = "0x 0y 0w 25h" } },
        { MODKEY|ControlMask|ShiftMask, XK_k,      moveresize,     {.v = "0x 0y 0w -25h" } },
        { MODKEY|ControlMask|ShiftMask, XK_l,      moveresize,     {.v = "0x 0y 25w 0h" } },
        { MODKEY|ControlMask|ShiftMask, XK_h,      moveresize,     {.v = "0x 0y -25w 0h" } },
	{ MODKEY,                    XK_Left,      focusmon,       {.i = -1 } },
	{ MODKEY,                   XK_Right,      focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,          XK_Left,      tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,         XK_Right,      tagmon,         {.i = +1 } },
	{ 0,          XF86XK_MonBrightnessUp,      spawn,          SHCMD("xbacklight -inc 15") },
	{ MODKEY,     XF86XK_MonBrightnessUp,      spawn,          SHCMD("xbacklight -inc 5") },
	{ 0,          XF86XK_MonBrightnessDown,    spawn,          SHCMD("xbacklight -dec 15") },
	{ MODKEY,     XF86XK_MonBrightnessDown,    spawn,          SHCMD("xbacklight -dec 5") },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          SHCMD("") },
	{ ClkStatusText,        0,              Button3,        spawn,          SHCMD("~/.local/bin/killbar") },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
        { ClkRootWin,           0,              Button3,        spawn,          SHCMD("~/.local/bin/menu") },
};
