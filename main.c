#include "STC12C5A60S2.h"
#include <intrins.h>



/* 定义 ISP/IAP/EEPROM 命令 */
#define CMD_IDLE     0    //Stand-By
#define CMD_READ     1    //读字节
#define CMD_PROGRAM  2    //写字节
#define CMD_ERASE    3    //扇区擦除

/* 定义 ISP/IAP/EEPROM 中操作常量IAP_CONTR */
#define ENABLE_IAP   0x82

/* 定义开始地址 */
#define IAP_ADDRESS 0x0000   


/* --------------------------------------------------
	关闭 ISP/IAP/EEPROM 方法，让单片机处于安全状态
-------------------------------------------------- */
void IapIdle(){
	IAP_CONTR = 0;         // 关闭IAP方法
	IAP_CMD = 0;		   // 清楚命令
	IAP_TRIG = 0;		   // 清楚触发寄存器
	IAP_ADDRH = 0x80;      // 数据地址指向非EEPROM的区域
	IAP_ADDRL = 0;         // 清楚IAP地址，防止滥用
}

/* ---------------------
	读对应地址的数据
--------------------- */
unsigned char IapReadByte(unsigned int addr){
	unsigned int dat;
	
	IAP_CONTR = ENABLE_IAP;       // 打开IAP方法
	IAP_CMD = CMD_READ;           // 设置读命令
	IAP_ADDRL = addr;             // 设置低八位地址
	IAP_ADDRH = addr >> 8;        // 设置高八位地址
	IAP_TRIG = 0x5A;              // 设置触发命令
	IAP_TRIG = 0xa5;              // 设置触发命令
	_nop_();
	
	dat = IAP_DATA;               // 读取数据
	IapIdle();                    // 关闭方法
	
	return dat;
}

/* ---------------------
	向对应地址写数据
--------------------- */
void IapProgramByte(unsigned int addr, unsigned char dat){
	IAP_CONTR = ENABLE_IAP;       // 打开IAP方法
	IAP_CMD = CMD_PROGRAM;        // 设置写命令
	IAP_ADDRL = addr;             // 设置低八位地址
	IAP_ADDRH = addr >> 8;        // 设置高八位地址
	IAP_DATA = dat;               // 写数据
	IAP_TRIG = 0x5A;              // 设置触发命令
	IAP_TRIG = 0xa5;              // 设置触发命令
	_nop_();
	
	IapIdle();                    // 关闭方法
}

/* ---------------------
	擦除对应地址扇区
--------------------- */
void IapEraseSector(unsigned int addr){
	IAP_CONTR = ENABLE_IAP;       // 打开IAP方法
	IAP_CMD = CMD_ERASE;          // 设置写擦除令
	IAP_ADDRL = addr;             // 设置低八位地址
	IAP_ADDRH = addr >> 8;        // 设置高八位地址
	IAP_TRIG = 0x5A;              // 设置触发命令
	IAP_TRIG = 0xa5;              // 设置触发命令
	_nop_();
	
	IapIdle();                    // 关闭方法
}


void main(){
	/* 擦除所有扇区的 */
	IapEraseSector(0x0000);
	IapEraseSector(0x0200);
	/* 往0x0000写进一个字节 */
	IapProgramByte(0x0000, 0x01);
	/* 读取0x0000的数据 */
	IapReadByte(0x0000);
}




