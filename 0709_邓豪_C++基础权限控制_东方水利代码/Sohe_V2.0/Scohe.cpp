#include "Scohe.h"

bool IPSG::CScohe::ModelInital()
{
	FILE_LOG(logDEBUG) << "CScohe::ModelsInital() call";

	std::string strTopCameraSrc = "rtsp://admin:Adminhik@192.168.0.64:554/mjpeg/ch1/sub/av_stream";  //搜索相机初始化，
	//std::string strDownCameraSrc = "rtsp://admin:Adminhik@192.168.0.63:554/mjpeg/ch2/sub/av_stream";

	if (!HIKCameraTop.OpenCamera(strTopCameraSrc))
	{
		FILE_LOG(logINFO) << "Can not open top camera";
		return false;
	}
	//if (!HIKCameraDown.OpenCamera(strDownCameraSrc))	//下方相机暂时不用
	//{
	//	FILE_LOG(logINFO) << "Can not open down camera";
	//	return false;
	//}

	//通信串口
	CSerialBase* pCommunicationTemp = new CSerialCommunication;
	if (!pCommunicationTemp->InitPort(3, 9600))
	{
		FILE_LOG(logINFO) << "Fail to inital the serial port for communication !";
		return false;
	}
	else FILE_LOG(logINFO) << "Success to inital the serial port for communication !";

	if (!pCommunicationTemp->OpenListenThread())
	{
		FILE_LOG(logINFO) << "Fail to inital listen thread for communication !";
		return false;
	}
	else FILE_LOG(logINFO) << "Success to inital listen thread for communication !";
	pCommunication = dynamic_cast<CSerialCommunication*>(pCommunicationTemp);

	//云台控制串口
	CSerialBase* pTableControTemp = new CSerialTableControl;
	if (! pTableControTemp->InitPort(4, 9600))
	{
		FILE_LOG(logINFO) << "Fail to inital the serial port for table control !";
		return false;
	}
	else FILE_LOG(logINFO) << "Success to inital the serial port for table control !";

	if (! pTableControTemp->OpenListenThread())
	{
		FILE_LOG(logINFO) << "Fail to inital listen thread for table control !";
		return false;
	}
	else FILE_LOG(logINFO) << "Success to inital listen thread for table control !";
	pTableControl = dynamic_cast<CSerialTableControl *>(pTableControTemp);  //有父类指针转换到子类指针

	//网络通信
	if (! pServer->ServerInit(9600, "192.168.0.65"))
	{
		FILE_LOG(logINFO) << "网络初始化失败！";
		return false;
	}
	else FILE_LOG(logINFO) << "网络初始化成功！";

	if (! pServer->OpenServerListenThread())
	{
		FILE_LOG(logINFO) << "服务器监听绑定失败！";
		return false;
	}
	else FILE_LOG(logINFO) << "服务器监听绑定成功！";

	//云台巡航点数据存储
	m_vTableLocPoint.clear();
	for (size_t i = 0; i < m_TableCruisePara.m_nTableVertPointNO; i++)	//水平横扫完再到下一垂直点
	{
		int nVertLastLocPoint = m_TableCruisePara.m_nTableVertInitLoc - i*m_TableCruisePara.m_nTableLocVertStep;
		for (size_t j = 0; j < m_TableCruisePara.m_nTableHoriPointNO; j++)
		{
			int nHoriLastLocPoint = m_TableCruisePara.m_nTableHoriInitLoc;
			if (i % 2 == 0)
			{
				nHoriLastLocPoint = m_TableCruisePara.m_nTableHoriInitLoc + j*m_TableCruisePara.m_nTableLocHoriStep;
			}
			else
			{
				nHoriLastLocPoint = m_TableCruisePara.m_nTableHoriInitLoc + m_TableCruisePara.m_nTableHoriAngleRange - j*m_TableCruisePara.m_nTableLocHoriStep;
			}
			m_vTableLocPoint.push_back(TableLocPoint(nHoriLastLocPoint, nVertLastLocPoint));
		}
	}

	//让云台执行一次自检
	//pTableControl->CallPresetN(101);
	//Sleep(3000);

	FILE_LOG(logDEBUG) << "CScohe::ModelsInital() return";
	return true;
}

bool IPSG::CScohe::Process()
{
	FILE_LOG(logDEBUG) << "CScohe::Process call";
	
	while (true)
	{
		//if (true == pServer->m_bCruise)
		//{
			FILE_LOG(logINFO) << "接受到客户端控制信号，开始巡航";
			TableCruise();
		//}
	}
	
	FILE_LOG(logDEBUG) << "CScohe::Process return";
	return true;
}

bool IPSG::CScohe::TableCruise()
{
	FILE_LOG(logDEBUG) << "CScohe::TableCruise call";
	
	int nCruiseNO = 0;	//监听变量
	FILE_LOG(logINFO) << "开始云台巡航";
	pTableControl->SetLocSpeed(48, 48);   //设置云台的巡航速度，不能超过63

	if (0 == m_vTableLocPoint.size())
	{
		FILE_LOG(logINFO) << "云台巡航参数设置错误";
		return false;
	}

	for (size_t i = 0; i < m_vTableLocPoint.size();i++)
	{
		STableLocPoint TableLocNextPoint(STableLocPoint(m_vTableLocPoint.at(i).m_fHoriLocPoint, m_vTableLocPoint.at(i).m_fVertLocPoint));
		FILE_LOG(logINFO) << "云台开始定位，目标定位点为：(" << m_vTableLocPoint.at(i).m_fHoriLocPoint << "," << m_vTableLocPoint.at(i).m_fVertLocPoint << ")";

		float fNextHoriPoint = m_vTableLocPoint.at(i).m_fHoriLocPoint;
		float fNextVertPoint = m_vTableLocPoint.at(i).m_fVertLocPoint;
		if (fNextHoriPoint > m_TableLimit.m_fTableRightThd || fNextHoriPoint < m_TableLimit.m_fTableLeftThd
			|| fNextVertPoint > m_TableLimit.m_fTableDownThd4Cruise || fNextVertPoint < m_TableLimit.m_fTableTopThd)
		{
			FILE_LOG(logINFO) << "云台目标定位点超过限制";
			continue;
		}

		float fCurrentHoriPoint = 0.0;   //云台当前的水平角度
		float fCurrentVertPoint = 0.0;   //云台当前的俯仰角度
		do 
		{
			pTableControl->GetHoriAngle(fCurrentHoriPoint);
			Sleep(30);
			pTableControl->GetVertAngle(fCurrentVertPoint);
			Sleep(30);

			pTableControl->HVLocate(TableLocNextPoint.m_fHoriLocPoint, TableLocNextPoint.m_fVertLocPoint);  //云台定位到指定点
		} while (std::abs(fCurrentHoriPoint - TableLocNextPoint.m_fHoriLocPoint) > 1 ||
			std::abs(fCurrentVertPoint - TableLocNextPoint.m_fVertLocPoint) > 1);

		pTableControl->TableStop();		//让云台停止

		FILE_LOG(logINFO) << "云台定位成功，云台当前位置为：(" << fCurrentHoriPoint << "," << fCurrentVertPoint << ")";
		FILE_LOG(logINFO) << "开始读取图像并处理";

		//尽管有容错，但是此处再检测一次，以防意外极端情况
		if (fCurrentHoriPoint > m_TableLimit.m_fTableRightThd || fCurrentHoriPoint < m_TableLimit.m_fTableLeftThd
			|| fCurrentVertPoint > m_TableLimit.m_fTableDownThd4Cruise || fCurrentVertPoint < m_TableLimit.m_fTableTopThd)
		{
			FILE_LOG(logINFO) << "当前云台位置异常";
			pTableControl->CallPresetN(101);	//云台自检，即云台归零位
			TableCruise();
		}

		Sleep(200);
		if (!CamTopImgProc(fCurrentHoriPoint, fCurrentVertPoint))
		{
			FILE_LOG(logINFO) << "当前定位点没有疑是漂浮物" << std::endl << std::endl;
			continue;
		}
			

		FILE_LOG(logINFO) << "当前点巡航结束，开始下一个点巡航";
	}

	//发送让云台停止指令  不要发送复位!!!!!
	pTableControl->TableStop();

	//当前点巡航完成，发送去下一个点的指令
	FILE_LOG(logINFO) << "当前点巡航完成，开始去下一个作业点" << std::endl << std::endl << std::endl;
	pServer->SendData("east1");
	pServer->m_bCruise = false;

	FILE_LOG(logDEBUG) << "CScohe::TableCruise return";
	return true;
}

bool IPSG::CScohe::TableFollow(SFloaterMsg FollowFloater)
{
	FILE_LOG(logDEBUG) << "CScohe::TableFollow call";
	
	pTableControl->SetLocSpeed(48, 48);	//定位速度

	FILE_LOG(logINFO) << "云台开始跟踪，目标与船的距离为：" << FollowFloater.m_fDistanceViaRobot << "，角度为：" << FollowFloater.m_fHoriAngleViaRobot - 185;

	float fTableRotPara = 0.35;
	if (FollowFloater.m_fDistanceViaRobot > 5.3)
		fTableRotPara = 0.5;

	float fCurHoriLoc = FollowFloater.m_fHoriAngleViaRobot - FollowFloater.m_Floater.m_fHoriAngle;	//云台当前位置
	float fCurVertLoc = FollowFloater.m_fVertAngleViaRobot - FollowFloater.m_Floater.m_fVertAngle;
	
	float fNextHoriLoc = fCurHoriLoc + FollowFloater.m_Floater.m_fHoriAngle*fTableRotPara;			//云台目标定位点
	float fNextVertLoc = fCurVertLoc + FollowFloater.m_Floater.m_fVertAngle*fTableRotPara;

	if (fNextHoriLoc > m_TableLimit.m_fTableRightThd || fNextHoriLoc < m_TableLimit.m_fTableLeftThd
		|| fNextVertLoc > m_TableLimit.m_fTableDownThd4Track || fNextVertLoc < m_TableLimit.m_fTableTopThd)
	{
		FILE_LOG(logINFO) << "目标跟踪时，云台目标定位点超过限制，重新进行巡航";
		TableCruise();
	}

	float fNextHoriLocNorm = std::abs(fNextHoriLoc - 185);
	float fNextVertLocNorm = fNextVertLoc;
	if (fNextHoriLocNorm < 15 && fNextVertLocNorm >= m_fVertThd2CamVC)
	{
		FILE_LOG(logINFO) << "云台跟踪结束，开启拖链和刷子，并重新进行巡航";
		CtrlDataSend4Brush(BRUSH_OPEN);	//加关闭控制
		TableCruise();
	}

	//只有明显差的比较多的时候，才转动云台，细小差别不转动
	if (std::abs(fCurHoriLoc - fNextHoriLoc) > 10 || std::abs(fCurVertLoc - fNextVertLoc) > 5)
	{
		FILE_LOG(logINFO) << "云台跟随中......";
		float fCurrentHoriPoint = 0.0;
		float fCurrentVertPoint = 0.0;
		do
		{
			pTableControl->GetHoriAngle(fCurrentHoriPoint);
			Sleep(30);
			pTableControl->GetVertAngle(fCurrentVertPoint);
			Sleep(30);

			pTableControl->HVLocate(fNextHoriLoc, fNextVertLoc);  //云台定位到指定点
		} while (std::abs(fCurrentHoriPoint - fNextHoriLoc) > 1 ||
			std::abs(fCurrentVertPoint - fNextVertLoc) > 1);
	}

	if (!CamTopImgProc(fCurHoriLoc, fCurVertLoc))
	{
		FILE_LOG(logINFO) << "不存在垃圾或垃圾已被清理，重新开始巡航";
		TableCruise();
	}	
		
	FILE_LOG(logDEBUG) << "CScohe::TableFollow return";
	return true;
}

bool IPSG::CScohe::CamTopImgProc(const float fCurrentHoriPoint, const float fCurrentVertPoint)
{
	FILE_LOG(logDEBUG) << "CScohe::CamTopImgProc call";
	
	cv::Mat SrcImage;
	do 
	{
		HIKCameraTop.ReadImage(SrcImage);
	} while (SrcImage.empty());

	FILE_LOG(logINFO) << "显著性处理...";
	std::vector<SSaliencyRegion> vSaliencyRegion;     //存储图像中所有显著性区域
	if ( !Saliency.ImageSaliency(SrcImage,vSaliencyRegion))
	{
		FILE_LOG(logINFO) << "当前图像中没有疑是漂浮物";
		SrcImage.release();
		return false;
	}

	//有垃圾则中心对齐
	std::vector<SFloaterMsg> vFloater;
	SaveFloaterMsg(vSaliencyRegion, fCurrentHoriPoint, fCurrentVertPoint, vFloater);
	vSaliencyRegion.clear();	//数据存储完成之后，删除缓存

	FILE_LOG(logINFO) << "当前图像中存在" << vFloater.size() << "个疑是漂浮物";
	if (0 == vFloater.size())
	{
		FILE_LOG(logINFO) << "当前图像中没有疑是漂浮物";
		SrcImage.release();
		vFloater.clear();
		return false;
	}

	//第一个int为key值，按key值升序排列，将漂浮物转换成厘米单位存入key值进行排序
	std::multimap<int, int, std::less<int>> mulmapFloater;	
	mulmapFloater.clear();
	for (int i = 0; i < vFloater.size(); i++)
	{
		mulmapFloater.insert(std::pair<int, int>(static_cast<int>(vFloater.at(i).m_fDistanceViaRobot * 100), i));
	}

	//取出待跟踪的目标
	SFloaterMsg AlignFloater = vFloater.at(0);
	mulmapFloater.clear();
	vFloater.clear();

	if (! CamCertralAlign(AlignFloater, 0.65))
	{
		FILE_LOG(logINFO) << "对疑是漂浮物的中心对齐失败";
		return false;
	}

	/************************重检测防误判处理*********************************/
	//如果成功完成中心对齐，则再次拍摄图片进行检测识别，预防误判
	do
	{
		HIKCameraTop.ReadImage(SrcImage);
	} while (SrcImage.empty());
	

	if (!Saliency.ImageSaliency(SrcImage, vSaliencyRegion))
	{
		FILE_LOG(logINFO) << "中心对齐后再次取的图中没有疑是漂浮物";
		SrcImage.release();
		return false;
	}
	
	SaveFloaterMsg(vSaliencyRegion, fCurrentHoriPoint, fCurrentVertPoint, vFloater);
	vSaliencyRegion.clear();	//数据存储完成之后，删除缓存

	FILE_LOG(logINFO) << "中心对齐后再次取的图中存在" << vFloater.size() << "个疑是漂浮物";
	if (0 == vFloater.size())
	{
		FILE_LOG(logINFO) << "中心对齐后再次取的图像中没有疑是漂浮物";
		SrcImage.release();
		vFloater.clear();
		return false;
	}

	//确认存在疑是漂浮物，取出距离最近的
	for (int i = 0; i < vFloater.size(); i++)
	{
		mulmapFloater.insert(std::pair<int, int>(static_cast<int>(vFloater.at(i).m_fDistanceViaRobot * 100), i));
	}
	SFloaterMsg FollowFloater = vFloater.at(0);

	//if (!Classifier.RecongitionClassifier(FollowFloater.m_Floater.m_SaliencyRegion))
	//{
	//	FILE_LOG(logINFO) << "中心对齐后再次取的图像中的疑是漂浮物不是垃圾";
	//	return false;
	//}

	cv::rectangle(SrcImage, FollowFloater.m_Floater.m_SaliencyRect, cv::Scalar(0, 0, 255));
	cv::imshow("The floater to be followed", SrcImage);
	cv::waitKey(10);

	//数据下发
	FILE_LOG(logINFO) << "确认存在垃圾，现在将方位信息进行控制策略解析及下发";
	float fAngle4Sgy = FollowFloater.m_fHoriAngleViaRobot - 185;	//由于控制策略中定义正前方是0°，而云台控制中正前方为185°，所以需要归一化
	float fDistance4Sgy = FollowFloater.m_fDistanceViaRobot;
	float fAngle4Send = 0.0, fDistance4Send = 0.0;
	ControlStrategy(fAngle4Sgy, fDistance4Sgy, fAngle4Send, fDistance4Send);	//控制策略解析
	int nAngle4Send = static_cast<int>(fAngle4Send * 100);
	int nDistance4Send = static_cast<int>(fDistance4Send);
	if (!CtrlSgyDataSend(nAngle4Send, nDistance4Send))								//控制数据发送
		FILE_LOG(logINFO) << "控制数据下发失败";
	FILE_LOG(logINFO) << "控制数据下发成功";

	//进入跟踪
	FILE_LOG(logINFO) << "确认存在垃圾目标，进入跟踪" << std::endl << std::endl;
	TableFollow(FollowFloater);

	FILE_LOG(logDEBUG) << "CScohe::CamTopImgProc return";
	return true;
}

bool IPSG::CScohe::SaveFloaterMsg(std::vector<SSaliencyRegion> vSaliencyRegion, float fCurrentHoriPoint, float fCurrentVertPoint,
	std::vector<SFloaterMsg>& vFloater)
{
	FILE_LOG(logDEBUG) << "CScohe::SaveFloaterMsg call";
	
	if (0 == vSaliencyRegion.size())
	{
		FILE_LOG(logINFO) << "Saliency region is empty";
		return false;
	}

	for (size_t i = 0; i < vSaliencyRegion.size(); i++)
	{
		if (vSaliencyRegion.at(i).m_SaliencyRect.area() < 100)
			continue;

		SFloaterMsg FloaterMsgTemp;
		FloaterMsgTemp.m_Floater = vSaliencyRegion.at(i);
		FloaterMsgTemp.m_fHoriAngleViaRobot = vSaliencyRegion.at(i).m_fHoriAngle + fCurrentHoriPoint;
		FloaterMsgTemp.m_fVertAngleViaRobot = vSaliencyRegion.at(i).m_fVertAngle + fCurrentVertPoint;
		if (90 == FloaterMsgTemp.m_fVertAngleViaRobot || 0 == FloaterMsgTemp.m_fVertAngleViaRobot)
			FloaterMsgTemp.m_fDistanceViaRobot = 0.0;//两种情况 角度为90是错误的 角度为0也是错的
		else
			FloaterMsgTemp.m_fDistanceViaRobot = m_fRobotHeightViaWater / (tan((FloaterMsgTemp.m_fVertAngleViaRobot / 360) * 2 * PI));

		vFloater.push_back(FloaterMsgTemp);
	}
	
	FILE_LOG(logDEBUG) << "CScohe::SaveFloaterMsg return";
	return true;
}

bool IPSG::CScohe::CamCertralAlign(SFloaterMsg FollowFloater, float fTableRotPara)
{
	FILE_LOG(logDEBUG) << "CScohe::CamCerterAlign call";
	
	if (fTableRotPara > 1 || fTableRotPara <= 0)
	{
		FILE_LOG(logINFO) << "中心对齐时云台转动系数错误";
		return false;
	}

	//云台当前位置
	float fCurHoriLoc = FollowFloater.m_fHoriAngleViaRobot - FollowFloater.m_Floater.m_fHoriAngle;
	float fCurVertLoc = FollowFloater.m_fVertAngleViaRobot - FollowFloater.m_Floater.m_fVertAngle;
	
	float fNextHoriLoc = fCurHoriLoc + FollowFloater.m_Floater.m_fHoriAngle*fTableRotPara;
	float fNextVertLoc = fCurVertLoc + FollowFloater.m_Floater.m_fVertAngle*fTableRotPara;

	if (fNextHoriLoc > m_TableLimit.m_fTableRightThd || fNextHoriLoc < m_TableLimit.m_fTableLeftThd
		|| fNextVertLoc > m_TableLimit.m_fTableDownThd4Track || fNextVertLoc < m_TableLimit.m_fTableTopThd)
	{
		FILE_LOG(logINFO) << "中心对齐时，云台目标定位点超过限制";
		return false;
	}

	float fCurrentHoriPoint = 0.0;
	float fCurrentVertPoint = 0.0;
	do
	{
		pTableControl->GetHoriAngle(fCurrentHoriPoint);
		Sleep(30);
		pTableControl->GetVertAngle(fCurrentVertPoint);
		Sleep(30);

		pTableControl->HVLocate(fNextHoriLoc, fNextVertLoc);  //云台定位到指定点
	} while (std::abs(fCurrentHoriPoint - fNextHoriLoc) > 1 ||
		std::abs(fCurrentVertPoint - fNextVertLoc) > 1);
	
	FILE_LOG(logINFO) << "云台中心对齐完成";

	FILE_LOG(logDEBUG) << "CScohe::CamCerterAlign return";
	return true;
}

bool IPSG::CScohe::CtrlSgyDataSend(int nAngle4Send, int nDistance4Send)
{
	FILE_LOG(logDEBUG) << "CScohe::CtrlSgyDataSend call";
	
	int nSendDataRequestNO = 0;

	while (nSendDataRequestNO++ < 50)
	{
		if (pCommunication->m_bSendFlag == true && pCommunication->m_bBusBusyFlag == false)
		{
			std::vector<BYTE> vData;
			pCommunication->ECode(nAngle4Send, nDistance4Send, vData);

			BYTE ucData[10];
			for (size_t i = 0; i < 10; i++)
				ucData[i] = vData[i];

			if (!pCommunication->WriteData(ucData, 10))
			{
				FILE_LOG(logINFO) << "控制数据下发失败";
				return false;
			}	
			else
			{
				FILE_LOG(logINFO) << "控制数据下发成功";
				return true;
			}
		}
	}
	FILE_LOG(logINFO) << "控制策略数据发送请求次数超过限制，请检查串口是否正常";
	FILE_LOG(logDEBUG) << "CScohe::CtrlSgDataSend return";

	return false;
}

bool IPSG::CScohe::CtrlDataSend4Brush(int nSwitch)
{
	FILE_LOG(logDEBUG) << "CScohe::CtrlDataSend4Brush call";
	
	int nSendDataRequestNO = 0;
	while (nSendDataRequestNO++ < 50)
	{
		if (pCommunication->m_bSendFlag == true && pCommunication->m_bBusBusyFlag == false)
		{
			std::vector<BYTE> vData;
			pCommunication->ECode4Brush(nSwitch, vData);

			BYTE ucData[10];
			for (size_t i = 0; i < 10; i++)
				ucData[i] = vData[i];

			if (!pCommunication->WriteData(ucData, 10))
			{
				FILE_LOG(logINFO) << "拖链控制数据下发失败";
				return false;
			}
			else
			{
				FILE_LOG(logINFO) << "拖链控制数据下发成功";
				return true;
			}
		}
	}
	FILE_LOG(logINFO) << "拖链数据发送请求次数超过限制，请检查串口是否正常";
	
	FILE_LOG(logDEBUG) << "CScohe::CtrlDataSend4Brush return";
	return true;
}