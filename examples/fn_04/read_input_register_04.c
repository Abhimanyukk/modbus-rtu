#include <stdio.h>

#include "mb_slave.h"

#define SLAVE_ID    2
#define AI_COUNT    10

#define ENDL        printf("\n")

static void get_input_1(void *value)
{
    /**
     * Write the user code here and pass the 16 bit value to **value**
    */
    *(unsigned short*)value = 0x1234;
}

int main()
{
    mb_def_t mb_def;
    mb_init(&mb_def, SLAVE_ID, 0, 0, AI_COUNT, 0);

    for (int i = 0; i < 10; i++) {
        mb_register_analog_in(&mb_def, i, get_input_1);
    }

    char in_buff[] = {2, 4, 0, 0, 0, 10, 0x3E, 0x70};
    unsigned char *response = NULL;
    unsigned char len;
    mb_set_command_frame(&mb_def, in_buff);
    mb_get_response(&mb_def, &response, &len);

    for (int i = 0; i < len; i++) {
        printf("%02x ", response[i]);
    }
    ENDL;
}
