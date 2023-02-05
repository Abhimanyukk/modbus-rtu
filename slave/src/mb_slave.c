#include "mb_slave.h"




void mb_init(mb_def_t *mb_def, unsigned char id, unsigned short di_count, unsigned short do_count, unsigned short ai_count, unsigned short ao_count) {
  mb_def->id = id;
  mb_def->d_in.size = di_count;
  mb_def->d_out.size = do_count;
  mb_def->a_in.size = ai_count;
  mb_def->a_out.size = ao_count;

  mb_def->d_in.params = malloc(sizeof(digital_t) * mb_def->d_in.size);
  mb_def->d_out.params = malloc(sizeof(digital_t) * mb_def->d_out.size);
  mb_def->a_in.params = malloc(sizeof(analog_t) * mb_def->a_in.size);
  mb_def->a_out.params = malloc(sizeof(analog_t) * mb_def->a_out.size);
}

void mb_register_digital_in(mb_def_t *mb_def, unsigned short address, FUNCTION_PTR action) {
  if (address > mb_def->d_in.size) {
    LOGE("Operation not allowed");
    LOGE("Address more than allocated in function mb_register_digital_in()");
    exit(0);
  }
  mb_def->d_in.params[address].address = address;
  mb_def->d_in.params[address].get = action;
}

void mb_register_digital_out(mb_def_t *mb_def, unsigned short address, FUNCTION_PTR set, FUNCTION_PTR get) {
  if (address > mb_def->d_out.size) {
    LOGE("Operation not allowed");
    LOGE("Address more than allocated in function mb_register_digital_out()");
    exit(0);
  }
  mb_def->d_out.params[address].address = address;
  mb_def->d_out.params[address].set = set;
  mb_def->d_out.params[address].get = get;

}

void mb_register_analog_in(mb_def_t *mb_def, unsigned short address, FUNCTION_PTR action) {
  if (address > mb_def->a_in.size) {
    LOGE("Operation not allowed");
    LOGE("Address more than allocated in function mb_register_analog_in()");
    exit(0);
  }
  mb_def->a_in.params[address].address = address;
  mb_def->a_in.params[address].get = action;
}

void mb_register_analog_out(mb_def_t *mb_def, unsigned short address, FUNCTION_PTR action) {
  if (address > mb_def->a_out.size) {
    LOGE("Operation not allowed");
    LOGE("Address more than allocated in function mb_register_analog_out()");
    exit(0);
  }
  mb_def->a_out.params[address].address = address;
  mb_def->a_out.params[address].get = action;
}

void mb_set_command_frame(mb_def_t *mbdef, char *inp_buff) {
  
  mb_single_frame_t *mb_single_frame = (mb_single_frame_t*) inp_buff;
  if (mbdef->id != mb_single_frame->slave_id) {
    LOGW("Invalid Slave ID");
    return;
  }
  unsigned char crc_position = 0;
  if (mb_single_frame->function_code >=READ_COIL_STATUS && mb_single_frame->function_code <= PRESET_SINGLE_REGISTER) {
    crc_position = 6;
  }
  else if (mb_single_frame->function_code >= FORCE_MULITPLE_COIL || mb_single_frame->function_code == PRESET_MULTIPLE_REGISTERS) {
    crc_position = inp_buff[6] + 7;
  }

  if (!check_crc((unsigned char *)inp_buff, crc_position)) {
    LOGE("Wrong CRC");
    return;
  }
  

  unsigned short reg_address = COMBINE(mb_single_frame->register_address_h, mb_single_frame->register_address_l);
  unsigned short no_of_register = COMBINE(mb_single_frame->value_h, mb_single_frame->value_l);
  unsigned char index = 0; 

  
  switch (mb_single_frame->function_code)
  {
    case READ_COIL_STATUS:
      LOGI("Read Coil Status");
      mbdef->output.slave_id = mb_single_frame->slave_id;
      mbdef->output.function_code = READ_COIL_STATUS;
      mbdef->output.data_len = 1;
      mbdef->output.data_buff = malloc((no_of_register / 8) + 1);

      if (no_of_register > mbdef->d_out.size) {
        LOGW("Register not assigned");
        no_of_register = mbdef->d_out.size;
      }

      while (no_of_register--)
      {
        if (index > 7) {
          mbdef->output.data_len++;
          index = 0;
        }
      
        unsigned char value = 0;
        mbdef->d_out.params[reg_address].get(&value);
        mbdef->output.data_buff[mbdef->output.data_len - 1] |= (value << index);
        index++;
        reg_address++;
      }
      break;
      
    case READ_INPUT_STATUS:
      LOGI("Read Input Status");
      mbdef->output.slave_id = mb_single_frame->slave_id;
      mbdef->output.function_code = READ_INPUT_STATUS;
      mbdef->output.data_len = 1;
      mbdef->output.data_buff = malloc((no_of_register / 8) + 1);
      
      if (no_of_register > mbdef->d_in.size) {
        LOGW("Register not assigned");
        no_of_register = mbdef->d_in.size;
      }
      while (no_of_register--) {
        if (index > 7) {
          mbdef->output.data_len++;
          index = 0;
        }
        unsigned char value = 0;
        mbdef->d_in.params[reg_address].get(&value);
        mbdef->output.data_buff[mbdef->output.data_len - 1] |= (value << index);
        index++;
        reg_address++;
      }
      
      break;
    case READ_HOLDING_REGISTER:
      LOGI("Read Holding Register");
      mbdef->output.slave_id = mb_single_frame->slave_id;
      mbdef->output.function_code = READ_HOLDING_REGISTER;
      mbdef->output.data_len = 0;
      mbdef->output.data_buff = malloc(no_of_register * 2);

      if (no_of_register > mbdef->a_in.size) {
        LOGW("Register not assigned");
        no_of_register = mbdef->a_in.size;
      }

      while (no_of_register--)
      {
        unsigned short value = 0;
        mbdef->a_in.params[reg_address].get(&value);
        mbdef->output.data_buff[mbdef->output.data_len++] = (unsigned char)(value >> 8);
        mbdef->output.data_buff[mbdef->output.data_len++] = (unsigned char)(value);
      }
      

      break;
    case FORCE_SINGLE_COIL:
      
      break;

    case PRESET_SINGLE_REGISTER:
    case READ_INPUT_REGISTER:
      break;

    case FORCE_MULITPLE_COIL:
    case PRESET_MULTIPLE_REGISTERS:
      break;
    
    default:
      break;
  }
}


void mb_get_response(mb_def_t *mbdef, unsigned char **buff, unsigned char *len)
{
  mbdef->output.buff_len = 0;
  mbdef->output.buff_len = mbdef->output.data_len + 3 + 2;
  *len = mbdef->output.buff_len;
  *buff = malloc((sizeof(char)) * (*len));

  memset(*buff, 0, mbdef->output.buff_len);
  memcpy(*buff, &mbdef->output, 3);
  memcpy((*buff) + 3, (char*)mbdef->output.data_buff, mbdef->output.data_len);
  if (*buff[0] != 0) {
    unsigned short crc = calc_crc(*buff, mbdef->output.buff_len - 2);
    (*buff)[mbdef->output.buff_len - 2] = crc >> 8;
    (*buff)[mbdef->output.buff_len - 1] = (unsigned char)crc;
  }
  memset(&mbdef->output, 0, mbdef->output.buff_len);
}