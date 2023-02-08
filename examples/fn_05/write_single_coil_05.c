#include <stdio.h>

#include "mb_slave.h"

#define SLAVE_ID    2
#define DO_COUNT    10

#define ENDL  printf("\n")

static void set_output_1(void *value)
{
  /**
   * Write the user here and pass value as 0 or 1 to **value**
  */
  printf("Value written is %d\n", *(unsigned short*)value);
  
}

static void get_output_1(void *value)
{
  /**
   * Write the user here and pass value as 0 or 1 to **value**
  */
  *(char*)value = 1;
}

int main()
{
  mb_def_t mb_def;
  mb_init(&mb_def, SLAVE_ID, 0, DO_COUNT, 0, 0);

  for (int i = 0; i < 10; i++) {
    mb_register_digital_out(&mb_def, i, set_output_1, get_output_1);
  }

  char in_buff[] = {2, 5, 0, 3, 0xFF, 0, 0x09, 0x7c};
  unsigned char *response = NULL;
  unsigned char len;

  mb_set_command_frame(&mb_def, in_buff);
  mb_get_response(&mb_def, &response, &len);

  for (int i = 0; i < len; i++) {
    printf("%02x ", response[i]);
  }
  ENDL;

}
