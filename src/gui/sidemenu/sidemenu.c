#include <SDL2/SDL.h>
#include <cairo.h>
#include <stdio.h>
#include <stdbool.h>
#include "gui/game/game_engine.h"
#include "game_logic/score.h"

#define WIDTH 900
#define HEIGHT 900

#define CELL_W 60
#define CELL_H 30

#define EASY_CHEST_ICON "assets/game/chest_easy.png"
#define NEUTRAL_CHEST_ICON "assets/game/chest_neutral.png"
#define HARD_CHEST_ICON "assets/game/chest_hard.png"


// Scrolling state
int scroll_y = 0;
int scroll_x = 0;
bool dragging_v_scrollbar = false;
bool dragging_h_scrollbar = false;
int dimension = 0;

// Sample data
static int** payoff_matrix;
static double* hider_probabilities;
static double* seeker_probabilities;
static float hider_score = 750;
static float seeker_score = 680;

static cairo_surface_t *cairo_surface = NULL;


static cairo_t *cr = NULL;
static SDL_Window *window = NULL;

bool side_menu_has_focus() {
    Uint32 window_flags = SDL_GetWindowFlags(window);
    return (window_flags & SDL_WINDOW_INPUT_FOCUS) != 0;
}
// Medieval style colors
#define PARCHMENT_COLOR 0.94, 0.9, 0.7
#define DARK_BROWN 0.4, 0.2, 0.1
#define MID_BROWN 0.6, 0.4, 0.2
#define LIGHT_BROWN 0.8, 0.6, 0.4
#define RED_INK 0.7, 0.1, 0.1
#define GREEN_INK 0.1, 0.5, 0.1
#define BLUE_INK 0.1, 0.1, 0.6

// Button structure and callback
typedef void (*ButtonCallback)(void);
typedef struct {
    int x, y, width, height;
    const char* label;
    ButtonCallback callback;
    bool hovered;
    bool pressed;
} MedievalButton;

void my_custom_function() ;
bool is_button_hovered(MedievalButton* button, int mouse_x, int mouse_y);
static MedievalButton* button = NULL;
static ButtonCallback custom_function = NULL;

void init_data() {
    // Initialize payoff matrix with sample values

    payoff_matrix = game_engine_get_game_matrix();
    dimension = game_engine_get_dimension();


    // Initialize hider probabilities (sum should be <= 1)

    hider_probabilities = game_engine_get_hider()->probabilities;


    // Initialize seeker probabilities with different distribution
    seeker_probabilities = game_engine_get_seeker()->probabilities;

    seeker_score = 0;
    hider_score = 0;
}


void update_data() {
    // Initialize payoff matrix with sample values
    struct score* score = get_score();
    float hider_score_ = score->hider_score;
    float seeker_score_ = score->seeker_score;


    seeker_score = seeker_score_;
    hider_score = hider_score_;

}

void draw_medieval_border(cairo_t *cr, int x, int y, int width, int height) {
    int border_width = 10;

    // Outer border
    cairo_set_source_rgb(cr, DARK_BROWN);
    cairo_rectangle(cr, x, y, width, height);
    cairo_set_line_width(cr, border_width);
    cairo_stroke(cr);

    // Inner border
    cairo_set_source_rgb(cr, MID_BROWN);
    cairo_rectangle(cr, x + border_width/2, y + border_width/2,
                    width - border_width, height - border_width);
    cairo_set_line_width(cr, 2);
    cairo_stroke(cr);

    // Corner decorations
    int corner_size = 15;
    cairo_set_source_rgb(cr, DARK_BROWN);

    // Top-left
    cairo_move_to(cr, x + corner_size, y + border_width/2);
    cairo_line_to(cr, x + border_width/2, y + border_width/2);
    cairo_line_to(cr, x + border_width/2, y + corner_size);
    cairo_set_line_width(cr, 3);
    cairo_stroke(cr);

    // Top-right
    cairo_move_to(cr, x + width - corner_size, y + border_width/2);
    cairo_line_to(cr, x + width - border_width/2, y + border_width/2);
    cairo_line_to(cr, x + width - border_width/2, y + corner_size);
    cairo_stroke(cr);

    // Bottom-left
    cairo_move_to(cr, x + border_width/2, y + height - corner_size);
    cairo_line_to(cr, x + border_width/2, y + height - border_width/2);
    cairo_line_to(cr, x + corner_size, y + height - border_width/2);
    cairo_stroke(cr);

    // Bottom-right
    cairo_move_to(cr, x + width - border_width/2, y + height - corner_size);
    cairo_line_to(cr, x + width - border_width/2, y + height - border_width/2);
    cairo_line_to(cr, x + width - corner_size, y + height - border_width/2);
    cairo_stroke(cr);
}

void draw_medieval_title(cairo_t *cr, int x, int y, const char *title) {
    // Title background
    cairo_set_source_rgb(cr, MID_BROWN);
    cairo_rectangle(cr, x, y - 25, strlen(title) * 14, 30);
    cairo_fill(cr);

    // Title border
    cairo_set_source_rgb(cr, DARK_BROWN);
    cairo_rectangle(cr, x, y - 25, strlen(title) * 14, 30);
    cairo_set_line_width(cr, 2);
    cairo_stroke(cr);

    // Title text
    cairo_select_font_face(cr, "Serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 20);
    cairo_set_source_rgb(cr, PARCHMENT_COLOR);
    cairo_move_to(cr, x + 10, y);
    cairo_show_text(cr, title);
}

void draw_legends(cairo_t *cr, int x, int y) {
    draw_medieval_title(cr, x, y, "Legends");

    int chest_size = 30;  // Bigger than the original box_size
    int spacing = 120;

    // Background for legend area
    cairo_set_source_rgb(cr, PARCHMENT_COLOR);
    cairo_rectangle(cr, x - 10, y + 5, WIDTH - 2*x + 20, 55);  // Made taller for bigger icons
    cairo_fill(cr);

    // Border for legend area
    cairo_set_source_rgb(cr, MID_BROWN);
    cairo_rectangle(cr, x - 10, y + 5, WIDTH - 2*x + 20, 55);
    cairo_set_line_width(cr, 2);
    cairo_stroke(cr);

    // Load chest images if they haven't been loaded yet
    static cairo_surface_t *easy_chest = NULL;
    static cairo_surface_t *neutral_chest = NULL;
    static cairo_surface_t *hard_chest = NULL;

    if (easy_chest == NULL) {
        easy_chest = cairo_image_surface_create_from_png(EASY_CHEST_ICON);
        if (cairo_surface_status(easy_chest) != CAIRO_STATUS_SUCCESS) {
            fprintf(stderr, "Failed to load easy chest icon: %s\n", EASY_CHEST_ICON);
            easy_chest = NULL;
        }
    }

    if (neutral_chest == NULL) {
        neutral_chest = cairo_image_surface_create_from_png(NEUTRAL_CHEST_ICON);
        if (cairo_surface_status(neutral_chest) != CAIRO_STATUS_SUCCESS) {
            fprintf(stderr, "Failed to load neutral chest icon: %s\n", NEUTRAL_CHEST_ICON);
            neutral_chest = NULL;
        }
    }

    if (hard_chest == NULL) {
        hard_chest = cairo_image_surface_create_from_png(HARD_CHEST_ICON);
        if (cairo_surface_status(hard_chest) != CAIRO_STATUS_SUCCESS) {
            fprintf(stderr, "Failed to load hard chest icon: %s\n", HARD_CHEST_ICON);
            hard_chest = NULL;
        }
    }

    // Draw Easy chest icon
    if (easy_chest != NULL) {
        cairo_save(cr);
        cairo_translate(cr, x, y + 15);
        cairo_scale(cr, (double)chest_size / cairo_image_surface_get_width(easy_chest),
                        (double)chest_size / cairo_image_surface_get_height(easy_chest));
        cairo_set_source_surface(cr, easy_chest, 0, 0);
        cairo_paint(cr);
        cairo_restore(cr);
    } else {
        // Fallback to colored box if image fails to load
        cairo_set_source_rgb(cr, 0.2, 0.6, 0.2);
        cairo_rectangle(cr, x, y + 15, chest_size, chest_size);
        cairo_fill(cr);
        cairo_set_source_rgb(cr, DARK_BROWN);
        cairo_rectangle(cr, x, y + 15, chest_size, chest_size);
        cairo_set_line_width(cr, 1);
        cairo_stroke(cr);
    }

    // Add Easy text
    cairo_set_source_rgb(cr, DARK_BROWN);
    cairo_select_font_face(cr, "Serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 16);
    cairo_move_to(cr, x + chest_size + 10, y + 35);
    cairo_show_text(cr, "Easy");

    // Draw Neutral chest icon
    if (neutral_chest != NULL) {
        cairo_save(cr);
        cairo_translate(cr, x + spacing, y + 15);
        cairo_scale(cr, (double)chest_size / cairo_image_surface_get_width(neutral_chest),
                        (double)chest_size / cairo_image_surface_get_height(neutral_chest));
        cairo_set_source_surface(cr, neutral_chest, 0, 0);
        cairo_paint(cr);
        cairo_restore(cr);
    } else {
        // Fallback to colored box
        cairo_set_source_rgb(cr, 0.8, 0.7, 0.2);
        cairo_rectangle(cr, x + spacing, y + 15, chest_size, chest_size);
        cairo_fill(cr);
        cairo_set_source_rgb(cr, DARK_BROWN);
        cairo_rectangle(cr, x + spacing, y + 15, chest_size, chest_size);
        cairo_set_line_width(cr, 1);
        cairo_stroke(cr);
    }

    // Add Neutral text
    cairo_set_source_rgb(cr, DARK_BROWN);
    cairo_move_to(cr, x + spacing + chest_size + 10, y + 35);
    cairo_show_text(cr, "Neutral");

    // Draw Hard chest icon
    if (hard_chest != NULL) {
        cairo_save(cr);
        cairo_translate(cr, x + spacing * 2, y + 15);
        cairo_scale(cr, (double)chest_size / cairo_image_surface_get_width(hard_chest),
                        (double)chest_size / cairo_image_surface_get_height(hard_chest));
        cairo_set_source_surface(cr, hard_chest, 0, 0);
        cairo_paint(cr);
        cairo_restore(cr);
    } else {
        // Fallback to colored box
        cairo_set_source_rgb(cr, 0.7, 0.2, 0.2);
        cairo_rectangle(cr, x + spacing * 2, y + 15, chest_size, chest_size);
        cairo_fill(cr);
        cairo_set_source_rgb(cr, DARK_BROWN);
        cairo_rectangle(cr, x + spacing * 2, y + 15, chest_size, chest_size);
        cairo_set_line_width(cr, 1);
        cairo_stroke(cr);
    }

    // Add Hard text
    cairo_set_source_rgb(cr, DARK_BROWN);
    cairo_move_to(cr, x + spacing * 2 + chest_size + 10, y + 35);
    cairo_show_text(cr, "Hard");
}

void draw_scores(cairo_t *cr, int x, int y) {
    draw_medieval_title(cr, x, y, "Score");

    // Background for scores area
    cairo_set_source_rgb(cr, PARCHMENT_COLOR);
    cairo_rectangle(cr, x - 10, y + 5, WIDTH - 2*x + 20, 40);
    cairo_fill(cr);

    // Border for scores area
    cairo_set_source_rgb(cr, MID_BROWN);
    cairo_rectangle(cr, x - 10, y + 5, WIDTH - 2*x + 20, 40);
    cairo_set_line_width(cr, 2);
    cairo_stroke(cr);

    // Hider score
    char hider_text[50];
    snprintf(hider_text, sizeof(hider_text), "Hider Score: %.2f", hider_score);

    cairo_select_font_face(cr, "Serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 16);
    cairo_set_source_rgb(cr, RED_INK);
    cairo_move_to(cr, x + 20, y + 30);
    cairo_show_text(cr, hider_text);

    // Seeker score
    char seeker_text[50];
    snprintf(seeker_text, sizeof(seeker_text), "Seeker Score: %.2f", seeker_score);

    cairo_set_source_rgb(cr, BLUE_INK);
    cairo_move_to(cr, x + 250, y + 30);
    cairo_show_text(cr, seeker_text);
}

void draw_payoff_matrix(cairo_t *cr, int x, int y, int scroll_x_offset, int scroll_y_offset) {

    Chest* chests = game_engine_get_chests();
    draw_medieval_title(cr, x, y, "Payoff Matrix");

    // Background for matrix area
    cairo_set_source_rgb(cr, PARCHMENT_COLOR);
    cairo_rectangle(cr, x - 10, y + 5, WIDTH - 2*x + 20, 305);
    cairo_fill(cr);

    // Border for matrix area
    cairo_set_source_rgb(cr, MID_BROWN);
    cairo_rectangle(cr, x - 10, y + 5, WIDTH - 2*x + 20, 305);
    cairo_set_line_width(cr, 2);
    cairo_stroke(cr);

    int start_x = x - scroll_x_offset;
    int start_y = y + 30 - scroll_y_offset;

    cairo_set_line_width(cr, 1);
    cairo_set_source_rgb(cr, DARK_BROWN);

    // Set up clipping to prevent drawing outside the intended area
    cairo_save(cr);
    // Increase right margin to fix the out-of-frame issue
    cairo_rectangle(cr, x, y + 30, WIDTH - x - 40, 280);
    cairo_clip(cr);

    // Draw grid
    for (int r = 0; r <= dimension; ++r) {
        cairo_move_to(cr, start_x, start_y + r * CELL_H);
        cairo_line_to(cr, start_x + dimension * CELL_W, start_y + r * CELL_H);
    }

    for (int c = 0; c <= dimension; ++c) {
        cairo_move_to(cr, start_x + c * CELL_W, start_y);
        cairo_line_to(cr, start_x + c * CELL_W, start_y + dimension * CELL_H);
    }

    cairo_stroke(cr);

    // Add cell text
    cairo_set_font_size(cr, 12);
    cairo_select_font_face(cr, "Serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    for (int r = 0; r < dimension; ++r) {
        for (int c = 0; c < dimension; ++c) {
            // Color cells based on value (green for low values, red for high)
            double value = payoff_matrix[r][c];

            // Background color
            switch(chests[r].difficulty)
            {
                case _EASY:
                    cairo_set_source_rgb(cr, 0.2, 0.6, 0.2); // Green
                    break;
                case _NEUTRAL:
                    cairo_set_source_rgb(cr, 0.8, 0.7, 0.2); // Yellow
                    break;

                case _HARD:
                    cairo_set_source_rgb(cr, 0.7, 0.2, 0.2); // Red
                    break;


            }


            cairo_rectangle(cr,
                start_x + c * CELL_W + 1,
                start_y + r * CELL_H + 1,
                CELL_W - 2,
                CELL_H - 2);
            cairo_fill(cr);

            // Cell text
            cairo_set_source_rgb(cr, DARK_BROWN);
            char text[16];
            snprintf(text, sizeof(text), "%.2f", value);
            cairo_move_to(cr, start_x + c * CELL_W + 5, start_y + r * CELL_H + 20);
            cairo_show_text(cr, text);
        }
    }

    cairo_restore(cr); // Remove clipping
}

void draw_probability_bars(cairo_t *cr, int x, int y, int scroll_x_offset, int mouse_x, int mouse_y,
                         double probabilities[], const char *title, double rgb_color[3]) {
    draw_medieval_title(cr, x, y, title);

    Chest* chests = game_engine_get_chests();
    // Background for probability area
    cairo_set_source_rgb(cr, PARCHMENT_COLOR);
    cairo_rectangle(cr, x - 10, y + 5, WIDTH - 2*x + 20, 130);
    cairo_fill(cr);

    // Border for probability area
    cairo_set_source_rgb(cr, MID_BROWN);
    cairo_rectangle(cr, x - 10, y + 5, WIDTH - 2*x + 20, 130);
    cairo_set_line_width(cr, 2);
    cairo_stroke(cr);

    int start_x = x - scroll_x_offset;
    int start_y = y + 30;
    int bar_height = 80; // Height for the tallest bar (representing 1.0)

    // Set up clipping
    cairo_save(cr);
    // Expand clipping to ensure bars don't cross border
    cairo_rectangle(cr, x, y + 30, WIDTH - x - 40, bar_height + 20);
    cairo_clip(cr);

    // Draw bar chart
    int hovered_column = -1;

    for (int c = 0; c < dimension; c++) {
        double prob = probabilities[c];
        int current_x = start_x + c * CELL_W;
        int current_bar_height = prob * bar_height;

        // Check if this bar is being hovered
        if (mouse_x >= current_x && mouse_x < current_x + CELL_W &&
            mouse_y >= start_y && mouse_y <= start_y + bar_height) {
            hovered_column = c;
        }

        // Draw axis tick marks and grid lines
        cairo_set_source_rgba(cr, MID_BROWN, 0.3);
        cairo_move_to(cr, current_x, start_y);
        cairo_line_to(cr, current_x, start_y + bar_height);
        cairo_stroke(cr);

        // Color bars based on probability value rather than using the passed color
        switch(chests[c].difficulty)
        {
            case _EASY:
                cairo_set_source_rgba(cr, 0.2, 0.6, 0.2, 0.7); // Green
                break;
            case _NEUTRAL:
                cairo_set_source_rgba(cr, 0.8, 0.7, 0.2, 0.7); // Yellow
                break;

            case _HARD:
                cairo_set_source_rgba(cr, 0.7, 0.2, 0.2, 0.7); // Red
                break;
        }


        // Draw the bar (starting from the bottom)
        cairo_rectangle(cr,
            current_x + 4, // Add a small margin
            start_y + (bar_height - current_bar_height),
            CELL_W - 8, // Width with margins
            current_bar_height);
        cairo_fill(cr);

        // Add medieval style to bar - use the passed color for the border only
        cairo_set_source_rgba(cr, rgb_color[0], rgb_color[1], rgb_color[2], 0.7);
        cairo_rectangle(cr,
            current_x + 4,
            start_y + (bar_height - current_bar_height),
            CELL_W - 8,
            current_bar_height);
        cairo_set_line_width(cr, 1);
        cairo_stroke(cr);

        // Add a pattern to the bar for medieval feel
        if (current_bar_height > 10) {
            cairo_set_source_rgba(cr, DARK_BROWN, 0.2);
            for (int i = 0; i < current_bar_height; i += 10) {
                cairo_move_to(cr, current_x + 4, start_y + bar_height - i);
                cairo_line_to(cr, current_x + CELL_W - 4, start_y + bar_height - i);
            }
            cairo_stroke(cr);
        }
    }

    // Draw wooden baseline
    cairo_set_source_rgb(cr, DARK_BROWN);
    cairo_set_line_width(cr, 3);
    cairo_move_to(cr, start_x, start_y + bar_height);
    cairo_line_to(cr, start_x + dimension * CELL_W, start_y + bar_height);
    cairo_stroke(cr);

    // Add y-axis labels (0, 0.5, 1.0) with medieval style
    cairo_set_source_rgb(cr, DARK_BROWN);
    cairo_select_font_face(cr, "Serif", CAIRO_FONT_SLANT_ITALIC, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 12);

    cairo_move_to(cr, start_x - 25, start_y + bar_height);
    cairo_show_text(cr, "0.0");

    cairo_move_to(cr, start_x - 25, start_y + bar_height/2);
    cairo_show_text(cr, "0.5");

    cairo_move_to(cr, start_x - 25, start_y);
    cairo_show_text(cr, "1.0");

    // Draw hover tooltip if a column is hovered
    if (hovered_column >= 0) {
        double prob = probabilities[hovered_column];
        char text[32];
        snprintf(text, sizeof(text), "Probability: %.2f", prob);

        // Measure text width for tooltip box
        cairo_text_extents_t extents;
        cairo_set_font_size(cr, 14);
        cairo_text_extents(cr, text, &extents);

        int tooltip_width = extents.width + 20;
        int tooltip_height = 30;
        int tooltip_x = start_x + hovered_column * CELL_W + CELL_W/2 - tooltip_width/2;
        int tooltip_y = start_y + (bar_height - (bar_height * prob)) - tooltip_height - 5;

        // Ensure tooltip stays within view
        if (tooltip_x < x) tooltip_x = x;
        if (tooltip_x + tooltip_width > WIDTH - 30) tooltip_x = WIDTH - 30 - tooltip_width;
        if (tooltip_y < y + 30) tooltip_y = y + 30;

        // Draw tooltip background with medieval parchment style
        cairo_set_source_rgb(cr, LIGHT_BROWN);
        cairo_rectangle(cr, tooltip_x, tooltip_y, tooltip_width, tooltip_height);
        cairo_fill(cr);

        // Draw tooltip border
        cairo_set_source_rgb(cr, DARK_BROWN);
        cairo_rectangle(cr, tooltip_x, tooltip_y, tooltip_width, tooltip_height);
        cairo_set_line_width(cr, 1);
        cairo_stroke(cr);

        // Draw tooltip text
        cairo_set_source_rgb(cr, DARK_BROWN);
        cairo_select_font_face(cr, "Serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_move_to(cr, tooltip_x + 10, tooltip_y + 20);
        cairo_show_text(cr, text);
    }

    cairo_restore(cr); // Remove clipping
}

void draw_medieval_scrollbar(cairo_t *cr, int x, int y, int width, int height,
                          int handle_pos, int handle_size, bool is_horizontal) {
    // Draw scrollbar track with wood texture effect
    cairo_set_source_rgb(cr, MID_BROWN);
    cairo_rectangle(cr, x, y, width, height);
    cairo_fill(cr);

    // Add wood grain
    cairo_set_source_rgba(cr, DARK_BROWN, 0.3);
    cairo_set_line_width(cr, 1);

    if (is_horizontal) {
        for (int i = 0; i < width; i += 5) {
            cairo_move_to(cr, x + i, y);
            cairo_line_to(cr, x + i, y + height);
        }
    } else {
        for (int i = 0; i < height; i += 5) {
            cairo_move_to(cr, x, y + i);
            cairo_line_to(cr, x + width, y + i);
        }
    }
    cairo_stroke(cr);

    // Draw scrollbar handle
    cairo_set_source_rgb(cr, LIGHT_BROWN);
    if (is_horizontal) {
        cairo_rectangle(cr, x + handle_pos, y, handle_size, height);
    } else {
        cairo_rectangle(cr, x, y + handle_pos, width, handle_size);
    }
    cairo_fill(cr);

    // Add handle decoration
    cairo_set_source_rgb(cr, DARK_BROWN);
    if (is_horizontal) {
        cairo_rectangle(cr, x + handle_pos, y, handle_size, height);
        cairo_set_line_width(cr, 1);
        cairo_stroke(cr);

        // Add handle grip
        int grip_width = handle_size / 3;
        cairo_move_to(cr, x + handle_pos + grip_width, y + 2);
        cairo_line_to(cr, x + handle_pos + grip_width, y + height - 2);
        cairo_move_to(cr, x + handle_pos + grip_width*2, y + 2);
        cairo_line_to(cr, x + handle_pos + grip_width*2, y + height - 2);
    } else {
        cairo_rectangle(cr, x, y + handle_pos, width, handle_size);
        cairo_set_line_width(cr, 1);
        cairo_stroke(cr);

        // Add handle grip
        int grip_height = handle_size / 3;
        cairo_move_to(cr, x + 2, y + handle_pos + grip_height);
        cairo_line_to(cr, x + width - 2, y + handle_pos + grip_height);
        cairo_move_to(cr, x + 2, y + handle_pos + grip_height*2);
        cairo_line_to(cr, x + width - 2, y + handle_pos + grip_height*2);
    }
    cairo_stroke(cr);
}

void draw_scrollbars(cairo_t *cr, int content_width, int content_height) {
    int scrollbar_height = 15;
    int scrollbar_width = 15;
    int view_width = WIDTH - 60; // Adjusted for margins
    int view_height = 400; // Visible area height

    // Calculate scrollbar handle sizes and positions
    double v_ratio = (double)view_height / content_height;
    if (v_ratio > 1.0) v_ratio = 1.0;
    int v_handle_size = v_ratio * (HEIGHT - 50 - scrollbar_height);
    int v_handle_pos = (scroll_y / (double)(content_height - view_height)) *
                        (HEIGHT - 50 - scrollbar_height - v_handle_size);

    double h_ratio = (double)view_width / content_width;
    if (h_ratio > 1.0) h_ratio = 1.0;
    int h_handle_size = h_ratio * (WIDTH - 50 - scrollbar_width);
    int h_handle_pos = (scroll_x / (double)(content_width - view_width)) *
                        (WIDTH - 50 - scrollbar_width - h_handle_size);

    // Draw medieval-style scrollbars
    draw_medieval_scrollbar(cr, WIDTH - scrollbar_width, 50, scrollbar_width,
                         HEIGHT - 50 - scrollbar_height, v_handle_pos, v_handle_size, false);

    draw_medieval_scrollbar(cr, 50, HEIGHT - scrollbar_height,
                         WIDTH - 50 - scrollbar_width, scrollbar_height, h_handle_pos, h_handle_size, true);
}

bool is_in_rect(int x, int y, int rx, int ry, int rw, int rh) {
    return x >= rx && x <= rx + rw && y >= ry && y <= ry + rh;
}


void
side_menu_init() {
    // Initialize Cairo context
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Medieval Game Statistics",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Surface *surface = SDL_GetWindowSurface(window);

    cairo_surface = cairo_image_surface_create_for_data(
        (unsigned char *)surface->pixels, CAIRO_FORMAT_RGB24,
        surface->w, surface->h, surface->pitch);
    cr = cairo_create(cairo_surface);
    init_data();

    // Initialize button
    initialize_button();
}

void side_menu_render() {


    int quit = 0;
    SDL_Event e;
    int mouse_x = 0, mouse_y = 0;
    int drag_start_x = 0, drag_start_y = 0;
    int drag_start_scroll_x = 0, drag_start_scroll_y = 0;


        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_MOUSEWHEEL) {
                if (SDL_GetModState() & KMOD_SHIFT) {
                    // Horizontal scrolling when Shift is held
                    scroll_x += e.wheel.y * 30;
                } else {
                    // Vertical scrolling
                    scroll_y -= e.wheel.y * 30;
                }

                // Clamp scrolling
                if (scroll_y < 0) scroll_y = 0;
                if (scroll_x < 0) scroll_x = 0;

                int max_scroll_y = dimension * CELL_H - 300;
                int max_scroll_x = dimension * CELL_W - (WIDTH - 100);

                if (max_scroll_y < 0) max_scroll_y = 0;
                if (max_scroll_x < 0) max_scroll_x = 0;

                if (scroll_y > max_scroll_y) scroll_y = max_scroll_y;
                if (scroll_x > max_scroll_x) scroll_x = max_scroll_x;
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                mouse_x = e.button.x;
                mouse_y = e.button.y;

                // Check button click
                if (button && is_button_hovered(button, mouse_x, mouse_y)) {
                    button->pressed = true;
                }

                // Check if clicked on vertical scrollbar
                if (is_in_rect(mouse_x, mouse_y, WIDTH - 15, 50, 15, HEIGHT - 50 - 15)) {
                    dragging_v_scrollbar = true;
                    drag_start_y = mouse_y;
                    drag_start_scroll_y = scroll_y;
                }

                // Check if clicked on horizontal scrollbar
                if (is_in_rect(mouse_x, mouse_y, 50, HEIGHT - 15, WIDTH - 50 - 15, 15)) {
                    dragging_h_scrollbar = true;
                    drag_start_x = mouse_x;
                    drag_start_scroll_x = scroll_x;
                }
            } else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
    // Check if button was clicked and released
    if (button && button->pressed) {
        button->pressed = false;
        if (is_button_hovered(button, e.button.x, e.button.y)) {
            // Button was clicked - execute callback if it exists
            if (button->callback) {
                button->callback();
            }
        }
    }

    // Your existing scrollbar release logic
    dragging_v_scrollbar = false;
    dragging_h_scrollbar = false;
} else if (e.type == SDL_MOUSEMOTION) {
                mouse_x = e.motion.x;
                mouse_y = e.motion.y;

                // Update button hover state
                if (button) {
                    button->hovered = is_button_hovered(button, mouse_x, mouse_y);
                }

                if (dragging_v_scrollbar) {
                    int delta_y = mouse_y - drag_start_y;
                    int scrollbar_track_height = HEIGHT - 50 - 15;
                    int content_height = dimension * CELL_H;
                    int view_height = 300;

                    scroll_y = drag_start_scroll_y + delta_y * (content_height - view_height) / scrollbar_track_height;

                    // Clamp scrolling
                    if (scroll_y < 0) scroll_y = 0;
                    int max_scroll_y = content_height - view_height;
                    if (max_scroll_y < 0) max_scroll_y = 0;
                    if (scroll_y > max_scroll_y) scroll_y = max_scroll_y;
                }

                if (dragging_h_scrollbar) {
                    int delta_x = mouse_x - drag_start_x;
                    int scrollbar_track_width = WIDTH - 50 - 15;
                    int content_width = dimension * CELL_W;
                    int view_width = WIDTH - 100;

                    scroll_x = drag_start_scroll_x + delta_x * (content_width - view_width) / scrollbar_track_width;

                    // Clamp scrolling
                    if (scroll_x < 0) scroll_x = 0;
                    int max_scroll_x = content_width - view_width;
                    if (max_scroll_x < 0) max_scroll_x = 0;
                    if (scroll_x > max_scroll_x) scroll_x = max_scroll_x;
                }
            }
        }


        // Clear background with parchment texture
        cairo_set_source_rgb(cr, PARCHMENT_COLOR);
        cairo_paint(cr);

        // Add parchment texture effect
        cairo_set_source_rgba(cr, DARK_BROWN, 0.03);
        for (int i = 0; i < WIDTH; i += 10) {
            for (int j = 0; j < HEIGHT; j += 10) {
                if ((i + j) % 20 == 0) {
                    cairo_rectangle(cr, i, j, 5, 5);
                }
            }
        }
        cairo_fill(cr);

        // Draw medieval border around the whole surface
        draw_medieval_border(cr, 10, 10, WIDTH - 20, HEIGHT - 20);

        // Draw all UI components with medieval style
        // Adjust y_offsets with more spacing between sections
        int y_offset = 40;
        draw_legends(cr, 50, y_offset);
        y_offset += 70; // Increased from 60

        draw_scores(cr, 50, y_offset);
        y_offset += 70; // Increased from 60

        draw_payoff_matrix(cr, 50, y_offset, scroll_x, scroll_y);
        y_offset += 340; // Increased from 320 to provide more space for the next title

        // Define colors for hider and seeker
        double hider_color[3] = {0.7, 0.1, 0.1}; // Red
        double seeker_color[3] = {0.1, 0.1, 0.6}; // Blue

        draw_probability_bars(cr, 50, y_offset, scroll_x, mouse_x, mouse_y, hider_probabilities, "Hider Probabilities", hider_color);
        y_offset += 150; // Increased from 140

        // Make sure there's enough space for the seeker probabilities
        if (y_offset + 140 > HEIGHT - 30) {
            y_offset = HEIGHT - 170; // Adjusted to ensure it fits on screen with more margin
        }

        draw_probability_bars(cr, 50, y_offset, scroll_x, mouse_x, mouse_y, seeker_probabilities, "Seeker Probabilities", seeker_color);

        // Draw scrollbars
        draw_scrollbars(cr, dimension * CELL_W, dimension * CELL_H);

        // Draw the custom action button
        if (button) {
            draw_medieval_button(cr, button);
        }

        SDL_UpdateWindowSurface(window);
        SDL_Delay(16);
    }





void draw_medieval_button(cairo_t *cr, MedievalButton* btn) {
    if (!btn) return;

    // Background for button - darker if pressed
    if (btn->pressed) {
        cairo_set_source_rgb(cr, DARK_BROWN);
    } else if (btn->hovered) {
        cairo_set_source_rgb(cr, MID_BROWN);
    } else {
        cairo_set_source_rgb(cr, MID_BROWN);
    }
    cairo_rectangle(cr, btn->x, btn->y, btn->width, btn->height);
    cairo_fill(cr);

    // Border
    cairo_set_source_rgb(cr, DARK_BROWN);
    cairo_rectangle(cr, btn->x, btn->y, btn->width, btn->height);
    cairo_set_line_width(cr, 2);
    cairo_stroke(cr);

    // Corner decorations
    int corner_size = 8;

    // Top-left
    cairo_move_to(cr, btn->x + corner_size, btn->y + 2);
    cairo_line_to(cr, btn->x + 2, btn->y + 2);
    cairo_line_to(cr, btn->x + 2, btn->y + corner_size);
    cairo_set_line_width(cr, 2);
    cairo_stroke(cr);

    // Top-right
    cairo_move_to(cr, btn->x + btn->width - corner_size, btn->y + 2);
    cairo_line_to(cr, btn->x + btn->width - 2, btn->y + 2);
    cairo_line_to(cr, btn->x + btn->width - 2, btn->y + corner_size);
    cairo_stroke(cr);

    // Bottom-left
    cairo_move_to(cr, btn->x + 2, btn->y + btn->height - corner_size);
    cairo_line_to(cr, btn->x + 2, btn->y + btn->height - 2);
    cairo_line_to(cr, btn->x + corner_size, btn->y + btn->height - 2);
    cairo_stroke(cr);

    // Bottom-right
    cairo_move_to(cr, btn->x + btn->width - 2, btn->y + btn->height - corner_size);
    cairo_line_to(cr, btn->x + btn->width - 2, btn->y + btn->height - 2);
    cairo_line_to(cr, btn->x + btn->width - corner_size, btn->y + btn->height - 2);
    cairo_stroke(cr);

    // Label text - slight offset if pressed for a "click" effect
    int text_y_offset = btn->pressed ? 2 : 0;
    int text_x_offset = btn->pressed ? 1 : 0;

    cairo_select_font_face(cr, "Serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 16);
    cairo_set_source_rgb(cr, PARCHMENT_COLOR);

    // Center the text
    cairo_text_extents_t extents;
    cairo_text_extents(cr, btn->label, &extents);
    double x_centered = btn->x + (btn->width - extents.width) / 2 + text_x_offset;
    double y_centered = btn->y + (btn->height + extents.height) / 2 + text_y_offset;

    cairo_move_to(cr, x_centered, y_centered);
    cairo_show_text(cr, btn->label);
}

bool is_button_hovered(MedievalButton* btn, int mouse_x, int mouse_y) {
    if (!btn) return false;
    return mouse_x >= btn->x && mouse_x <= btn->x + btn->width &&
           mouse_y >= btn->y && mouse_y <= btn->y + btn->height;
}

void button_set_custom_function(ButtonCallback func) {
    custom_function = func;
}

void initialize_button() {
    if (!button) {
        button = (MedievalButton*)malloc(sizeof(MedievalButton));
        if (!button) return;

        // Position in bottom right with some margin
        button->x = WIDTH - 150 - 30;  // 30px margin from right
        button->y = HEIGHT - 60 - 30;  // 30px margin from bottom
        button->width = 150;
        button->height = 60;
        button->label = "Menu / Reset";
        button->callback = custom_function;
        button->hovered = false;
        button->pressed = false;
        // Set the callback
        side_menu_set_button_callback(my_custom_function);
    }
}

void side_menu_set_button_callback(void (*callback_function)(void)) {
    button_set_custom_function(callback_function);
    if (button) {
        button->callback = callback_function;
    }
}

// Example custom function
void my_custom_function()
{
    game_engine_set_play_menu(true);
    game_engine_set_run_status(RUN_STATUS_QUIT);
}



void side_menu_destroy() {
    // Free the button
    if (button) {
        free(button);
        button = NULL;
    }

    // Free Cairo resources (only if they exist)
    if (cr) {
        cairo_destroy(cr);
        cr = NULL;
    }

    if (cairo_surface) {
        cairo_surface_destroy(cairo_surface);
        cairo_surface = NULL;
    }

    // Destroy the window
    if (window) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
}
