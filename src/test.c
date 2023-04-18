#include <extron.h>
#include <unistd.h>

void main() {
	int fd = extron_connect("/dev/ttyS0");
	extron_test(fd, EXTRON_TEST_COLOR_BARS);
	sleep(1);
	extron_test(fd, EXTRON_TEST_GRAYSCALE);
	sleep(1);
	extron_test(fd, EXTRON_TEST_4x4_CROSSHATCH);
	sleep(1);
	extron_test(fd, EXTRON_TEST_ALTERNATING_PIXELS);
	sleep(1);
	extron_test(fd, EXTRON_TEST_CROP);
	sleep(1);
	extron_test(fd, EXTRON_TEST_1_33_ASPECT);
	sleep(1);
	extron_test(fd, EXTRON_TEST_1_78_ASPECT);
	sleep(1);
	extron_test(fd, EXTRON_TEST_1_85_ASPECT);
	sleep(1);
	extron_test(fd, EXTRON_TEST_2_35_ASPECT);
	sleep(1);
	extron_test(fd, EXTRON_TEST_OFF);
	extron_disconnect(fd);
}
