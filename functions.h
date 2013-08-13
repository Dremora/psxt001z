#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

wchar_t * char2wchar_t(s8 * oldstring);
u32 calculate_edc(const u8 * src, u32  size, u32 edc_lut[256]);
u32 getfilesize(FILE * file);
u8 buffercmp(u8 *goodmd5, u8 *badmd5, u32 bsize);
u8 zerocmp(u8 *buffer, u32 bsize);
void msf(u32 lba, u8 * string);
u8 getedc(FILE * file);
u16 crc16(u8 *, int);

#endif