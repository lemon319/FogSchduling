// simple.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "server.h"
#include"processing.h"
//  three tiers scheduling
//  Created by wucg15 on 18/3/7.
//  Copyright (c) year 2017 wucg15. All rights reserved.
//  Altered and settled down on 


////input and output files for test////
//intance amount
int intance_number = 180;

//app_struct This_App;
//vector<vector<int>> prec_constraint;

float Calculate_lowbound(app_struct *This_App, environment_struct This_envi)
{
	int  task, taskp;
	int i, j, k;
	float maxv;
	/*int nump = (*This_App).Partition[1].size();
	vector<int> b_level(nump);

	for (i = 0; i < nump; i++)
	{
	b_level[i] = 0;
	}*/
	int now = (*This_App).Node.size() - 1;

	vector<node> Reverse_nodelist;
	vector<node> t_node;
	for (i = 0; i < (*This_App).Node[now].size(); i++)
	{
		t_node.push_back((*This_App).Node[now][i]);
	}
	/*	for (i = 0; i < (*This_App).Node[now].size(); i++)
	{
	if ((*This_App).Node[now][i].mach == 1)
	t_node.push_back((*This_App).Node[now][i]);
	}
	*/
	int flag1 = t_node.size() - 1, flag;
	//Obtain the reverse node list
	while (Reverse_nodelist.size() < (*This_App).n)
	{
		flag = flag1;
		flag1 = 0;
		for (i = flag; i >= 0; i--)
		{
			if ((t_node[i].children.empty()) && (t_node[i].Num >= 0))
			{
				Reverse_nodelist.push_back(t_node[i]);
				for (j = 0; j < t_node[i].parents.size(); j++)
				{
					int tt = t_node[i].parents[j];
					k = t_node[tt].children.size() - 1;
					while (k >= 0)
					{
						if (t_node[tt].children[k] == i)
						{
							t_node[tt].children.erase(t_node[tt].children.begin() + k);
							break;
						}
						k--;
					}
				}
				t_node[i].Num = -1;
			}
			else if ((!t_node[i].children.empty()) && (flag1 <= i))
			{
				flag1 = i;
			}
		}
	}
	for (i = 0; i < (*This_App).n; i++)
	{
		t_node[i].blvl = 0;
	}

	//node tNode;
	for (i = 0; i < (*This_App).n; i++)
	{
		maxv = 0;

		//just a reverse list
		//tNode = Reverse_nodelist[i];
		task = Reverse_nodelist[i].Num;

		for (j = 0; j<(*This_App).Node[now][task].children.size(); j++)
		{
			taskp = (*This_App).Node[now][task].children[j];
			if ((*This_App).Node[now][taskp].blvl + (*This_App).Node[now][task].comp> maxv)
				maxv = (*This_App).Node[now][taskp].blvl + (*This_App).Node[now][task].comp;
		}
		(*This_App).Node[now][task].blvl = maxv;
	}

	//sort((*This_App).Node[now].begin(), (*This_App).Node[now].end(), cmparebl);
	float maxblvl = 0, maxcmm = 0;
	int tasknum;
	for (i = 0; i < (*This_App).Node[1][0].children.size(); i++)
	{
		task = (*This_App).Node[1][0].children[i];
		if ((*This_App).Node[1][task].blvl == (*This_App).Node[1][0].blvl)
		{
			maxblvl = (*This_App).Node[1][task].blvl;
			//tasknum = (*This_App).Node[1][task].Num;
			break;
		}
	}
	//ofile << filename << endl;
	for (i = 0; i < (*This_App).Node[1][task].children.size(); i++)
	{
		tasknum = (*This_App).Node[1][task].children[i];
		if ((*This_App).pro_time1[task] + (*This_App).Node[1][tasknum].blvl == (*This_App).Node[1][task].blvl)
		{
			maxcmm = (*This_App).edge_matrix[task][tasknum];
			break;
		}
	}

	float lowbound = (*This_App).pro_time1[task] / This_envi.speed[0] + maxcmm / This_envi.bandwidth[1];
	lowbound += ((maxblvl - (*This_App).pro_time1[task]) / This_envi.speed[2] + This_envi.delay) < (maxblvl - (*This_App).pro_time1[task]) / This_envi.speed[1] ?
		((maxblvl - (*This_App).pro_time1[task]) / This_envi.speed[2] + This_envi.delay) : (maxblvl - (*This_App).pro_time1[task]) / This_envi.speed[1];
	return lowbound;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int i, j, k, times = 10;
	int sum3;
	

	//printout filename
	char filename_out[17];
	char outfilename;

	int times1 = 0, times2 = 0, numa, numb, numc, temp, temp1, temp2, temp3, ccr1, task;
	
	int num1, num2;
	double avgniq, avgwait;
	char ch = 'a';
	//int m = 107;
	int start;
	double niq_sum = 0;
	double wait_sum = 0;
	double niq_total = 0, wait_total = 0;

	int count = 1, type=1;
	float a;
	cout << "please put in the type,1-3" << endl;
	scanf_s("%d", &type);
	cout << "please put in the ccr,1-4" << endl;
	scanf_s("%d", &ccr1);

	a = 0.1;
	num1 = 100;
	int m = 100;
	ofstream ofile;	
	char filename1[30];
	sprintf_s(filename1, "normal_test_%d_%d.txt", type, ccr1);
	ofile.open(filename1, ios_base::out);
	ofile << "lifetime" << "	" << "tenergy" << "	" << "makespan" << "	";

	srand((unsigned int)time(0));
	for (k = 1; k <= m; k++)
	{
		server mserver(num1, type, a, ccr1);
		mserver.init();
		//cout << "	" << "arrive time:" << "	" << "start time:" << "	" << "makespan" << "	" << "energy" << endl;
		//srand(k);
		while (!mserver.over)
		{
			mserver.timing();
			if (mserver.next == 1)
				mserver.arrive(20,50);
			else
				mserver.depart();
		}
		ofile << mserver.This_Envi.lifetime << "	" << mserver.This_Envi.tenergy << "	" << mserver.makespan << "	";
		ofile << mserver.waittime / mserver.napp << endl;
	}
	system("pause");
	return 0;
}


