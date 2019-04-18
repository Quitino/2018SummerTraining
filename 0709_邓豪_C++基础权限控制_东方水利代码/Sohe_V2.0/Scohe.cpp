#include "Scohe.h"

bool IPSG::CScohe::ModelInital()
{
	FILE_LOG(logDEBUG) << "CScohe::ModelsInital() call";

	std::string strTopCameraSrc = "rtsp://admin:Adminhik@192.168.0.64:554/mjpeg/ch1/sub/av_stream";  //���������ʼ����
	//std::string strDownCameraSrc = "rtsp://admin:Adminhik@192.168.0.63:554/mjpeg/ch2/sub/av_stream";

	if (!HIKCameraTop.OpenCamera(strTopCameraSrc))
	{
		FILE_LOG(logINFO) << "Can not open top camera";
		return false;
	}
	//if (!HIKCameraDown.OpenCamera(strDownCameraSrc))	//�·������ʱ����
	//{
	//	FILE_LOG(logINFO) << "Can not open down camera";
	//	return false;
	//}

	//ͨ�Ŵ���
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

	//��̨���ƴ���
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
	pTableControl = dynamic_cast<CSerialTableControl *>(pTableControTemp);  //�и���ָ��ת��������ָ��

	//����ͨ��
	if (! pServer->ServerInit(9600, "192.168.0.65"))
	{
		FILE_LOG(logINFO) << "�����ʼ��ʧ�ܣ�";
		return false;
	}
	else FILE_LOG(logINFO) << "�����ʼ���ɹ���";

	if (! pServer->OpenServerListenThread())
	{
		FILE_LOG(logINFO) << "������������ʧ�ܣ�";
		return false;
	}
	else FILE_LOG(logINFO) << "�����������󶨳ɹ���";

	//��̨Ѳ�������ݴ洢
	m_vTableLocPoint.clear();
	for (size_t i = 0; i < m_TableCruisePara.m_nTableVertPointNO; i++)	//ˮƽ��ɨ���ٵ���һ��ֱ��
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

	//����ִ̨��һ���Լ�
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
			FILE_LOG(logINFO) << "���ܵ��ͻ��˿����źţ���ʼѲ��";
			TableCruise();
		//}
	}
	
	FILE_LOG(logDEBUG) << "CScohe::Process return";
	return true;
}

bool IPSG::CScohe::TableCruise()
{
	FILE_LOG(logDEBUG) << "CScohe::TableCruise call";
	
	int nCruiseNO = 0;	//��������
	FILE_LOG(logINFO) << "��ʼ��̨Ѳ��";
	pTableControl->SetLocSpeed(48, 48);   //������̨��Ѳ���ٶȣ����ܳ���63

	if (0 == m_vTableLocPoint.size())
	{
		FILE_LOG(logINFO) << "��̨Ѳ���������ô���";
		return false;
	}

	for (size_t i = 0; i < m_vTableLocPoint.size();i++)
	{
		STableLocPoint TableLocNextPoint(STableLocPoint(m_vTableLocPoint.at(i).m_fHoriLocPoint, m_vTableLocPoint.at(i).m_fVertLocPoint));
		FILE_LOG(logINFO) << "��̨��ʼ��λ��Ŀ�궨λ��Ϊ��(" << m_vTableLocPoint.at(i).m_fHoriLocPoint << "," << m_vTableLocPoint.at(i).m_fVertLocPoint << ")";

		float fNextHoriPoint = m_vTableLocPoint.at(i).m_fHoriLocPoint;
		float fNextVertPoint = m_vTableLocPoint.at(i).m_fVertLocPoint;
		if (fNextHoriPoint > m_TableLimit.m_fTableRightThd || fNextHoriPoint < m_TableLimit.m_fTableLeftThd
			|| fNextVertPoint > m_TableLimit.m_fTableDownThd4Cruise || fNextVertPoint < m_TableLimit.m_fTableTopThd)
		{
			FILE_LOG(logINFO) << "��̨Ŀ�궨λ�㳬������";
			continue;
		}

		float fCurrentHoriPoint = 0.0;   //��̨��ǰ��ˮƽ�Ƕ�
		float fCurrentVertPoint = 0.0;   //��̨��ǰ�ĸ����Ƕ�
		do 
		{
			pTableControl->GetHoriAngle(fCurrentHoriPoint);
			Sleep(30);
			pTableControl->GetVertAngle(fCurrentVertPoint);
			Sleep(30);

			pTableControl->HVLocate(TableLocNextPoint.m_fHoriLocPoint, TableLocNextPoint.m_fVertLocPoint);  //��̨��λ��ָ����
		} while (std::abs(fCurrentHoriPoint - TableLocNextPoint.m_fHoriLocPoint) > 1 ||
			std::abs(fCurrentVertPoint - TableLocNextPoint.m_fVertLocPoint) > 1);

		pTableControl->TableStop();		//����ֹ̨ͣ

		FILE_LOG(logINFO) << "��̨��λ�ɹ�����̨��ǰλ��Ϊ��(" << fCurrentHoriPoint << "," << fCurrentVertPoint << ")";
		FILE_LOG(logINFO) << "��ʼ��ȡͼ�񲢴���";

		//�������ݴ����Ǵ˴��ټ��һ�Σ��Է����⼫�����
		if (fCurrentHoriPoint > m_TableLimit.m_fTableRightThd || fCurrentHoriPoint < m_TableLimit.m_fTableLeftThd
			|| fCurrentVertPoint > m_TableLimit.m_fTableDownThd4Cruise || fCurrentVertPoint < m_TableLimit.m_fTableTopThd)
		{
			FILE_LOG(logINFO) << "��ǰ��̨λ���쳣";
			pTableControl->CallPresetN(101);	//��̨�Լ죬����̨����λ
			TableCruise();
		}

		Sleep(200);
		if (!CamTopImgProc(fCurrentHoriPoint, fCurrentVertPoint))
		{
			FILE_LOG(logINFO) << "��ǰ��λ��û������Ư����" << std::endl << std::endl;
			continue;
		}
			

		FILE_LOG(logINFO) << "��ǰ��Ѳ����������ʼ��һ����Ѳ��";
	}

	//��������ָֹ̨ͣ��  ��Ҫ���͸�λ!!!!!
	pTableControl->TableStop();

	//��ǰ��Ѳ����ɣ�����ȥ��һ�����ָ��
	FILE_LOG(logINFO) << "��ǰ��Ѳ����ɣ���ʼȥ��һ����ҵ��" << std::endl << std::endl << std::endl;
	pServer->SendData("east1");
	pServer->m_bCruise = false;

	FILE_LOG(logDEBUG) << "CScohe::TableCruise return";
	return true;
}

bool IPSG::CScohe::TableFollow(SFloaterMsg FollowFloater)
{
	FILE_LOG(logDEBUG) << "CScohe::TableFollow call";
	
	pTableControl->SetLocSpeed(48, 48);	//��λ�ٶ�

	FILE_LOG(logINFO) << "��̨��ʼ���٣�Ŀ���봬�ľ���Ϊ��" << FollowFloater.m_fDistanceViaRobot << "���Ƕ�Ϊ��" << FollowFloater.m_fHoriAngleViaRobot - 185;

	float fTableRotPara = 0.35;
	if (FollowFloater.m_fDistanceViaRobot > 5.3)
		fTableRotPara = 0.5;

	float fCurHoriLoc = FollowFloater.m_fHoriAngleViaRobot - FollowFloater.m_Floater.m_fHoriAngle;	//��̨��ǰλ��
	float fCurVertLoc = FollowFloater.m_fVertAngleViaRobot - FollowFloater.m_Floater.m_fVertAngle;
	
	float fNextHoriLoc = fCurHoriLoc + FollowFloater.m_Floater.m_fHoriAngle*fTableRotPara;			//��̨Ŀ�궨λ��
	float fNextVertLoc = fCurVertLoc + FollowFloater.m_Floater.m_fVertAngle*fTableRotPara;

	if (fNextHoriLoc > m_TableLimit.m_fTableRightThd || fNextHoriLoc < m_TableLimit.m_fTableLeftThd
		|| fNextVertLoc > m_TableLimit.m_fTableDownThd4Track || fNextVertLoc < m_TableLimit.m_fTableTopThd)
	{
		FILE_LOG(logINFO) << "Ŀ�����ʱ����̨Ŀ�궨λ�㳬�����ƣ����½���Ѳ��";
		TableCruise();
	}

	float fNextHoriLocNorm = std::abs(fNextHoriLoc - 185);
	float fNextVertLocNorm = fNextVertLoc;
	if (fNextHoriLocNorm < 15 && fNextVertLocNorm >= m_fVertThd2CamVC)
	{
		FILE_LOG(logINFO) << "��̨���ٽ���������������ˢ�ӣ������½���Ѳ��";
		CtrlDataSend4Brush(BRUSH_OPEN);	//�ӹرտ���
		TableCruise();
	}

	//ֻ�����Բ�ıȽ϶��ʱ�򣬲�ת����̨��ϸС���ת��
	if (std::abs(fCurHoriLoc - fNextHoriLoc) > 10 || std::abs(fCurVertLoc - fNextVertLoc) > 5)
	{
		FILE_LOG(logINFO) << "��̨������......";
		float fCurrentHoriPoint = 0.0;
		float fCurrentVertPoint = 0.0;
		do
		{
			pTableControl->GetHoriAngle(fCurrentHoriPoint);
			Sleep(30);
			pTableControl->GetVertAngle(fCurrentVertPoint);
			Sleep(30);

			pTableControl->HVLocate(fNextHoriLoc, fNextVertLoc);  //��̨��λ��ָ����
		} while (std::abs(fCurrentHoriPoint - fNextHoriLoc) > 1 ||
			std::abs(fCurrentVertPoint - fNextVertLoc) > 1);
	}

	if (!CamTopImgProc(fCurHoriLoc, fCurVertLoc))
	{
		FILE_LOG(logINFO) << "�����������������ѱ��������¿�ʼѲ��";
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

	FILE_LOG(logINFO) << "�����Դ���...";
	std::vector<SSaliencyRegion> vSaliencyRegion;     //�洢ͼ������������������
	if ( !Saliency.ImageSaliency(SrcImage,vSaliencyRegion))
	{
		FILE_LOG(logINFO) << "��ǰͼ����û������Ư����";
		SrcImage.release();
		return false;
	}

	//�����������Ķ���
	std::vector<SFloaterMsg> vFloater;
	SaveFloaterMsg(vSaliencyRegion, fCurrentHoriPoint, fCurrentVertPoint, vFloater);
	vSaliencyRegion.clear();	//���ݴ洢���֮��ɾ������

	FILE_LOG(logINFO) << "��ǰͼ���д���" << vFloater.size() << "������Ư����";
	if (0 == vFloater.size())
	{
		FILE_LOG(logINFO) << "��ǰͼ����û������Ư����";
		SrcImage.release();
		vFloater.clear();
		return false;
	}

	//��һ��intΪkeyֵ����keyֵ�������У���Ư����ת�������׵�λ����keyֵ��������
	std::multimap<int, int, std::less<int>> mulmapFloater;	
	mulmapFloater.clear();
	for (int i = 0; i < vFloater.size(); i++)
	{
		mulmapFloater.insert(std::pair<int, int>(static_cast<int>(vFloater.at(i).m_fDistanceViaRobot * 100), i));
	}

	//ȡ�������ٵ�Ŀ��
	SFloaterMsg AlignFloater = vFloater.at(0);
	mulmapFloater.clear();
	vFloater.clear();

	if (! CamCertralAlign(AlignFloater, 0.65))
	{
		FILE_LOG(logINFO) << "������Ư��������Ķ���ʧ��";
		return false;
	}

	/************************�ؼ������д���*********************************/
	//����ɹ�������Ķ��룬���ٴ�����ͼƬ���м��ʶ��Ԥ������
	do
	{
		HIKCameraTop.ReadImage(SrcImage);
	} while (SrcImage.empty());
	

	if (!Saliency.ImageSaliency(SrcImage, vSaliencyRegion))
	{
		FILE_LOG(logINFO) << "���Ķ�����ٴ�ȡ��ͼ��û������Ư����";
		SrcImage.release();
		return false;
	}
	
	SaveFloaterMsg(vSaliencyRegion, fCurrentHoriPoint, fCurrentVertPoint, vFloater);
	vSaliencyRegion.clear();	//���ݴ洢���֮��ɾ������

	FILE_LOG(logINFO) << "���Ķ�����ٴ�ȡ��ͼ�д���" << vFloater.size() << "������Ư����";
	if (0 == vFloater.size())
	{
		FILE_LOG(logINFO) << "���Ķ�����ٴ�ȡ��ͼ����û������Ư����";
		SrcImage.release();
		vFloater.clear();
		return false;
	}

	//ȷ�ϴ�������Ư���ȡ�����������
	for (int i = 0; i < vFloater.size(); i++)
	{
		mulmapFloater.insert(std::pair<int, int>(static_cast<int>(vFloater.at(i).m_fDistanceViaRobot * 100), i));
	}
	SFloaterMsg FollowFloater = vFloater.at(0);

	//if (!Classifier.RecongitionClassifier(FollowFloater.m_Floater.m_SaliencyRegion))
	//{
	//	FILE_LOG(logINFO) << "���Ķ�����ٴ�ȡ��ͼ���е�����Ư���ﲻ������";
	//	return false;
	//}

	cv::rectangle(SrcImage, FollowFloater.m_Floater.m_SaliencyRect, cv::Scalar(0, 0, 255));
	cv::imshow("The floater to be followed", SrcImage);
	cv::waitKey(10);

	//�����·�
	FILE_LOG(logINFO) << "ȷ�ϴ������������ڽ���λ��Ϣ���п��Ʋ��Խ������·�";
	float fAngle4Sgy = FollowFloater.m_fHoriAngleViaRobot - 185;	//���ڿ��Ʋ����ж�����ǰ����0�㣬����̨��������ǰ��Ϊ185�㣬������Ҫ��һ��
	float fDistance4Sgy = FollowFloater.m_fDistanceViaRobot;
	float fAngle4Send = 0.0, fDistance4Send = 0.0;
	ControlStrategy(fAngle4Sgy, fDistance4Sgy, fAngle4Send, fDistance4Send);	//���Ʋ��Խ���
	int nAngle4Send = static_cast<int>(fAngle4Send * 100);
	int nDistance4Send = static_cast<int>(fDistance4Send);
	if (!CtrlSgyDataSend(nAngle4Send, nDistance4Send))								//�������ݷ���
		FILE_LOG(logINFO) << "���������·�ʧ��";
	FILE_LOG(logINFO) << "���������·��ɹ�";

	//�������
	FILE_LOG(logINFO) << "ȷ�ϴ�������Ŀ�꣬�������" << std::endl << std::endl;
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
			FloaterMsgTemp.m_fDistanceViaRobot = 0.0;//������� �Ƕ�Ϊ90�Ǵ���� �Ƕ�Ϊ0Ҳ�Ǵ��
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
		FILE_LOG(logINFO) << "���Ķ���ʱ��̨ת��ϵ������";
		return false;
	}

	//��̨��ǰλ��
	float fCurHoriLoc = FollowFloater.m_fHoriAngleViaRobot - FollowFloater.m_Floater.m_fHoriAngle;
	float fCurVertLoc = FollowFloater.m_fVertAngleViaRobot - FollowFloater.m_Floater.m_fVertAngle;
	
	float fNextHoriLoc = fCurHoriLoc + FollowFloater.m_Floater.m_fHoriAngle*fTableRotPara;
	float fNextVertLoc = fCurVertLoc + FollowFloater.m_Floater.m_fVertAngle*fTableRotPara;

	if (fNextHoriLoc > m_TableLimit.m_fTableRightThd || fNextHoriLoc < m_TableLimit.m_fTableLeftThd
		|| fNextVertLoc > m_TableLimit.m_fTableDownThd4Track || fNextVertLoc < m_TableLimit.m_fTableTopThd)
	{
		FILE_LOG(logINFO) << "���Ķ���ʱ����̨Ŀ�궨λ�㳬������";
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

		pTableControl->HVLocate(fNextHoriLoc, fNextVertLoc);  //��̨��λ��ָ����
	} while (std::abs(fCurrentHoriPoint - fNextHoriLoc) > 1 ||
		std::abs(fCurrentVertPoint - fNextVertLoc) > 1);
	
	FILE_LOG(logINFO) << "��̨���Ķ������";

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
				FILE_LOG(logINFO) << "���������·�ʧ��";
				return false;
			}	
			else
			{
				FILE_LOG(logINFO) << "���������·��ɹ�";
				return true;
			}
		}
	}
	FILE_LOG(logINFO) << "���Ʋ������ݷ�����������������ƣ����鴮���Ƿ�����";
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
				FILE_LOG(logINFO) << "�������������·�ʧ��";
				return false;
			}
			else
			{
				FILE_LOG(logINFO) << "�������������·��ɹ�";
				return true;
			}
		}
	}
	FILE_LOG(logINFO) << "�������ݷ�����������������ƣ����鴮���Ƿ�����";
	
	FILE_LOG(logDEBUG) << "CScohe::CtrlDataSend4Brush return";
	return true;
}