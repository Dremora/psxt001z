#ifndef __MAIN_H__
#define __MAIN_H__

void antizektor(s8 * filename);
u8 checksums(int argc, char *argv[]);
void dump();
void generate(u32 argc, s8 * argv[]);
void m3s(s8 * filename);

void matrix(s8 * argv[]);
void scanfix(s8 *filename, u8 fix);
void str(s8 * argv[]);
void str2bs(s8 * argv[]);
void sub(s8 * filename, s8 * strsectors);
u8 umd(int argc, char *argv[]);
void zektor(s8 * filename);

#endif