#include <mraa.h>

int main(int argc, char** argv) {
	const int ledPort = 32;
	mraa_gpio_context ledPin = mraa_gpio_init(ledPort);
	mraa_gpio_dir(ledPin, MRAA_GPIO_OUT_LOW);
	int sygnal = 1,i;
	for (i = 0; i < 20 ; ++i ) {
		sygnal != sygnal;
		mraa_gpio_write(ledPin, sygnal);
	}
	mraa_gpio_close(ledPin);
}