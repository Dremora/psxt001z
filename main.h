#ifndef __MAIN_H__
#define __MAIN_H__


class crc32{
protected:
	unsigned table[256];
public:
	unsigned m_crc32;
	crc32();
	void ProcessCRC(void* pData, register int nLen);
};


void antizektor(s8 * filename);
u8 checksums(int argc, char *argv[]);
void dump();
void generate(u32 argc, s8 * argv[]);
void m3s(s8 * filename);

void matrix(s8 * argv[]);
void scanfix(char *filename, int fix);
void str(s8 * argv[]);
void str2bs(s8 * argv[]);
void sub(s8 * filename, s8 * strsectors);
u8 umd(int argc, char *argv[]);
void zektor(s8 * filename);

#endif