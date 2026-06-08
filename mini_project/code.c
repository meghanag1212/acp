#include <stdio.h>
#include <stdlib.h>

#define WIDTH 60
#define HEIGHT 20

#define MY_ABS(x) ((x) < 0 ? -(x) : (x))

char canvas[HEIGHT][WIDTH];

/* Helper to clear stdin buffer to prevent infinite loops on malformed inputs */
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Helper to read an integer with a prompt */
int read_int(const char* prompt) {
    int val;
    printf("%s", prompt);
    while (scanf("%d", &val) != 1) {
        printf("Invalid input. Please enter an integer: ");
        clear_input_buffer();
    }
    clear_input_buffer();
    return val;
}

/* Initialize the canvas with the background character '_' */
void clear_canvas() {
    int y, x;
    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            canvas[y][x] = '_';
        }
    }
}

/* Display the canvas in the terminal */
void display_canvas() {
    int y, x;
    printf("\n+");
    for (x = 0; x < WIDTH; x++) {
        printf("-");
    }
    printf("+\n");

    for (y = 0; y < HEIGHT; y++) {
        printf("|");
        for (x = 0; x < WIDTH; x++) {
            putchar(canvas[y][x]);
        }
        printf("|\n");
    }

    printf("+");
    for (x = 0; x < WIDTH; x++) {
        printf("-");
    }
    printf("+\n\n");
}

/* Draw a line using Bresenham's Line Algorithm */
void draw_line(int x1, int y1, int x2, int y2, char ch) {
    int dx = MY_ABS(x2 - x1);
    int sx = x1 < x2 ? 1 : -1;
    int dy = -MY_ABS(y2 - y1);
    int sy = y1 < y2 ? 1 : -1;
    int err = dx + dy;
    int e2;

    while (1) {
        if (x1 >= 0 && x1 < WIDTH && y1 >= 0 && y1 < HEIGHT) {
            canvas[y1][x1] = ch;
        }
        if (x1 == x2 && y1 == y2) {
            break;
        }
        e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y1 += sy;
        }
    }
}

/* Draw a rectangle by drawing four lines */
void draw_rectangle(int x, int y, int w, int h, char ch) {
    if (w <= 0 || h <= 0) return;
    draw_line(x, y, x + w - 1, y, ch);
    draw_line(x, y + h - 1, x + w - 1, y + h - 1, ch);
    draw_line(x, y, x, y + h - 1, ch);
    draw_line(x + w - 1, y, x + w - 1, y + h - 1, ch);
}

/* Draw a circle using Midpoint Circle Algorithm */
void draw_circle(int cx, int cy, int r, char ch) {
    int x, y, p, i;
    if (r < 0) return;
    x = r;
    y = 0;
    p = 1 - r;

    while (x >= y) {
        int points[8][2];
        points[0][0] = cx + x; points[0][1] = cy + y;
        points[1][0] = cx + y; points[1][1] = cy + x;
        points[2][0] = cx - y; points[2][1] = cy + x;
        points[3][0] = cx - x; points[3][1] = cy + y;
        points[4][0] = cx - x; points[4][1] = cy - y;
        points[5][0] = cx - y; points[5][1] = cy - x;
        points[6][0] = cx + y; points[6][1] = cy - x;
        points[7][0] = cx + x; points[7][1] = cy - y;

        for (i = 0; i < 8; i++) {
            int px = points[i][0];
            int py = points[i][1];
            if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT) {
                canvas[py][px] = ch;
            }
        }

        y++;
        if (p < 0) {
            p += 2 * y + 1;
        } else {
            x--;
            p += 2 * (y - x) + 1;
        }
    }
}

/* Draw a triangle by drawing lines between three vertices */
void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, char ch) {
    draw_line(x1, y1, x2, y2, ch);
    draw_line(x2, y2, x3, y3, ch);
    draw_line(x3, y3, x1, y1, ch);
}

int main() {
    int choice;
    int x1, y1, x2, y2, x3, y3, w, h, r;

    clear_canvas();
    printf("=== Interactive 2D Graphics Editor (Direct Mode) ===\n");
    printf("Canvas size is %d x %d. Directly editing pixels.\n", WIDTH, HEIGHT);

    while (1) {
        printf("\n--- Direct Canvas Menu ---\n");
        printf("1. Draw Line (*)\n");
        printf("2. Draw Rectangle (*)\n");
        printf("3. Draw Circle (*)\n");
        printf("4. Draw Triangle (*)\n");
        printf("5. Erase Line (_)\n");
        printf("6. Erase Rectangle (_)\n");
        printf("7. Erase Circle (_)\n");
        printf("8. Erase Triangle (_)\n");
        printf("9. Display Canvas\n");
        printf("10. Clear Canvas\n");
        printf("11. Exit\n");

        choice = read_int("Enter selection (1-11): ");
        switch (choice) {
            case 1:
                printf("Enter Line Coordinates:\n");
                x1 = read_int("  x1: ");
                y1 = read_int("  y1: ");
                x2 = read_int("  x2: ");
                y2 = read_int("  y2: ");
                draw_line(x1, y1, x2, y2, '*');
                printf("Line drawn!\n");
                break;
            case 2:
                printf("Enter Rectangle Coordinates:\n");
                x1 = read_int("  x (top-left): ");
                y1 = read_int("  y (top-left): ");
                w = read_int("  width: ");
                h = read_int("  height: ");
                draw_rectangle(x1, y1, w, h, '*');
                printf("Rectangle drawn!\n");
                break;
            case 3:
                printf("Enter Circle Coordinates:\n");
                x1 = read_int("  cx (center x): ");
                y1 = read_int("  cy (center y): ");
                r = read_int("  radius: ");
                draw_circle(x1, y1, r, '*');
                printf("Circle drawn!\n");
                break;
            case 4:
                printf("Enter Triangle Coordinates:\n");
                x1 = read_int("  x1: ");
                y1 = read_int("  y1: ");
                x2 = read_int("  x2: ");
                y2 = read_int("  y2: ");
                x3 = read_int("  x3: ");
                y3 = read_int("  y3: ");
                draw_triangle(x1, y1, x2, y2, x3, y3, '*');
                printf("Triangle drawn!\n");
                break;
            case 5:
                printf("Enter Line Coordinates to Erase:\n");
                x1 = read_int("  x1: ");
                y1 = read_int("  y1: ");
                x2 = read_int("  x2: ");
                y2 = read_int("  y2: ");
                draw_line(x1, y1, x2, y2, '_');
                printf("Line erased!\n");
                break;
            case 6:
                printf("Enter Rectangle Coordinates to Erase:\n");
                x1 = read_int("  x (top-left): ");
                y1 = read_int("  y (top-left): ");
                w = read_int("  width: ");
                h = read_int("  height: ");
                draw_rectangle(x1, y1, w, h, '_');
                printf("Rectangle erased!\n");
                break;
            case 7:
                printf("Enter Circle Coordinates to Erase:\n");
                x1 = read_int("  cx (center x): ");
                y1 = read_int("  cy (center y): ");
                r = read_int("  radius: ");
                draw_circle(x1, y1, r, '_');
                printf("Circle erased!\n");
                break;
            case 8:
                printf("Enter Triangle Coordinates to Erase:\n");
                x1 = read_int("  x1: ");
                y1 = read_int("  y1: ");
                x2 = read_int("  x2: ");
                y2 = read_int("  y2: ");
                x3 = read_int("  x3: ");
                y3 = read_int("  y3: ");
                draw_triangle(x1, y1, x2, y2, x3, y3, '_');
                printf("Triangle erased!\n");
                break;
            case 9:
                display_canvas();
                break;
            case 10:
                clear_canvas();
                printf("Canvas cleared!\n");
                break;
            case 11:
                printf("Exiting. Goodbye!\n");
                return 0;
            default:
                printf("Invalid selection.\n");
        }
    }
    return 0;
}
