#include "ControlStrategy.h"

bool IPSG::ControlStrategy(float angel, float distance, float &fAngle, float &fDistance)
{
	float w = 0.0;//角速度 w

	if (70 <= angel && angel <= 100 && distance > 0) {

		fAngle = -70;  //70
		fDistance = 60 - 25; //60
	}
	else if (-100 <= angel && angel <= -70 && distance > 0) {
		fAngle = 70; //70
		fDistance = 60 - 15; //60
	}
	else if (40 <= angel && angel < 70 && distance>0) {
		fAngle = -(angel);  //-10
		fDistance = 60 - 10;   //60

	}
	else if (-70 < angel && angel <= -40 && distance > 0) {
		fAngle = -(angel); //+10
		fDistance = 60 - 10;  //60
	}
	else if (20 <= angel && angel < 40 && distance>0) {
		fAngle = -(angel);  //-10
		fDistance = 50;   //60

	}
	else if (-40 < angel && angel <= -20 && distance > 0) {
		fAngle = -(angel); //+10
		fDistance = 50;  //60
	}

	else if (8 < angel && angel < 20 && distance>0.000001) {
		fAngle = -10;  //-40
		fDistance = 40; //30

	}

	else if (-20 < angel && angel < -8 && distance>0) {
		fAngle = 10;  //
		fDistance = 40; //30
	}

	else if (abs(angel) <= 8 && distance >= 0.00001) {
		fAngle = -(angel);
		fDistance = 60 - 10;//正对垃圾
		std::cout << "正对垃圾" << std::endl;
	}
	else if (abs(angel) <= 8 && distance < 0.00001) {
		fAngle = 0;
		fDistance = 0;
		std::cout << "装入垃圾，停止给力" << std::endl;
		std::cout << "距离：" << distance << std::endl;
	}
	return true;
}