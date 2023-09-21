#include <extron.h>
#include <unistd.h>
#include <stdio.h>

#define NCURSES_WIDECHAR 1
#include <ncurses.h>

#define BOX_SINGLE 0,0,0,0,0,0,0,0
#define BOX_DOUBLE WACS_D_VLINE, WACS_D_VLINE, WACS_D_HLINE, WACS_D_HLINE, WACS_D_ULCORNER, WACS_D_URCORNER, WACS_D_LLCORNER, WACS_D_LRCORNER
#define BOX_NONE ' ',' ',' ',' ',' ',' ',' ',' '

/* Allocated keys

p:	Pan
s:	Start
h:	Phase
t:	Total
a:	Active
i:	Size
z:	Zoom
b:	Brightness
c:	Contrast
d:	Detail
r:  Resolution
f:  Refresh
*/

WINDOW *debugwindow;


const char *resolutions[] = {
	NULL,
	"640x480",   // 1
	"800x600",   // 2
	"852x480",   // 3
	"1024x768",  // 4
	"1024x852",  // 5
	"1024x1024", // 6
	"1280x768",  // 7
	"1280x800",  // 8
	"1280x1024", // 9
	"1360x765",  // 10
	"1360x768",  // 11
	"1365x768",  // 12
	"1366x768",  // 13
	"1365x1024", // 14
	"1440x900",  // 15
	"1440x1050", // 16
	"1680x1050", // 17
	"1600x1200", // 18
	"1920x1200", // 19
	"480p",      // 20
	"576p",      // 21
	"720p",      // 22
	"1080i",     // 23
	"1080p",     // 24
	"2048x1080", // 25
	"1600x900",  // 26
	NULL,
	NULL,
	NULL,
	"AUTO",      // 30
	"LOCK",      // 31
	"OUTPUT",    // 32
};

const char *refreshes[] = {
	"Auto",
	"23.98Hz",
	"24Hz",
	"25Hz",
	"29.97Hz",
	"30Hz",
	"50Hz",
	"59.94Hz",
	"60Hz",
	"75Hz"
};

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

	int brightness;
	int contrast;
	int detail;
	int resolution;
	int refresh;
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
	settings->brightness = extron_get_brightness(fd);
	settings->contrast = extron_get_contrast(fd);
	settings->detail = extron_get_detail_filter(fd);
	extron_get_scaler_rate(fd, &settings->resolution, &settings->refresh);
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

WINDOW *init_image() {
	WINDOW *image = newwin(12, 28, 2, 30);

	wbkgd(image, COLOR_PAIR(1));
	wborder_set(image, BOX_SINGLE);

	mvwaddstr(image, 0, 1, "[ Image ]");
	mvwaddstr(image, 3, 2, "B: Brightness");
	mvwaddstr(image, 4, 2, "C: Contrast");
	mvwaddstr(image, 5, 2, "D: Detail");
	mvwaddstr(image, 6, 2, "R: Resolution");
	mvwaddstr(image, 7, 2, "F: Refresh");

	return image;
}

void debug(const char *str) {
	waddstr(debugwindow, str);
	waddstr(debugwindow, "\r\n");
	wrefresh(debugwindow);
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

void update_image(WINDOW *win, struct extron *settings, int mode) {
	char temp[20];

	snprintf(temp, 20, "%5d", settings->brightness);
	wcolor_set(win, mode == 'b' ? 2 : 1, NULL);
	mvwaddstr(win, 3, 20, temp);

	snprintf(temp, 20, "%5d", settings->contrast);
	wcolor_set(win, mode == 'c' ? 2 : 1, NULL);
	mvwaddstr(win, 4, 20, temp);

	snprintf(temp, 20, "%5d", settings->detail);
	wcolor_set(win, mode == 'd' ? 2 : 1, NULL);
	mvwaddstr(win, 5, 20, temp);

	snprintf(temp, 20, "%10s", resolutions[settings->resolution]);
	wcolor_set(win, mode == 'r' ? 2 : 1, NULL);
	mvwaddstr(win, 6, 15, temp);

	snprintf(temp, 20, "%10s", refreshes[settings->refresh]);
	wcolor_set(win, mode == 'f' ? 2 : 1, NULL);
	mvwaddstr(win, 7, 15, temp);

	wrefresh(win);
}

void inc_resolution(int fd, struct extron *settings) {
	if (settings->resolution >= 32) {
		return;
	}
	settings->resolution++;
	if (settings->resolution == 27) settings->resolution = 30;
	if (settings->resolution == 28) settings->resolution = 30;
	if (settings->resolution == 29) settings->resolution = 30;
	extron_set_scaler_rate(fd, settings->resolution, settings->refresh);
}

void dec_resolution(int fd, struct extron *settings) {
	if (settings->resolution <= 1) {
		return;
	}
	settings->resolution--;
	if (settings->resolution == 27) settings->resolution = 26;
	if (settings->resolution == 28) settings->resolution = 26;
	if (settings->resolution == 29) settings->resolution = 26;
	extron_set_scaler_rate(fd, settings->resolution, settings->refresh);
}

void inc_refresh(int fd, struct extron *settings) {
	if (settings->refresh >= 9) {
		return;
	}
	settings->refresh++;
	extron_set_scaler_rate(fd, settings->resolution, settings->refresh);
}

void dec_refresh(int fd, struct extron *settings) {
	if (settings->refresh <= 0) {
		return;
	}
	settings->refresh--;
	extron_set_scaler_rate(fd, settings->resolution, settings->refresh);
}


void main() {
	int rval;

	extron_debug(debug);


	initscr();
	start_color();

	raw();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);


	refresh();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);

	debugwindow = newwin(5, 80, 20, 0);
	idlok(debugwindow, 1);
	scrollok(debugwindow, 1);
	mvwaddstr(debugwindow, 0, 0, "Debug\r\n");

	mvaddstr(0, 0, "F1: Auto-align | F5: Refresh | F10: Quit");

	int fd = extron_connect("/dev/ttyS0");

	struct extron settings;

	update_settings(fd, &settings);

	WINDOW *scaling = init_scaling();
	WINDOW *image = init_image();

	int mode = 0;

	WINDOW *popup;

	while (1) {

		update_scaling(scaling, &settings, mode);
		update_image(image, &settings, mode);

		int c = getch();

		switch (c) {
			case 'p':
			case 's':
			case 'h':
			case 't':
			case 'a':
			case 'i':
			case 'z':
			case 'b':
			case 'c':
			case 'd':
			case 'r':
			case 'f':
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
					case 'b': rval = extron_inc_brightness(fd); if (rval > -1) settings.brightness = rval; break;
					case 'c': rval = extron_inc_contrast(fd); if (rval > -1) settings.contrast = rval; break;
					case 'd': rval = extron_inc_detail_filter(fd); if (rval > -1) settings.detail = rval; break;
					case 'r': inc_resolution(fd, &settings); break;
					case 'f': inc_refresh(fd, &settings); break;
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
					case 'b': rval = extron_dec_brightness(fd); if (rval > -1) settings.brightness = rval; break;
					case 'c': rval = extron_dec_contrast(fd); if (rval > -1) settings.contrast = rval; break;
					case 'd': rval = extron_dec_detail_filter(fd); if (rval > -1) settings.detail = rval; break;
					case 'r': dec_resolution(fd, &settings); break;
					case 'f': dec_refresh(fd, &settings); break;
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
					case 'r': dec_resolution(fd, &settings); break;
					case 'f': dec_refresh(fd, &settings); break;
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
					case 'r': inc_resolution(fd, &settings);
					case 'f': inc_refresh(fd, &settings);
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
				touchwin(image);
				break;

			case KEY_F(5):
				write(fd, "\r\r\r\r\r", 5);
				extron_disconnect(fd);
				sleep(1);
    			fd = extron_connect("/dev/ttyS0");
				update_settings(fd, &settings);
				touchwin(scaling);
				touchwin(image);
				wrefresh(scaling);
				wrefresh(image);
				break;
				

			case KEY_F(10):
				extron_disconnect(fd);
				echo();
				cbreak();
				endwin();
				return;
		}
	}

	echo();
	cbreak();
	endwin();


}
