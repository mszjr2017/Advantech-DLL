// daq_dlls_noexport.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

//#define EXPORT __declspec(dllexport)
//
//
//#include<iostream>
//using namespace std;
//
//
//class TestDLL{
//public:
//	void hello();
//};
//void TestDLL::hello() {
//	cout << "hello world" << endl;
//}
//
//
//
//extern "C" {
//	TestDLL td;
//	EXPORT void hello() {
//		td.hello();
//	}
//
//	EXPORT void hello1() {
//		cout << "hello world 111111" << endl;
//	}
//
//	EXPORT int func(){
//		return 42;
//	}
//}

#define EXPORT __declspec(dllexport)
#include <ctime>
#include <time.h>
#include<iostream>
using namespace std;
#include <stdlib.h>
#include <stdio.h>
#include "C:\MyFiles\课题\数据采集\py_daq\daq_dlls\Heads\compatibility.h"
#include "C:\MyFiles\课题\数据采集\py_daq\daq_dlls\Heads\bdaqctrl.h"
using namespace Automation::BDaq;
//-----------------------------------------------------------------------------------
// Configure the following three parameters before running the example
//-----------------------------------------------------------------------------------
#define      deviceDescription  L"DemoDevice,BID#0"
typedef unsigned char byte;
int32    startChannelAI = 0;
const int32    channelCountAI = 3;

int32    startChannelAO = 0;
const int32    channelCountAO = 3;

int32    startPortDI = 0;
int32    portCountDI = 1;

int32    startPortDO = 0;
int32    portCountDO = 1;

extern "C" {
	// Create a 'instantAiCtrl' for InstantAI function.
	InstantAiCtrl * instantAiCtrl = AdxInstantAiCtrlCreate();
	// Create a 'InstantAoCtrl' for Static AO function.
	InstantAoCtrl * instantAoCtrl = AdxInstantAoCtrlCreate();
	// Create a 'InstantDiCtrl' for DI function.
	InstantDiCtrl * instantDiCtrl = AdxInstantDiCtrlCreate();
	// Create a instantDoCtrl for DO function.
	InstantDoCtrl * instantDoCtrl = AdxInstantDoCtrlCreate();

	DeviceInformation devInfo(deviceDescription);

	EXPORT inline void waitAnyKey()
	{
		do{ SLEEP(1); } while (!kbhit());
	}

	EXPORT void InstantAI()
	{
		ErrorCode        ret = Success;
		// Select a device by device number or device description and specify the access mode.
		// in this example we use AccessWriteWithReset(default) mode so that we can 
		// fully control the device, including configuring, sampling, etc.
		ret = instantAiCtrl->setSelectedDevice(devInfo);
		// Read samples and do post-process, we show data here.
		double   scaledData[channelCountAI] = { 0 };//the count of elements in this array should not be less than the value of the variable channelCount
		int32 channelCountMax = instantAiCtrl->getFeatures()->getChannelCountMax();
		//read samples and save to buffer 'scaledData'.
		ret = instantAiCtrl->Read(startChannelAI, channelCountAI, scaledData);
		// process the acquired data. only show data here.
		for (int32 i = startChannelAI; i< startChannelAI + channelCountAI; ++i)
		{
			//printf("Channel %d data: %10.6f\n", i % channelCountMax, scaledData[i - startChannel]);
			cout << scaledData[i - startChannelAI] << endl;
		}
	}

	EXPORT void CheckAI(){
		// If something wrong in this execution, print the error code on screen for tracking.
		ErrorCode ret = Success;
		ret = instantAiCtrl->setSelectedDevice(devInfo);
		if (BioFailed(ret))
		{
			cout << "AI:Some error occurred. And the last error code is ";
			cout << ret << endl;
		}
	}

	EXPORT void DisposeAI(){
		// Close device and release any allocated resource.
		instantAiCtrl->Dispose();
	}

	EXPORT void StaticAO(double a[channelCountAO]){
		ErrorCode ret = Success;
		// Select a device by device number or device description and specify the access mode.
		// in this example we use AccessWriteWithReset(default) mode so that we can 
		// fully control the device, including configuring, sampling, etc.
		ret = instantAoCtrl->setSelectedDevice(devInfo);
		// Output data 
		ret = instantAoCtrl->Write(startChannelAO, channelCountAO, a);
		for (int32 j = 0; j < channelCountAO; j++){
			cout << a[j] << endl;
		}
	}

	EXPORT void CheckAO(){
		ErrorCode ret = Success;
		ret = instantAoCtrl->setSelectedDevice(devInfo);
		// If something wrong in this execution, print the error code on screen for tracking.
		if (BioFailed(ret))
		{
			cout << "AO:Some error occurred. And the last error code is ";
			cout << ret << endl;
		}
	}

	EXPORT void DisposeAO(){
		// Close device and release any allocated resource.
		instantAoCtrl->Dispose();
	}

	EXPORT void StaticDI()
	{
		ErrorCode ret = Success;
		// Step 2: select a device by device number or device description and specify the access mode.
		// in this example we use AccessWriteWithReset(default) mode so that we can 
		// fully control the device, including configuring, sampling, etc.
		ret = instantDiCtrl->setSelectedDevice(devInfo);
		// Step 3: Read DI ports' status and show.
		byte  bufferForReading[64] = { 0 };//the first element of this array is used for start port
		ret = instantDiCtrl->Read(startPortDI, portCountDI, bufferForReading);
		//Show ports' status
		for (int32 i = startPortDI; i < startPortDI + portCountDI; ++i)
		{
			cout << bufferForReading[i - startPortDI] << endl;
		}
	}

	EXPORT void CheckDI(){
		ErrorCode        ret = Success;
		ret = instantDiCtrl->setSelectedDevice(devInfo);
		// If something wrong in this execution, print the error code on screen for tracking.
		if (BioFailed(ret))
		{
			cout << "DI:Some error occurred. And the last error code is ";
			cout << ret << endl;
		}
	}

	EXPORT void DisposeDI(){
		// Close device and release any allocated resource.
		instantDiCtrl->Dispose();
	}

	EXPORT void StaticDO(){
		ErrorCode        ret = Success;
		// Step 2: Select a device by device number or device description and specify the access mode.
		// in this example we use AccessWriteWithReset(default) mode so that we can 
		// fully control the device, including configuring, sampling, etc.
		ret = instantDoCtrl->setSelectedDevice(devInfo);
		// Step 3: Write DO ports
		byte  bufferForWriting[64] = { 0 };//the first element is used for start port
		for (int32 i = startPortDO; i < portCountDO + startPortDO; ++i)
		{
			printf(" Input a 16 hex number for DO port %d to output(for example, 0x00): ", i);
			//scanf("%x", &bufferForWriting[i - startPort]);
			cout << bufferForWriting[i - startPortDO] << endl;
		}
		ret = instantDoCtrl->Write(startPortDO, portCountDO, bufferForWriting);
	}

	EXPORT void CheckDO(){
		ErrorCode ret = Success;
		ret = instantDiCtrl->setSelectedDevice(devInfo);
		// If something wrong in this execution, print the error code on screen for tracking.
		if (BioFailed(ret))
		{
			cout << "DO:Some error occurred. And the last error code is ";
			cout << ret << endl;
		}
	}

	EXPORT void DisposeDO(){
		// Close device and release any allocated resource.
		instantDoCtrl->Dispose();
	}
}



