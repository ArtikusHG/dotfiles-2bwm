///---User configurable stuff---///
///---Modifiers---///
#define MOD             XCB_MOD_MASK_4       /* Super/Windows key  or check xmodmap(1) with -pm  defined in /usr/include/xcb/xproto.h */
///--Speed---///
/* Move this many pixels when moving or resizing with keyboard unless the window has hints saying otherwise.
 *0)move step slow   1)move step fast
 *2)mouse slow       3)mouse fast     */
static const uint16_t movements[] = {20,40,20,400};
/* resize by line like in mcwm -- jmbi */
static const bool     resize_by_line          = true;
/* the ratio used when resizing and keeping the aspect */
static const float    resize_keep_aspect_ratio= 1.03;
///---Offsets---///
/*0)offsetx          1)offsety
 *2)maxwidth         3)maxheight */
static const uint8_t offsets[] = {0,0,0,0};
///---Colors---///
/*0)focuscol         1)unfocuscol
 *2)fixedcol         3)unkilcol
 *4)fixedunkilcol    5)outerbordercol
 *6)emptycol         */
static const char *colors[] = {"#e48a4b","#373e4c","#7a8c5c","#ff6666","#cc9933","#e48a4b","#000000"};
/* if this is set to true the inner border and outer borders colors will be swapped */
static const bool inverted_colors = true;
///---Cursor---///
/* default position of the cursor:
 * correct values are:
 * TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT, MIDDLE
 * All these are relative to the current window. */
#define CURSOR_POSITION MIDDLE
///---Borders---///
/*0) Outer border size. If you put this negative it will be a square.
 *1) Full borderwidth    2) Magnet border size
 *3) Resize border size  */
static const uint8_t borders[] = {3,5,5,4};
/* Windows that won't have a border.
 * It uses substring comparison with what is found in the WM_NAME
 * attribute of the window. You can test this using `xprop WM_NAME`
 */
#define LOOK_INTO "WM_NAME"
static const char *ignore_names[] = {"bar", "xclock"};
///--Menus and Programs---///
static const char *menucmd[]     = { "bash", "-c", "nohup $(ls --color=off /usr/bin | dmenu -l 10 -nb \"#272D3A\" -fn \"t cherry\")", NULL };
static const char *terminalcmd[] = { "urxvt", "-g", "60x20+26+43", NULL };
static const char *firefoxcmd[]  = { "firefox", NULL };
static const char *screenshot[]  = { "scrot", "%Y-%m-%d-%T_$wx$h_scrot.png", "-e", "mv ./*scrot.png ~/Screenshots && notify-send Done 'Screenshot saved.'", NULL };
///--Volume and Brigntess control stuff--///
static const char *volumetoggle[]   = { "amixer", "set", "Master", "toggle", NULL };
static const char *volumeincrease[] = { "amixer", "-q", "set", "Master", "5%+", NULL };
static const char *volumedecrease[] = { "amixer", "-q", "set", "Master", "5%-", NULL };
static const char *brightnessdec[]  = { "xbacklight", "-set", "-dec", "-10%", NULL };
static const char *brightnessinc[]  = { "xbacklight", "-set", "-inc", "+10%", NULL };
///--mpd playback control-///
static const char *nextsong[]   = { "bash", "-c", "mpc next" };
static const char *prevsong[]   = { "bash", "-c", "mpc prev" };
static const char *togglesong[] = { "bash", "-c", "mpc toggle" };
///--Custom foo---///
static void halfandcentered(const Arg *arg)
{
	Arg arg2 = {.i=TWOBWM_MAXHALF_VERTICAL_LEFT};
	maxhalf(&arg2);
	Arg arg3 = {.i=TWOBWM_TELEPORT_CENTER};
	teleport(&arg3);
}
///---Shortcuts---///
/* Check /usr/include/X11/keysymdef.h for the list of all keys
 * 0x000000 is for no modkey
 * If you are having trouble finding the right keycode use the `xev` to get it
 * For example:
 * KeyRelease event, serial 40, synthetic NO, window 0x1e00001,
 *  root 0x98, subw 0x0, time 211120530, (128,73), root:(855,214),
 *  state 0x10, keycode 171 (keysym 0x1008ff17, XF86AudioNext), same_screen YES,
 *  XLookupString gives 0 bytes: 
 *  XFilterEvent returns: False
 *
 *  The keycode here is keysym 0x1008ff17, so use  0x1008ff17
 *
 *
 * For AZERTY keyboards XK_1...0 should be replaced by :
 *      DESKTOPCHANGE(     XK_ampersand,                     0)
 *      DESKTOPCHANGE(     XK_eacute,                        1)
 *      DESKTOPCHANGE(     XK_quotedbl,                      2)
 *      DESKTOPCHANGE(     XK_apostrophe,                    3)
 *      DESKTOPCHANGE(     XK_parenleft,                     4)
 *      DESKTOPCHANGE(     XK_minus,                         5)
 *      DESKTOPCHANGE(     XK_egrave,                        6)
 *      DESKTOPCHANGE(     XK_underscore,                    7)
 *      DESKTOPCHANGE(     XK_ccedilla,                      8)
 *      DESKTOPCHANGE(     XK_agrave,                        9)*
 */
#define DESKTOPCHANGE(K,N) \
{  MOD ,             K,              changeworkspace, {.i=N}}, \
{  MOD |SHIFT,       K,              sendtoworkspace, {.i=N}},
static key keys[] = {
    /* modifier           key            function           argument */
    // Focus to next/previous window
    {  MOD,               XK_Left,       focusnext,         {.i=TWOBWM_FOCUS_NEXT}},
    {  MOD,               XK_Right,      focusnext,         {.i=TWOBWM_FOCUS_PREVIOUS}},
    // Kill a window
    {  MOD,               XK_q,          deletewin,         {}},
    // Resize a window
    {  ALT |SHIFT,        XK_Up,         resizestep,        {.i=TWOBWM_RESIZE_UP}},
    {  ALT |SHIFT,        XK_Down,       resizestep,        {.i=TWOBWM_RESIZE_DOWN}},
    {  ALT |SHIFT,        XK_Right,      resizestep,        {.i=TWOBWM_RESIZE_RIGHT}},
    {  ALT |SHIFT,        XK_Left,       resizestep,        {.i=TWOBWM_RESIZE_LEFT}},
    // Resize a window slower
    {  ALT |SHIFT|MOD,XK_Up,             resizestep,        {.i=TWOBWM_RESIZE_UP_SLOW}},
    {  ALT |SHIFT|MOD,XK_Down,           resizestep,        {.i=TWOBWM_RESIZE_DOWN_SLOW}},
    {  ALT |SHIFT|MOD,XK_Right,          resizestep,        {.i=TWOBWM_RESIZE_RIGHT_SLOW}},
    {  ALT |SHIFT|MOD,XK_Left,           resizestep,        {.i=TWOBWM_RESIZE_LEFT_SLOW}},
    // Move a window
    {  ALT ,              XK_Up,         movestep,          {.i=TWOBWM_MOVE_UP}},
    {  ALT ,              XK_Down,       movestep,          {.i=TWOBWM_MOVE_DOWN}},
    {  ALT ,              XK_Right,      movestep,          {.i=TWOBWM_MOVE_RIGHT}},
    {  ALT ,              XK_Left,       movestep,          {.i=TWOBWM_MOVE_LEFT}},
    // Move a window slower
    {  ALT |MOD,          XK_Up,         movestep,          {.i=TWOBWM_MOVE_UP_SLOW}},
    {  ALT |MOD,          XK_Down,       movestep,          {.i=TWOBWM_MOVE_DOWN_SLOW}},
    {  ALT |MOD,          XK_Right,      movestep,          {.i=TWOBWM_MOVE_RIGHT_SLOW}},
    {  ALT |MOD,          XK_Left,       movestep,          {.i=TWOBWM_MOVE_LEFT_SLOW}},
    // Teleport the window to an area of the screen.
    // Center:
    {  ALT,               XK_c,          teleport,          {.i=TWOBWM_TELEPORT_CENTER}},
    // Top left:
    {  MOD,               XK_y,          teleport,          {.i=TWOBWM_TELEPORT_TOP_LEFT}},
    // Top right:
    {  MOD,               XK_u,          teleport,          {.i=TWOBWM_TELEPORT_TOP_RIGHT}},
    // Bottom left:
    {  MOD,               XK_b,          teleport,          {.i=TWOBWM_TELEPORT_BOTTOM_LEFT}},
    // Bottom right:
    {  MOD,               XK_n,          teleport,          {.i=TWOBWM_TELEPORT_BOTTOM_RIGHT}},
    // Resize while keeping the window aspect
    {  MOD,               XK_Home,       resizestep_aspect, {.i=TWOBWM_RESIZE_KEEP_ASPECT_GROW}},
    {  MOD,               XK_End,        resizestep_aspect, {.i=TWOBWM_RESIZE_KEEP_ASPECT_SHRINK}},
    // Full screen window without borders
    {  ALT,               XK_f,          maximize,          {.i=TWOBWM_FULLSCREEN}},
    // Maximize and move
    // vertically left
    {  MOD |SHIFT,        XK_y,          maxhalf,           {.i=TWOBWM_MAXHALF_VERTICAL_LEFT}},
    // vertically right
    {  MOD |SHIFT,        XK_u,          maxhalf,           {.i=TWOBWM_MAXHALF_VERTICAL_RIGHT}},
    // horizontally left
    {  MOD |SHIFT,        XK_b,          maxhalf,           {.i=TWOBWM_MAXHALF_HORIZONTAL_BOTTOM}},
    // horizontally right
    {  MOD |SHIFT,        XK_n,          maxhalf,           {.i=TWOBWM_MAXHALF_HORIZONTAL_TOP}},
    //fold half vertically
    {  MOD |SHIFT|CONTROL,XK_y,          maxhalf,           {.i=TWOBWM_MAXHALF_FOLD_VERTICAL}},
    //fold half horizontally
    {  MOD |SHIFT|CONTROL,XK_b,          maxhalf,           {.i=TWOBWM_MAXHALF_FOLD_HORIZONTAL}},
    //unfold vertically
    {  MOD |SHIFT|CONTROL,XK_u,          maxhalf,           {.i=TWOBWM_MAXHALF_UNFOLD_VERTICAL}},
    //unfold horizontally
    {  MOD |SHIFT|CONTROL,XK_n,          maxhalf,           {.i=TWOBWM_MAXHALF_UNFOLD_HORIZONTAL}},
    // Next/Previous workspace
    {  MOD ,              XK_v,          nextworkspace,     {}},
    {  MOD ,              XK_c,          prevworkspace,     {}},
    // Move to Next/Previous workspace
    {  MOD |SHIFT,        XK_v,          sendtonextworkspace,{}},
    {  MOD |SHIFT,        XK_c,          sendtoprevworkspace,{}},
    // Make the window stay on all workspaces
    {  MOD ,              XK_f,          fix,               {}},
    // Start programs
    {  ALT,               XK_t,          start,             {.com = terminalcmd}},
    {  ALT,               XK_m,          start,             {.com = firefoxcmd}},
    {  CONTROL,           XK_Return,     start,             {.com = menucmd}},
    {  CONTROL,           XK_Print,      start,             {.com = screenshot}},
    // Volume and brightness control
    {  ALT,               XK_F1,         start,             {.com = volumetoggle}},
    {  ALT,               XK_F2,         start,             {.com = volumedecrease}},
    {  ALT,               XK_F3,         start,             {.com = volumeincrease}},
    {  ALT,               XK_F11,        start,             {.com = brightnessdec}},
    {  ALT,               XK_F12,        start,             {.com = brightnessinc}},
    // mpd playback control
    {  ALT,               XK_F4,         start,             {.com = prevsong}},
    {  ALT,               XK_F5,         start,             {.com = togglesong}},
    {  ALT,               XK_F6,         start,             {.com = nextsong}},
    // Exit or restart 2bwm
    {  MOD |CONTROL,      XK_q,          twobwm_exit,       {.i=0}},
    {  MOD |CONTROL,      XK_r,          twobwm_restart,    {.i=0}},
    {  MOD,               XK_space,      halfandcentered,   {.i=0}},
    // Change current workspace
       DESKTOPCHANGE(     XK_1,                             0)
       DESKTOPCHANGE(     XK_2,                             1)
       DESKTOPCHANGE(     XK_3,                             2)
       DESKTOPCHANGE(     XK_4,                             3)
       DESKTOPCHANGE(     XK_5,                             4)
       DESKTOPCHANGE(     XK_6,                             5)
       DESKTOPCHANGE(     XK_7,                             6)
       DESKTOPCHANGE(     XK_8,                             7)
       DESKTOPCHANGE(     XK_9,                             8)
       DESKTOPCHANGE(     XK_0,                             9)
};
// the last argument makes it a root window only event
static Button buttons[] = {
    {  MOD          ,XCB_BUTTON_INDEX_1,     mousemotion,   {.i=TWOBWM_MOVE}, false},
    {  MOD          ,XCB_BUTTON_INDEX_3,     mousemotion,   {.i=TWOBWM_RESIZE}, false},
    {  0            ,XCB_BUTTON_INDEX_3,     start,         {.com = menucmd}, true},
    {  MOD|SHIFT,    XCB_BUTTON_INDEX_1,     changeworkspace, {.i=0}, false},
    {  MOD|SHIFT,    XCB_BUTTON_INDEX_3,     changeworkspace, {.i=1}, false},
    {  MOD|ALT,      XCB_BUTTON_INDEX_1,     changescreen,    {.i=1}, false},
    {  MOD|ALT,      XCB_BUTTON_INDEX_3,     changescreen,    {.i=0}, false}
};
