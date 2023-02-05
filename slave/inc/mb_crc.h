#ifndef __MB_CRC_H__
#define __MB_CRC_H__

#include <stdbool.h>

bool check_crc(unsigned char *inp_buff, unsigned char position);
unsigned short calc_crc(unsigned char *buff, unsigned char len);

#endif /* __MB_CRC_H__ */
