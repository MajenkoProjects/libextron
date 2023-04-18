#ifndef _EXTRON_H
#define _EXTRON_H

// X1
#define EXTRON_VIDEO_FORMAT_NONE	0
#define EXTRON_VIDEO_FORMAT_RGB		1
#define EXTRON_VIDEO_FORMAT_YUV		2
#define EXTRON_VIDEO_FORMAT_AUTO	3

// X2
#define EXTRON_DISABLED			0
#define EXTRON_ENABLED			1

// X15
#define EXTRON_RESOLUTIOM_640x480	1
#define EXTRON_RESOLUTION_800x600	2
#define EXTRON_RESOLUTION_852x480	3
#define EXTRON_RESOLUTION_1024x768	4
#define EXTRON_RESOLUTION_1024x852	5
#define EXTRON_RESOLUTION_1024x1024	6
#define EXTRON_RESOLUTION_1280x768	7
#define EXTRON_RESOLUTION_1280x800	8
#define EXTRON_RESOLUTION_1280x1024	9
#define EXTRON_RESOLUTION_1360x765	10
#define EXTRON_RESOLUTION_1360x768	11
#define EXTRON_RESOLUTION_1365x768	12
#define EXTRON_RESOLUTION_1366x768	13
#define EXTRON_RESOLUTION_1365x1024	14
#define EXTRON_RESOLUTION_1440x900	15
#define EXTRON_RESOLUTION_1440x1050	16
#define EXTRON_RESOLUTION_1680x1050	17
#define EXTRON_RESOLUTION_1600x1200	18
#define EXTRON_RESOLUTION_1920x1200	19
#define EXTRON_RESOLUTION_480p		20
#define EXTRON_RESOLUTION_576p		21
#define EXTRON_RESOLUTION_720p		22
#define EXTRON_RESOLUTION_1080i		23
#define EXTRON_RESOLUTION_1080p		24
#define EXTRON_RESOLUTION_2048x1080	25
#define EXTRON_RESOLUTION_1600x900	26
#define EXTRON_RESOLUTION_AUTO		30
#define EXTRON_RESOLUTION_LOCK		31
#define EXTRON_RESOLUTION_OUTPUT	32

// X16
#define EXTRON_RATE_AUTO		0
#define EXTRON_RATE_23_98		1
#define EXTRON_RATE_24			2
#define EXTRON_RATE_25			3
#define EXTRON_RATE_29_97		4
#define EXTRON_RATE_30			5
#define EXTRON_RATE_50			6
#define EXTRON_RATE_59_94		7
#define EXTRON_RATE_60			8
#define EXTRON_RATE_75			9

// X17
#define EXTRON_SYNC_RGB			0
#define EXTRON_SYNC_YUV			2

// X20
#define EXTRON_TEST_OFF			0
#define EXTRON_TEST_COLOR_BARS		1
#define EXTRON_TEST_GRAYSCALE		2
#define EXTRON_TEST_4x4_CROSSHATCH	3
#define EXTRON_TEST_ALTERNATING_PIXELS	4
#define EXTRON_TEST_CROP		5
#define EXTRON_TEST_1_33_ASPECT		6
#define EXTRON_TEST_1_78_ASPECT		7
#define EXTRON_TEST_1_85_ASPECT		8
#define EXTRON_TEST_2_35_ASPECT		9

// X35
#define EXTRON_ASPECT_FILL			1
#define EXTRON_ASPECT_FOLLOW		2


int extron_connect(const char *);
int extron_disconnect(int);
int extron_set_input_video_format(int, int);
int extron_get_input_video_format(int);
int extron_set_auto_image(int, int);
int extron_get_auto_image(int);
int extron_do_auto_image(int);
int extron_set_edid(int, int, int);
int extron_get_edid(int, int *, int *);
int extron_set_horizontal_start(int, int);
int extron_get_horizontal_start(int);
int extron_inc_horizontal_start(int);
int extron_dec_horizontal_start(int);
int extron_set_vertical_start(int, int);
int extron_get_vertical_start(int);
int extron_inc_vertical_start(int);
int extron_dec_vertical_start(int);
int extron_set_phase(int, int);
int extron_get_phase(int);
int extron_inc_phase(int);
int extron_dec_phase(int);
int extron_set_total_pixels(int, int);
int extron_get_total_pixels(int);
int extron_inc_total_pixels(int);
int extron_dec_total_pixels(int);
int extron_set_active_pixels(int, int);
int extron_get_active_pixels(int);
int extron_inc_active_pixels(int);
int extron_dec_active_pixels(int);
int extron_set_active_lines(int, int);
int extron_get_active_lines(int);
int extron_inc_active_lines(int);
int extron_dec_active_lines(int);
int extron_set_video_mute(int, int);
int extron_get_video_mute(int);
int extron_set_contrast(int, int);
int extron_get_contrast(int);
int extron_inc_contrast(int);
int extron_dec_contrast(int);
int extron_set_brightness(int, int);
int extron_get_brightness(int);
int extron_inc_brightness(int);
int extron_dec_brightness(int);
int extron_set_detail_filter(int, int);
int extron_get_detail_filter(int);
int extron_inc_detail_filter(int);
int extron_dec_detail_filter(int);
int extron_set_horizontal_shift(int, int);
int extron_get_horizontal_shift(int);
int extron_inc_horizontal_shift(int);
int extron_dec_horizontal_shift(int);
int extron_set_vertical_shift(int, int);
int extron_get_vertical_shift(int);
int extron_inc_vertical_shift(int);
int extron_dec_vertical_shift(int);
int extron_set_horizontal_size(int, int);
int extron_get_horizontal_size(int);
int extron_inc_horizontal_size(int);
int extron_dec_horizontal_size(int);
int extron_set_vertical_size(int, int);
int extron_get_vertical_size(int);
int extron_inc_vertical_size(int);
int extron_dec_vertical_size(int);
int extron_zoom_in(int, int*, int*);
int extron_zoom_out(int, int*, int*);
int extron_get_zoom(int, int*, int*);
int extron_set_aspect(int, int);
int extron_get_aspect(int);
int extron_set_scaler_rate(int, int, int);
int extron_get_scaler_rate(int, int*, int*);
int extron_set_output_format(int, int);
int extron_get_output_format(int);
int extron_test(int, int);
int extron_set_freeze(int, int);
int extron_get_freeze(int);
int extron_set_rgb_delay(int, int);
int extron_get_rgb_delay(int);
int extron_set_audio_mute(int, int);
int extron_get_audio_mute(int);
int extron_set_audio_gain(int, int);
int extron_set_audio_attenuation(int, int);
int extron_get_audio_level(int);
int extron_inc_audio_level(int);
int extron_dec_audio_level(int);
int extron_set_audio_delay(int, int);
int extron_get_audio_delay(int);
int extron_set_audio_bit_depth(int, int);
int extron_get_audio_bit_depth(int);


#endif
