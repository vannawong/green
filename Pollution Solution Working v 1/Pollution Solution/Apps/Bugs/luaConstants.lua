--game setup
W_LEVEL_1_DIR 			= "data/levels/village/"
W_LEVEL_1_NAME 			= "village.tmx"
W_LEVEL_1_PATH 			= "data/levels/village/village.tmx"
W_INIT_FILE				= 'data/BugsInit.txt'
W_GUI_INIT_FILE			= "data/BugsGUI.txt"
W_DEBUG_FILE			= "DebugOutput.txt"

--game properties loading from file (change to load from lua?) 
W_TITLE					= "TITLE"
W_SCREEN_WIDTH 			= "SCREEN_WIDTH"
W_SCREEN_HEIGHT			= "SCREEN_HEIGHT"
W_USE_FULLSCREEN_MODE	= "USE_FULLSCREEN_MODE"
W_TEXT_FONT_SIZE		= "TEXT_FONT_SIZE"
W_VIEWPORT_OFFSET_X		= "VIEWPORT_OFFSET_X"
W_VIEWPORT_OFFSET_Y		= "VIEWPORT_OFFSET_Y";
W_TOGGLE_OFFSET_Y		= "TOGGLE_OFFSET_Y";
W_FONT_COLOR_RED		= "FONT_COLOR_RED";
W_FONT_COLOR_GREEN		= "FONT_COLOR_GREEN";
W_FONT_COLOR_BLUE		= "FONT_COLOR_BLUE";
W_COLOR_KEY_RED			= "COLOR_KEY_RED";
W_COLOR_KEY_GREEN		= "COLOR_KEY_GREEN";
W_COLOR_KEY_BLUE		= "COLOR_KEY_BLUE";

--Constants for artwork
W_GO_TO_MM_COMMAND		= "Go To Main Menu";
W_START_COMMAND			= "Start";
W_EXIT_COMMAND			= "Exit";
W_QUIT_COMMAND			= "Quit";
W_TRASH_CURSOR_PATH		= "data/gui/cursor/trash_cursor.png";
W_RED_CURSOR_PATH		= "data/gui/cursor/red_cursor.png";
W_SPLASH_SCREEN_PATH	= "data/gui/overlays/green_splash_screen_overlay.png";
W_MAIN_MENU_PATH		= "data/gui/overlays/green_main_menu_overlay.png";
W_START_IMAGE_PATH		= "data/gui/buttons/start_game.png";
W_START_IMAGE_MO_PATH	= "data/gui/buttons/start_game_mo.png";
W_EXIT_IMAGE_PATH		= "data/gui/buttons/exit_game.png";
W_EXIT_IMAGE_MO_PATH	= "data/gui/buttons/exit_game_mo.png";
W_QUIT_IMAGE_PATH		= "data/gui/buttons/quit.png";
W_QUIT_IMAGE_MO_PATH	= "data/gui/buttons/quit_mo.png";
W_GRID_CELL_SELECTOR_PATH = "data/gui/overlays/GridCellSelector.png";

--For Sprite
SPRITE_TYPES_DIR = "data/sprite_types/";
SPRITE_TYPES_LIST = SPRITE_TYPES_DIR + "sprite_type_list.xm";
JUMPING = "JUMPING";

--game world
MAX_VIEWPORT_AXIS_VELOCITY = 20.0;
MAX_PLAYER_VELOCITY = 7.5;
W_GRAVITY = 0.0;
JUMPING_BOT_MIN_CYCLES = 30;
JUMPING_BOT_MAX_CYCLES = 120;
JUMPING_BOT_MAX_VELOCITY = 40;
PLAYER_INIT_X = 80.0;
PLAYER_INIT_Y = 105.0;
JUMP_SPEED = -24.0;
PLAYER_SPEED = 6.0;
MIN_FPS = 5;
MAX_FPS = 100;
FPS_INC = 1;

--animation states
ATTACKING_RIGHT("ATTACKING_RIGHT");
ATTACKING_LEFT("ATTACKING_LEFT");
IDLE_DOWN("IDLE");
IDLE_UP("IDLE");
IDLE_LEFT("IDLE");
IDLE_RIGHT("IDLE");

DANCING("DANCING");
DYING("DYING");
WALKING("WALKING");
RUNNING("RUNNING");

--movement
W_KEY = 'W';
A_KEY = 'A';
S_KEY = 'S';
D_KEY = 'D';
P_KEY = 'P';
T_KEY = 'T';
L_KEY = 'L';
F_KEY = 'F';
R_KEY = 'R';
I_KEY = 'I';
Y_KEY = 'Y';
UP_KEY = VK_UP;
DOWN_KEY = VK_DOWN;
LEFT_KEY = VK_LEFT;
RIGHT_KEY = VK_RIGHT;
SPACE_KEY = VK_SPACE;
