/************************************************************************************************************
 *  Copyright Notice
 *  Copyright (c) 2018,邓豪
 *
 *  @File    : SerialTableCommand.h
 *  @Brief   : 云台控制的命令组
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

//#define CMD_DOWN			(0x10)	//俯
//#define CMD_UP			(0x08)	//仰
//#define CMD_LEFT			(0x04)	//左
//#define CMD_RIGHT			(0x02)	//右
#define CMD_STOP			(0x00)	//停

#define CMD_OPEN_AUX		(0x09)	//打开辅助开关
#define CMD_CLOSE_AUX		(0x0B)	//关闭辅助开关

#define CMD_SET_PRESET		(0x03)	//设置预置位
#define CMD_CLEAR_PRESET	(0x05)	//删除预置位
#define CMD_CALL_PRESET		(0x07)	//调用预置位

#define CMD_ASK_H_ANGLE		(0x51)	//水平角度查询
#define CMD_ASK_V_ANGLE		(0x53)	//俯仰角度查询
#define CMD_BACK_H_ANGLE	(0x59)	//水平角度回传
#define CMD_BACK_V_ANGLE	(0x5B)	//俯仰角度回传

#define CMD_LOC_H_ANGLE		(0x4B)	//水平角度定位
#define CMD_LOC_V_ANGLE		(0x4D)	//俯仰角度定位
#define CMD_SET_LOC_SPEED	(0x5F)	//定位速度设置

#define CMD_RESET_TABLE		(0x29)	//恢复云台出厂参数

#define CMD_ASK_LOC_SPEED	(0x73)	//定位速度查询
#define CMD_BACK_LOC_SPEED	(0x73)	//定位速度回传

#endif // !SERIAL_TABLE_CONTROL_COMMAND_H_
