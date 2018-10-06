#pragma once
#include"stdafx.h"
#include"processing.h"

/*processing::processing(int indi_num, float speed)
{
	Indi_Num = indi_num;
	Population.resize(Indi_Num);
	a = speed;
	min_tmach_time = 0;
	start_time = 0;
	end_time = 0;

}*/
/*int processing::machine_assign_EFT_in(vector<machine> tmach, mytask tempt, int tm)
{
	int key, i;
	//int tm = tmach.size();
	//cout<< tmach.size()<<endl;
	vector<float> cpl_time(tm);
	key = tm;

	for (i = 0; i<tm; i++)
	{
		cpl_time[i] = tempt.job_earliest[i]>tmach[i].mach_time ? tempt.job_earliest[i] : tmach[i].mach_time;
		cpl_time[i] += tempt.comp_time / tmach[i].speed;
	}

	key = get_min(cpl_time);
	return key;
}*/
int processing::machine_assign_EFT_in(vector<machine> tmach, mytask tempt, int begin, int end)
{
	int key, i, k;
	//int tm = tmach.size();
	//cout<< tmach.size()<<endl;
	int tm = end - begin;
	vector<float> cpl_time(tm);
	key = tm;
	k = 0;

	for (i = begin; i<end; i++)
	{
		cpl_time[k] = tempt.job_earliest[i]>tmach[i].mach_time ? tempt.job_earliest[i] : tmach[i].mach_time;
		cpl_time[k] += tempt.comp_time / tmach[i].speed;
		k++;
	}
	 
	key = get_min(cpl_time);
	return (begin + key);
}
void processing::evalution(int l, app_struct This_App, environment_struct This_Envi)
{
	int i, j, k, task, mach1, vol1, task1, now = 1;
	
	int cluster = This_Envi.things_one[This_Envi.now_cluster];
	now = cluster + This_Envi.fog_num;
	//machine: certain cluster+fog+cloud
	vector <machine> tmach(cluster + This_Envi.cloud_num + This_Envi.fog_num);
	vector <float> begin_time(tmach.size());
	//battery consumption
	Population[l].battery_consumption.resize(cluster);
	vector<node> Node;

	//choose the correct Node
	Node = This_App.Node[0];

	//initialize the temp machine, things_certain_cluster+fog+cloud
	for (j = 0; j < tmach.size(); j++)
	{
		if (j < cluster)
		{
			i = 0;
			tmach[j].speed = This_Envi.speed[i];
			tmach[j].type = i;
		}
		else if ((j >= cluster) && (j < now))
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
		tmach[j].mach_time = 0;

		//machine time 
		/*if (This_Envi.mach_time[j] > This_App.start_time)
			tmach[j].mach_time = This_Envi.mach_time[j] - This_App.start_time;
		else
			tmach[j].mach_time = 0;*/

		
		
	}

	//initial battery consumption
	for (i = 0; i < Population[l].battery_consumption.size(); i++)
		Population[l].battery_consumption[i] = 0;
	
	//initialize the machine time
	int sum1=0;
	for (i = 0; i < This_Envi.now_cluster; i++)
		sum1 += This_Envi.things_one[i];

	//refresh the processing time of the machines
	for (i = 0; i < This_Envi.things_one[This_Envi.now_cluster]; i++)
	{
		tmach[i].mach_time = This_Envi.mach_time[sum1+i] - This_App.start_time;
		begin_time[i] = This_Envi.mach_time[sum1+i] - This_App.start_time;
		if (tmach[i].mach_time < 0)
			tmach[i].mach_time = 0;
	}
	for (i = 0; i < This_Envi.fog_num + This_Envi.cloud_num; i++)
	{
		//tmach[This_Envi.things_num + i].mach_time = This_Envi.mach_time[This_Envi.things_one[cluster] + i] - This_App.start_time;
		tmach[cluster + i].mach_time = This_Envi.mach_time[This_Envi.things_num + i] - This_App.start_time;
		begin_time[cluster + i] = This_Envi.mach_time[This_Envi.things_num + i] - This_App.start_time;
		if (tmach[i].mach_time < 0)
			tmach[i].mach_time = 0;
		//This_Envi.mach_time[This_Envi.things_num + i] = This_App.start_time + This_Envi.tmach_time[This_Envi.things_one[cluster] + i];
	}
	
	//begin time
	//begin_time[j] = This_Envi.mach_time[j] - This_App.start_time;


	//subtype
	k = 0;
	
	for (j = 0; j < This_Envi.things_one[This_Envi.now_cluster]; j++)
	{
		tmach[j].subtype = This_Envi.now_cluster;
			
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
	bool avaliable = false;
	bool succ;
	vector<int> begin;

	for (i = 0; i < Population[l].permutation.size(); i++)
	{
		if (Population[l].permutation[i].size()>0)
		{
			begin.push_back(0);
			//tiers.push_back(i);
		}
	}

	while (flag1 < This_App.n)
	{
		if (algorithm_type == 1)
		{
			avaliable = false;
			if ((begin[0] < Population[l].permutation[0].size()) && (begin[1] < Population[l].permutation[1].size()))
			{
				task = Population[l].permutation[0][begin[0]];
				avaliable = true;
				for (j = 0; j < Node[task].parents.size(); j++)
				{
					if (!ttask[Node[task].parents[j]].choose)
					{
						avaliable = false;
						break;
					}
				}

				if (avaliable)
				{
					begin[0]++;
					type = 0;
				}
				else
				{
					task = Population[l].permutation[1][begin[1]];

					avaliable = true;
					for (j = 0; j < Node[task].parents.size(); j++)
					{
						if (!ttask[Node[task].parents[j]].choose)
						{
							avaliable = false;
							break;
						}
					}
					if (!avaliable)
					{
						for (k = begin[1] + 1; k < Population[l].permutation[1].size(); k++)
						{
							task = Population[l].permutation[1][k];
							succ = true;
							for (j = 0; j < Node[task].parents.size(); j++)
							{
								succ &= ttask[Node[task].parents[j]].choose;
								if (!succ)
									break;
							}
							if (succ)
								break;
						}

						//insert
						if (succ)
						{
							for (j = k; j >= begin[1] + 1; j--)
								Population[l].permutation[1][j] = Population[l].permutation[1][j - 1];
							Population[l].permutation[1][begin[1]] = task;
						}
					}

					begin[1]++;
					type = 1;
				}
			}
			else
			{
				task = Population[l].permutation[1][begin[1]];
				type = 1;
				begin[1]++;
			}

			if (type == 0)
			{
				//mach1 = flag1 % (This_Envi.things_num);
				mach1 = machine_assign_EFT_in(tmach, ttask[task], 0, cluster);
				ttask[task].type = 0;
			}
			else
			{
				mach1 = machine_assign_EFT_in(tmach, ttask[task], cluster, tmach.size());
				if (mach1 < now)
					ttask[task].type = 1;
				else
					ttask[task].type = 2;
			}
		}
		else
		{
			task = Population[l].permutation[0][flag1];
			//machine assign
			if ((task == 0) || ((Node[task].parents[0] == 0)&&(Node[task].parents.size()==1)))
			{
				mach1 = machine_assign_EFT_in(tmach, ttask[task], 0, cluster);
			}
			else
				mach1 = machine_assign_EFT_in(tmach, ttask[task], 0, tmach.size());

			if (mach1 < cluster)
			{
				ttask[task].type = 0;
			}
			else if ((mach1 >= cluster) && (mach1 < now))
			{
				ttask[task].type = 1;
			}
			else
			{
				ttask[task].type = 2;
			}
		}

		flag1++;
		ttask[task].choose = true;
		//type = ttask[task].type;
		ttask[task].job_start = ttask[task].job_earliest[mach1] > tmach[mach1].mach_time ? ttask[task].job_earliest[mach1] : tmach[mach1].mach_time;

		//processing time
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
				for (k = 0; k < cluster; k++)
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
				for (k = cluster; k < now; k++)
				{
					temptime = cmm * (1 / This_Envi.bandwidth[1] + 1 / This_Envi.bandwidth_in[1]) + ttask[task].job_complete;
					//the certain fog node of the things cluster
					if (k - cluster == tmach[mach1].subtype)
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

				ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k] > ttask[task].job_complete + cmm / This_Envi.bandwidth[1] + This_Envi.delay*cmm + cmm / This_Envi.bandwidth[4] ?
					ttask[task1].job_earliest[k] : ttask[task].job_complete + cmm / This_Envi.bandwidth[1] + This_Envi.delay*cmm + cmm / This_Envi.bandwidth[4];
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
				for (k = 0; k < cluster; k++)
				{
					temptime = cmm * (1 / This_Envi.bandwidth[1] + 1 / This_Envi.bandwidth_in[1]) + ttask[task].job_complete;
					//the certain fog node of the things cluster
					if (mach1 - cluster == tmach[k].subtype)
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
				for (k = cluster; k < now; k++)
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
				ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k] > ttask[task].job_complete + This_Envi.delay+cmm/This_Envi.bandwidth[4] ?
					ttask[task1].job_earliest[k] : ttask[task].job_complete + This_Envi.delay*cmm + cmm / This_Envi.bandwidth[4];

				for (k = now + 1; k < ttask[task1].job_earliest.size(); k++)
					ttask[task1].job_earliest[k] = ttask[task1].job_earliest[now]>ttask[task1].job_earliest[k] ?
					ttask[task1].job_earliest[now] : ttask[task1].job_earliest[k];
			}

			//if the parent is in cloud
			else
			{
				//cloud-things
				k = 0;
				ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k] > ttask[task].job_complete + This_Envi.delay*cmm + cmm / This_Envi.bandwidth[1] ?
					ttask[task1].job_earliest[k] : ttask[task].job_complete + This_Envi.delay*cmm + cmm / This_Envi.bandwidth[1];
				for (k = 1; k < cluster; k++)
				{
					ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k]>ttask[task1].job_earliest[0] ?
						ttask[task1].job_earliest[k] : ttask[task1].job_earliest[0];

				}

				//cloud-fog
				k = cluster;
				ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k] > ttask[task].job_complete + This_Envi.delay*cmm + cmm / This_Envi.bandwidth[4] ?
					ttask[task1].job_earliest[k] : ttask[task].job_complete + This_Envi.delay*cmm + cmm / This_Envi.bandwidth[4];
				for (k = cluster + 1; k < now; k++)
				{
					ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k]>ttask[task1].job_earliest[cluster] ?
						ttask[task1].job_earliest[k] : ttask[task1].job_earliest[cluster];
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

	//calculate the fog time
	float time_f = 0, time_c = 0;
	for (i = cluster; i < cluster + This_Envi.fog_num; i++)
	{
		for (j = 0; j < tmach[i].mach_job.size(); j++)
		{
			task = tmach[i].mach_job[j];
			time_f += ttask[task].job_period;
		}
	}

	for (i = cluster; i < now; i++)
	{
		time_c += tmach[i].mach_time - begin_time[i];
	}

	//calculate the makespan
	/*float makespan1 = 0;
	for (j = 0; j<tmach.size(); j++)
	{
		if (tmach[j].mach_time>makespan1)
			makespan1 = tmach[j].mach_time;
	}*/
	float tt = 0;
	for(j=0; j<ttask.size(); j++)
	{
		cout<<ttask[j].job_complete<<"	"<<This_App.ddl[j]<<endl;
		if(!This_App.if_hard_ddl[j])
			tt += ttask[j].job_complete>This_App.ddl[j]?ttask[j].job_complete-This_App.ddl[j]:0;
		else
			tt += 100*(ttask[j].job_complete>This_App.ddl[j]?ttask[j].job_complete-This_App.ddl[j]:0);
	}
	
	//gantt paint
	/*	if (makespan1 < 30)
	{*/
	
	/*if (This_App.start_time > 0)
	{
		cout << (int)(makespan1) << endl;
		cout << tmach.size() << endl;
		for (i = 0; i < tmach.size(); i++)
		{
			cout << tmach[i].mach_job.size() << endl;
			for (j = 0; j < tmach[i].mach_job.size(); j++)
			{
				cout << tmach[i].mach_job[j] << endl;
				cout << 0 << endl;
				task = tmach[i].mach_job[j];
				cout << (int)(ttask[task].job_start) << endl;
				cout << (int)(ttask[task].job_complete) << endl;
			}
		}
	}
	//}*/
	//cout << endl;

//	Population[l].makespan = makespan1;
	tenergy = E_static_fog*time_c + (E_max_fog - E_static_fog)*time_f;
	//kJ
	tenergy /= 1000;
	Population[l].energy_t = tenergy;
	Population[l].time_f = time_f;
	Population[l].tmach_time.clear();
	Population[l].tmach_time.resize(tmach.size());
	for (i = 0; i < tmach.size(); i++)
		Population[l].tmach_time[i] = tmach[i].mach_time;
}

//evalute the l-th individual
//j:= the order the sequence is going to be assigned
mytask processing::happen_permutation2(int j, mytask *x)
{
	int i, k, begin1, end1;
	vector<mytask> p_array;

	if (algorithm_type == 2)
	{
		int length = Q.size();
		for (i = 0; i < length; i++)
		{
			begin1 = beforearray[x[i].num];
			end1 = length - afterarray[x[i].num];
			if ((!x[i].choose) && (j >= begin1) && (j <= end1))
				//if (!x[i].choose)
			{
				x[i].p = 1000;
				p_array.push_back(x[i]);
			}
		}

		//calculate the probability
		for (i = 0; i < p_array.size(); i++)
		{
			for (k = 0; k < length; k++)
			{
				if ((!x[k].choose) && (Q[p_array[i].order][k] != 0))
				{
					p_array[i].p *= Q[p_array[i].order][k];
				}
				else if ((!x[k].choose) && (Q[p_array[i].order][k] == 0))
				{
					p_array[i].p = 0;
					break;
				}
			}
		}
	}
	else if (algorithm_type == 1)
	{
		int length = Q_partiton[1].size();
		for (i = 0; i < length; i++)
		{
			begin1 = beforearray[x[i].num];
			end1 = length - afterarray[x[i].num];
			if ((!x[i].choose) && (j >= begin1) && (j <= end1))
				//if (!x[i].choose)
			{
				x[i].p = 1000;
				p_array.push_back(x[i]);
			}
		}

		//calculate the probability
		for (i = 0; i < p_array.size(); i++)
		{
			for (k = 0; k < length; k++)
			{
				if ((!x[k].choose) && (Q_partiton[1][p_array[i].order][k] != 0))
				{
					p_array[i].p *= Q_partiton[1][p_array[i].order][k];
				}
				else if ((!x[k].choose) && (Q_partiton[1][p_array[i].order][k] == 0))
				{
					p_array[i].p = 0;
					break;
				}
			}
		}
	}

	//initialize
	double sum = 0;
	for (i = 0; i<p_array.size(); i++)
		sum += p_array[i].p;

	mytask job;
	if (p_array.size()>0)
	{
		p_array[0].p = p_array[0].p / sum;
		for (i = 0; i < p_array.size() - 1; i++)
		{
			p_array[i + 1].p = p_array[i + 1].p / sum + p_array[i].p;
		}

		float temp = 1;
		while (temp >= 1)
		{
			temp = ((rand() + 1) / (double)RAND_MAX);
		}
		if (temp < p_array[0].p)
			job = p_array[0];
		else
		{
			for (i = 0; i < p_array.size() - 1; i++)
			{
				if ((temp < p_array[i + 1].p) && (temp >= p_array[i].p))
				{
					job = p_array[i + 1];
					break;
				}
			}
		}
	}

	return job;
}
void processing::Calculate_B(app_struct *This_App)
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
			if ((*This_App).Node[now][taskp].blvl + (*This_App).Node[now][task].comp + (*This_App).prec_value[task][taskp]> maxv)
				maxv = (*This_App).Node[now][taskp].blvl + (*This_App).Node[now][task].comp + (*This_App).prec_value[task][taskp];
		}
		(*This_App).Node[now][task].blvl = maxv;
	}

	sort((*This_App).Node[now].begin(), (*This_App).Node[now].end(), cmparebl);
	Population.resize(1);
	Population[0].permutation.resize(1);
	Population[0].permutation[0].resize(0);
	Population[0].makespan = 0;
	Population[0].energy_t = 0;
	for (i = 0; i < (*This_App).Node[now].size(); i++)
	{
		//if ((*This_App).Node[now][i].mach == 1)
		Population[0].permutation[0].push_back((*This_App).Node[now][i].Num);
	}

}
void processing::decode(int l, app_struct This_App)//for the l-th individual
{
	int i, j, k;
	vector<mytask> tempt;
	mytask job;
	///initialize the task list///
	
	if (algorithm_type == 1)
	{
		k = 1;
		tempt.resize(This_App.Partition[k].size());
		for (i = 0; i < This_App.Partition[k].size(); i++)
		{
			tempt[i].num = This_App.Partition[k][i];
			tempt[i].choose = false;
			tempt[i].order = i;
			tempt[i].p = 1;

			Population[l].permutation[k][i] = -1;
		}

		for (i = 0; i < Q_partiton[k].size(); i++)
		{
			job = happen_permutation2(i, &(tempt[0]));
			Population[l].permutation[k][i] = job.num;
			tempt[job.order].choose = true;
		}
		//}
	}
	else
	{
		k = 0;
		tempt.resize(This_App.n);
		Population[l].permutation.resize(1);
		Population[l].permutation[0].resize(This_App.n);

		for (i = 0; i < This_App.n; i++)
		{
			tempt[i].num = i;
			tempt[i].choose = false;
			tempt[i].order = i;
			tempt[i].p = 1;

			Population[l].permutation[k][i] = -1;
		}
		for (i = 0; i < Q.size(); i++)
		{
			job = happen_permutation2(i, &(tempt[0]));
			//cout <<job.num;
			Population[l].permutation[k][i] = job.num;
			tempt[job.order].choose = true;
		}
	}


}
void processing::Evolution(app_struct This_App, environment_struct This_Envi)
{
	int i, j, l;

	for (l = 0; l < Indi_Num; l++)
	{
		decode(l, This_App);
		evalution(l, This_App, This_Envi);
		Update_AS2(Population[l]);
	}
	Calculate_p(This_App, Update_AS2(Population[l-1]));

}

void processing::init(int indi_num, float speed, int t, float a1, app_struct This_App)
{

	int i, j, k;
	int sum1, maxone;
	maxone = 0;
	sum1 = 0;
	
	Indi_Num = indi_num;
	Population.resize(Indi_Num);
	a = speed;
	min_tmach_time = 0;
	start_time = 0;
	end_time = 0;
	times = t;
	alpha = a1;
	
	Population.resize(Indi_Num);
		
	if (algorithm_type == 2)
	{
		Q.resize(This_App.n);
		for (i = 0; i < Q.size(); i++)
		{
			Q[i].resize(This_App.n);
			for (j = 0; j < Q[i].size(); j++)
			{
				Q[i][j] = 0.5;
				if (i == j)
					Q[i][j] = 1;
			}
		}
		//}

		int tsk1, tsk2;
		//the probability of task before and after relation
		//for (k = 0; k < Q.size(); k++)
		//{
		for (i = 0; i < This_App.n; i++)
		{
			for (j = i + 1; j < This_App.n; j++)
			{
				tsk1 = i;
				tsk2 = j;
				//	tsk2 = This_App.Partition[k][j];
				if (This_App.prec_constraint[tsk1][tsk2])
				{
					Q[i][j] = 1;
					Q[j][i] = 0;
				}
				else if (This_App.prec_constraint[tsk2][tsk1])
				{
					Q[i][j] = 0;
					Q[j][i] = 1;
				}
			}
		}
		//}

		//initialize the population
		for (i = 0; i < Indi_Num; i++)
		{
			//Population[i].permutation.resize(3);
			//for (k = 0; k < 3; k++)
			//	Population[i].permutation[k] = This_App.Partition[k];

			//Population[i].mach_assign.resize(This_App.n);
			Population[i].permutation.resize(This_App.n);
			//Population[i].job_earliest.resize(This_App.n);
		}

		afterarray.resize(This_App.n);
		beforearray.resize(This_App.n);

		for (i = 0; i < This_App.n; i++)
		{
			afterarray[i] = 0;
			beforearray[i] = 0;
		}

		for (i = 0; i < This_App.n; i++)
		{
			for (j = 0; j < This_App.n; j++)
			{
				//if (This_App.Node[1][i].mach == This_App.Node[1][j].mach)
				//{
				beforearray[j] += This_App.prec_constraint[i][j];
				afterarray[i] += This_App.prec_constraint[i][j];
				//}
			}
		}


	}
	else if (algorithm_type == 1)
	{
		Q_partiton.resize(2);

		for (k = 0; k < Q_partiton.size(); k++)
		{
			Q_partiton[k].resize(This_App.Partition[k].size());
			for (i = 0; i < Q_partiton[k].size(); i++)
			{
				Q_partiton[k][i].resize(Q_partiton[k].size());
				for (j = 0; j < Q_partiton[k].size(); j++)
				{
					Q_partiton[k][i][j] = 0.5;
					if (i == j)
						Q_partiton[k][i][j] = 1;
				}
			}
		}

		int tsk1, tsk2;
		//the probability of task before and after relation
		for (k = 0; k < Q_partiton.size(); k++)
		{
			for (i = 0; i < This_App.Partition[k].size(); i++)
			{
				tsk1 = This_App.Partition[k][i];
				for (j = i + 1; j < This_App.Partition[k].size(); j++)
				{
					tsk2 = This_App.Partition[k][j];
					if (This_App.prec_constraint[tsk1][tsk2])
					{
						Q_partiton[k][i][j] = 1;
						Q_partiton[k][j][i] = 0;
					}
					else if (This_App.prec_constraint[tsk2][tsk1])
					{
						Q_partiton[k][i][j] = 0;
						Q_partiton[k][j][i] = 1;
					}
				}
			}
		}
		Population.clear();
		Population.resize(Indi_Num);
		//initialize the population
		for (i = 0; i < Indi_Num; i++)
		{
			Population[i].permutation.resize(3);
			for (k = 0; k < 3; k++)
				Population[i].permutation[k] = This_App.Partition[k];

		//	Population[i].mach_assign.resize(This_App.n);
		}

		afterarray.resize(This_App.n);
		beforearray.resize(This_App.n);

		for (i = 0; i < This_App.n; i++)
		{
			afterarray[i] = 0;
			beforearray[i] = 0;
		}

		//calculate the before and after array 
		for (i = 0; i < This_App.n; i++)
		{
			for (j = 0; j < This_App.n; j++)
			{
				if (This_App.Node[1][i].type == This_App.Node[1][j].type)
				{
					beforearray[j] += This_App.prec_constraint[i][j];
					afterarray[i] += This_App.prec_constraint[i][j];
				}
			}
		}


	}

	AS_2.clear();
}

int processing::Calculate_p(app_struct This_App, vector<Indi> AS_2)
{
	//float slr_array[Indi_Num], ecr_array[Indi_Num], sum1[max_n][4], b, bb;
	int i, j, task, task1, q, k;
	vector<int> array(Indi_Num);
	int *arr = &(array[0]);
	vector<vector<int>> sum;
	sum.clear();
	if (algorithm_type == 2)
	{
		//sum.clear();
		sum.resize(This_App.n);
		for (i = 0; i < sum.size(); i++)
		{
			sum[i].resize(This_App.n);
			for (j = 0; j < This_App.n; j++)
			{
				if (i == j)
					sum[i][j] = AS_2.size();
				else
					sum[i][j] = 0;
			}
		}
		q = 0;
		for (k = 0; k < AS_2.size(); k++)
		{
			//permutation[q] = AS_2[k].permutation[q];
			for (i = 0; i < sum.size(); i++)
			{
				for (j = i + 1; j < sum.size(); j++)
				{
					task = AS_2[k].permutation[q][i];
					task1 = AS_2[k].permutation[q][j];
					sum[task][task1]++;
				}
			}
		}

		for (j = 0; j < This_App.n; j++)
		{
			for (i = 0; i < This_App.n; i++)
				Q[i][j] = (1 - a)*Q[i][j] + a*(sum[i][j] * 1.0) / (double)AS_2.size();
		}
	}
	else
	{
		vector<int> tempt;
		tempt.resize(This_App.n);

		q = 1;
		for (i = 0; i < This_App.Partition[q].size(); i++)
		{
			tempt[This_App.Partition[q][i]] = i;
		}
	
		sum.resize(This_App.Partition[q].size());
		for (i = 0; i < sum.size(); i++)
		{
			sum[i].resize(This_App.Partition[q].size());
			for (j = 0; j < sum[i].size(); j++)
			{
				if (i == j)
					//	sum[i][j] = AS_2.size()+3;
					sum[i][j] = AS_2.size();
				else
					sum[i][j] = 0;
			}
		}

		for (k = 0; k < AS_2.size(); k++)
		{
			//permutation[q] = AS_2[k].permutation[q];
			for (i = 0; i < sum.size(); i++)
			{
				for (j = i + 1; j < sum.size(); j++)
				{
					task = AS_2[k].permutation[q][i];
					task1 = AS_2[k].permutation[q][j];
					sum[tempt[task]][tempt[task1]]++;
				}
			}
		}
		/*for (k = 0; k < 3; k++)
		{
		for (i = 0; i < sum.size(); i++)
		{
		for (j = i + 1; j < sum.size(); j++)
		{
		task = Population[*(arr+k)].permutation[q][i];
		task1 = Population[*(arr + k)].permutation[q][j];
		sum[tempt[task]][tempt[task1]]++;
		}

		}
		}*/

		for (j = 0; j < This_App.Partition[q].size(); j++)
		{
			for (i = 0; i<This_App.Partition[q].size(); i++)
				Q_partiton[q][i][j] = (1 - a)*Q_partiton[q][i][j] + a*(sum[i][j] * 1.0) / (double)(AS_2.size());
		}
	}
	return 0;
	//    return(*(arr+min_num));
}
vector<Indi> processing::Update_AS(Indi &x)
{
	int j, k, flag, sus;//sus=0表示新解可存入档案集，sus=1表示新解不可存入
	sus = 0;
	for (j = 0; j < AS.size(); j++)
	{
		flag = check_dominance(x, AS[j]);
		if (flag == -1)
		{
			sus = 1;
			break;
		}

		else if (flag == 1)
		{
			/*for (k = j; k < AS_num - 1; k++)
			AS[k] = AS[k + 1];
			AS_num--;*/
			AS.erase(AS.begin() + j);
			j--;
		}
		else
		{
			sus = check_equal(x, AS[j]);
			if (sus == 1)
				break;
		}
	}
	if (sus == 0)
	{
		AS.push_back(x);
		//AS[AS_num].initial = x.initial;
		//cout<<"Update_AS successful"<<x.makespan<<" "<<x.energy_t<<endl;
	}
	//return sus;
	return(AS);
}
vector<Indi> processing::Update_AS2(Indi &x)
{
	int j, k, flag, sus;//sus=0表示新解可存入档案集，sus=1表示新解不可存入
	sus = 0;
	for (j = 0; j < AS_2.size(); j++)
	{
		flag = check_dominance(x, AS_2[j]);
		if (flag == -1)
		{
			sus = 1;
			break;
		}

		else if (flag == 1)
		{
			AS_2.erase(AS_2.begin() + j);
			j--;
			/*	for (k = j; k < AS_num2 - 1; k++)
			AS_2[k] = AS_2[k + 1];
			AS_num2--;
			j--;*/
		}
		else
		{
			sus = check_equal(x, AS_2[j]);
			if (sus == 1)
				break;
		}
	}
	if (sus == 0)
	{
		AS_2.push_back(x);
		//AS_2[AS_num2++] = x;
		//AS_2[AS_num2].initial = x.initial;
		//  cout<<"Update_AS successful"<<x.makespan<<" "<<x.energy_t<<endl;
	}
	//return sus;
	return(AS_2);
}
int processing::check_dominance(Indi &a, Indi &b)  //返回1代表a支配b, 返回-1代表b支配a, 0表示互不支配
{
	int flag1 = 0;
	int flag2 = 0;
	float delta1 = float(0.0);

	if ((b.makespan - a.makespan > delta1) || (b.energy_t - a.energy_t > delta1))
		flag1 = 1;

	if ((a.makespan - b.makespan > delta1) || (a.energy_t - b.energy_t > delta1))
		flag2 = 1;


	if (flag1 == 1 && flag2 == 0)
		return (1);
	else if (flag1 == 0 && flag2 == 1)
		return (-1);
	else
		return (0);
}
int processing::check_equal(Indi &a, Indi &b)//返回0说明不相同
{
	float delta1 = float(0.0);

	if (((a.makespan - b.makespan)*(a.makespan - b.makespan) > delta1*delta1) || ((a.energy_t - b.energy_t)*(a.energy_t - b.energy_t) > delta1*delta1))
		return(0);

	return(1);
}
void processing::Processing_Algorithm(app_struct This_App, environment_struct Temp_Envi)
{
	int i, k;
	if (algorithm_type == 3)
	{
		Calculate_B(&(This_App));
		evalution(0, This_App, Temp_Envi);
		AS_2.clear();
		Update_AS2(Population[0]);
	}
	else
	{
		for (i = 0; i < times; i++)
			Evolution(This_App, Temp_Envi);
	}

	float min_results=RAND_MAX;
	for (i = 0; i < AS_2.size(); i++)
	{
		if (min_results>(alpha*AS_2[i].makespan + (1 - alpha)*AS_2[i].energy_t))
		{
			min_results = alpha*AS_2[i].makespan + (1 - alpha)*AS_2[i].energy_t;
			k = i;
		}
	}

	solution = AS_2[k];
	min_tmach_time = RAND_MAX;
	for (i = 0; i < solution.tmach_time.size(); i++)
	{
		if (min_tmach_time>solution.tmach_time[i])
			min_tmach_time = solution.tmach_time[i];
	}
	//min_tmach_time += This_App.start_time;
	//ofile << AS_2.size() << endl;
	/*for (k = 0; k<AS_2.size(); k++)
	{
		ofile << AS_2[k].makespan << " " << AS_2[k].energy_t << endl;

	}*/
}
void processing::Calculate_B1(app_struct *This_App)
{
	int  task, taskp;
	int i, j, k;
	float maxv;
	int now = 1;

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
			if ((*This_App).Node[now][taskp].blvl + (*This_App).Node[now][task].comp + (*This_App).prec_value[task][taskp]> maxv)
				maxv = (*This_App).Node[now][taskp].blvl + (*This_App).Node[now][task].comp + (*This_App).prec_value[task][taskp];
		}
		(*This_App).Node[now][task].blvl = maxv;
	}

	sort((*This_App).Node[now].begin(), (*This_App).Node[now].end(), cmparebl);
}
void processing::complex_partition(app_struct *This_App, environment_struct This_Envi)
{
	int i, j, k, flag1 = 0, task, taskp, flag2;
	vector<mytask> ttask((*This_App).n);
	bool test = true;
	vector<float> mach_time;
	float comp, cmm;

	for (i = 0; i < (*This_App).n; i++)
	{
		ttask[i].choose = false;
		ttask[i].comp_time = (*This_App).pro_time1[i];
		ttask[i].job_complete = 0;
		ttask[i].job_period = 0;
		ttask[i].job_start = 0;
		ttask[i].machine = 0;//0:=things, 1:=fog
		ttask[i].job_earliest.resize(2);
		ttask[i].type = 0;
		ttask[i].num = i;
		ttask[i].order = i;
	}

	//b-lvl permutation
	vector<node> Node = (*This_App).Node[(*This_App).Node.size() - 1];
	vector<int> permutation;

	Calculate_B1(This_App);
	for (i = 0; i < (*This_App).n; i++)
	{
		//if ((*This_App).Node[now][i].mach == 1)
		permutation.push_back((*This_App).Node[(*This_App).Node.size() - 1][i].Num);
	}

	//calculate the ccr
	comp = 0;
	cmm = 0;
	k = 0;
	for (i = 0; i < (*This_App).pro_time1.size(); i++)
	{
		comp += (*This_App).pro_time1[i];
	}
	comp /= (*This_App).n;

	for (i = 0; i < (*This_App).edge_matrix.size(); i++)
	{
		for (j = 0; j < (*This_App).edge_matrix[i].size(); j++)
		{
			if ((*This_App).edge_matrix[i][j]>0)
			{
				cmm += (*This_App).edge_matrix[i][j];
				k++;
			}
		}
	}
	cmm /= k;

	float ccr = cmm / comp;


	float sensors = 0;
	sensors = This_Envi.things_one[This_Envi.now_cluster];
	//sensor+1 machines' model
	mach_time.resize(sensors + 1);
	for (i = 0; i < mach_time.size(); i++)
		mach_time[i] = 0;
	//calculate the speed of the fog node
	float fog_speed = 0;
	fog_speed = This_Envi.speed[1] * This_Envi.fog_num + This_Envi.speed[2] * This_Envi.cloud_num;
	float avg_things_dis = 1, avg_cmm;
	avg_cmm = 0;
	vector<float> max_time;

	//initialize the machine time
	int sum1 = 0;
	for (i = 0; i < This_Envi.now_cluster; i++)
		sum1 += This_Envi.things_one[i];

	//refresh the processing time of the machines
	for (i = 0; i < This_Envi.things_one[This_Envi.now_cluster]; i++)
	{
		mach_time[i]= This_Envi.mach_time[sum1 + i] - (*This_App).start_time;
		if (mach_time[i]< 0)
			mach_time[i]= 0;
	}
	for (i = 0; i < This_Envi.fog_num + This_Envi.cloud_num; i++)
	{
		mach_time[sensors] = This_Envi.mach_time[This_Envi.things_num + i] - (*This_App).start_time>mach_time[sensors] ? This_Envi.mach_time[This_Envi.things_num+i] - (*This_App).start_time : mach_time[sensors];
		//This_Envi.mach_time[This_Envi.things_num + i] = This_App.start_time + This_Envi.tmach_time[This_Envi.things_one[cluster] + i];
	}
	/*for (i = 0; i < mach_time.size(); i++)
		mach_time[i] = 0;*/

	//cout << Node[0].children.size() + 1 << endl;
	float energy[2], comp_time[2];
	i = 0;
	while (i < (*This_App).n)
	{
		task = permutation[i];
		test = true;
		float input_cmm, output_cmm;
		input_cmm = 0;
		output_cmm = 0;
		float min_sum;
		int min_num;

		
		if ((task == 0) || ((Node[task].parents[0] == 0) && (Node[task].parents.size() == 1)))
		{
			min_sum = RAND_MAX;
			ttask[task].choose = true;
			ttask[task].type = 0; //things

			//for each sensor
			for (j = 0; j < sensors; j++)
			{
				if (mach_time[j] < min_sum)
				{
					min_sum = mach_time[j];
					min_num = j;
				}
			}
			ttask[task].machine = min_num;
			ttask[task].job_complete = min_sum + (*This_App).pro_time1[task] / This_Envi.speed[0];
			mach_time[min_num] = ttask[task].job_complete;
			ttask[task].type = 0;

			for (j = 0; j < Node[task].children.size(); j++)
			{
				test = true;
				ttask[Node[task].children[j]].job_earliest[0] = ttask[Node[task].children[j]].job_earliest[0]>ttask[task].job_complete ?
					ttask[Node[task].children[j]].job_earliest[0] : ttask[task].job_complete;

				ttask[Node[task].children[j]].job_earliest[1] = ttask[Node[task].children[j]].job_earliest[1] > ttask[task].job_complete ?
					ttask[Node[task].children[j]].job_earliest[1] : ttask[task].job_complete;
			}
		}
		else
		{	//input communication amount
			for (j = 0; j < Node[task].parents.size(); j++)
			{
				if (ttask[Node[task].parents[j]].type != 0)
				{
					test = false;
					//break;
				}
				input_cmm += (*This_App).edge_matrix[Node[task].parents[j]][task];
			}

			//output communication amount
			for (j = 0; j < Node[task].children.size(); j++)
			{
				output_cmm += (*This_App).edge_matrix[task][Node[task].children[j]];
			}

			if (input_cmm < output_cmm)
				test = false;

			//parents are not in the same cluster
			if (!test)
			{
				ttask[task].type = 1;
				ttask[task].machine = sensors;
				ttask[task].choose = true;
				ttask[task].job_complete = mach_time[sensors] > ttask[task].job_earliest[1] ? mach_time[sensors] : ttask[task].job_earliest[1];
				ttask[task].job_complete += (*This_App).pro_time1[task] / fog_speed;
				mach_time[sensors] = ttask[task].job_complete;
			}
			else
			{
				int taskp_mach;
				float temp_max = RAND_MAX, sum_energy = 0;

				//earliest beginning time
				max_time.clear();
				max_time.resize(sensors);
				for (j = 0; j < Node[task].parents.size(); j++)
				{
					taskp = Node[task].parents[j];
					taskp_mach = ttask[taskp].machine;
					max_time[taskp_mach] += (*This_App).edge_matrix[taskp][task];
					sum_energy += (*This_App).edge_matrix[taskp][task];
				}

				for (j = 0; j < sensors; j++)
				{
					if (temp_max > mach_time[j])
					{
						temp_max = mach_time[j]>ttask[task].job_earliest[0] ? mach_time[j] : ttask[task].job_earliest[0];
						taskp_mach = j;

					}
				}

				temp_max = max_time[taskp_mach];
				ttask[task].machine = taskp_mach;
				energy[0] = 0;
				energy[1] = 0;

				comp_time[0] = mach_time[ttask[task].machine]>ttask[task].job_earliest[0] ? mach_time[ttask[task].machine] : ttask[task].job_earliest[0];
				comp_time[1] = mach_time[sensors] > ttask[task].job_earliest[1] ? mach_time[sensors] : ttask[task].job_earliest[1];

				//send
				energy[1] += sum_energy * E_elec + deta_amp*sum_energy * avg_things_dis * avg_things_dis;
				//transfer
				comp_time[1] += sum_energy / This_Envi.bandwidth[1];

				//receive
				energy[0] += (sum_energy - temp_max)* E_elec;
				energy[0] += (sum_energy - temp_max) * E_elec + deta_amp*(sum_energy - temp_max) * avg_things_dis * avg_things_dis;

				//comp
				comp_time[1] += 1.0*(*This_App).pro_time1[task] / fog_speed;
				comp_time[0] += 1.0*(*This_App).pro_time1[task] / (1.0*This_Envi.speed[0]);

				//comp energy
				energy[0] += E_comp_things*((*This_App).pro_time1[task] / This_Envi.speed[0]);
				//transfer energy
				/*avg_cmm = 0;
				for (j = 0; j < Node[task].children.size(); j++)
				{
					taskp = Node[task].children[j];
					avg_cmm += (*This_App).edge_matrix[task][taskp];
				}
				energy[0] += avg_cmm * E_elec + deta_amp*avg_cmm * avg_things_dis * avg_things_dis;
				*/
				//make a judge
				if ((comp_time[0] < comp_time[1]) && (energy[0] < energy[1]))
				{
					ttask[task].type = 0;
					//ttask[task].machine = ttask[taskp].machine;
				}
				else if ((comp_time[0] >= comp_time[1]) && (energy[0] >= energy[1]))
				{
					ttask[task].type = 1;

				}
				else
				{
					//double xishu = 1 / (1 + exp(-1 * ccr) )- 0.5;
					//double xishu = 0.02*ccr + 0.3;
					double xishu = 0.5;
					if (xishu*(energy[0] - energy[1]) / (energy[0] + energy[1]) + (1 - xishu)*(comp_time[0] - comp_time[1]) / (comp_time[0] + comp_time[1]) > 0)
					{
						ttask[task].type = 1;
						//	ttask[task].machine = This_Envi.fog_num;
					}
					else
					{
						ttask[task].type = 0;
						//ttask[task].machine = ttask[taskp].machine;
					}
				}
				ttask[task].choose = true;

				if (ttask[task].type == 0)
				{
					mach_time[ttask[task].machine] = comp_time[0];
					for (j = 0; j < Node[task].children.size(); j++)
					{
						ttask[Node[task].children[j]].job_earliest[0] = ttask[Node[task].children[j]].job_earliest[0]>ttask[task].job_complete ?
							ttask[Node[task].children[j]].job_earliest[0] : ttask[task].job_complete;
					}
				}
				else
				{
					ttask[task].machine = sensors;
					mach_time[ttask[task].machine] = comp_time[1];
				}
				//	ttask[task].job_complete = comp_time[ttask[task].machine];
				//	mach_time[ttask[task].type] = ttask[task].job_complete>mach_time[ttask[task].type] ? ttask[task].job_complete : mach_time[ttask[task].type];
			}
		}
		i++;
	}

	(*This_App).Partition.resize(3);
	for (i = 0; i < (*This_App).n - 1; i++)
	{
		task = permutation[i];
		(*This_App).Partition[ttask[task].type].push_back(task);
		(*This_App).Node[1][task].mach = ttask[task].machine;
		(*This_App).Node[1][task].type = ttask[task].type;
	}
	(*This_App).Partition[1].push_back((*This_App).n - 1);
	(*This_App).Node[1][(*This_App).n - 1].type = 1;
	//cout << (*This_App).Partition[0].size() << endl;
}