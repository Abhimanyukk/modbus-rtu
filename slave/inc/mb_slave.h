#ifndef __MB_SLAVE_H__
#define __MB_SLAVE_H__

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "mb_crc.h"

#define LOGE(x) printf("ERROR: %s\n", x)
#define LOGW(x) printf("WARNING: %s\n", x)
#define LOGI(x) printf("INFO: %s\n", x)

#define COMBINE(H, L) (H << 8) | L

typedef enum {
  CRC_ERR = -2,
  SLAVE_ID_ERR,
  SUCCESS
} error_t;

typedef enum {
  READ_COIL_STATUS = 0x01,
  READ_INPUT_STATUS,
  READ_HOLDING_REGISTER,
  READ_INPUT_REGISTER,
  FORCE_SINGLE_COIL,
  PRESET_SINGLE_REGISTER,
  FORCE_MULITPLE_COIL = 0x0F,
  PRESET_MULTIPLE_REGISTERS
} function_codes_t;

typedef enum {
  READ = 0,
  READ_WRITE
} reg_access_t;

typedef void (*FUNCTION_PTR) (void *value);

#pragma 1
typedef struct {
  unsigned char slave_id             :8;
  function_codes_t function_code     :8;
  unsigned char register_address_h   :8;
  unsigned char register_address_l   :8;
  unsigned char value_h              :8;
  unsigned char value_l              :8;
  unsigned char crc_h                :8;
  unsigned char crc_l                :8;
} mb_single_frame_t;

#pragma 1
typedef struct {
  unsigned char slave_id : 8;
  function_codes_t function_code : 8;
  unsigned char data_len : 8;
  unsigned char *data_buff;
  unsigned char buff_len;
} mb_out_frame_t;

typedef struct {
  unsigned short address;
  FUNCTION_PTR get;
  FUNCTION_PTR set;
  char index;
  reg_access_t access;
} digital_t;

typedef struct {
  unsigned short address;
  short reg_data;
  FUNCTION_PTR get;
  FUNCTION_PTR set;
  reg_access_t access;
} analog_t;


typedef struct {
  unsigned short size;
  digital_t *params;
} digital_input_t;

typedef struct {
  unsigned short size;
  digital_t *params;
} digital_output_t;

typedef struct {
  unsigned short size;
  analog_t *params;
} analog_input_t;

typedef struct {
  unsigned short size;
  analog_t *params;
} analog_output_t;

typedef struct {
  unsigned char id;
  digital_input_t d_in;
  digital_output_t d_out;
  analog_input_t a_in;
  analog_output_t a_out;
  mb_out_frame_t output;
  mb_single_frame_t *mb_single_frame;
} mb_def_t;



void mb_init(mb_def_t *mb_def, unsigned char id, unsigned short di_count, unsigned short do_count, unsigned short ai_count, unsigned short ao_count);
void mb_register_digital_in(mb_def_t *mb_def, unsigned short address, FUNCTION_PTR action);
void mb_register_digital_out(mb_def_t *mb_def, unsigned short reg, FUNCTION_PTR set, FUNCTION_PTR get);
void mb_register_analog_in(mb_def_t *mb_def, unsigned short address, FUNCTION_PTR action);
void mb_register_analog_out(mb_def_t *mb_def, unsigned short address, FUNCTION_PTR set, FUNCTION_PTR get);
error_t mb_set_command_frame(mb_def_t *mbdef, char *inp_buff);
void mb_get_response(mb_def_t *mbdef, unsigned char **buff, unsigned char *len);

#endif /* __MB_SLAVE_H__ */
