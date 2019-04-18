#include "calibrate.h"

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	IPSG::CCalibrate _Calibrate("calibdata.txt");
	_Calibrate.Calibrate();

	system("pause");
	return 0;
}
