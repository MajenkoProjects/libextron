#include <extron.h>
#include <unistd.h>
#include <stdio.h>

#define NCURSES_WIDECHAR 1
#include <ncurses.h>

#define BOX_SINGLE 0,0,0,0,0,0,0,0
#define BOX_DOUBLE WACS_D_VLINE, WACS_D_VLINE, WACS_D_HLINE, WACS_D_HLINE, WACS_D_ULCORNER, WACS_D_URCORNER, WACS_D_LLCORNER, WACS_D_LRCORNER
#define BOX_NONE ' ',' ',' ',' ',' ',' ',' ',' '

struct extron {
	int pan_x;
	int pan_y;
	int start_x;
	int start_y;
	int phase;
	int total_pixels;
	int active_pixels;
	int active_lines;
	int size_x;
	int size_y;
	int zoom_x;
	int zoom_y;
};

void update_settings(int fd, struct extron *settings) {
	settings->pan_x = extron_get_horizontal_shift(fd);
	settings->pan_y = extron_get_vertical_shift(fd);
	settings->start_x = extron_get_horizontal_start(fd);
	settings->start_y = extron_get_vertical_start(fd);
	settings->phase = extron_get_phase(fd);
	settings->total_pixels = extron_get_total_pixels(fd);
	settings->active_pixels = extron_get_active_pixels(fd);
	settings->active_lines = extron_get_active_lines(fd);
	settings->size_x = extron_get_horizontal_size(fd);
	settings->size_y = extron_get_vertical_size(fd);
	extron_get_zoom(fd, &settings->zoom_x, &settings->zoom_y);
}

WINDOW *init_scaling() {
	WINDOW *scaling = newwin(12, 28, 2, 2);
	wbkgd(scaling, COLOR_PAIR(1));
	wborder_set(scaling, BOX_SINGLE);
	mvwaddstr(scaling, 0, 1, "[ Up-scaling ]");
	mvwaddstr(scaling, 3, 2, "P: Pan");
	mvwaddstr(scaling, 4, 2, "S: Start");
	mvwaddstr(scaling, 5, 2, "H: Phase");
	mvwaddstr(scaling, 6, 2, "T: Total");
	mvwaddstr(scaling, 7, 2, "A: Active");
	mvwaddstr(scaling, 8, 2, "I: Size");
	mvwaddstr(scaling, 9, 2, "Z: Zoom");

	return scaling;
}

void update_scaling(WINDOW *win, struct extron *settings, int mode) {
	char temp[20];

	
	snprintf(temp, 20, "%5d x %5d", settings->pan_x, settings->pan_y);
	wcolor_set(win, mode == 'p' ? 2 : 1, NULL);
	mvwaddstr(win, 3, 12, temp);

	snprintf(temp, 20, "%5d x %5d", settings->start_x, settings->start_y);
	wcolor_set(win, mode == 's' ? 2 : 1, NULL);
	mvwaddstr(win, 4, 12, temp);

	snprintf(temp, 20, "%5d", settings->phase);
	wcolor_set(win, mode == 'h' ? 2 : 1, NULL);
	mvwaddstr(win, 5, 20, temp);

	snprintf(temp, 20, "%5d", settings->total_pixels);
	wcolor_set(win, mode == 't' ? 2 : 1, NULL);
	mvwaddstr(win, 6, 20, temp);

	snprintf(temp, 20, "%5d x %5d", settings->active_pixels, settings->active_lines);
	wcolor_set(win, mode == 'a' ? 2 : 1, NULL);
	mvwaddstr(win, 7, 12, temp);

	snprintf(temp, 20, "%5d x %5d", settings->size_x, settings->size_y);
	wcolor_set(win, mode == 'i' ? 2 : 1, NULL);
	mvwaddstr(win, 8, 12, temp);

	snprintf(temp, 20, "%5d x %5d", settings->zoom_x, settings->zoom_y);
	wcolor_set(win, mode == 'z' ? 2 : 1, NULL);
	mvwaddstr(win, 9, 12, temp);

	wrefresh(win);

}

void main() {
	int rval;

	int fd = extron_connect("/dev/ttyS0");

	struct extron settings;

	update_settings(fd, &settings);

	initscr();
	start_color();

	raw();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);

	refresh();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);

	mvaddstr(0, 0, "F1: Auto-align | F10: Quit");

	WINDOW *scaling = init_scaling();

	int mode = 0;

	WINDOW *popup;

	while (1) {

		update_scaling(scaling, &settings, mode);

		int c = getch();

		switch (c) {
			case 'p':
			case 's':
			case 'h':
			case 't':
			case 'a':
			case 'i':
			case 'z':
				mode = c;
				break;

			case KEY_UP:
				switch (mode) {
					case 'p': rval = extron_dec_vertical_shift(fd); if (rval > -1) settings.pan_y = rval; break;
					case 's': rval = extron_dec_vertical_start(fd); if (rval > -1) settings.start_y = rval; break;
					case 'h': rval = extron_inc_phase(fd); if (rval > -1) settings.phase = rval; break;
					case 't': rval = extron_inc_total_pixels(fd); if (rval > -1) settings.total_pixels = rval; break;
					case 'a': rval = extron_inc_active_lines(fd); if (rval > -1) settings.active_lines = rval; break;
					case 'i': rval = extron_inc_vertical_size(fd); if (rval > -1) settings.size_y = rval; break;
					case 'z': extron_zoom_in(fd, &settings.zoom_x, &settings.zoom_y); break;
				}
				break;

			case KEY_DOWN:
				switch (mode) {
					case 'p': rval = extron_inc_vertical_shift(fd); if (rval > -1) settings.pan_y = rval; break;
					case 's': rval = extron_inc_vertical_start(fd); if (rval > -1) settings.start_y = rval; break;
					case 'h': rval = extron_dec_phase(fd); if (rval > -1) settings.phase = rval; break;
					case 't': rval = extron_dec_total_pixels(fd); if (rval > -1) settings.total_pixels = rval; break;
					case 'a': rval = extron_dec_active_lines(fd); if (rval > -1) settings.active_lines = rval; break;
					case 'i': rval = extron_dec_vertical_size(fd); if (rval > -1) settings.size_y = rval; break;
					case 'z': extron_zoom_out(fd, &settings.zoom_x, &settings.zoom_y); break;
				}
				break;

			case KEY_LEFT:
				switch (mode) {
					case 'p': rval = extron_dec_horizontal_shift(fd); if (rval > -1) settings.pan_x = rval; break;
					case 's': rval = extron_dec_horizontal_start(fd); if (rval > -1) settings.start_x = rval; break;
					case 'h': rval = extron_dec_phase(fd); if (rval > -1) settings.phase = rval; break;
					case 't': rval = extron_dec_total_pixels(fd); if (rval > -1) settings.total_pixels = rval; break;
					case 'a': rval = extron_dec_active_pixels(fd); if (rval > -1) settings.active_pixels = rval; break;
					case 'i': rval = extron_dec_horizontal_size(fd); if (rval > -1) settings.size_x = rval; break;
					case 'z': extron_zoom_out(fd, &settings.zoom_x, &settings.zoom_y); break;
				}
				break;

			case KEY_RIGHT:
				switch (mode) {
					case 'p': rval = extron_inc_horizontal_shift(fd); if (rval > -1) settings.pan_x = rval; break;
					case 's': rval = extron_inc_horizontal_start(fd); if (rval > -1) settings.start_x = rval; break;
					case 'h': rval = extron_inc_phase(fd); if (rval > -1) settings.phase = rval; break;
					case 't': rval = extron_inc_total_pixels(fd); if (rval > -1) settings.total_pixels = rval; break;
					case 'a': rval = extron_inc_active_pixels(fd); if (rval > -1) settings.active_pixels = rval; break;
					case 'i': rval = extron_inc_horizontal_size(fd); if (rval > -1) settings.size_x = rval; break;
					case 'z': extron_zoom_in(fd, &settings.zoom_x, &settings.zoom_y); break;
				}
				break;

			case KEY_F(1):
				popup = newwin(5, 30, 9, 25);
				wborder_set(popup, BOX_SINGLE);
				mvwaddstr(popup, 2, 9, "Auto-aligning");
				wrefresh(popup);

				extron_do_auto_image(fd);
				update_settings(fd, &settings);

				wclear(popup);
				wborder(popup, BOX_NONE);
				wrefresh(popup);
				delwin(popup);
				touchwin(scaling);
				break;

			case KEY_F(10):
				endwin();
				extron_disconnect(fd);
				return;
		}
	}

	echo();
	cbreak();
	endwin();


}
