// stdafx.cpp : source file that includes just the standard includes
// simple.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "server.h"
#include"processing.h"
// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
//-------------------------------------------------------------------------------------
//---------------------------------class server-----------------------------------
//-------------------------------------------------------------------------------------

server::server(int App_num, int type, float a, int ccr1)
{
	napp = App_num;
	narrv = 0;
	ndpt = 0;
	curtime = 0.0;
	tnevt[0] = 0;
	tnevt[1] = 0;
	tnevt[2] = 0;
	next = 0;
	status = 0;
	//niq = 0;
	tlevt = 0.0;
	tstart = 0.0;
	over = false;
	waittime = 0;
	Processing_algorithm.algorithm_type = type;
	a1 = a;
	ccr = ccr1;
	//Processing_algorithm.init(This_App);
	//tarrvl = new double[App_num + 1];
	
}

//------------------------void timing(),时间推进函数，事件调度法------------------------------
void server::timing()
{
	int i;
	double tmin = 1.0E+29;
	next = 0;
	for (i = 1; i <= 2; i++)
	{
		if (tnevt[i] < tmin)
		{
			tmin = tnevt[i];
			next = i;
		}
	}
	if (next == 0)
	{
		printf("\n Event list empty!");
		system("pause");
		exit(1);
	}
	curtime = tnevt[next];
}

void server::face_an_application(app_struct this_app_now)
{
	int indi_num = 30, t = 10, i, tempnum;
	float	speed = 0.05;
	//This_Envi.now_cluster = (This_Envi.now_cluster+1)%This_Envi.things_one.size();

	if (Processing_algorithm.algorithm_type == 1) //EDA+partition
		Processing_algorithm.complex_partition(&(this_app_now), This_Envi);
	if (Processing_algorithm.algorithm_type != 3)
	{
		Processing_algorithm.init(indi_num, speed, t, a1, this_app_now);
	}

	Processing_algorithm.Processing_Algorithm(this_app_now, This_Envi);
	Refresh_Envi(this_app_now);

	//print out
	cout << this_app_now.order << "	" << this_app_now.arrive_time << "	" << this_app_now.start_time;
	cout << "	" <<  this_app_now.start_time - this_app_now.arrive_time << "	" << Processing_algorithm.solution.energy_t << endl;
	waittime += this_app_now.start_time - this_app_now.arrive_time;
	/*for (i = 0; i < This_Envi.mach_time.size(); i++)
		cout << This_Envi.mach_time[i] << " ";
	cout << endl;
	*/
	//cout << Processing_algorithm.solution.makespan << endl;

	//for next application, choose a new cluster number for it
	This_Envi.now_cluster = rand() % This_Envi.things_one.size();
	int begin = 0;
	for (i = 0; i < This_Envi.now_cluster; i++)
		begin += This_Envi.things_one[i];
	float min_value = RAND_MAX;
	//refresh the processing time of the machines
	for (i = 0; i < This_Envi.things_one[This_Envi.now_cluster]; i++)
	{
		min_value = min_value < This_Envi.mach_time[begin + i] ? min_value : This_Envi.mach_time[begin + i];
	}
	if (min_value < curtime)
		min_value = curtime;
	tnevt[2] = curtime + Processing_algorithm.min_tmach_time<min_value ? curtime + Processing_algorithm.min_tmach_time:min_value;
	
	//tnevt[2] = curtime + Processing_algorithm.min_tmach_time;
}


//-------------------------------------void arrive()--------------------------------------
void server::arrive(int rangea, int rangeb)
{
	int indi_num = 30, t = 10, i, tempnum, ccr_now;
	float speed = 0.05;

	//app_struct Temp_App;
	//if the number is larger than setting, set tnevt[1]>1.0E+29 and in timing(), next won't =1
	if (narrv == napp)
	{
		tnevt[1] = 1.0E+30;
	}
	//not enough, produce the arriving time of the next application
	else
	{
		narrv++;
		//random arriving time 
		tnevt[1] = curtime + rangea + (float)(rand() % (rangeb - rangea)) + (float)(rand() % 101) / 101;//update tnevtp[1]
		//tempnum = rand()%1000;
		//tempnum = rand() % 180;
		//ccr = rand() % 4+1; //
		//tempnum = narrv;
		tempnum = rand() % 1000;
		//ccr = rand()%4+1;

		//tempnum = narrv;
		//ccr = 3;
		ccr_now = ccr;
		if (ccr_now == 5)
			ccr_now = rand() % 4 + 1;

		int numa = tempnum % 1000;
		int numb = tempnum % 100;
		int numc = tempnum % 10;

		int temp = numc + 48;
		int temp1 = (numb - numc) / 10 + 48;
		int temp2 = (numa - numb) / 100 + 48;
		int temp3 = ccr_now + 48;
		

		//sprintf_s(filename, "rand0%c%c%c%c.txt", temp, temp1, temp2, temp3);
		sprintf_s(filename, "rand%c_00%c%c%c.txt", temp3, temp2, temp1, temp);
		//sprintf_s(filename, "rand3_00000.txt");
		//read the new application
		ReadData_rnc1();
		This_App.arrive_time = curtime;
		This_App.order = narrv;
		tarrvl.push_back(This_App);
		tlevt = curtime;

		if (status == 1)//the server is busy
		{			
			if (tarrvl.size() > napp)
			{
				printf("\n Overflow of the array tarrvl!");
				system("pause");
				exit(1);
			}
		}
		else//server is empty
		{
			//begin to be served
			status = 1;
			//This_App.start_time = curtime>This_App.arrive_time?curtime:This_App.arrive_time;
			This_App.start_time = curtime;
			face_an_application(This_App);
		}
	}
}

//--------------------------------void depart(), leave---------------------------------
void server::depart()
{
	int indi_num = 30, t = 10, i;
	float speed = 0.1, makespan1=0, time_sum=0;

	//if the queue is not empty, tarrv1[1]leave and the depart() happens
	tlevt = curtime;
	//niq--;
	ndpt++;
	if (ndpt == napp)
	{
		over = true;
		//tend = curtime;
		for (i = 0; i < This_Envi.mach_time.size(); i++)
		{
			if (makespan1 < This_Envi.mach_time[i])
				makespan1 = This_Envi.mach_time[i];
		}
		makespan = makespan1;

		for (i = This_Envi.things_num; i < This_Envi.things_num + This_Envi.fog_num; i++)
			time_sum += This_Envi.mach_time[i];

		This_Envi.tenergy = E_static_fog*time_sum + (E_max_fog - E_static_fog)*This_Envi.tenergy;
		//kJ
		This_Envi.tenergy /= 1000;

	}

	//move forward
	//the first one leaves
	tarrvl.erase(tarrvl.begin());
	//if the queue is not empty
	if (tarrvl.size() > 0)
	{
		//calculate the next one
		//tarrvl[0].start_time = curtime>This_App.arrive_time ? curtime : This_App.arrive_time;
		tarrvl[0].start_time = curtime;
		face_an_application(tarrvl[0]);
		/*if (Processing_algorithm.algorithm_type == 1) //EDA+partition
			Processing_algorithm.complex_partition(&(tarrvl[0]), This_Envi);
		if (Processing_algorithm.algorithm_type != 3)
		{
			Processing_algorithm.init(indi_num, speed, t, a1, tarrvl[0]);
		}
		Processing_algorithm.Processing_Algorithm(tarrvl[0], This_Envi);
		tnevt[2] = curtime + Processing_algorithm.min_tmach_time;
		Refresh_Envi();
		
		cout << tarrvl[0].order << "	"  << tarrvl[0].arrive_time << "	"<< tarrvl[0].start_time;
		cout << "	" << Processing_algorithm.solution.makespan + tarrvl[0].start_time - tarrvl[0].arrive_time << "	" << Processing_algorithm.solution.energy_t << endl;
		waittime += Processing_algorithm.solution.makespan + tarrvl[0].start_time - tarrvl[0].arrive_time;
	*/
		//	cout << "wait" << endl;
		//tnevt[2] = curtime + rdS.randompro(betaS);
	}

	//if the queue is empty
	else
	{
		status = 0;
		tnevt[2] = 1.0E+30;//set tnevt[2] as a number larger than 1.0E+29
	}
}

//----------------------------------void init(),initialize---------------------------------
void server::init()
{
	narrv = 0;
	ndpt = 0;
	curtime = 0.0;
	tnevt[0] = 0;
	//tnevt[1] = curtime + rdA.randompro(betaA);
	tnevt[1] = curtime;
	tnevt[2] = 1.0E+30;
	next = 0;
	status = 0;
	//niq = 0;
	tlevt = 0.0;
	tstart = 0.0;
	//sumniq = 0.0;
	//sumwait = 0.0;
	over = false;
	//tend = 0;

	//initialize the environment
	This_Envi.now_cluster = 0;
	This_Envi.fog_num = 6;
	This_Envi.cloud_num = 2;
	//This_Envi.things_num = This_Envi.fog_num;
	
	//This_Envi.things_one.resize(This_Envi.fog_num);
	This_Envi.things_one.clear();
	
	for (int i = 0; i < 3; i++)
		This_Envi.things_one.push_back(4);
	for (int i = 3; i < This_Envi.fog_num; i++)
		This_Envi.things_one.push_back(3);
	This_Envi.things_num = 21;

	//update the environment data
	This_Envi.bandwidth_in.resize(3);
	This_Envi.bandwidth_in[0] = 1;
	This_Envi.bandwidth_in[1] = 5;
	This_Envi.bandwidth_in[2] = 10;

	This_Envi.speed.resize(3);
	This_Envi.speed[0] = 1;
	This_Envi.speed[1] = 5;
	This_Envi.speed[2] = 10;

	//vector<float> bandwidth;
	int mach_num = 3;
	//initialize the bandwidth
	This_Envi.bandwidth.resize(mach_num*(mach_num + 1) / 2);
	This_Envi.bandwidth[0] = 0; //[0][0]
	This_Envi.bandwidth[1] = 1; //[0][1] things-fog, 2
	This_Envi.bandwidth[2] = 0; //[0][2] things-cloud, no connection
	This_Envi.bandwidth[3] = 0; //[1][1]
	This_Envi.bandwidth[4] = 8; //[1][2] fog-cloud, 5
	This_Envi.bandwidth[5] = 0; //[2][2] 
	This_Envi.delay = 0.1;

	int i, j;
	float battery_begin = 0.1;
	//This_Envi.battery_max = battery_begin;
	This_Envi.battery_capacity.resize(This_Envi.fog_num);
	for (i = 0; i < This_Envi.battery_capacity.size(); i++)
	{
		This_Envi.battery_capacity[i].resize(This_Envi.things_one[i]);
		for (j = 0; j < This_Envi.battery_capacity[i].size(); j++)
			This_Envi.battery_capacity[i][j] = battery_begin;
	}

	This_Envi.mach_time.resize(This_Envi.things_num + This_Envi.fog_num + This_Envi.cloud_num);
	This_Envi.lifetime = RAND_MAX;


	//initialize the situation of the application
	This_App.low_bound_ddl = 1.5;
	This_App.high_bound_ddl = 3;
	This_App.percentage_hard = 0.1;

}

void server::Calculate_T1(app_struct *This_App)
{
	int  task, taskp;
	int i, j, k;
	float maxv;
	int now = 1;

	vector<int> nodelist;
	vector<node> t_node;
	node temp;

	bool flag=false;
	//Obtain node list
	t_node.push_back((*This_App).Node[now][0]);
	(*This_App).Node[now][0].blvl = 1;

	while(t_node.size()>0)
	{
		//temp = t_node[0];
		flag = false;
		//need to change: after its parents are all considered
		for(i=0; i<nodelist.size(); i++)
		{
			if(nodelist[i] == t_node[0].Num)
			{
				flag = true;
				break;
			}
		}
		if(!flag)
		{	
			nodelist.push_back(t_node[0].Num);

			//push back the tasks whose parents have been push back
			for(i=0; i<t_node[0].children.size(); i++)
			{
				temp = (*This_App).Node[now][t_node[0].children[i]];
				flag = false;
				for(j=0; j<temp.parents.size(); j++)
				{
					if((*This_App).Node[now][temp.parents[j]].blvl != 1)
					{
						flag = true;
						break;
					}
				}
				if(!flag)
				{
					t_node.push_back(temp);
					(*This_App).Node[now][temp.Num].blvl = 1;
				}
			}
		}
		t_node.erase(t_node.begin());	
		
		/*for(i=0; i<t_node.size()-1; i++)
			{
				t_node[i] = t_node[i+1];
			}*/
		//}
		
	}

	//node tNode;
	for (i = 0; i < (*This_App).n; i++)
	{
		maxv = 0;
		task = nodelist[i];
		(*This_App).Node[now][task].tlvl = 0;

		for (j = 0; j<(*This_App).Node[now][task].parents.size(); j++)
		{
			taskp = (*This_App).Node[now][task].parents[j];
			if ((*This_App).Node[now][taskp].tlvl + (*This_App).Node[now][task].comp + (*This_App).prec_value[taskp][task]> maxv)
				maxv = (*This_App).Node[now][taskp].tlvl + (*This_App).Node[now][task].comp + (*This_App).prec_value[taskp][task];
		}
		(*This_App).Node[now][task].tlvl = maxv;
	}
}

void server::ReadData_rnc1()
{
	FILE *fp;
	int i, j, temp, temp2, n;
	float xxx;

	vector<vector <int> > parents;//parents[i][j] means that j is the direct parent of i
	vector<vector <int> > children;//children[i][j] means that j is the direct child of i

	///write the filename and open the file///

	fopen_s(&(fp), filename, "r");
	if (fp == NULL)
		exit(-1);

	//read the number of tasks
	int temp1;
	fscanf_s(fp, "         %d\n", &temp);
	This_App.n = temp;
	n = temp;

	//initialize the arrays
	children.clear();
	parents.clear();

	//This_App.Edge.clear();
	This_App.prec_constraint.clear();
	This_App.pro_time1.clear();
	This_App.edge_matrix.clear();
	This_App.Partition.clear();
	This_App.edge_matrix.clear();
	This_App.Node.clear();
	This_App.partition_things.clear();
	This_App.prec_value.clear();

	//initialize the precedence constraint matrix
	This_App.prec_constraint.resize(n);
	This_App.prec_value.resize(n);
	This_App.edge_matrix.resize(n);
	//This_App.Edge.resize(n);
	This_App.Node.resize(2);
	
	This_App.ddl.resize(n);
	This_App.if_hard_ddl.resize(n);
	parents.resize(n);


	for (i = 0; i < n; i++)
	{
		This_App.prec_value[i].resize(n);
		This_App.prec_constraint[i].resize(n);
		This_App.edge_matrix[i].resize(n);
	}
	float temp4;
	vector<int> children_t;
	edge edge_t;
	for (i = 0; i < n; i++)
	{
		//temp1->the task number, temp->the processing time, temp2->the number of its parents
		fscanf_s(fp, "%d %f  %d", &temp1, &temp4, &temp2);
		
		fscanf_s(fp, "\n");

		//read the processing time
		This_App.pro_time1.push_back(temp4);
		//read the number of children
		children_t.resize(temp2);
		//edge_num += temp2;

		//prec_value[i].resize(temp2);
		//read the children
		for (j = 0; j < temp2; j++)
		{
			fscanf_s(fp, "%d	", &children_t[j]);
			fscanf_s(fp, "%f", &(This_App.prec_value[i][children_t[j]]));

			if (This_App.prec_value[i][children_t[j]] < 0)
				This_App.prec_value[i][children_t[j]] *= -1;

			//the information of the edge
			edge_t.u_parent = i;
			edge_t.v_child = children_t[j];
			edge_t.weight = This_App.prec_value[i][children_t[j]];
			This_App.edge_matrix[edge_t.u_parent][edge_t.v_child] = This_App.prec_value[edge_t.u_parent][edge_t.v_child];
			//This_App.Edge.push_back(edge_t);

			fscanf_s(fp, "\n");
		}
		children.push_back(children_t);
		fscanf_s(fp, "\n");
	}
	fclose(fp);

	int task;
	//record the precedence constraints
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < children[i].size(); j++)
		{
			task = children[i][j];
			This_App.prec_constraint[i][task] = true;
			parents[task].push_back(i);
		}
	}

	This_App.Node[0].resize(n);
	This_App.Node[1].resize(n);
	//store the nodes
	for (i = 0; i < n; i++)
	{
		This_App.Node[0][i].children = children[i];
		This_App.Node[0][i].parents = parents[i];
		This_App.Node[0][i].comp = This_App.pro_time1[i];
		This_App.Node[0][i].consist.push_back(i);
		This_App.Node[0][i].Num = i;

		This_App.Node[1][i].children = children[i];
		This_App.Node[1][i].parents = parents[i];
		This_App.Node[1][i].comp = This_App.pro_time1[i];
		This_App.Node[1][i].consist.push_back(i);
		This_App.Node[1][i].Num = i;

	}
	int k;
	//if i->j, j->k, then i->k
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (This_App.prec_constraint[i][j])
			{
				for (k = 0; k < n; k++)
				{
					if (This_App.prec_constraint[j][k])
					{
						This_App.prec_constraint[i][k] = true;
					}

				}
			}
			//	cout << j << endl;
		}
		//	cout << i << endl;
	}
	Calculate_T1(&(This_App));
	for(i=0; i<This_App.n; i++)
	{
		if(rand()%101<This_App.percentage_hard*100)
			This_App.if_hard_ddl[i] = true;
		else
			This_App.if_hard_ddl[i] = false;

		This_App.ddl[i] = ((This_App.high_bound_ddl-This_App.low_bound_ddl)*rand()/RAND_MAX+This_App.low_bound_ddl)*This_App.Node[1][i].tlvl;
	}


	This_App.pro_time1[0] = 0;
	for (i = 0; i < This_App.n; i++)
	{
		This_App.prec_value[0][i] = 0;
		This_App.edge_matrix[0][i] = 0;
	}
}

void server::Refresh_Envi(app_struct this_app_now)
{
	int i, j, k=0, begin=0;
	int cluster = This_Envi.now_cluster;
	
	for (i = 0; i < cluster; i++)
		begin += This_Envi.things_one[i];

	//refresh the processing time of the machines
	for (i = 0; i < This_Envi.things_one[cluster]; i++)
	{
		This_Envi.mach_time[begin + i] = this_app_now.start_time + Processing_algorithm.solution.tmach_time[i];
	}
	for (i = 0; i < This_Envi.fog_num + This_Envi.cloud_num; i++)
	{
		This_Envi.mach_time[This_Envi.things_num + i] = this_app_now.start_time + Processing_algorithm.solution.tmach_time[This_Envi.things_one[cluster] + i];
	}

	//refresh the battery capacity
	//cout << "battery"<<endl;
	for (j = 0; j < This_Envi.battery_capacity[cluster].size(); j++)
	{
		//cout << Processing_algorithm.solution.battery_consumption[j]<<"	";
		This_Envi.battery_capacity[cluster][j] -= Processing_algorithm.solution.battery_consumption[j];
		k++;
		if ((This_Envi.battery_capacity[cluster][j] < 0) && (This_Envi.lifetime == RAND_MAX))
		{
			//record the lifetime

			This_Envi.lifetime = This_Envi.mach_time[begin+j];

			break;
		}
		//k++;
	}
	//cout << endl;
	This_Envi.tenergy += Processing_algorithm.solution.time_f;
	
}
