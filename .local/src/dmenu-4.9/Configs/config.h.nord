/* See LICENSE file for copyright and license details. */
/* Default settings; can be overriden by command line. */

/* -b  option; if 0, dmenu appears at bottom */
static int topbar = 1;
/* -c option; centers dmenu on screen */
static int centered = 0;
/* minimum width when centered */
static int min_width = 50;
/* Size of the window border */
static const unsigned int border_width = 5;
static const char *fonts[] = { "iosevka comfy:size=20:style=bold:antialias=true:autohint=true" };
static const char *prompt      = NULL;      /* -p  option; prompt to the left of input field */
static const char *colors[SchemeLast][2] = {
                        /*     fg         bg       */
	[SchemeNorm]  = { "#e5e9f0", "#2e3440" },
	[SchemeSel]   = { "#2e3440", "#81a1c1" },
	/* [SchemeSel]   = { "#eceff4", "#4c566a" }, */
	[SchemeOut]   = { "#000000", "#00ffff" },
};
/* -l option; if nonzero, dmenu uses vertical list with given number of lines */
static unsigned int lines = 0;

/*
 * Characters not considered part of a word while deleting words
 * for example: " /?\"&[]"
 */
static const char worddelimiters[] = " ";
