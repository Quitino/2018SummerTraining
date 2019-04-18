/************************************************************************************************************
 *  Copyright Notice
 *  Copyright (c) 2018,�ƺ�
 *
 *  @File    : ControlStrategy.h
 *  @Brief   : ���������󣬹��������ǶȺ;���Ŀ��Ʋ��Է���
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
	 *  @Descrip : ���Ʋ���
	 *  @Para    : [in]		angel��Ư��������ڴ������ߵĽǶ�
	 *  @Para    : [in]		distance��Ư��������������˰�װ��ľ���
	 *  @Para    : [in]		delta_t��������һ�����ݷ��͵�ʱ��
	 *	@Para    : [in/out]	fAngle����Ҫ������������ĽǶȻ��һ������
	 *	@Para    : [in/out] fDistance����Ҫ������������ľ����ڶ�������
	 *  @Return  : bool true:success false:failed
	 *  @Notes   : �˲�������Ϊͨ����ƺ���д���ɵ˺����ɣ��ƺ���ϵ��ʽ��QQ 373565467
	 ***********************************************/
	bool ControlStrategy(float angel, float distance, float &fAngle, float &fDistance);
}

#endif // !CONTROL_STRATEGY_H_
