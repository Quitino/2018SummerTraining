/************************************************************************************************************
 *  Copyright Notice
 *  Copyright (c) 2018,黄鹤
 *
 *  @File    : ControlStrategy.h
 *  @Brief   : 发现垃圾后，关于垃圾角度和距离的控制策略分析
 *
 *  @Version : V1.0
 *  @Date    : 2018/5/1
 
 *	@History : 
 *    Author : Hughie  E_mail:denghao@mails.swust.edu.cn
 *    Descrip: Creat File
 *************************************************************************************************************/
#ifndef CONTROL_STRATEGY_H_
#define CONTROL_STRATEGY_H_

#include <iostream>

namespace IPSG
{
	/***********************************************
	 *  @Name    : ControlStrategy
	 *  @Descrip : 控制策略
	 *  @Para    : [in]		angel：漂浮物相对于船中心线的角度
	 *  @Para    : [in]		distance：漂浮物相对于升降杆安装点的距离
	 *  @Para    : [in]		delta_t：距离上一次数据发送的时间
	 *	@Para    : [in/out]	fAngle：需要发送至控制组的角度或第一个数据
	 *	@Para    : [in/out] fDistance：需要发送至控制组的距离或第二个数据
	 *  @Return  : bool true:success false:failed
	 *  @Notes   : 此部分内容为通信组黄鹤所写，由邓豪集成，黄鹤联系方式：QQ 373565467
	 ***********************************************/
	bool ControlStrategy(float angel, float distance, float &fAngle, float &fDistance);
}

#endif // !CONTROL_STRATEGY_H_
