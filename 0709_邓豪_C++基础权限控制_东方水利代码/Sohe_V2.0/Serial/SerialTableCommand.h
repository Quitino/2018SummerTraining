/************************************************************************************************************
 *  Copyright Notice
 *  Copyright (c) 2018,�˺�
 *
 *  @File    : SerialTableCommand.h
 *  @Brief   : ��̨���Ƶ�������
 *
 *  @Version : V1.0
 *  @Date    : 2018/1/23
 
 *	@History : 
 *    Author : Hughie  E_mail:denghao@mails.swust.edu.cn
 *    Descrip: Creat File
 *************************************************************************************************************/
#ifndef SERIAL_TABLE_CONTROL_COMMAND_H_
#define SERIAL_TABLE_CONTROL_COMMAND_H_

#include <iostream>

//#define CMD_DOWN			(0x10)	//��
//#define CMD_UP			(0x08)	//��
//#define CMD_LEFT			(0x04)	//��
//#define CMD_RIGHT			(0x02)	//��
#define CMD_STOP			(0x00)	//ͣ

#define CMD_OPEN_AUX		(0x09)	//�򿪸�������
#define CMD_CLOSE_AUX		(0x0B)	//�رո�������

#define CMD_SET_PRESET		(0x03)	//����Ԥ��λ
#define CMD_CLEAR_PRESET	(0x05)	//ɾ��Ԥ��λ
#define CMD_CALL_PRESET		(0x07)	//����Ԥ��λ

#define CMD_ASK_H_ANGLE		(0x51)	//ˮƽ�ǶȲ�ѯ
#define CMD_ASK_V_ANGLE		(0x53)	//�����ǶȲ�ѯ
#define CMD_BACK_H_ANGLE	(0x59)	//ˮƽ�ǶȻش�
#define CMD_BACK_V_ANGLE	(0x5B)	//�����ǶȻش�

#define CMD_LOC_H_ANGLE		(0x4B)	//ˮƽ�Ƕȶ�λ
#define CMD_LOC_V_ANGLE		(0x4D)	//�����Ƕȶ�λ
#define CMD_SET_LOC_SPEED	(0x5F)	//��λ�ٶ�����

#define CMD_RESET_TABLE		(0x29)	//�ָ���̨��������

#define CMD_ASK_LOC_SPEED	(0x73)	//��λ�ٶȲ�ѯ
#define CMD_BACK_LOC_SPEED	(0x73)	//��λ�ٶȻش�

#endif // !SERIAL_TABLE_CONTROL_COMMAND_H_
