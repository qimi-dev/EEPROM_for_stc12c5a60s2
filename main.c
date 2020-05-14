#include "STC12C5A60S2.h"
#include <intrins.h>



/* ���� ISP/IAP/EEPROM ���� */
#define CMD_IDLE     0    //Stand-By
#define CMD_READ     1    //���ֽ�
#define CMD_PROGRAM  2    //д�ֽ�
#define CMD_ERASE    3    //��������

/* ���� ISP/IAP/EEPROM �в�������IAP_CONTR */
#define ENABLE_IAP   0x82

/* ���忪ʼ��ַ */
#define IAP_ADDRESS 0x0000   


/* --------------------------------------------------
	�ر� ISP/IAP/EEPROM �������õ�Ƭ�����ڰ�ȫ״̬
-------------------------------------------------- */
void IapIdle(){
	IAP_CONTR = 0;         // �ر�IAP����
	IAP_CMD = 0;		   // �������
	IAP_TRIG = 0;		   // ��������Ĵ���
	IAP_ADDRH = 0x80;      // ���ݵ�ַָ���EEPROM������
	IAP_ADDRL = 0;         // ���IAP��ַ����ֹ����
}

/* ---------------------
	����Ӧ��ַ������
--------------------- */
unsigned char IapReadByte(unsigned int addr){
	unsigned int dat;
	
	IAP_CONTR = ENABLE_IAP;       // ��IAP����
	IAP_CMD = CMD_READ;           // ���ö�����
	IAP_ADDRL = addr;             // ���õͰ�λ��ַ
	IAP_ADDRH = addr >> 8;        // ���ø߰�λ��ַ
	IAP_TRIG = 0x5A;              // ���ô�������
	IAP_TRIG = 0xa5;              // ���ô�������
	_nop_();
	
	dat = IAP_DATA;               // ��ȡ����
	IapIdle();                    // �رշ���
	
	return dat;
}

/* ---------------------
	���Ӧ��ַд����
--------------------- */
void IapProgramByte(unsigned int addr, unsigned char dat){
	IAP_CONTR = ENABLE_IAP;       // ��IAP����
	IAP_CMD = CMD_PROGRAM;        // ����д����
	IAP_ADDRL = addr;             // ���õͰ�λ��ַ
	IAP_ADDRH = addr >> 8;        // ���ø߰�λ��ַ
	IAP_DATA = dat;               // д����
	IAP_TRIG = 0x5A;              // ���ô�������
	IAP_TRIG = 0xa5;              // ���ô�������
	_nop_();
	
	IapIdle();                    // �رշ���
}

/* ---------------------
	������Ӧ��ַ����
--------------------- */
void IapEraseSector(unsigned int addr){
	IAP_CONTR = ENABLE_IAP;       // ��IAP����
	IAP_CMD = CMD_ERASE;          // ����д������
	IAP_ADDRL = addr;             // ���õͰ�λ��ַ
	IAP_ADDRH = addr >> 8;        // ���ø߰�λ��ַ
	IAP_TRIG = 0x5A;              // ���ô�������
	IAP_TRIG = 0xa5;              // ���ô�������
	_nop_();
	
	IapIdle();                    // �رշ���
}


void main(){
	/* �������������� */
	IapEraseSector(0x0000);
	IapEraseSector(0x0200);
	/* ��0x0000д��һ���ֽ� */
	IapProgramByte(0x0000, 0x01);
	/* ��ȡ0x0000������ */
	IapReadByte(0x0000);
}




