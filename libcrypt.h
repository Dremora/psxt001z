#ifndef __LIBCRYPT_H__
#define __LIBCRYPT_H__

typedef struct ScsiPassThroughDirect {
	WORD Length;
	BYTE ScsiStatus;
	BYTE PathId;
	BYTE TargetId;
	BYTE Lun;
	BYTE CDBLength;
	BYTE SenseInfoLength;
	BYTE DataIn;
	DWORD DataTransferLength;
	DWORD TimeOutValue;
	PVOID DataBuffer;
	DWORD SenseInfoOffset;
	BYTE CDB[16];
} ScsiPassThroughDirect;

#define IOCTL_SCSI_PASS_THROUGH_DIRECT 0x4D014
#define SCSI_IOCTL_DATA_IN             0x1
#define RAW_READ_CMD                   0xBE
#define BUFFER_LEN                     96
#define SENSE_SIZE                     0 //14
#define F_NAME                        "sectors.log"
#define CYCLES                         5
#define LIBCRYPT_NUM_SECTORS           64
#define READ_TIMES                     5

u8 libcrypt(int argc, char *argv[]);
u8 libcryptdrv(int argc, char *argv[]);
int libcryptdrvfast(int argc, char *argv[]);
void ReadSub(LPBYTE, DWORD, FILE *, char, HANDLE, ScsiPassThroughDirect);
void ClearCache(LPBYTE, FILE *, char, HANDLE, ScsiPassThroughDirect);
BYTE matrix(LPBYTE, LPBYTE, LPBYTE, LPBYTE, DWORD);
void deinterleave(LPBYTE);
int xorlibcrypt(void);

#endif