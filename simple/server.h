#pragma once
#include "stdafx.h"
#include "processing.h"

//--------------------------the definition of Class server-------------------------
class server{

public:
	int napp;//number of application
	int narrv;//number of applications have arrived
	int ndpt;//number of applications have left
	int ccr;//the CCR setting

	double curtime;//时钟
	double tnevt[3];//time of next event
	int nevt; //number of event
	int next;//下一事件在tnevt中的下标
	
	int status;//status of the server,1:busy, 0:idle
	//int niq;//num in queue,队列实体人数，含正在被服务的人
	double tlevt;//time of last event
	double tstart;//time of the start of service
	
	app_struct This_App;
	environment_struct This_Envi;
	processing Processing_algorithm;
	char filename[30];


	//若到达时服务台空闲，则tstart=time;其余情况下tstart=time of last departure
	//double sumniq;//稳态队长之和，Queue对t的积分
	//double sumwait;//总等待时间
	bool over;
	double waittime;
	float a1;
	double makespan;
	//long long seedA;//随机数A发生器的种子
	//long long seedS;//随机数S发生器的种子
	//myrandom rdA;//产生到达时间间隔
	//myrandom rdS;//产生顾客服务时间间隔

	//-----------The application: arrive(->in queue)->in service->depart--------------
	vector<app_struct> tarrvl;//队列中顾客的到达时间,从tarrvl开始记录，tarrvl[1]为正在处理的application


	server(int n1, int type, float a, int ccr);
	void init();
	void timing();
	void arrive(int rangea, int rangeb);
	void depart();
	void report();
	void ReadData_rnc1();
	void Refresh_Envi(app_struct this_app_now);
	void face_an_application(app_struct this_app);
	void Calculate_T1(app_struct *This_App);
};