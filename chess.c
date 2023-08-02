#include <uxn.h>

#define BOARD_MARGIN 16

int wSize = BOARD_MARGIN * 2 + 8 * 16;

enum COLOR {
    WHITE = 1,
    BLACK = 0,
};

enum PIECE_COLOR {
    WHITE_PIECE = 0x15,
    BLACK_PIECE = 0x0f,
};

int ORIGIN_SELECT_X = -1;
int ORIGIN_SELECT_y;
int TARGET_SELECT_X = -1;
int TARGET_SELECT_Y;

int IS_WHITE_TURN = 1;

int SELECTED_X = 9;
int SELECTED_Y = 9;

enum PIECE_TYPE {
    EMPTY = 0,
    PAWN = 1,
    ROOK = 2,
    KNIGHT = 3,
    BISHOP = 4,
    QUEEN = 5,
    KING = 6,
};

char pieces_images[7][8] = {
    {}, // EMPTY
    {0x18, 0x3c, 0x3c, 0x18, 0x18, 0x18, 0x7e, 0xff},   // PAWN
    {0xdb, 0xff, 0x7e, 0x3c, 0x3c, 0x3c, 0x7e, 0xff},   // ROOK
    {0x4, 0x1e, 0x2f, 0x7f, 0x1e, 0x1c, 0x3e, 0x7f},    // KNIGHT
    {0x18, 0x3c, 0x3c, 0x18, 0x3c, 0x18, 0x3c, 0x7e},   // BISHOP
    {0x0, 0x99, 0xdb, 0xdb, 0xff, 0x7e, 0x3c, 0x7e},   // QUEEN
    {0x18, 0x3c, 0x7e, 0xdb, 0x7e, 0x7e, 0x3c, 0x7e},   // KING
};

char mouse[8] = {0x0, 0x0, 0x10, 0x38, 0x10, 0x0, 0x0, 0x0};
char selected[8] = {0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa};

typedef struct {
    int x;
    int y;
    enum COLOR color;
    enum PIECE_COLOR piece_color;
    enum PIECE_TYPE piece_type;
} Tile;

Tile tiles[8][8] = {};

char tile_background[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void reset(void) {
     for (int i=0; i < 8; i++) {
        for (int j=0; j < 8; j++) {
            tiles[i][j].color = ((j % 2 == 0 && i % 2 == 0) || (j % 2 > 0 && i % 2 > 0)) 
                ? WHITE : BLACK;
            tiles[i][j].x = j;
            tiles[i][j].y = i;
            
            if (j == 0 || j == 7) {
                if (i == 0 || i == 7) {
                    tiles[i][j].piece_type = ROOK;
                }
                if (i == 1 || i == 6) {
                    tiles[i][j].piece_type = KNIGHT;
                }
                if (i == 2 || i == 5) {
                    tiles[i][j].piece_type = BISHOP;
                }
                if (i == 3) {
                    tiles[i][j].piece_type = QUEEN;
                }
                if (i == 4) {
                    tiles[i][j].piece_type = KING;
                }
            }
            if (j == 1 || j == 6) {
                tiles[i][j].piece_type = PAWN;
            }
            
            tiles[i][j].piece_color = j < 2 ? BLACK_PIECE : WHITE_PIECE;
        }
    }
}

void on_mouse(void) {
    int x = mouse_x();
    int y = mouse_y();
    set_screen_addr(mouse);
    set_screen_xy(x, y);
    draw_sprite(0x0f);

    if (mouse_state() == 1) {   // LMB
        int pos_x = (x - BOARD_MARGIN) / 16;
        int pos_y = (y - BOARD_MARGIN) / 16;

        int is_origin_set = ORIGIN_SELECT_X >= 0 && ORIGIN_SELECT_y >= 0;
        int is_target_set = TARGET_SELECT_X >= 0 && TARGET_SELECT_Y >= 0;
        int is_same_origin_tile = pos_x == ORIGIN_SELECT_X && pos_y == ORIGIN_SELECT_y;
        int is_same_target_tile = pos_x == TARGET_SELECT_X && pos_y == TARGET_SELECT_Y;

        if ((IS_WHITE_TURN == 1 && is_target_set && tiles[pos_x][pos_y].piece_color != WHITE_PIECE) ||
            IS_WHITE_TURN == -1 && is_target_set && tiles[pos_x][pos_y].piece_color != BLACK_PIECE) {
            putchar('B');
            return;
        }

        if ((!is_origin_set && !is_target_set) || (is_origin_set && is_target_set)) {
            putchar('O');

            ORIGIN_SELECT_X = pos_x;
            ORIGIN_SELECT_y = pos_y;

            TARGET_SELECT_X = -1;
            TARGET_SELECT_Y = -1;
        } else if (is_origin_set && !is_target_set && !is_same_origin_tile) {
            putchar('T');

            TARGET_SELECT_X = pos_x;
            TARGET_SELECT_Y = pos_y;

            tiles[pos_x][pos_y].piece_type = tiles[ORIGIN_SELECT_X][ORIGIN_SELECT_y].piece_type;
            tiles[pos_x][pos_y].piece_color = tiles[ORIGIN_SELECT_X][ORIGIN_SELECT_y].piece_color;
            
            tiles[ORIGIN_SELECT_X][ORIGIN_SELECT_y].piece_type = EMPTY;

            IS_WHITE_TURN *= -1;
        }
    }
}

void draw_board(void) {
    set_screen_addr(tile_background);

    // margin
    for (int i = 0; i < 20; i ++) {
        for (int j= 0; j < 20; j ++) {
            set_screen_xy(i * 8, j * 8);
            draw_sprite(0x2);
        }
    }

    // board
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

            int is_selected = (i == ORIGIN_SELECT_X && j == ORIGIN_SELECT_y) ||
                (i == TARGET_SELECT_X && j == TARGET_SELECT_Y);

            if (is_selected) {
                set_screen_addr(selected);
            } else {
                set_screen_addr(tile_background);
            }

            
            for (int ii = 0; ii < 2; ii++) {
                for (int jj = 0; jj < 2; jj++) {
                    set_screen_xy(
                        BOARD_MARGIN + i * 16 + ii * 8,
                        BOARD_MARGIN + j * 16 + jj * 8
                    );
                    draw_sprite(tiles[i][j].color ? 0x8 : 0x2);
                } 
            }

            set_screen_addr(pieces_images[tiles[i][j].piece_type % 10]);
            set_screen_xy(BOARD_MARGIN + i * 16 + 4, BOARD_MARGIN + j * 16 + 4);
            draw_sprite(tiles[i][j].piece_color);
        }
    }
}

void on_screen(void) {
    draw_board();
    on_mouse();
}

void main(void) {
    set_palette(0x2ce9, 0x01c0, 0x2ce5);
    // set_palette(0x09d4, 0x89e4, 0x59d4);
    set_screen_size(wSize, wSize);
    reset();
    draw_board();
    
}
