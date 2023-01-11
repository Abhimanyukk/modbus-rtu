#include <stdio.h>

#include "mb_slave.h"

#define SLAVE_ID    2
#define DI_COUNT    10
#define DO_COUNT    2
#define AI_COUNT    2
#define AO_COUNT    2

static void get_input_1(void *value)
{
  LOGI("Input 1");
  *(char*)value = 0;
}

static void get_input_2(void *value)
{
  LOGI("Input 2");
  *(char*)value = 1;
}

FUNCTION_PTR foo[] = {get_input_1, get_input_2};
int main()
{
  mb_def_t mb_def;
  mb_init(&mb_def, SLAVE_ID, DI_COUNT, DO_COUNT, AI_COUNT, AO_COUNT);

  for (int i = 0; i < 10; i++) {
    mb_register_digital_in(&mb_def, i, foo[i%2]);
  }

  char in_buff[] = {2, 1, 0, 0, 0, 11, 0, 0};
  unsigned char *response = NULL;
  unsigned char len;

  mb_set_command_frame(&mb_def, in_buff);
  mb_get_response(&mb_def, &response, &len);

  for (int i = 0; i < len; i++) {
    printf("0x%02x\n", response[i]);
  }
}
