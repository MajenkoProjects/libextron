#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/time.h>
#include <string.h>
#include <extron.h>

int extron_connect(const char *dev) {
	int fd = open(dev, O_RDWR|O_NOCTTY);

	struct termios options;
	fcntl(fd, F_SETFL, 0);
	tcgetattr(fd, &options);
	cfmakeraw(&options);
	options.c_cflag |= (CLOCAL | CREAD);
	options.c_cflag &= ~CRTSCTS;
	options.c_cflag &= ~CBAUD;
	cfsetispeed(&options, B9600);
	cfsetospeed(&options, B9600);
	tcsetattr(fd, TCSANOW, &options);
	return fd;
}

int extron_disconnect(int fd) {
	close(fd);
	return 1;
}

static int extron_command(int fd, const char *out, char *in) {
	write(fd, out, strlen(out));
	int pos = 0;
	char c;
	while (read(fd, &c, 1) == 1) {
		switch (c) {
			case '\n':
				return pos;
			case '\r':
				break;
			default:
				in[pos++] = c;
				in[pos] = 0;
		}
	}
}

static int extron_simple_set(int fd, int val, const char *set, const char *get, int len) {
	char out[30];
	char in[30];
	sprintf(out, set, val);
	if (extron_command(fd, out, in) > 0) {
		if (strncmp(in, get, len) == 0) {
			return atoi(in + len);
		}
	}
	return -1;
}

static int extron_simple_get(int fd, const char *set, const char *get, int len) {
	char in[30];
	if (extron_command(fd, set, in) > 0) {
		if (strncmp(in, get, len) == 0) {
			return atoi(in + len);
		}
	}
	return -1;
}

int extron_set_input_video_format(int fd, int format) { return extron_simple_set(fd, format, "1*%d\\", "Atyp1*", 6); }
int extron_get_input_video_format(int fd) { return extron_simple_get(fd, "1*\\", "Atyp1*", 6); }

int extron_set_auto_image(int fd, int enable) { return extron_simple_set(fd, enable, "1*%dA", "Img1*", 5); }
int extron_get_auto_image(int fd) { return extron_simple_get(fd, "1A", "Img1*", 5); }

int extron_do_auto_image(int fd) {
	char out[30];
	char in[30];
	sprintf(out, "A");
	if (extron_command(fd, out, in) > 0) {
		if (strcmp(in, "Img") == 0) {
			return 1;
		}
	}
	return -1;
}

int extron_set_edid(int fd, int resolution, int rate) {
	char out[30];
	char in[30];
	sprintf(out, "\e%d*%dEDID\r", resolution, rate);
	if (extron_command(fd, out, in) > 0) {
		if (strncmp(in, "Edid", 4) == 0) {
			return 1;
		}
	}
	return -1;
}

int extron_get_edid(int fd, int *resolution, int *rate) {
	char out[30];
	char in[30];
	sprintf(out, "\eEDID\r");
	if (extron_command(fd, out, in) > 0) {
		if (strncmp(in, "Edid", 4) == 0) {
			char *p = in + 4;
			char *res = strtok(p, "*");
			char *rat = strtok(NULL, "*");
			if (rat != NULL) {
				if (resolution != NULL) {
					*resolution = atoi(res);
				}
				if (rate != NULL) {
					*rate = atoi(rat);
				}
				return 1;
			}
		}
	}
	return -1;
}

int extron_set_horizontal_start(int fd, int val) { return extron_simple_set(fd, val, "\e%dHSRT\r", "Hsrt", 4); }
int extron_get_horizontal_start(int fd) { return extron_simple_get(fd, "\eHSRT\r", "Hsrt", 4); }
int extron_inc_horizontal_start(int fd) { return extron_simple_get(fd, "\e+HSRT\r", "Hsrt", 4); }
int extron_dec_horizontal_start(int fd) { return extron_simple_get(fd, "\e-HSRT\r", "Hsrt", 4); }

int extron_set_vertical_start(int fd, int val) { return extron_simple_set(fd, val, "\e%dVSRT\r", "Vsrt", 4); }
int extron_get_vertical_start(int fd) { return extron_simple_get(fd, "\eVSRT\r", "Vsrt", 4); }
int extron_inc_vertical_start(int fd) { return extron_simple_get(fd, "\e+VSRT\r", "Vsrt", 4); }
int extron_dec_vertical_start(int fd) { return extron_simple_get(fd, "\e-VSRT\r", "Vsrt", 4); }

int extron_set_phase_start(int fd, int val) { return extron_simple_set(fd, val, "\e%dPHAS\r", "Phas", 4); }
int extron_get_phase_start(int fd) { return extron_simple_get(fd, "\ePHAS\r", "Phas", 4); }
int extron_inc_phase_start(int fd) { return extron_simple_get(fd, "\e+PHAS\r", "Phas", 4); }
int extron_dec_phase_start(int fd) { return extron_simple_get(fd, "\e-PHAS\r", "Phas", 4); }

int extron_set_total_pixels(int fd, int val) { return extron_simple_set(fd, val, "\e%dTPIX\r", "Tpix", 4); }
int extron_get_total_pixels(int fd) { return extron_simple_get(fd, "\eTPIX\r", "Tpix", 4); }
int extron_inc_total_pixels(int fd) { return extron_simple_get(fd, "\e+TPIX\r", "Tpix", 4); }
int extron_dec_total_pixels(int fd) { return extron_simple_get(fd, "\e-TPIX\r", "Tpix", 4); }

int extron_set_active_pixels(int fd, int val) { return extron_simple_set(fd, val, "\e%dAPIX\r", "Apix", 4); }
int extron_get_active_pixels(int fd) { return extron_simple_get(fd, "\eAPIX\r", "Apix", 4); }
int extron_inc_active_pixels(int fd) { return extron_simple_get(fd, "\e+APIX\r", "Apix", 4); }
int extron_dec_active_pixels(int fd) { return extron_simple_get(fd, "\e-APIX\r", "Apix", 4); }

int extron_set_active_lines(int fd, int val) { return extron_simple_set(fd, val, "\e%dALIN\r", "Alin", 4); }
int extron_get_active_lines(int fd) { return extron_simple_get(fd, "\eALIN\r", "Alin", 4); }
int extron_inc_active_lines(int fd) { return extron_simple_get(fd, "\e+ALIN\r", "Alin", 4); }
int extron_dec_active_lines(int fd) { return extron_simple_get(fd, "\e-ALIN\r", "Alin", 4); }


int extron_set_video_mute(int fd, int v) { return extron_simple_set(fd, v, "%dB", "Vmt", 3); }
int extron_get_video_mute(int fd) { return extron_simple_get(fd, "B", "Vmt", 3); }

int extron_set_contrast(int fd, int val) { return extron_simple_set(fd, val, "\e%dCONT\r", "Cont", 4); }
int extron_get_contrast(int fd) { return extron_simple_get(fd, "\eCONT\r", "Cont", 4); }
int extron_inc_contrast(int fd) { return extron_simple_get(fd, "\e+CONT\r", "Cont", 4); }
int extron_dec_contrast(int fd) { return extron_simple_get(fd, "\e-CONT\r", "Cont", 4); }

int extron_set_brightness(int fd, int val) { return extron_simple_set(fd, val, "\e%dBRIT\r", "Brit", 4); }
int extron_get_brightness(int fd) { return extron_simple_get(fd, "\eBRIT\r", "Brit", 4); }
int extron_inc_brightness(int fd) { return extron_simple_get(fd, "\e+BRIT\r", "Brit", 4); }
int extron_dec_brightness(int fd) { return extron_simple_get(fd, "\e-BRIT\r", "Brit", 4); }

int extron_set_detail_filter(int fd, int val) { return extron_simple_set(fd, val, "\e%dHDET\r", "Hdet", 4); }
int extron_get_detail_filter(int fd) { return extron_simple_get(fd, "\eHDET\r", "Hdet", 4); }
int extron_inc_detail_filter(int fd) { return extron_simple_get(fd, "\e+HDET\r", "Hdet", 4); }
int extron_dec_detail_filter(int fd) { return extron_simple_get(fd, "\e-HDET\r", "Hdet", 4); }

int extron_set_horizontal_shift(int fd, int val) { return extron_simple_set(fd, val, "\e%dHCTR\r", "Hctr", 4); }
int extron_get_horizontal_shift(int fd) { return extron_simple_get(fd, "\eHCTR\r", "Hctr", 4); }
int extron_inc_horizontal_shift(int fd) { return extron_simple_get(fd, "\e+HCTR\r", "Hctr", 4); }
int extron_dec_horizontal_shift(int fd) { return extron_simple_get(fd, "\e-HCTR\r", "Hctr", 4); }

int extron_set_vertical_shift(int fd, int val) { return extron_simple_set(fd, val, "\e%dVCTR\r", "Vctr", 4); }
int extron_get_vertical_shift(int fd) { return extron_simple_get(fd, "\eVCTR\r", "Vctr", 4); }
int extron_inc_vertical_shift(int fd) { return extron_simple_get(fd, "\e+VCTR\r", "Vctr", 4); }
int extron_dec_vertical_shift(int fd) { return extron_simple_get(fd, "\e-VCTR\r", "Vctr", 4); }

int extron_set_horizontal_size(int fd, int val) { return extron_simple_set(fd, val, "\e%dHSIZ\r", "Hsiz", 4); }
int extron_get_horizontal_size(int fd) { return extron_simple_get(fd, "\eHSIZ\r", "Hsiz", 4); }
int extron_inc_horizontal_size(int fd) { return extron_simple_get(fd, "\e+HSIZ\r", "Hsiz", 4); }
int extron_dec_horizontal_size(int fd) { return extron_simple_get(fd, "\e-HSIZ\r", "Hsiz", 4); }

int extron_set_vertical_size(int fd, int val) { return extron_simple_set(fd, val, "\e%dVSIZ\r", "Vsiz", 4); }
int extron_get_vertical_size(int fd) { return extron_simple_get(fd, "\eVSIZ\r", "Vsiz", 4); }
int extron_inc_vertical_size(int fd) { return extron_simple_get(fd, "\e+VSIZ\r", "Vsiz", 4); }
int extron_dec_vertical_size(int fd) { return extron_simple_get(fd, "\e-VSIZ\r", "Vsiz", 4); }

int extron_zoom_in(int fd, int *hor, int *vert) {
	char out[30];
	char in[30];
	sprintf(out, "\e+ZOOM\r");
	if (extron_command(fd, out, in) > 0) {
		if (strncmp(in, "Zoom", 4) == 0) {
			char *p = in + 4;
			char *h = strtok(p, "*");
			char *v = strtok(NULL, "*");
			if (h != NULL) {
				if (hor != NULL) {
					*hor = atoi(h);
				}
				if (vert != NULL) {
					*vert = atoi(v);
				}
				return 1;
			}
		}
	}
	return -1;
}

int extron_zoom_out(int fd, int *hor, int *vert) {
	char out[30];
	char in[30];
	sprintf(out, "\e-ZOOM\r");
	if (extron_command(fd, out, in) > 0) {
		if (strncmp(in, "Zoom", 4) == 0) {
			char *p = in + 4;
			char *h = strtok(p, "*");
			char *v = strtok(NULL, "*");
			if (h != NULL) {
				if (hor != NULL) {
					*hor = atoi(h);
				}
				if (vert != NULL) {
					*vert = atoi(v);
				}
				return 1;
			}
		}
	}
	return -1;
}

int extron_get_zoom(int fd, int *hor, int *vert) {
	char out[30];
	char in[30];
	sprintf(out, "\eZOOM\r");
	if (extron_command(fd, out, in) > 0) {
		if (strncmp(in, "Zoom", 4) == 0) {
			char *p = in + 4;
			char *h = strtok(p, "*");
			char *v = strtok(NULL, "*");
			if (h != NULL) {
				if (hor != NULL) {
					*hor = atoi(h);
				}
				if (vert != NULL) {
					*vert = atoi(v);
				}
				return 1;
			}
		}
	}
	return -1;
}

int extron_set_aspect(int fd, int val) { return extron_simple_set(fd, val, "\e%dASPR\r", "Aspr", 4); }
int extron_get_aspect(int fd) { return extron_simple_get(fd, "\eASPR\r", "Aspr", 4); }

int extron_set_scaler_rate(int fd, int resolution, int rate) {
	char out[30];
	char in[30];
	sprintf(out, "\e%d*%dRATE\r", resolution, rate);
	if (extron_command(fd, out, in) > 0) {
		if (strncmp(in, "Zoom", 4) == 0) {
			return 1;
		}
	}
	return -1;
}

int extron_get_scaler_rate(int fd, int *resolution, int *rate) {
	char out[30];
	char in[30];
	sprintf(out, "\eRATE\r");
	if (extron_command(fd, out, in) > 0) {
		if (strncmp(in, "Rate", 4) == 0) {
			char *p = in + 4;
			char *res = strtok(p, "*");
			char *rat = strtok(NULL, "*");
			if (rat != NULL) {
				if (resolution != NULL) {
					*resolution = atoi(res);
				}
				if (rate != NULL) {
					*rate = atoi(rat);
				}
				return 1;
			}
		}
	}
	return -1;
}

int extron_set_output_format(int fd, int val) { return extron_simple_set(fd, val, "\e%dVTPO\r", "Vtpo", 4); }
int extron_get_output_format(int fd) { return extron_simple_get(fd, "\eVTPO\r", "Vtpo", 4); }

int extron_test(int fd, int mode) { return extron_simple_set(fd, mode, "\e%dTEST\r", "Test", 4); }

int extron_set_freeze(int fd, int val) { return extron_simple_set(fd, val, "%dF", "Frz", 3); }
int extron_get_freeze(int fd) { return extron_simple_get(fd, "F", "Frz", 3); }

int extron_set_rgb_delay(int fd, int val) { return extron_simple_set(fd, val, "\e%dVDLY\r", "Vdly", 4); }
int extron_get_rgb_delay(int fd) { return extron_simple_get(fd, "\eVDLY\r", "Vdly", 4); }

int extron_set_audio_mute(int fd, int val) { return extron_simple_set(fd, val, "%dZ", "Amt", 3); }
int extron_get_audio_mute(int fd) { return extron_simple_get(fd, "Z", "Amt", 3); }

int extron_set_audio_gain(int fd, int val) { return extron_simple_set(fd, val, "%dG", "Aud", 3); }
int extron_set_audio_attenuation(int fd, int val) { return extron_simple_set(fd, val, "%dg", "Aud", 3); }
int extron_get_audio_level(int fd) { return extron_simple_get(fd, "G", "Aud", 3); }
int extron_inc_audio_level(int fd) { return extron_simple_get(fd, "+G", "Aud", 3); }
int extron_dec_audio_level(int fd) { return extron_simple_get(fd, "-G", "Aud", 3); }

int extron_set_audio_delay(int fd, int val) { return extron_simple_set(fd, val, "\eS%dADLY\r", "AdlyS", 5); }
int extron_get_audio_delay(int fd) { return extron_simple_get(fd, "\eSADLY\r", "AdlyS", 5); }

int extron_set_audio_bit_depth(int fd, int val) { return extron_simple_set(fd, val, "\eA%dBITD\r", "BitdA", 5); }
int extron_get_audio_bit_depth(int fd) { return extron_simple_get(fd, "\eABITD\r", "BitdA", 5); }

