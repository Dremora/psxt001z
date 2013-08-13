#include "common.h"

class crc32{
protected:
	unsigned table[256];
public:
	unsigned m_crc32;
	crc32() {
		const unsigned CRC_POLY = 0xEDB88320;
		unsigned i, j, r;
		for (i = 0; i < 256; i++){
			for (r = i, j = 8; j; j--)
				r = r & 1? (r >> 1) ^ CRC_POLY: r >> 1;
			table[i] = r;
		}
		m_crc32 = 0;
	};
	void ProcessCRC(void* pData, register int nLen) {
		const unsigned CRC_MASK = 0xD202EF8D;
		register unsigned char* pdata = reinterpret_cast<unsigned char*>(pData);
		register unsigned crc = m_crc32;
		while (nLen--){
			crc = table[(unsigned char)crc ^ *pdata++] ^ crc >> 8;
			crc ^= CRC_MASK;
		}
		m_crc32 = crc;
	};
};

class track {
private:
	FILE *f;
	char *path;
	unsigned long start, crc, size;
	unsigned char riff[44], isriff, mode, smallfile, * fcontents, savetrack;
	signed long fastoffset[77], offset, current;
public:
	track(char *argv[], int argc) {
		char *end;
		fopen_s ( &f, argv[2], "rb" );
		start = strtoul(argv[3],&end,10);
		size = strtoul(argv[4],&end,10);
		crc = strtoul(argv[5],&end,16);
		offset = 0;
		isriff = 0;
		current = 0;
		smallfile = 0;
		mode = 1;
		savetrack = 0;
		printf ("File: %s\nStart: %d\nSize: %d\nCRC-32: %08x\n\n", argv[2], start, size, crc);
		for(char i = 6; i < argc; i++)
		if(argc >= 7) {
			if(!strcmp(argv[i],"r")) {
				riff[0] = 0x52;
				riff[1] = 0x49;
				riff[2] = 0x46;
				riff[3] = 0x46;
				riff[8] = 0x57;
				riff[9] = 0x41;
				riff[10] = 0x56;
				riff[11] = 0x45;
				riff[12] = 0x66;
				riff[13] = 0x6D;
				riff[14] = 0x74;
				riff[15] = 0x20;
				riff[16] = 0x10;
				riff[17] = 0x00;
				riff[18] = 0x00;
				riff[19] = 0x00;
				riff[20] = 0x01;
				riff[21] = 0x00;
				riff[22] = 0x02;
				riff[23] = 0x00;
				riff[24] = 0x44;
				riff[25] = 0xAC;
				riff[26] = 0x00;
				riff[27] = 0x00;
				riff[28] = 0x10;
				riff[29] = 0xB1;
				riff[30] = 0x02;
				riff[31] = 0x00;
				riff[32] = 0x04;
				riff[33] = 0x00;
				riff[34] = 0x10;
				riff[35] = 0x00;
				riff[36] = 0x64;
				riff[37] = 0x61;
				riff[38] = 0x74;
				riff[39] = 0x61;
				isriff = 1;
				*(unsigned long*)&riff[4] = size-8;
				*(unsigned long*)&riff[40] = size-44;
				size -= 44;
			}
			else if(argv[i][0]=='+') mode = 'p';
			else if(argv[i][0]=='-') mode = 'n';
			else if(argv[i][0]=='f') mode = 'f';
			else if(!strcmp(argv[i],"s")) {
				savetrack = 1;
				i++;
				path = argv[i];
			}
		}
		if(size <= 100000000) {
			fcontents = (unsigned char*)malloc(size + 40000);
			fseek(f, start - 20000, SEEK_SET);
			//fread(fcontents, 1, size, f);
			for(unsigned long i = 0; i < size + 40000; i++) {
				if(start + i <= 20000) fseek(f, start + i - 20000, SEEK_SET);
				if(!fread(&fcontents[i], 1, 1, f)) fcontents[i] = 0x00;
			}
			smallfile = 1;
		}
		if(mode == 'f') {
			fastoffset[0] = -1164;
			fastoffset[1] = -582;
			fastoffset[2] = -496;
			fastoffset[3] = -494;
			fastoffset[4] = -491;
			fastoffset[5] = -472;
			fastoffset[6] = -436;
			fastoffset[7] = -54;
			fastoffset[8] = -24;
			fastoffset[9] = -20;
			fastoffset[10] = +0;
			fastoffset[11] = +6;
			fastoffset[12] = +12;
			fastoffset[13] = +30;
			fastoffset[14] = +48;
			fastoffset[15] = +86;
			fastoffset[16] = +87;
			fastoffset[17] = +91;
			fastoffset[18] = +92;
			fastoffset[19] = +94;
			fastoffset[20] = +96;
			fastoffset[21] = +97;
			fastoffset[22] = +98;
			fastoffset[23] = +99;
			fastoffset[24] = +102;
			fastoffset[25] = +103;
			fastoffset[26] = +106;
			fastoffset[27] = +108;
			fastoffset[28] = +116;
			fastoffset[29] = +117;
			fastoffset[30] = +120;
			fastoffset[31] = +145;
			fastoffset[32] = +355;
			fastoffset[33] = +564;
			fastoffset[34] = +572;
			fastoffset[35] = +594;
			fastoffset[36] = +600;
			fastoffset[37] = +618;
			fastoffset[38] = +667;
			fastoffset[39] = +674;
			fastoffset[40] = +675;
			fastoffset[41] = +676;
			fastoffset[42] = +678;
			fastoffset[43] = +679;
			fastoffset[44] = +680;
			fastoffset[45] = +681;
			fastoffset[46] = +682;
			fastoffset[47] = +684;
			fastoffset[48] = +685;
			fastoffset[49] = +686;
			fastoffset[50] = +687;
			fastoffset[51] = +688;
			fastoffset[52] = +689;
			fastoffset[53] = +690;
			fastoffset[54] = +691;
			fastoffset[55] = +692;
			fastoffset[56] = +694;
			fastoffset[57] = +696;
			fastoffset[58] = +704;
			fastoffset[59] = +708;
			fastoffset[60] = +733;
			fastoffset[61] = +738;
			fastoffset[62] = +740;
			fastoffset[63] = +775;
			fastoffset[64] = +943;
			fastoffset[65] = +974;
			fastoffset[66] = +976;
			fastoffset[67] = +1160;
			fastoffset[68] = +1182;
			fastoffset[69] = +1263;
			fastoffset[70] = +1268;
			fastoffset[71] = +1272;
			fastoffset[72] = +1292;
			fastoffset[73] = +1473;
			fastoffset[74] = +1508;
			fastoffset[75] = +1776;
		}
	}
	int trackmain(void) {
		if(mode == 'f') {
			if(current > 75) {
				mode = 0;
				return 1;
			}
			offset = fastoffset[current] * 4;
			current++;
			return calculate();
		}
		else if(mode == 'p') {
			if(current > 20000) {
				mode = 0;
				return 1;
			}
			offset = current;
			current += 4;
			return calculate();
		}
		else if(mode == 'n') {
			if(current > 20000) {
				mode = 0;
				return 1;
			}
			offset = - current;
			current += 4;
			return calculate();
		}
		else {
			if(current > 20000) {
				mode = 0;
				return 1;
			}
			offset = current;
			if(calculate()) return 1;
			else {
				offset = - current;
				current += 4;
				return calculate();
			}
		}
	}
	u32 calculate(void) {
		crc32 calc;
		if(smallfile) {
			if(isriff) calc.ProcessCRC(riff, 44);
			calc.ProcessCRC(&fcontents[20000 + offset], size);
		}
		else {
			unsigned char buffer;
			unsigned long i;
			fseek(f, start+offset, SEEK_SET);
			if(isriff) calc.ProcessCRC(riff, 44);
			for(i = 0; i < size; i++) {
				if(!fread(&buffer, 1, 1, f)) {
					buffer = 0x00;
					fseek(f, start + offset + i + 1, SEEK_SET);
				}
				calc.ProcessCRC(&buffer, 1);
			}
		}
		printf ("Offset correction %d bytes, %d samples, CRC-32 %08x\n", offset, offset/4, calc.m_crc32);
		return (calc.m_crc32 == crc);
	}
	void done() {
		if(smallfile) free(fcontents);
		if(!mode) printf ("\nCan't find offset!\n");
		else {
			if(savetrack) {
				FILE *f2;
				unsigned char buffer;
				fopen_s ( &f2, path, "w+b" );
				if(isriff) fwrite(riff, 1, 44, f2);
				fseek(f, start + offset, SEEK_SET);
				for(unsigned long i = 0; i < size; i++) {
					if(!fread(&buffer, 1, 1, f)) {
						buffer = 0x00;
						fseek(f, start + offset + i + 1, SEEK_SET);
					}
					fwrite(&buffer, 1, 1, f2);
				}
			}
			printf ("\nDONE!\n\nOffset correction: %d bytes / %d samples\n", offset, offset/4);
		}
	}
};
class block {
private:
	unsigned int blocksize;
	FILE *f1,*f2,*f3,*f4,*f5;
	unsigned char header[12];
public:
	block(char *argv[]) {
		unsigned int len;
		unsigned long blocksize,fsize,fsizeor,fblocks,fblocksor,i,p;
		unsigned char buffer[2336];
		double fblocksd;
		header[0] = 0x00;
		header[1] = 0xFF;
		header[2] = 0xFF;
		header[3] = 0xFF;
		header[4] = 0xFF;
		header[5] = 0xFF;
		header[6] = 0xFF;
		header[7] = 0xFF;
		header[8] = 0xFF;
		header[9] = 0xFF;
		header[10] = 0xFF;
		header[11] = 0x00;

		fopen_s(&f1, argv[2], "rb"); //rebuilded image
		fopen_s(&f2, argv[3], "w+b"); //new image
		fopen_s(&f3, argv[4], "rb"); //file with chesksums

		//Original image
		fseek(f3, 6, SEEK_SET);
		fread(&fsizeor, 4, 1, f3);
		fread(&blocksize, 4, 1, f3);
		fseek(f3, 0, SEEK_END);
		fblocksor = (ftell(f3)-14)/4;
		printf("Old image:\n");
		printf("Block size: %d bytes\n",blocksize);
		printf("Image size: %d bytes\n",fsizeor);
		printf("Number of blocks: %d\n\n",fblocksor);
		fseek( f3, 14, SEEK_SET );

		//Rebuilded image
		fseek( f1, 0, SEEK_END );
		fsize = ftell(f1);
		if(fsize % blocksize != 0 || fsize < blocksize) {
			printf("Wrong filesize!");
			return;
		} 
		fblocksd = fsize / blocksize;
		fblocks = ceil(fblocksd);
		printf("New image:\n");
		printf("Image size: %d bytes\n",fsize);
		printf("Number of blocks: %d\n\n",fblocks);
		fseek( f1, 0, SEEK_SET );

		u32 *imagechecksumsor = (u32*)malloc(fblocksor *4);
		u32 *newimagechecksums = (u32*)malloc(fblocksor *4);
		fread(imagechecksumsor, 4, fblocksor, f3);
		struct blocks {
			u32 adress;
			u32 crc;
		};
		blocks *block1 = (blocks*)malloc(fblocks *8);
		blocks *block2 = (blocks*)malloc(fblocks *8);
		blocks *block3 = (blocks*)malloc(fblocks *8);
		blocks *block4 = (blocks*)malloc(fblocks *8);
		blocks *block5 = (blocks*)malloc(fblocks *8);
		blocks *block6 = (blocks*)malloc(fblocks *8);
		blocks *block7 = (blocks*)malloc(fblocks *8);
		blocks *block8 = (blocks*)malloc(fblocks *8);
		blocks *block9 = (blocks*)malloc(fblocks *8);
		blocks *block10 = (blocks*)malloc(fblocks *8);
		blocks *block11 = (blocks*)malloc(fblocks *8);
		blocks *block12 = (blocks*)malloc(fblocks *8);
		blocks *block13 = (blocks*)malloc(fblocks *8);
		blocks *block14 = (blocks*)malloc(fblocks *8);
		blocks *block15 = (blocks*)malloc(fblocks *8);
		blocks *block16 = (blocks*)malloc(fblocks *8);
		u32 i1 = 0,i2 = 0,i3 = 0,i4 = 0,i5 = 0,i6 = 0,i7 = 0,i8 = 0,i9 = 0,i10 = 0,i11 = 0,i12 = 0,i13 = 0,i14 = 0,i15 = 0,i16 = 0;

		//1) Calculating checksums
		printf("Calculating checksums...\n");
		for(i = 0; i < fblocks; i++) {
			crc32 crccalc;
			if(blocksize==2048) len = fread(buffer, 1, 2048, f1);
			if(blocksize==2352) {
				fseek(f1, 16, SEEK_CUR);
				len = fread(buffer, 1, 2336, f1);
			}
			crccalc.ProcessCRC(buffer, len);
			     if(crccalc.m_crc32 < 0x10000000) {block1[i1].crc = crccalc.m_crc32;block1[i1].adress = i;i1++;}
			else if(crccalc.m_crc32 < 0x20000000) {block2[i2].crc = crccalc.m_crc32;block2[i2].adress = i;i2++;}
			else if(crccalc.m_crc32 < 0x30000000) {block3[i3].crc = crccalc.m_crc32;block3[i3].adress = i;i3++;}
			else if(crccalc.m_crc32 < 0x40000000) {block4[i4].crc = crccalc.m_crc32;block4[i4].adress = i;i4++;}
			else if(crccalc.m_crc32 < 0x50000000) {block5[i5].crc = crccalc.m_crc32;block5[i5].adress = i;i5++;}
			else if(crccalc.m_crc32 < 0x60000000) {block6[i6].crc = crccalc.m_crc32;block6[i6].adress = i;i6++;}
			else if(crccalc.m_crc32 < 0x70000000) {block7[i7].crc = crccalc.m_crc32;block7[i7].adress = i;i7++;}
			else if(crccalc.m_crc32 < 0x80000000) {block8[i8].crc = crccalc.m_crc32;block8[i8].adress = i;i8++;}
			else if(crccalc.m_crc32 < 0x90000000) {block9[i9].crc = crccalc.m_crc32;block9[i9].adress = i;i9++;}
			else if(crccalc.m_crc32 < 0xA0000000) {block10[i10].crc = crccalc.m_crc32;block10[i10].adress = i;i10++;}
			else if(crccalc.m_crc32 < 0xB0000000) {block11[i11].crc = crccalc.m_crc32;block11[i11].adress = i;i11++;}
			else if(crccalc.m_crc32 < 0xC0000000) {block12[i12].crc = crccalc.m_crc32;block12[i12].adress = i;i12++;}
			else if(crccalc.m_crc32 < 0xD0000000) {block13[i13].crc = crccalc.m_crc32;block13[i13].adress = i;i13++;}
			else if(crccalc.m_crc32 < 0xE0000000) {block14[i14].crc = crccalc.m_crc32;block14[i14].adress = i;i14++;}
			else if(crccalc.m_crc32 < 0xF0000000) {block15[i15].crc = crccalc.m_crc32;block15[i15].adress = i;i15++;}
			else if(crccalc.m_crc32 <= 0xFFFFFFFF) {block16[i16].crc = crccalc.m_crc32;block16[i16].adress = i;i16++;}
		}
		//2) Comparing checksums
		printf("Comparing checksums...\n");
		for(i = 0; i < fblocksor; i++) {
			if(*(imagechecksumsor+i) < 0x10000000) {
				for(p = 0; p < i1; p++) {
					if(*(imagechecksumsor+i) == block1[p].crc) {
						*(newimagechecksums+i) = block1[p].adress;
						break;
					}
					else if(p + 1 == i1) *(newimagechecksums+i) = 0xFFFFFFFF;
				}
			}
			else if(*(imagechecksumsor+i) < 0x20000000) {
				for(p = 0; p < i2; p++) {
					if(*(imagechecksumsor+i) == block2[p].crc) {
						*(newimagechecksums+i) = block2[p].adress;
						break;
					}
					else if(p + 1 == i2) *(newimagechecksums+i) = 0xFFFFFFFF;
				}
			}
			else if(*(imagechecksumsor+i) < 0x30000000) {
				for(p = 0; p < i3; p++) {
					if(*(imagechecksumsor+i) == block3[p].crc) {
						*(newimagechecksums+i) = block3[p].adress;
						break;
					}
					else if(p + 1 == i3) *(newimagechecksums+i) = 0xFFFFFFFF;
				}
			}
			else if(*(imagechecksumsor+i) < 0x40000000) {
				for(p = 0; p < i4; p++) {
					if(*(imagechecksumsor+i) == block4[p].crc) {
						*(newimagechecksums+i) = block4[p].adress;
						break;
					}
					else if(p + 1 == i4) *(newimagechecksums+i) = 0xFFFFFFFF;
				}
			}
			else if(*(imagechecksumsor+i) < 0x50000000) {
				for(p = 0; p < i5; p++) {
					if(*(imagechecksumsor+i) == block5[p].crc) {
						*(newimagechecksums+i) = block5[p].adress;
						break;
					}
					else if(p + 1 == i5) *(newimagechecksums+i) = 0xFFFFFFFF;
				}
			}
			else if(*(imagechecksumsor+i) < 0x60000000) {
				for(p = 0; p < i6; p++) {
					if(*(imagechecksumsor+i) == block6[p].crc) {
						*(newimagechecksums+i) = block6[p].adress;
						break;
					}
					else if(p + 1 == i6) *(newimagechecksums+i) = 0xFFFFFFFF;
				}
			}
			else if(*(imagechecksumsor+i) < 0x70000000) {
				for(p = 0; p < i7; p++) {
					if(*(imagechecksumsor+i) == block7[p].crc) {
						*(newimagechecksums+i) = block7[p].adress;
						break;
					}
					else if(p + 1 == i7) *(newimagechecksums+i) = 0xFFFFFFFF;
				}
			}
			else if(*(imagechecksumsor+i) < 0x80000000) {
				for(p = 0; p < i8; p++) {
					if(*(imagechecksumsor+i) == block8[p].crc) {
						*(newimagechecksums+i) = block8[p].adress;
						break;
					}
					else if(p + 1 == i8) *(newimagechecksums+i) = 0xFFFFFFFF;
				}
			}
			else if(*(imagechecksumsor+i) < 0x90000000) {
				for(p = 0; p < i9; p++) {
					if(*(imagechecksumsor+i) == block9[p].crc) {
						*(newimagechecksums+i) = block9[p].adress;
						break;
					}
					else if(p + 1 == i9) *(newimagechecksums+i) = 0xFFFFFFFF;
				}
			}
			else if(*(imagechecksumsor+i) < 0xA0000000) {
				for(p = 0; p < i10; p++) {
					if(*(imagechecksumsor+i) == block10[p].crc) {
						*(newimagechecksums+i) = block10[p].adress;
						break;
					}
					else if(p + 1 == i10) *(newimagechecksums+i) = 0xFFFFFFFF;
				}
			}
			else if(*(imagechecksumsor+i) < 0xB0000000) {
				for(p = 0; p < i11; p++) {
					if(*(imagechecksumsor+i) == block11[p].crc) {
						*(newimagechecksums+i) = block11[p].adress;
						break;
					}
					else if(p + 1 == i11) *(newimagechecksums+i) = 0xFFFFFFFF;
				}
			}
			else if(*(imagechecksumsor+i) < 0xC0000000) {
				for(p = 0; p < i12; p++) {
					if(*(imagechecksumsor+i) == block12[p].crc) {
						*(newimagechecksums+i) = block12[p].adress;
						break;
					}
					else if(p + 1 == i12) *(newimagechecksums+i) = 0xFFFFFFFF;
				}
			}
			else if(*(imagechecksumsor+i) < 0xD0000000) {
				for(p = 0; p < i13; p++) {
					if(*(imagechecksumsor+i) == block13[p].crc) {
						*(newimagechecksums+i) = block13[p].adress;
						break;
					}
					else if(p + 1 == i13) *(newimagechecksums+i) = 0xFFFFFFFF;
				}
			}
			else if(*(imagechecksumsor+i) < 0xE0000000) {
				for(p = 0; p < i14; p++) {
					if(*(imagechecksumsor+i) == block14[p].crc) {
						*(newimagechecksums+i) = block14[p].adress;
						break;
					}
					else if(p + 1 == i14) *(newimagechecksums+i) = 0xFFFFFFFF;
				}
			}
			else if(*(imagechecksumsor+i) < 0xF0000000) {
				for(p = 0; p < i15; p++) {
					if(*(imagechecksumsor+i) == block15[p].crc) {
						*(newimagechecksums+i) = block15[p].adress;
						break;
					}
					else if(p + 1 == i15) *(newimagechecksums+i) = 0xFFFFFFFF;
				}
			}
			else if(*(imagechecksumsor+i) <= 0xFFFFFFFF) {
				for(p = 0; p < i16; p++) {
					if(*(imagechecksumsor+i) == block16[p].crc) {
						*(newimagechecksums+i) = block16[p].adress;
						break;
					}
					else if(p + 1 == i16) *(newimagechecksums+i) = 0xFFFFFFFF;
				}
			}
		}

		//3) Rebuilding image
		printf("Rebuilding image...\n");
		for (i = 0; i < fblocksor; i++) {
			if (newimagechecksums[i]!=0xFFFFFFFF) {
				if (blocksize == 2048) {
					fseek(f1, newimagechecksums[i]*2048, SEEK_SET);
					fread(buffer, 1, 2048, f1);
					fwrite(buffer, 1, 2048, f2);
				} else if(blocksize==2352) {
					fwrite(header, 1, 12, f2);
					block::msf(i);
					putc(0x02,f2);
					fseek(f1, (newimagechecksums[i]*2352)+16, SEEK_SET);
					fread(buffer, 1, 2336, f1);
					fwrite(buffer, 1, 2336, f2);
				}
			} else {
				fseek(f2, blocksize-4, SEEK_CUR);
				putw(0x00000000,f2);
			}
		}
		free(imagechecksumsor);
		free(newimagechecksums);
		free(block1);
		free(block2);
		free(block3);
		free(block4);
		free(block5);
		free(block6);
		free(block7);
		free(block8);
		free(block9);
		free(block10);
		free(block11);
		free(block12);
		free(block13);
		free(block14);
		free(block15);
		free(block16);
		printf("Image was successfully rebuilt!\n");
	}
	void msf(unsigned long lba) {
		lba += 150;
		double mindbl = lba/60/75;
		char min = floor(mindbl);
		double secdbl = (lba-(min*60*75))/75;
		char sec = floor(secdbl);
		char frame = lba-(min*60*75)-(sec*75);
		char buffer[3] = {itob(min), itob(sec), itob(frame)};
		fwrite(buffer,1,3,f2);
	}
};
class makechk {
public:
	makechk(char *argv[]) {
	unsigned long blocksize,fsize,i;
	FILE *f1,*f2;
	unsigned char buffer[2336];
	fopen_s(&f1, argv[2], "rb"); //image
	if(!strcmp(argv[3],"-fm")) blocksize = 2048;
	else if(!strcmp(argv[3],"-im")) blocksize = 2352;
	else {
		printf("Wrong blocksize!");
		return;
	}
	strcat(argv[2],".chk");
	fopen_s(&f2, argv[2], "w+b"); //chk
	fseek(f1, 0, SEEK_END);
	fsize = ftell(f1);
	fseek(f1, 0, SEEK_SET);
	if(fsize % blocksize != 0 || fsize < blocksize) {
		printf("Wrong filesize!");
		return;
	} 
	fwrite("PSXCHK",6,1,f2);
	fwrite(&fsize,4,1,f2);
	fwrite(&blocksize,4,1,f2);
	for(i=0; i < fsize/blocksize; i++) {
		if(blocksize==2048) {
			crc32 crc;
			fread(buffer, 1, 2048, f1);
			crc.ProcessCRC(buffer, 2048);
			fwrite(&crc.m_crc32, 4, 1, f2);
		}
		if(blocksize==2352) {
			crc32 crc;
			fseek(f1, 16, SEEK_CUR);
			fread(buffer, 1, 2336, f1);
			crc.ProcessCRC(buffer, 2336);
			fwrite(&crc.m_crc32, 4, 1, f2);
		}
	}
	}
};
class filetools {
private:
	FILE *f;
	char exename[20];
	char datevalue[11];
public:
	filetools(FILE * file) {
		f = file;
	}
	u32 size() {
		fseek(f, 0, SEEK_END);
		return ftell(f);
	}
	char *exe() {
		char buffer[20];
		u32 lba;
		fseek(f, 51744, SEEK_SET);
		while(strcmp(buffer,"SYSTEM.CNF") != 0)
		{
			fread(buffer, 1, 10, f);
			buffer[10] = 0;
			fseek(f, -9, SEEK_CUR);
		}
		fseek(f, -32, SEEK_CUR);
		fread(&lba, 4, 1, f);
		fseek(f, (2352 * lba) + 29, SEEK_SET);
		fread(&buffer, 1, 6, f);
		buffer[6] = 0;
		while(strcmp(buffer,"cdrom:") != 0) {
			fseek(f, -5, SEEK_CUR);
			fread(&buffer, 1, 6, f);
		}
		fread(&buffer, 1, 1, f);
		if(buffer[0] != '\\') fseek(f, -1, SEEK_CUR);
		long i = -1;
		do fread(&buffer[++i], 1, 1, f); while(buffer[i] != ';');
		for(long a = 0; a < i; a++ ) exename[a] = (char)toupper(buffer[a]);
		exename[i] = 0;
		return exename;
	}
	char *date() {
		char buffer[12],datenofrmt[3];
		fseek( f, 51744, SEEK_SET );
		do {
			fread( buffer, 1, 11, f );
			buffer[11]=NULL;
			fseek( f, -10, SEEK_CUR);
		} while(strcmp(exename,buffer)!=0);
		fseek(f, -16, SEEK_CUR);
		fread(datenofrmt, 1, 3, f);
		if (datenofrmt[0] < 50) itoa(datenofrmt[0]+2000, datevalue, 10);
		else itoa(datenofrmt[0]+1900, datevalue, 10);
		*(datevalue+4) = '-';
		if(datenofrmt[1] < 10) {
			datevalue[5] = '0';
			itoa (datenofrmt[1],&datevalue[6],10);
		} else itoa (datenofrmt[1],&datevalue[5],10);
		*(datevalue+7) = '-';
		if(datenofrmt[2] < 10) {
			datevalue[8] = '0';
			itoa (datenofrmt[2],&datevalue[9],10);
		} else itoa (datenofrmt[2],&datevalue[8],10);
		*(datevalue+10) = 0;
		return datevalue;
	}
	int resize(const char *filename, u32 newsize) {
		u32 oldsize = size();
		if (oldsize == newsize) return 0;
		else if (oldsize < newsize) {
			fseek(f, newsize-1, SEEK_SET);
			fputc(0, f);
			return 1;
		}
		else if (oldsize > newsize) {
			fclose(f);
			wchar_t newfilename[1024];
			for(u8 i = 0; i < strlen(filename); i++) {
				newfilename[i] = filename[i];
				*((char *)newfilename +i*2 +1) = 0;
			}
			newfilename[strlen(filename)] = 0;
			HANDLE hfile = CreateFileW(newfilename, GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);
			SetFilePointer(hfile, newsize, 0, FILE_BEGIN);
			SetEndOfFile(hfile);
			CloseHandle(hfile);
			fopen_s(&f, filename, "rb");
			return 2;
		}
	}
	u32 imagesize() {
		u32 sectors;
		fseek(f, 0x9368, SEEK_SET);
		fread(&sectors, 4, 1, f);
		return sectors;
	}
};

void info(const char * filename) {
	FILE *image;
	crc32 crc;
	u8 buffer[2352];

	if(fopen_s(&image, filename, "rb") != 0) {
		printf("Error opening file \"%s\"!\n",filename);
		return;
	}

	filetools file(image);
	u32 imagesize = getfilesize(image);
	printf("File: %s\n", filename);
	if (imagesize % 2352 != 0) {
		printf("File \"%s\" is not Mode2/2352 image!\n", filename);
		fclose(image);
		return;
	}
	u32 realsectors = file.imagesize();
	u32 imagesectors = imagesize / 2352;
	u32 realsize = realsectors * 2352;
	if (imagesize == realsize) {
		printf("Size (bytes):   %d (OK)\n", imagesize);
		printf("Size (sectors): %d (OK)\n", imagesectors);
	} else {
		printf("Size (bytes):   %d\n", imagesize);
		printf("From image:     %d\n", realsize);
		printf("Size (sectors): %d\n", imagesectors);
		printf("From image:     %d\n", realsectors);
	}


	printf("EDC in Form 2 sectors: ");
	if (getedc(image)) printf("YES\n");
	else printf("NO\n");
	char *exe = file.exe();
	printf("ID: %c%c%c%c-%c%c%c%c%c\n",exe[0],exe[1],exe[2],exe[3],exe[5],exe[6],exe[7],exe[9],exe[10]);
	printf("Date: %s\n",file.date());
	printf("System area: ");
	fseek(image, 0, SEEK_SET);
	for (u32 i = 0; i < 16; i++) {
		fread(buffer, 1, 2352, image);
		crc.ProcessCRC(buffer, 2352);
	}
	u32 imagecrc = crc.m_crc32;
	switch (imagecrc) {
		case 0x11e3052d: printf("Eu EDC"); break;
		case 0x808c19f6: printf("Eu NoEDC"); break;
		case 0x70ffa73e: printf("Eu Alt NoEDC"); break;
		case 0x7f9a25b1: printf("Eu Alt 2 EDC"); break;
		case 0x783aca30: printf("Jap EDC"); break;
		case 0xe955d6eb: printf("Jap NoEDC"); break;
		case 0x9b519a2e: printf("US EDC"); break;
		case 0x0a3e86f5: printf("US NoEDC"); break;
		case 0x6773d4db: printf("US Alt NoEDC"); break;
		default: printf("Unknown, crc %x", imagecrc);
	}
	printf("\n\n");
	fclose(image);
	return;
}
void help() {
	printf("Usage:\n");
	printf("======\n");
	printf("psxt001z.exe image.bin\n");
	printf("  Display image's info.\n\n");
	printf("psxt001z.exe --checksums file [start [end]]\n");
	printf("  Calculate file's checksums (CRC-32, MD5, SHA-1).\n");
	printf("  [in] file   Specifies the file, which checksums will be calculated.\n");
	printf("       start  Specifies start position for checksums calculation.\n");
	printf("       size   Specifies size of block for checksums calculation.\n\n");
	printf("psxt001z.exe --zektor image.bin\n");
	printf("  Zektor. Replace EDC in Form 2 Mode 2 sectors with zeroes.\n\n");
	printf("psxt001z.exe --antizektor image.bin\n");
	printf("  Antizektor. Restore EDC in Form 2 Mode 2 sectors.\n\n");
	printf("psxt001z.exe --resize image.bin size\n");
	printf("  Resize file to requested size.\n\n");
	printf("psxt001z.exe --patch image.bin patch.bin offset\n");
	printf("  Insert patch.bin into image.bin, skipping given number of bytes from the\n  offset.\n\n");
	printf("psxt001z.exe --track image.bin bytes_to_skip size crc-32 [r] [+/-/f] [s filename]\n");
	printf("  Try to guess an offset correction of the image dump by searching a track with\n  given size and CRC-32.\n  r - Calculate crc with RIFF header.\n  +/- - Search only for positive or negative offset correction.\n  f - Search only for common offset corrections.\n  s - Save track with given filename.\n\n");
	printf("psxt001z.exe --makechk image.bin -fm|im\n");
	printf("  Make checksum file with given blocksize:\n  -fm - 2048 bytes/sector.\n  -im - 2352 bytes/sector.\n\n");
	printf("psxt001z.exe --build old_image.bin new_image.bin checksum.chk\n");
	printf("  Rebuild sectors from image to new image using checksum file.\n  USE THIS AT YOUR OWN RISK!\n\n");
	printf("psxt001z.exe --gen file.bin filesize [-r]\n");
	printf("  Generate a file of the requested size.\n  -r - add RIFF header.\n\n");
	printf("psxt001z.exe --str file.str video.str audio.xa\n");
	printf("  Deinterleave file.str to video.str and audio.xa.\n\n");
	printf("psxt001z.exe --str2bs file.str\n");
	printf("  Convert file.str to .bs-files.\n\n");
	printf("psxt001z.exe --makederus badimage.bin goodimage.bin derus.drs\n");
	printf("  Make the patch file for badimage.bin to goodimage in .drs format.\n\n");
	printf("psxt001z.exe --derus image.bin derus.drs\n");
	printf("  Recover image.bin using derus.drs.\n\n");
	printf("psxt001z.exe --scan image.bin\n");
	printf("  Scan image.bin for errors.\n\n");
	printf("psxt001z.exe --fix image.bin\n");
	printf("  Scan image.bin for errors and fix them.\n\n");
	printf("psxt001z.exe --sub subchannel.sub size\n");
	printf("  Generate RAW subchannel with given size (in sectors).\n\n");
	printf("psxt001z.exe --m3s subchannel.m3s\n");
	printf("  Generate M3S subchannel.\n\n");
	printf("psxt001z.exe --libcrypt <sub> [<sbi>]\n");
	printf("Usage: psxt001z.exe --libcrypt <sub> [<sbi>]\n");
	printf("  Check subchannels for LibCrypt protection. (file)\n");
	printf("  [in]  <sub>   Specifies the subchannel file to be scanned.\n");
	printf("  [out] <sbi>   Specifies the subchannel file in SBI format where protected\n  sectors will be written.\n\n");
	printf("psxt001z.exe --libcryptdrv <drive letter>\n");
	printf("  Check subchannels for LibCrypt protection (disc).\n\n");
	printf("psxt001z.exe --libcryptdrvfast <drive letter>\n");
	printf("  Check subchannels for LibCrypt protection using new detection\n  method (disc).\n\n");
	printf("Press any key to continue...");
	_getch();
}
int patch(char * argv[]) {
	FILE * f1, * f2;
	u8 buffer;
	u32 i = 0;
	if (fopen_s(&f1, argv[2], "r+b") != 0) {
		printf("Error opening file \"%s\"!\n", argv[2]);
		return 0;
	}
	if (fopen_s(&f2, argv[3], "rb") != 0) {
		printf("Error opening file \"%s\"!\n", argv[3]);
		return 0;
	}
	u32 skip = atol(argv[4]);
	fseek(f1, skip, SEEK_SET);
	printf("Patching \"%s\" with \"%s\", skipping %d bytes...\n", argv[2], argv[3], skip);
	for (; fread(&buffer, 1, 1, f2); i++) fwrite(&buffer, 1, 1, f1);
	printf("Done!\n\n%d bytes were replaced\n",i);
	printf("File was successully patched!\n");
	return 1;
}
int resize(char * argv[]) {
	FILE *f;
	if(fopen_s(&f, argv[2], "r+b") != 0) {
		printf("Error opening file \"%s\"!\n",argv[2]);
		return 0;
	}
	filetools image(f);
	unsigned long newsize = atol(argv[3]);
	switch(image.resize(argv[2],newsize)) {
		case 0: printf("File's \"%s\" size is already %d bytes!\n",argv[2],newsize);break;
		case 1: printf("File \"%s\" was successfully resized to %d bytes!\n",argv[2],newsize);break;
		case 2: printf("File \"%s\" was successfully truncated to %d bytes!\n",argv[2],newsize);break;
	}
	return 1;
}
int copy(char * argv[]) {
	//argv[1] - infile
	//argv[2] - outfile
	//argv[3] - startbyte
	//argv[4] - length
	char *errorstring;
	FILE * infile;
	__int64 startbyte = _strtoi64(argv[3], &errorstring, 10);
	if (fopen_s(&infile, argv[1], "rb") != 0) {
		printf("File %s can't be found.", argv[1]);
		return 1;
	}
	return 1;
	//HANDLE hfile = CreateFileW(argv[1], GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);
	//SetFilePointer(hfile, newsize, 0, FILE_BEGIN);
	//SetEndOfFile(hfile);
}
int main(int argc, char *argv[]) {
	printf("psxt001z by Dremora, %s\n\n", VERSION);
	     if (argc > 1 && (!strcmp(argv[1], "--checksums") || !strcmp(argv[1], "-c"))) checksums(argc, argv);
	else if (argc > 1 && (!strcmp(argv[1], "--libcrypt") || !strcmp(argv[1], "-l"))) libcrypt(argc - 2, argv + 2);
	else if (argc > 1 && !strcmp(argv[1], "--libcryptdrv")) libcryptdrv(argc - 2, argv + 2);
	else if (argc > 1 && !strcmp(argv[1], "--libcryptdrvfast")) libcryptdrvfast(argc - 2, argv + 2);
	else if (argc > 1 && !strcmp(argv[1], "--xorlibcrypt")) xorlibcrypt();
	
	else if (argc == 2) info(argv[1]);

	else if (argc == 3 && !strcmp(argv[1], "--zektor")) zektor(argv[2]);
	else if (argc == 3 && !strcmp(argv[1], "--antizektor")) antizektor(argv[2]);
	else if (argc == 5 && !strcmp(argv[1], "--patch")) patch(argv);
	else if (argc == 4 && !strcmp(argv[1], "--resize")) resize(argv);
	//else if (argc == 6 && !strcmp(argv[1],"chopfile")) copy(argv);
	else if (argc >= 6 && argc <= 10 && !strcmp(argv[1], "--track")) {track trackfix(argv, argc); while(!trackfix.trackmain()){;} trackfix.done();}
	else if (argc == 5 && !strcmp(argv[1], "--build")) block blockfix(argv);
	else if (argc == 4 && !strcmp(argv[1], "--makechk")) makechk chk(argv);
	else if (argc == 5 && !strcmp(argv[1], "--str")) str(argv);
	else if (argc == 3 && !strcmp(argv[1], "--str2bs")) str2bs(argv);
	else if ((argc == 4 || argc == 5) && !strcmp(argv[1], "--gen")) generate(argc, argv);
	else if (argc == 5 && !strcmp(argv[1], "--makederus")) makederus(argc, argv);
	else if (argc == 4 && !strcmp(argv[1], "--derus")) derus(argv);
	else if (argc == 3 && !strcmp(argv[1], "--scan")) scanfix(argv[2], 0);
	else if (argc == 3 && !strcmp(argv[1], "--fix")) scanfix(argv[2], 1);
	else if (argc == 4 && !strcmp(argv[1], "--sub")) sub(argv[2], argv[3]);
	else if (argc == 3 && !strcmp(argv[1], "--m3s")) m3s(argv[2]);
	else if (argc == 6 && !strcmp(argv[1], "--matrix")) matrix(argv);
	else help();
	return 1;
}

void antizektor(s8 * filename) {
	FILE * image;
	u32 filesize, sectors, sector;
	u8 z, buffer[2332];
	if (fopen_s(&image, filename, "r+b") != 0) {
		perror(filename);
		return;
	}

	static u8  ecc_f_lut[256];
	static u8  ecc_b_lut[256];
	static u32 edc_lut[256];
	u32 a, b, edc_init;
	for(a = 0; a < 256; a++) {
		b = (a << 1) ^ (a & 0x80 ? 0x11D : 0);
		ecc_f_lut[a] = b;
		ecc_b_lut[a ^ b] = a;
		edc_init = a;
		for (b = 0; b < 8; b++) edc_init = (edc_init >> 1) ^ (edc_init & 1 ? 0xD8018001 : 0);
		edc_lut[a] = edc_init;
	}

	filesize = getfilesize(image);
	if (filesize % 2352 != 0) {
		printf("File \"%s\" is not Mode2/2352 image!\n", filename);
		fclose(image);
		return;
	} 
	sectors = filesize / 2352;
	printf("Converting image...\n");
	for (sector = 0; sector < sectors; sector++) {
		fseek(image, sector * 2352 + 18, SEEK_SET);
		fread(&z, 1, 1, image);
		if (z >> 5 & 0x1) {
			fseek(image, -3, SEEK_CUR);
			fread(buffer, 1, 2332, image);
			fseek(image, 0, SEEK_CUR);
			putw(calculate_edc(buffer, 2332, edc_lut), image);
		}
	}
	fclose(image);
	printf("Done!\n");
	return;
}
u8 checksums(int argc, char *argv[]) {
	if (argc < 3 || argc > 5) {
		printf("psxt001z.exe --checksums file [start [end]]\n");
		printf("  Calculate file's checksums (CRC-32, MD5, SHA-1).\n");
		printf("  [in] file   Specifies the file, which checksums will be calculated.\n");
		printf("       start  Specifies start position for checksums calculation.\n");
		printf("       size   Specifies size of block for checksums calculation.\n\n");
		return 0;
	}

	// opening file
	FILE * file;
	if (fopen_s(&file, argv[2], "rb") != 0) {
		perror(argv[2]);
		return 0;
	}
	printf("File:   %s\n", argv[2]);

	s8 *byte;
	u8 percents = 0;
	u32 start, block, filesize, total;

	filesize = getfilesize(file);
	if (argc > 3) {
		start = strtoul(argv[3], &byte, 10);
		if (start >= filesize) {
			printf("Error:  start position can't be larger than filesize!\n");
			return 0;
		}
		printf("Start:  %u\n", start);
	} else
		start = 0;

	if (argc > 4) {
		block = strtoul(argv[4], &byte, 10);
		if (block > filesize) {
			printf("Error:  block size can't be larger than filesize!\n");
			return 0;
		}
		if (block == 0) {
			printf("Error:  block size can't equal with zero!\n");
			return 0;
		}
	} else
		block = filesize - start;
	if (block + start > filesize) {
		printf("Error:  block size and start position can't be larger than file size!\n");
		return 0;
	}
	printf("Size:   %u\n", block);
	total = ceil(double(double(block) / 1024));

	// checksums
	uint8_t Message_Digest[20];
	crc32 crc;
	u8 buffer[1024], digest[16];
	u32 len;
	SHA1Context sha;
	MD5_CTX context;
	MD5Init(&context);
	SHA1Reset(&sha);
	fseek(file, start, SEEK_SET);
	for (u32 i = 0; i < total; i++) {
		if (i * 100 / total > percents) {
			percents = i * 100 / total;
			printf("\rCalculating checksums: %02u%%", percents);
		}
		len = fread(buffer, 1, 1024, file);
		if (block <= len) {
			len = block;
			block = 0;
		} else
			block -= 1024;
		MD5Update(&context, buffer, len);
		SHA1Input(&sha, buffer, len);
		crc.ProcessCRC(buffer, len);
	}
	MD5Final (digest, &context);
	SHA1Result(&sha, Message_Digest);
	printf ("\rCRC-32: %08x                      \n", crc.m_crc32);
	printf ("MD5:    ");
	for (int i = 0; i < 16; i++ )
		printf ( "%02x", digest[i] );
	printf ("\nSHA-1:  ");
	for (int i = 0; i < 20 ; i++ )
		printf ( "%02x", Message_Digest[i] );
	printf ("\n\n");
	return 1;
}

void generate(u32 argc, s8 * argv[]) {
	FILE *f;
	u8 riff[44];
	u32 size = atol(argv[3]);
	if (fopen_s(&f, argv[2], "wb") != 0) {
		printf("Error creating file \"%s\"!\n", argv[2]);
		return;
	}
	if (argc == 5) {
		if (!strcmp(argv[4], "-r")) {
			riff[0] = 0x52;
			riff[1] = 0x49;
			riff[2] = 0x46;
			riff[3] = 0x46;
			riff[8] = 0x57;
			riff[9] = 0x41;
			riff[10] = 0x56;
			riff[11] = 0x45;
			riff[12] = 0x66;
			riff[13] = 0x6D;
			riff[14] = 0x74;
			riff[15] = 0x20;
			riff[16] = 0x10;
			riff[17] = 0x00;
			riff[18] = 0x00;
			riff[19] = 0x00;
			riff[20] = 0x01;
			riff[21] = 0x00;
			riff[22] = 0x02;
			riff[23] = 0x00;
			riff[24] = 0x44;
			riff[25] = 0xAC;
			riff[26] = 0x00;
			riff[27] = 0x00;
			riff[28] = 0x10;
			riff[29] = 0xB1;
			riff[30] = 0x02;
			riff[31] = 0x00;
			riff[32] = 0x04;
			riff[33] = 0x00;
			riff[34] = 0x10;
			riff[35] = 0x00;
			riff[36] = 0x64;
			riff[37] = 0x61;
			riff[38] = 0x74;
			riff[39] = 0x61;
			*(unsigned long*)&riff[4] = size-8;
			*(unsigned long*)&riff[40] = size-44;
			fwrite(riff, 1, 44, f);
		}
	}
	fseek(f, size - 1, SEEK_SET);
	putc(0x00, f);
	fclose(f);
	printf("File \"%s\" with size %d bytes was successfully generated!\n", argv[2], size);
	return;
}
void m3s(s8 * filename) {
	FILE * subchannel;
	u8 buffer[10], min, sec, frame;
	u16 crc;
	u32 sectors, sector, sector2, i;
	double mindbl, secdbl;

	if (fopen_s(&subchannel, filename, "w+b") != 0) {
		perror(filename);
		return;
	}

	printf("File: %s\n", filename);

	buffer[0] = 0x41;
	buffer[1] = 0x01;
	buffer[2] = 0x01;
	buffer[6] = 0x00;

	for (sector = 13350, sector2 = sector + 150; sector < 17850; sector++, sector2++) {
		mindbl = sector/60/75;
		min = floor(mindbl);
		secdbl = (sector - (min * 60 * 75)) / 75;
		sec = floor(secdbl);
		frame = sector - (min * 60 * 75) - (sec * 75);
		buffer[3] = itob(min);
		buffer[4] = itob(sec);
		buffer[5] = itob(frame);

		mindbl = sector2/60/75;
		min = floor(mindbl);
		secdbl = (sector2 - (min * 60 * 75)) / 75;
		sec = floor(secdbl);
		frame = sector2 - (min * 60 * 75) - (sec * 75);
		buffer[7] = itob(min);
		buffer[8] = itob(sec);
		buffer[9] = itob(frame);

		crc = crc16(buffer, 10);
		fwrite(buffer, 1, 10, subchannel);
		putc((u8)*(((u8 *)&(crc))+1), subchannel);
		putc((u8)crc, subchannel);

		for (i = 0; i < 4; i++) putc(0x00, subchannel);
		printf("Creating M3S: %02u%%\r", (100*sector) / sectors);
	}
	printf("Creating M3S: 100%%\n");
	fclose(subchannel);
	printf("Done!\n");
	return;
}


void matrix(s8 * argv[]) {
	FILE *f1, *f2, *f3, *f4;
	fopen_s ( &f1, argv[2], "r+b");
	fopen_s ( &f2, argv[3], "r+b");
	fopen_s ( &f3, argv[4], "r+b");
	fopen_s ( &f4, argv[5], "w+b");

	u8 r1, r2, r3;
	u32 subsize, i = 0;
	subsize = getfilesize(f1);
	for (; i < subsize; i++)
	{
		fread(&r1, 1, 1, f1);
		fread(&r2, 1, 1, f2);
		fread(&r3, 1, 1, f3);
		if (r1 == r2) fwrite(&r1, 1, 1, f4);
		else if (r1 == r3) fwrite(&r1, 1, 1, f4);
		else if (r2 == r3)fwrite(&r2, 1, 1, f4);
		else {
			printf("Byte 0x%x (%d) is different!\n%s: %02x\n%s: %02x\n%s: %02x\n\n", i, i, argv[2], r1, argv[3], r2, argv[4], r3);
			return;
		}
	}
	printf("Done!\n");
	return;
}
void scanfix(s8 *filename, u8 fix) {
	FILE *image;
	crc32 crc;
	u8 buffer[2352];
	u32 i, imagesector, imagesectors, realsectors, imagesize, realsize;

	if (fopen_s(&image, filename, "r+b") != 0) {
		perror(filename);
		return;
	}

	imagesize = getfilesize(image);
	printf("File: %s\n", filename);
	if (imagesize % 2352 != 0) {
		printf("File \"%s\" is not Mode2/2352 image!\n", filename);
		fclose(image);
		return;
	}
	imagesectors = imagesize / 2352;
	fseek(image, 0x9368, SEEK_SET);
	fread(&realsectors, 4, 1, image);
	fseek(image, 0, SEEK_SET);
	realsize = realsectors * 2352;
	if (imagesectors == realsectors) {
		printf("Size (bytes):   %d (OK)\n", imagesize);
		printf("Size (sectors): %d (OK)\n", imagesectors);
	} else {
		printf("Size (bytes):   %d\n", imagesize);
		printf("From image:     %d\n", realsize);
		printf("Size (sectors): %d\n", imagesectors);
		printf("From image:     %d\n", realsectors);
	}

	u8 imageedc = getedc(image);
	printf("EDC in Form 2 sectors: ");
	if (imageedc) printf("YES\n");
	else printf("NO\n");
	printf("System area: ");
	fseek(image, 0, SEEK_SET);
	for (u32 i = 0; i < 16; i++) {
		fread(buffer, 1, 2352, image);
		crc.ProcessCRC(buffer, 2352);
	}
	u32 imagecrc = crc.m_crc32;
	switch (imagecrc) {
		case 0x11e3052d: printf("Eu EDC"); break;
		case 0x808c19f6: printf("Eu NoEDC"); break;
		case 0x70ffa73e: printf("Eu Alt NoEDC"); break;
		case 0x783aca30: printf("Jap EDC"); break;
		case 0xe955d6eb: printf("Jap NoEDC"); break;
		case 0x9b519a2e: printf("US EDC"); break;
		case 0x0a3e86f5: printf("US NoEDC"); break;
		case 0x6773d4db: printf("US Alt NoEDC"); break;
		default: printf("Unknown, crc %x", imagecrc);
	}

	u8 synсheader[16] = {0x00, 0xFF, 0xFF, 0xFF,
						 0xFF, 0xFF, 0xFF, 0xFF, 
						 0xFF, 0xFF, 0xFF, 0x00, 
						 0x00, 0x00, 0x00, 0x02};
	u8 subheader08[8] = {0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00};
	u8 subheader09[8] = {0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x09, 0x00};
	u8 subheader20[8] = {0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00};
	u8 subheader89[8] = {0x00, 0x00, 0x89, 0x00, 0x00, 0x00, 0x89, 0x00};
	u32 subheader08count = 0;
	u32 subheader09count = 0;
	u32 subheader20count = 0;
	u32 subheader89count = 0;
	u32 subheaderothercount = 0;
	u32 form1count = 0;
	u32 form2count = 0;
	u32 badheaderscount = 0;
	u8 bad, form2, edc3f[4] = {0x3F, 0x13, 0xB0, 0xBE};
	u32 edc;

	/* EDC/ECC Init*/
	static u8  ecc_f_lut[256];
	static u8  ecc_b_lut[256];
	static u32 edc_lut[256];
	u32 a, b, edc_init;
	for(a = 0; a < 256; a++) {
		b = (a << 1) ^ (a & 0x80 ? 0x11D : 0);
		ecc_f_lut[a] = b;
		ecc_b_lut[a ^ b] = a;
		edc_init = a;
		for (b = 0; b < 8; b++) edc_init = (edc_init >> 1) ^ (edc_init & 1 ? 0xD8018001 : 0);
		edc_lut[a] = edc_init;
	}

	printf("\n\nChecking data...");
	for (u32 imagesector = 0; imagesector < imagesectors; imagesector++) {
		bad = 0;
		fseek(image, imagesector * 2352, SEEK_SET);
		fread(buffer, 1, 2352, image);

		//Sync
		msf(imagesector, synсheader + 12);
		if (!buffercmp(synсheader, buffer, 12)) {
			if (bad) printf(", Sync");
			else {
				printf("\nSector %u: Sync", imagesector);
				bad = 1;
			}
			if (fix) {
				fseek(image, imagesector * 2352, SEEK_SET);
				fwrite(synсheader, 1, 12, image);
				printf(" (fixed)");
			}
		}

		//Header
		if (!buffercmp(synсheader + 12, buffer + 12, 4)) {
			if (bad) printf(", Header");
			else {
				printf("\nSector %u: Header", imagesector);
				bad = 1;
			}
			if (fix) {
				fseek(image, imagesector * 2352 + 12, SEEK_SET);
				fwrite(synсheader + 12, 1, 4, image);
				printf(" (fixed)");
			}
		}

		//Subheader
		if (*(buffer + 18) >> 5 & 0x1) {
			form2 = 1;
			form2count++;
		} else {
			form2 = 0;
			form1count++;
		}
		if (!buffercmp(buffer + 16, buffer + 20, 4)) badheaderscount++;
		if (buffercmp(buffer + 16, subheader08, 8)) subheader08count++;
		else if (buffercmp(buffer + 16, subheader09, 8)) subheader09count++;
		else if (buffercmp(buffer + 16, subheader20, 8)) subheader20count++;
		else if (buffercmp(buffer + 16, subheader89, 8)) subheader89count++;
		else subheaderothercount++;

		//Form 2
		if (form2) {
			//if EDC in Form 2
			if (imageedc) {
				edc = calculate_edc(buffer + 16, 2332, edc_lut);
				//printf("\nSector %u: Form 2 Data/EDC, %02x %02x %02x %02x, %02x %02x %02x %02x", imagesector, *(buffer + 2348), *(buffer + 2349), *(buffer + 2350), *(buffer + 2351), *(u8 *)&edc, *(((u8 *)&edc)+1), *(((u8 *)&edc)+2), *(((u8 *)&edc)+3));
				if (!buffercmp(buffer + 2348, (u8 *)&edc, 4)) {
					if (bad) printf(", Form 2 Data/EDC");
					else {
						printf("\nSector %u: Form 2 Data/EDC", imagesector);
						bad = 1;
					}
					if (fix) printf(" (can't fix)");
				}
				//if no EDC in Form 2
			} else {
				if (!zerocmp(buffer + 2348, 4)) {
					if (bad) printf(", EDC");
					else {
						printf("\nSector %u: EDC", imagesector);
						bad = 1;
					}
					if (fix) {
						fseek(image, imagesector * 2352 + 2348, SEEK_SET);
						for (i = 0; i < 4; i++) putc(0x00, image);
						printf(" (fixed)");
					}
				}
			}
		} else {
			edc = calculate_edc(buffer + 16, 2056, edc_lut);
			if (!buffercmp(buffer + 2072, (u8 *)&edc, 4)) {
				if (bad) printf(", Form 1 Data/EDC");
				else {
					printf("\nSector %u: Form 1 Data/EDC", imagesector);
					bad = 1;
				}
				if (fix) printf(" (fixing later...)");
			}
		}
	}

	printf("\n\nSubheaders count:\n");
	printf("00 00 08 00: %u\n", subheader08count);
	printf("00 00 09 00: %u\n", subheader09count);
	printf("00 00 20 00: %u\n", subheader20count);
	printf("00 00 89 00: %u\n", subheader89count);
	printf("      Other: %u\n\n", subheaderothercount);
	printf("Form 1 sectors: %u\n", form1count);
	printf("Form 2 sectors: %u\n", form2count);
	printf(" \"Bad\" headers: %u\n\n", badheaderscount);

	printf("Checking postgap...\n");
	fseek(image, (imagesectors - 150) * 2352 + 18, SEEK_SET);
	fread(&form2, 1, 1, image);
	form2 =  form2 >> 5 & 0x1;
	if (form2) printf("Postgap type: Form 2\n");
	else printf("Postgap type: Form 1\n");
	for (imagesector = imagesectors - 150; imagesector < imagesectors; imagesector++) {
		bad = 0;
		fseek(image, imagesector * 2352, SEEK_SET);
		fread(buffer, 1, 2352, image);
		if (form2) {
			if (!buffercmp(buffer + 16, subheader20, 8)) {
				if (bad) printf(", Form 2 Subheader");
				else {
					printf("\nSector %u: Form 2 Subheader", imagesector);
					bad = 1;
				}
				if (fix) {
					fseek(image, imagesector * 2352 + 16, SEEK_SET);
					fwrite(subheader20, 1, 8, image);
					printf(" (fixed)");
				}
			}
			if (!zerocmp(buffer + 24, 2324)) {
				if (bad) printf(", Form 2 Data");
				else {
					printf("\nSector %u: Form 2 Data", imagesector);
					bad = 1;
				}
				if (fix) {
					fseek(image, imagesector * 2352 + 24, SEEK_SET);
					for (i = 0; i < 2324; i++) putc(0x00, image);
					printf(" (fixed)");
				}
			}
			if (imageedc && !buffercmp(buffer + 2348, edc3f, 4)) {
				if (bad) printf(", Form 2 EDC");
				else {
					printf("\nSector %u: Form 2 EDC", imagesector);
					bad = 1;
				}
				if (fix) {
					fseek(image, imagesector * 2352 + 2348, SEEK_SET);
					fwrite(edc3f, 1, 4, image);
					printf(" (fixed)");
				}
			} else if (!imageedc && !zerocmp(buffer + 2348, 4)) {
				if (bad) printf(", Form 2 EDC");
				else {
					printf("\nSector %u: Form 2 EDC", imagesector);
					bad = 1;
				}
				if (fix) {
					fseek(image, imagesector * 2352 + 2348, SEEK_SET);
					for (i = 0; i < 4; i++) putc(0x00, image);
					printf(" (fixed)");
				}
			}
		} else if (!zerocmp(buffer + 16, 2336)) {
			printf("\nSector %u: Form 1 Subheader/Data/EDC/ECC", imagesector);
			bad = 1;
			if (fix) {
				fseek(image, imagesector * 2352 + 16, SEEK_SET);
				for (i = 0; i < 2336; i++) putc(0x00, image);
				printf(" (fixed)");
			}
		}
	}
	printf("\n\n");
	fclose(image);
	return;
}
void str(s8 * argv[]) {
	FILE * str, * audio, * video;
	u8 ctrlbyte, buffer[2336];
	u32 filesize, sectors;

	if (fopen_s(&str, argv[2], "rb") != 0) {
		printf("Error opening file \"%s\" for reading!\n", argv[2]);
		return;
	}
	filesize = getfilesize(str);
	if (filesize % 2336 != 0) {
		printf("File \"%s\" is not in STR format!\n", argv[2]);
		fclose(str);
		return;
	}
	sectors = filesize / 2336;
	if (fopen_s(&video, argv[3], "wb") != 0) {
		printf("Error creating file \"%s\"!\n", argv[3]);
		return;
	}
	sectors = filesize / 2336;
	if (fopen_s(&audio, argv[4], "wb") != 0) {
		printf("Error creating file \"%s\"!\n", argv[4]);
		return;
	}
	for (u32 i = 0; i < sectors; i++)  {
		fseek(str, 2, SEEK_CUR);
		fread(&ctrlbyte, 1, 1, str);
		if (ctrlbyte >> 5 & 0x1) {
			fseek(str, -3, SEEK_CUR);
			fread(&buffer, 1, 2336, str);
			fwrite(&buffer, 1, 2336, audio);
		}
		else {
			fseek(str, 5, SEEK_CUR);
			fread(&buffer, 1, 2048, str);
			fwrite(&buffer, 1, 2048, video);
			fseek(str, 280, SEEK_CUR);
		}
	}
	fclose(str);
	fclose(audio);
	fclose(video);
	printf("Done!\n");
	return;
}
void str2bs(s8 * argv[]) {
	FILE * str, * bs;					// инициализация файловых потоков
	u8 buffer[2016];						// инициализация буффера
	u8 byte;							// инициализация проверяемого байта
	u16 filenamesize = strlen(argv[2]);		// инициализация длины пути
	s8 * filename = (s8*)calloc(1024, 1);	// инициализация filename
	strcpy(filename, argv[2]);				// записываем путь + имя str-файла в filename
	strcat(filename, "-bs");				// записываем "-bs" в filename
	if (fopen_s(&str, argv[2], "rb") != 0) {
		perror(argv[2]);
		return;
	}
	u32 filesize = getfilesize(str);		// инициализация размера файла
	if (filesize % 2048 != 0) {
		printf("File \"%s\" is not in STR format!\n", argv[2]);
		fclose(str);
		return;
	}

	/*wchar_t newfilename[2048];
	for (u8 f = 0; f < strlen(filename); f++) {
		newfilename[f] = filename[f];
		*((char *)newfilename +f*2 +1) = 0;
	}*/
	CreateDirectoryA((const char *)filename, 0);
	//CreateDirectoryW((const wchar_t *)char2wchar_t((u8 *)filename), 0);	// создаем папку с именем filename
	printf("%u\n", GetLastError());
	strcpy(&*(filename + filenamesize + 3), "\\000001.bs");				// записываем "\000001.bs" в filename
	u32 numblocks = filesize / 2048;									// инициализация кол-ва блоков
	fopen_s(&bs, (const char *)filename, "wb");							// открываем .bs-файл
	u16 a = 1;															//
	u16 b = 0;															// счетчики 
	u16 c = 0;															//
	u8 ax[2] = {0, 0};
	u8 bx[2] = {0, 0};
	u8 cx[2] = {0, 0};
	fseek(str, 32, SEEK_CUR);
	fread(&buffer, 1, 2016, str);
	printf("%s\n",filename);
	fwrite(&buffer, 1, 2016, bs);
	printf("2\n");
	printf("Creating: %s\n", filename);	

	for (u32 i = 1; i < numblocks; i++) {
		fseek(str, 1, SEEK_CUR);
		fread(&byte, 1, 1, str);
		if (!byte) {
			if (a < 99) a++;
			else if (b < 99) {b++; a = 0;}
			else {c++; b = 0; a = 0;}
			fclose(bs);
			itoa(a, (char *)ax, 10);
			itoa(b, (char *)bx, 10);
			itoa(c, (char *)cx, 10);
			if (c < 10) {
				*(filename + filenamesize + 4) = '0';
				strcpy(&*(filename + filenamesize + 5), (const char *)cx);
			} else strcpy(&*(filename + filenamesize + 4), (const char *)cx);

			if (b < 10) {
				*(filename + filenamesize + 6) = '0';
				strcpy(&*(filename + filenamesize + 7), (const char *)bx);
			} else strcpy(&*(filename + filenamesize + 6), (const char *)bx);

			if (a < 10) {
				*(filename + filenamesize + 8) = '0';
				strcpy(&*(filename + filenamesize + 9), (const char *)ax);
			} else strcpy(&*(filename + filenamesize + 8), (const char *)ax);

			*(filename + filenamesize + 10) = '.';

			if (fopen_s(&bs, (const char *)filename, "wb") != 0) printf("Can't open file!\n");
			printf("Creating: %s\n", filename);	
		}
		fseek(str, 30, SEEK_CUR);
		fread(&buffer, 1, 2016, str);
		fwrite(&buffer, 1, 2016, bs);
	}
	fclose(bs);
	fclose(str);
	printf("\nDone!\n");
	free(filename);
	return;
}
void sub(s8 * filename, s8 * strsectors) {
	FILE * subchannel;
	u8 buffer[10], min, sec, frame;
	s8 * end;
	u16 crc;
	u32 sector, sector2, sectors, i;
	double mindbl, secdbl;

	sectors = strtoul(strsectors, &end, 10);
	if (sectors == 0 || sectors == -1) {
		printf("Wrong size!\n");
		return;
	}

	if (fopen_s(&subchannel, filename, "w+b") != 0) {
		perror(filename);
		return;
	}

	printf("File: %s\n", filename);
	printf("Size (bytes): %d\n", sectors * 96);
	printf("Size (sectors): %d\n", sectors);

	buffer[0] = 0x41;
	buffer[1] = 0x01;
	buffer[2] = 0x01;
	buffer[6] = 0x00;

	for (sector = 0, sector2 = 150; sector < sectors; sector++, sector2++) {
		mindbl = sector/60/75;
		min = floor(mindbl);
		secdbl = (sector - (min * 60 * 75)) / 75;
		sec = floor(secdbl);
		frame = sector - (min * 60 * 75) - (sec * 75);
		buffer[3] = itob(min);
		buffer[4] = itob(sec);
		buffer[5] = itob(frame);

		mindbl = sector2/60/75;
		min = floor(mindbl);
		secdbl = (sector2 - (min * 60 * 75)) / 75;
		sec = floor(secdbl);
		frame = sector2 - (min * 60 * 75) - (sec * 75);
		buffer[7] = itob(min);
		buffer[8] = itob(sec);
		buffer[9] = itob(frame);

		crc = crc16(buffer, 10);

		for (i = 0; i < 12; i++) putc(0x00, subchannel);
		fwrite(buffer, 1, 10, subchannel);
		putc((u8)*(((u8 *)&(crc))+1), subchannel);
		putc((u8)crc, subchannel);
		for (i = 0; i < 72; i++) putc(0x00, subchannel);

		printf("Creating: %02u%%\r", (100*sector) / sectors);
	}
	fseek(subchannel, 0, SEEK_SET);
	for (i = 0; i < 12; i++) putc(0xFF, subchannel);
	printf("Creating: 100%%\n");
	fclose(subchannel);
	printf("Done!\n");
	return;
}
void zektor(s8 * filename) {
	FILE * image;
	u32 filesize, sectors, sector;
	u8 z, zero[4] = {0x00, 0x00, 0x00, 0x00};
	if (fopen_s(&image, filename, "r+b") != 0) {
		perror(filename);
		return;
	}
	filesize = getfilesize(image);
	if (filesize % 2352 != 0) {
		printf("File \"%s\" is not Mode2/2352 image!\n", filename);
		fclose(image);
		return;
	} 
	sectors = filesize / 2352;
	printf("Converting image...\n");
	for (sector = 0; sector < sectors; sector++) {
		fseek(image, sector * 2352 + 18, SEEK_SET);
		fread(&z, 1, 1, image);
		if (z >> 5 & 0x1) {
			fseek(image, 2329, SEEK_CUR);
			fwrite(zero, 1, 4, image);
		}
	}
	fclose(image);
	printf("Done!\n");
	return;
}