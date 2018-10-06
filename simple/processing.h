#pragma once

//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//#include <random>
#include "stdafx.h"

//--------------------------the definition of Class server-------------------------
class processing{

public:
	vector<vector <float> > Q;
	vector<vector<vector <float>>> Q_partiton;
	int Indi_Num;
	//vector<float> makespan(Indi_Num);
	vector<Indi> Population;//the group
	float a;
	vector<int> beforearray;
	vector<int> afterarray;
	//int AS_num, AS_num2;
	vector<Indi> AS;   //global AS
	vector<Indi> AS_2; //temp AS
	Indi solution;
	int times;

	float min_tmach_time;
	float start_time;
	float end_time;

	float alpha;

	int algorithm_type; //1:= total(EDA+partition); 2:= single EDA; 3:=b-lvl

	//processing(int indi_num, float speed);
	void init(int indi_num, float speed, int t, float a, app_struct This_App);
	//int machine_assign_EFT_in(vector<machine> tmach, mytask tempt, int tm);
	int processing::machine_assign_EFT_in(vector<machine> tmach, mytask tempt, int begin, int end);
	void evalution(int l, app_struct This_App, environment_struct This_Envi);
	mytask happen_permutation2(int j, mytask *x);
	void decode(int l, app_struct This_App);
	void Evolution(app_struct This_App, environment_struct This_Envi);
	int Calculate_p(app_struct This_App, vector<Indi> AS_2);
	void Calculate_B(app_struct *This_App);
	void Calculate_B1(app_struct *This_App);
	void complex_partition(app_struct *This_App, environment_struct This_Envi);

	//process the algorithm
	void Processing_Algorithm(app_struct This_App, environment_struct Tmep_Envi);

	vector<Indi> Update_AS(Indi &x);
	vector<Indi> Update_AS2(Indi &x);
	int check_dominance(Indi &a, Indi &b);
	int check_equal(Indi &a, Indi &b);
};

/*void processing::evalution(int l, app_struct This_App, environment_struct This_Envi)
{
	int i, j, k, task, mach1, task1, now = 1;

	vector<node> Node = This_App.Node[0];
	now = This_Envi.fog_num + This_Envi.things_num*This_Envi.things_one;
	vector <machine> tmach(now + This_Envi.cloud_num);

	//machine initialization
	k = 0;
	//Population[l].mach_assign.resize(This_App.n);
	Population[l].battery_consumption.resize(This_Envi.things_num);
	Population[l].tmach_time.resize(now + This_Envi.cloud_num);

	for (j = 0; j < tmach.size(); j++)
	{
		if (j < This_Envi.things_num)
		{
			i = 0;
			tmach[j].speed = This_Envi.speed[i];
			tmach[j].type = i;
			tmach[j].subtype = k;//things
			Population[l].battery_consumption[j] = 0;

			if (j%This_Envi.things_one == (This_Envi.things_one - 1))
				k++;
		}
		else if ((j >= This_Envi.things_num*This_Envi.things_one) && (j < now))
		{
			i = 1;
			tmach[j].speed = This_Envi.speed[i];
			tmach[j].type = i;//fog
		}
		else
		{
			i = 2;
			tmach[j].speed = This_Envi.speed[i];
			tmach[j].type = i;//cloud?
		}

		if (This_Envi.mach_time[j] > This_App.start_time)
			tmach[j].mach_time = This_Envi.mach_time[j] - This_App.start_time;
		else
			tmach[j].mach_time = 0;

	}

	float tenergy = 0, sumtime, tenergy_things = 0, cmm, temptime;
	vector<mytask> ttask(This_App.n);
	for (i = 0; i < This_App.n; i++)
	{
		ttask[i].choose = false;
		ttask[i].comp_time = This_App.pro_time1[i];
		ttask[i].job_complete = 0;
		ttask[i].job_period = 0;
		ttask[i].job_start = 0;
		ttask[i].job_earliest.resize(now + This_Envi.cloud_num);
		ttask[i].type = 0;
	}

	int flag1 = 0, type, sum;
	while (flag1 < This_App.n)
	{
		task = Population[l].permutation[0][flag1];
		flag1++;

		//machine assign
		if ((task == 0) || (Node[task].parents[0] == 0))
		{
			mach1 = machine_assign_EFT_in(tmach, ttask[task], This_Envi.things_num*This_Envi.things_one);
			//mach1 = flag1 % (This_Envi.things_num*This_Envi.things_one);
		}
		else
			mach1 = machine_assign_EFT_in(tmach, ttask[task], now + This_Envi.cloud_num);

		if (mach1 < This_Envi.things_num*This_Envi.things_one)
		{
			ttask[task].type = 0;
		}
		else if ((mach1 >= This_Envi.things_num*This_Envi.things_one) && (mach1 < now))
		{
			ttask[task].type = 1;
		}
		else
		{
			ttask[task].type = 2;
		}

		//update the task information
		ttask[task].choose = true;

		//type = ttask[task].type;
		ttask[task].job_start = ttask[task].job_earliest[mach1] > tmach[mach1].mach_time ? ttask[task].job_earliest[mach1] : tmach[mach1].mach_time;
		ttask[task].job_period = ttask[task].comp_time / tmach[mach1].speed;
		ttask[task].job_complete = ttask[task].job_start + ttask[task].job_period;
		ttask[task].machine = mach1;

		//update the machine information
		tmach[mach1].mach_time = ttask[task].job_complete;
		tmach[mach1].mach_job.push_back(task);

		//considering the earliest time it could be processed
		for (j = 0; j < Node[task].children.size(); j++)
		{
			//for all its children
			task1 = Node[task].children[j];
			cmm = This_App.edge_matrix[task][task1];

			//parent in the things tier
			if (ttask[task].type == 0)
			{
				//things-things
				for (k = 0; k < This_Envi.things_num*This_Envi.things_one; k++)
				{
					//if they are set on the same sensor
					if (k == mach1)
						ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k]>ttask[task].job_complete ?
						ttask[task1].job_earliest[k] : ttask[task].job_complete;
					//if they are set in the same cluster
					else if (tmach[mach1].subtype == tmach[k].subtype)
						ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k] > ttask[task].job_complete + cmm / This_Envi.bandwidth_in[0] ?
						ttask[task1].job_earliest[k] : ttask[task].job_complete + cmm / This_Envi.bandwidth_in[0];
					//if they are set in the different clusters
					else
					{
						//first send to the fog, and then fog->fog, and then fog->things
						temptime = cmm * (2 / This_Envi.bandwidth[1] + 1 / This_Envi.bandwidth_in[1]) + ttask[task].job_complete;
						ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k] > temptime ?
							ttask[task1].job_earliest[k] : temptime;
					}
				}
				//things-fog
				for (k = This_Envi.things_num*This_Envi.things_one; k < now; k++)
				{
					temptime = cmm * (1 / This_Envi.bandwidth[1] + 1 / This_Envi.bandwidth_in[1]) + ttask[task].job_complete;
					//the certain fog node of the things cluster
					if (k - This_Envi.things_num*This_Envi.things_one == tmach[mach1].subtype)
						ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k]>ttask[task].job_complete + cmm / This_Envi.bandwidth[1] ?
						ttask[task1].job_earliest[k] : ttask[task].job_complete + cmm / This_Envi.bandwidth[1];
					else
					{
						//fog node with other things clusters,things->fog and then fog->fog
						ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k] > temptime ?
							ttask[task1].job_earliest[k] : temptime;
					}
				}
				//things-cloud
				k = now;
				ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k] > ttask[task].job_complete + cmm / This_Envi.bandwidth[1] + This_Envi.delay ?
					ttask[task1].job_earliest[k] : ttask[task].job_complete + cmm / This_Envi.bandwidth[1] + This_Envi.delay;
				for (k = now + 1; k < ttask[task1].job_earliest.size(); k++)
				{
					ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k]>ttask[task1].job_earliest[now] ?
						ttask[task1].job_earliest[k] : ttask[task1].job_earliest[now];
				}
			}
			//if the parent is in fog
			else if (ttask[task].type == 1)
			{
				//fog-things
				for (k = 0; k < This_Envi.things_num*This_Envi.things_one; k++)
				{
					temptime = cmm * (1 / This_Envi.bandwidth[1] + 1 / This_Envi.bandwidth_in[1]) + ttask[task].job_complete;
					//the certain fog node of the things cluster
					if (mach1 - This_Envi.things_num*This_Envi.things_one == tmach[k].subtype)
						ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k]>ttask[task].job_complete + cmm / This_Envi.bandwidth[1] ?
						ttask[task1].job_earliest[k] : ttask[task].job_complete + cmm / This_Envi.bandwidth[1];
					else
					{
						//fog node with other things clusters,things->fog and then fog->fog
						ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k] > temptime ?
							ttask[task1].job_earliest[k] : temptime;
					}
				}
				//fog-fog
				for (k = This_Envi.things_num*This_Envi.things_one; k < now; k++)
				{
					if (k == mach1)
						ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k]>ttask[task].job_complete ?
						ttask[task1].job_earliest[k] : ttask[task].job_complete;
					else
						ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k] > ttask[task].job_complete + cmm / This_Envi.bandwidth_in[1] ?
						ttask[task1].job_earliest[k] : ttask[task].job_complete + cmm / This_Envi.bandwidth_in[1];
				}
				//fog-cloud
				k = now;
				ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k] > ttask[task].job_complete + This_Envi.delay ?
					ttask[task1].job_earliest[k] : ttask[task].job_complete + This_Envi.delay;

				for (k = now + 1; k < ttask[task1].job_earliest.size(); k++)
					ttask[task1].job_earliest[k] = ttask[task1].job_earliest[now]>ttask[task1].job_earliest[k] ?
					ttask[task1].job_earliest[now] : ttask[task1].job_earliest[k];
			}

			//if the parent is in cloud
			else
			{
				//cloud-things
				k = 0;
				ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k] > ttask[task].job_complete + This_Envi.delay + cmm / This_Envi.bandwidth[1] ?
					ttask[task1].job_earliest[k] : ttask[task].job_complete + This_Envi.delay + cmm / This_Envi.bandwidth[1];
				for (k = 1; k < This_Envi.things_num*This_Envi.things_one; k++)
				{
					ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k]>ttask[task1].job_earliest[0] ?
						ttask[task1].job_earliest[k] : ttask[task1].job_earliest[0];

				}

				//cloud-fog
				k = This_Envi.things_num*This_Envi.things_one;
				ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k] > ttask[task].job_complete + This_Envi.delay ?
					ttask[task1].job_earliest[k] : ttask[task].job_complete + This_Envi.delay;
				for (k = This_Envi.things_num*This_Envi.things_one + 1; k < now; k++)
				{
					ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k]>ttask[task1].job_earliest[This_Envi.things_num*This_Envi.things_one] ?
						ttask[task1].job_earliest[k] : ttask[task1].job_earliest[This_Envi.things_num*This_Envi.things_one];
				}

				//cloud-cloud
				for (k = now; k < ttask[task1].job_earliest.size(); k++)
				{
					if (k == mach1)
						ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k]>ttask[task].job_complete ?
						ttask[task1].job_earliest[k] : ttask[task].job_complete;
					else
						ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k] > ttask[task].job_complete + cmm / This_Envi.bandwidth_in[2] ?
						ttask[task1].job_earliest[k] : ttask[task].job_complete + cmm / This_Envi.bandwidth_in[2];
				}
			}

		}

		//calculate the energy consumption
		for (j = 0; j < Node[task].parents.size(); j++)
		{
			task1 = Node[task].parents[j];
			if (ttask[task1].type == 0)
			{
				//if (tmach[mach1].subtype == tmach[ttask[task1].machine].subtype)
				//{
				//send
				if (mach1 == ttask[task1].machine)
					Population[l].battery_consumption[ttask[task1].machine] += E_elec*This_App.edge_matrix[task][task1];
				else
					Population[l].battery_consumption[ttask[task1].machine] += E_elec*This_App.edge_matrix[task][task1] + deta_amp*This_App.edge_matrix[task][task1];
			}
			if (ttask[task].type == 0)
			{
				Population[l].battery_consumption[mach1] += E_comp_things*ttask[task].comp_time;
				//receive
				Population[l].battery_consumption[mach1] += E_elec*This_App.edge_matrix[task][task1];
			}
		}
	}
	float makespan1 = 0;
	for (j = 0; j<tmach.size(); j++)
	{
		if (tmach[j].mach_time>makespan1)
			makespan1 = tmach[j].mach_time;
	}
	/*cout << (int)makespan1 << endl;
	cout << tmach.size() << endl;
	for (i = 0; i < tmach.size(); i++)
	{
	cout << tmach[i].mach_job.size() << endl;
	for (j = 0; j < tmach[i].mach_job.size(); j++)
	{
	cout << tmach[i].mach_job[j] <<endl;
	cout << 0 << endl;
	task = tmach[i].mach_job[j];
	cout << (int)ttask[task].job_start << endl;
	cout << (int)ttask[task].job_complete << endl;
	}
	}*/

	/*float makespan_f = 0;
	for (j = 0; j<This_Envi.fog_num; j++)
	{
	if (tmach[j].mach_time>makespan_f)
	makespan_f = tmach[1][j].mach_time;
	}*/


	//calculate the total energy
	//N?

	/*float time_f = 0, time_c = 0;
	for (i = 0; i < ttask.size(); i++)
	{
		//if ((ttask[i].type == 1)&&(ttask[i].machine<This_Envi.fog_num))//fog
		if (ttask[i].type == 1)
			time_f += ttask[i].comp_time;
		//else if (ttask[i].type == 1)//cloud
		//	time_c += ttask[i].comp_time;
	}
	//fog
	//tenergy += E_static_fog*This_Envi.fog_num*makespan1 + (E_max_fog - E_static_fog)*time_f;
	tenergy += E_static_fog + (E_max_fog - E_static_fog)*time_f / (makespan1*This_Envi.fog_num);

	Population[l].makespan = makespan1;
	Population[l].energy_t = tenergy;
	for (i = 0; i < tmach.size(); i++)
		Population[l].tmach_time[i] = tmach[i].mach_time;

	//cout << l << "	" << makespan1 << "	" << tenergy << endl;

}*/
