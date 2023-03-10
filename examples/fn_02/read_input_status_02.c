#include <stdio.h>

#include "mb_slave.h"

#define SLAVE_ID    2
#define DI_COUNT    10

#define ENDL  printf("\n")

static void get_input_1(void *value)
{
  /**
   * Write the user here and pass value as 0 or 1 to **value**
  */
  *(char*)value = 1;
}

int main()
{
  mb_def_t mb_def;
  mb_init(&mb_def, SLAVE_ID, DI_COUNT, 0, 0, 0);

  for (int i = 0; i < 10; i++) {
    mb_register_digital_in(&mb_def, i, get_input_1);
  }

  char in_buff[] = {2, 2, 0, 0, 0, 10, 0x3e, 0xf8};
  unsigned char *response = NULL;
  unsigned char len;

  mb_set_command_frame(&mb_def, in_buff);
  mb_get_response(&mb_def, &response, &len);

  for (int i = 0; i < len; i++) {
    printf("%02x ", response[i]);
  }
  ENDL;

}
