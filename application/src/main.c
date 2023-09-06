#include <zephyr.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(main);

void main(void)
{
	while(1){
		k_msleep(1000);
	}
}
