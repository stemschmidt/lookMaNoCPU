#include <zephyr.h>
#include <logging/log.h>
#include <hal/nrf_gpiote.h>
#include <hal/nrf_gpio.h>
#include <hal/nrf_spim.h>
#include <hal/nrf_dppi.h>

#define INPUT_CONFIG_0 0
#define OUTPUT_CONFIG 2

#define LED_4 0x05

#define BUTTON_1 0x06

static uint8_t receive_buffer[8];

LOG_MODULE_REGISTER(main);

void main(void)
{
	const uint8_t trigger_spi_channel = 0;
	const uint8_t trigger_led_channel = 1;

	// initialize receive buffer to all 0x55
	memset(receive_buffer, 0x55, 8);

	// Configure BUTTON_1 -> uses CONFIG[0] register in GPIOTE for event
	nrf_gpio_cfg_input(BUTTON_1, NRF_GPIO_PIN_PULLUP);
	nrf_gpiote_event_configure(NRF_GPIOTE, INPUT_CONFIG_0, BUTTON_1, NRF_GPIOTE_POLARITY_LOTOHI);
	nrf_gpiote_event_enable(NRF_GPIOTE, INPUT_CONFIG_0);

	// Configure LED_4 -> uses CONFIG[2] register in GPIOTE
	nrf_gpiote_task_configure(NRF_GPIOTE, OUTPUT_CONFIG, LED_4, NRF_GPIOTE_POLARITY_TOGGLE, 0);
	nrf_gpiote_task_enable(NRF_GPIOTE, OUTPUT_CONFIG);

	// Configure SPIM0 -> it will only read ones as no GPIO pin is assigned
	nrf_spim_frequency_set(NRF_SPIM0, NRF_SPIM_FREQ_2M);
	nrf_spim_rx_buffer_set(NRF_SPIM0, receive_buffer, 4); // setup buffer and data to be read
	nrf_spim_enable(NRF_SPIM0);

	// now connect Button_1 event with SPIM0 task on DPPI channel 0
	nrf_gpiote_publish_set(NRF_GPIOTE, NRF_GPIOTE_EVENT_IN_0, trigger_spi_channel);
	nrf_spim_subscribe_set(NRF_SPIM0, NRF_SPIM_TASK_START, trigger_spi_channel);

	// and connect SPIM0 transaction end event with LED_4 task on DPPI channel 1
	nrf_spim_publish_set(NRF_SPIM0, NRF_SPIM_EVENT_END, trigger_led_channel);
	nrf_gpiote_subscribe_set(NRF_GPIOTE, NRF_GPIOTE_TASK_OUT_2, trigger_led_channel);

	// and enable DPPI for channel 0 and 1 -> 0x00000003
	nrf_dppi_channels_enable(NRF_DPPIC, 0x00000003);

	/*
		The while(1) loop at the end of main prints the content of the receive_buffer every second.
		The buffer is initialized with 0x55. When button 1 is pressed on the nrf9160dk you will see LED4
		light up and the log shows that the first four bytes are now 0xff (the value that is read by the
		SPI with no external device attached)
	*/
	while(1){
		k_msleep(1000);
		LOG_INF("0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x",
			receive_buffer[0],
			receive_buffer[1],
			receive_buffer[2],
			receive_buffer[3],
			receive_buffer[4],
			receive_buffer[5],
			receive_buffer[6],
			receive_buffer[7]);
	}
}
