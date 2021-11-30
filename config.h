/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 0;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#00151a";
static const char col_gray2[]       = "#00151a";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#268bd2";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title      tags mask  isfloating  iscentered  isterminal  noswallow  monitor */
	{ "St",      NULL,     NULL,      0,         0,          0,          1,           0,        -1 },
	{ NULL,      NULL,     "urxvtc",  0,         0,          0,          1,           0,        -1 },
	{ NULL,      NULL,     "urxvt",   0,         0,          0,          1,           0,        -1 },
	{ NULL,      NULL,     "float",   0,         1,          1,          1,           0,        -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TERM "urxvt"

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define TERMCMD(cmd) { .v = (const char*[]){ TERM, "-title", "float", "-e", "sh", "-c", cmd, NULL } }

/* commands */
// static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
// static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { TERM, NULL };


int currentlayout[10] = {0,0,0,0,0,0,0,0,0,0};
float currentmfactor[10] = {0.55,0.55,0.55,0.55,0.55,0.55,0.55,0.55,0.55,0.55};
int currenttag = 0;

void changemfactor(Arg* arg) {
	float f = currentmfactor[currenttag] + arg->f;
	if(f < 0.1 || f > 0.9)
		return;
	currentmfactor[currenttag] = f;
	setmfact(&(Arg){.f = 1.0 + f});
}

void swaplayouts() {
	currentlayout[currenttag] = !currentlayout[currenttag];
	setlayout(&(Arg){.v = &layouts[currentlayout[currenttag]]});
}

void settag(Arg* arg) {
	currenttag = arg->i;
	setlayout(&(Arg){.v = &layouts[currentlayout[currenttag]]});
	setmfact(&(Arg){.f = 1.0 + currentmfactor[currenttag]});
	view(&(Arg){.ui = 1 << arg->i});
}

void setalltags() {
	view(&(Arg){.ui = ~0});
	currenttag = 9;
	setlayout(&(Arg){.v = &layouts[currentlayout[currenttag]]});
}

#define TAGKEYS(KEY,TAG) \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY,                       KEY,      settag,         {.i = TAG} },
	/*{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, */

#include "movestack.c"

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          SHCMD("terminxwd") },
	{ MODKEY,                       XK_b,      spawn,          SHCMD("$BROWSER") },
	{ MODKEY,                       XK_s,      spawn,          SHCMD("screenshot") },
	{ MODKEY,                       XK_u,      spawn,          TERMCMD("utfchars") },
	{ MODKEY,                       XK_t,      spawn,          TERMCMD("htop") },
	{ MODKEY,                       XK_a,      spawn,          TERMCMD("pulsemixer") },
	{ MODKEY,                       XK_n,      spawn,          TERMCMD("nmtui") },
	{ MODKEY,                       XK_c,      spawn,          TERMCMD("cht") },
//	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_m,      focusmaster,    {0} },
	{ MODKEY,                       XK_space,  focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_m,      zoom,           {0} },
	{ MODKEY|ShiftMask,             XK_space,  movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      changemfactor,  {.f = -0.05} },
	{ MODKEY,                       XK_l,      changemfactor,  {.f = +0.05} },
	{ MODKEY,                       XK_Tab,    swaplayouts,    {0} },
	{ MODKEY,                       XK_x,      killclient,     {0} },
//	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
//	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
//	{ MODKEY|ControlMask,           XK_space,  setlayout,      {0} },
	{ MODKEY|ControlMask,           XK_space,  togglefloating, {0} },
//	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY,                       XK_0,      setalltags,     {0} },
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
//	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
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

