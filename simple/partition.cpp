#include "stdafx.h"

/*void rating(app_struct *This_App)
{
	//vector<float> edge_rate;
	float c_u, c_v;
	//edge_rate.resize(Edge.size());
	for (int i = 0; i < (*This_App).Edge.size(); i++)
	{
		c_u = (*This_App).pro_time1[(*This_App).Edge[i].u_parent];
		c_v = (*This_App).pro_time1[(*This_App).Edge[i].v_child];
		if (c_u*c_v == 0)
			(*This_App).Edge[i].rate = 0;
		else
		{
			(*This_App).Edge[i].rate = (*This_App).Edge[i].weight*(*This_App).Edge[i].weight / (c_u*c_v);
		}
	}

	sort((*This_App).Edge.begin(), (*This_App).Edge.end(), cmpare);
}
vector<edge> GRDY(app_struct *This_App)
{
	int edge_num = (*This_App).Edge.size();
	vector<edge> M;
	int i = 0, j, k;

	//copy the matrix of edge;
	vector<vector <bool> > edge_matrix1((*This_App).prec_value.size());

	for (i = 0; i < (*This_App).n; i++)
	{
		edge_matrix1[i].resize((*This_App).n);
		for (j = 0; j<(*This_App).n; j++)
		{
			if ((*This_App).prec_value[i][j]>0)
				edge_matrix1[i][j] = true;
		}
	}

	i = 0;
	while ((edge_num > 0) && (i < (*This_App).Edge.size()))
	{
		//add it to M
		if (edge_matrix1[(*This_App).Edge[i].u_parent][(*This_App).Edge[i].v_child])
		{
			M.push_back((*This_App).Edge[i]);

			//remove all the edges adjacent to its endpoints from Edge
			for (j = 0; j < (*This_App).n; j++)
			{
				//remove the edges connected with these nodes
				if (edge_matrix1[(*This_App).Edge[i].u_parent][j])
				{
					edge_num--;
					edge_matrix1[(*This_App).Edge[i].u_parent][j] = false;
				}

				if (edge_matrix1[(*This_App).Edge[i].v_child][j])
				{
					edge_num--;
					edge_matrix1[(*This_App).Edge[i].v_child][j] = false;
				}
			}
			for (j = 0; j < (*This_App).n; j++)
			{
				if (edge_matrix1[j][(*This_App).Edge[i].u_parent])
				{
					edge_num--;
					edge_matrix1[j][(*This_App).Edge[i].u_parent] = false;
				}
				if (edge_matrix1[j][(*This_App).Edge[i].v_child])
				{
					edge_num--;
					edge_matrix1[j][(*This_App).Edge[i].v_child] = false;
				}
			}
		}
		i++;
	}
	return M;
}
void coarsening(vector<edge> M, app_struct *This_App)
{
	vector<node> tnodelist;
	int now = 1;
	node merge_t;
	int u, v, i, j;
	vector<int> nodes_to_delete;

	for (i = 0; i < (*This_App).Node[now].size(); i++)
	{
		tnodelist.push_back((*This_App).Node[now][i]);
	}

	for (i = 0; i < M.size(); i++)
	{
		//merge the nodes in M
		u = M[i].u_parent;
		v = M[i].v_child;
		merge_t = tnodelist[v];

		//the node vector
		tnodelist[u].consist.push_back(v);
		//tnodelist[u].children.insert(tnodelist[u].children.end(), merge_t.children.begin(), merge_t.children.end());
		//tnodelist[u].parents.insert(tnodelist[u].parents.end(), merge_t.parents.begin(), merge_t.parents.end());
		tnodelist[u].comp += merge_t.comp;
		tnodelist[v].consist.clear();


		for (j = 0; j < (*This_App).n; j++)
		{
			if ((u != j) && (v != j))
			{
				//merge the edges of the nodes (edges)
				//edge_matrix[u][j] = edge_matrix[u][j] || edge_matrix[v][j];
				//edge_matrix[j][u] = edge_matrix[j][u] || edge_matrix[j][v];

				//merge the edges of the nodes (cmn costs)
				(*This_App).prec_value[u][j] += (*This_App).prec_value[v][j];
				(*This_App).prec_value[j][u] += (*This_App).prec_value[j][v];
			}

		}
	}


	//erase the precedence constraints matrix rows and columns
	for (i = 0; i < M.size(); i++)
	{
		nodes_to_delete.push_back(M[i].v_child);
	}
	sort(nodes_to_delete.begin(), nodes_to_delete.end(), complare);

	//erase the nodes
	*//*for (i = 0; i < tnodelist.size(); i++)
	{
	if (tnodelist[i].consist.empty())
	{
	tnodelist.erase(tnodelist.begin() + i);
	i--;
	}
	}*//*
	for (i = 0; i < nodes_to_delete.size(); i++)
	{
		tnodelist.erase(tnodelist.begin() + nodes_to_delete[i]);
	}

	for (i = 0; i < nodes_to_delete.size(); i++)
	{
		//edge_matrix.erase(edge_matrix.begin() + nodes_to_delete[i]);
		(*This_App).prec_value.erase((*This_App).prec_value.begin() + nodes_to_delete[i]);

		for (j = 0; j < (*This_App).prec_value.size(); j++)
		{
			//edge_matrix[j].erase(edge_matrix[j].begin() + nodes_to_delete[i]);
			(*This_App).prec_value[j].erase((*This_App).prec_value[j].begin() + nodes_to_delete[i]);
		}
	}

	//insert the nodelist to node matrix
	(*This_App).Node[1].clear();
	(*This_App).Node[1] = tnodelist;


	//update n
	//now = Node.size() - 1;
	(*This_App).n = (*This_App).Node[now].size();

	for (i = 0; i < (*This_App).Node[now].size(); i++)
		(*This_App).Node[now][i].Num = i;

	//update the Edge vector
	(*This_App).Edge.clear();
	edge tedge;

	for (i = 0; i < (*This_App).n; i++)
	{
		//clear the children and parents
		(*This_App).Node[now][i].children.clear();
		(*This_App).Node[now][i].parents.clear();
	}

	for (i = 0; i < (*This_App).n; i++)
	{
		//clear the children and parents
		//Node[now][i].children.clear();
		//Node[now][i].parents.clear();

		for (j = i; j < (*This_App).n - 1; j++)
		{
			if ((*This_App).prec_value[i][j]>0)
			{
				tedge.u_parent = i;
				tedge.v_child = j;
				tedge.weight = (*This_App).prec_value[i][j];
				(*This_App).Edge.push_back(tedge);

				//add the child and parent
				(*This_App).Node[now][i].children.push_back(j);
				(*This_App).Node[now][j].parents.push_back(i);
			}
		}
	}

	//start and the end
	for (i = 1; i < (*This_App).n - 1; i++)
	{
		if ((*This_App).Node[now][i].parents.empty())
		{
			(*This_App).Node[now][0].children.push_back(i);
			(*This_App).Node[now][i].parents.push_back(0);

		}
		if ((*This_App).Node[now][i].children.empty())
		{
			(*This_App).Node[now][(*This_App).n - 1].parents.push_back(i);
			(*This_App).Node[now][i].children.push_back((*This_App).n - 1);
		}
	}

}
void Repair(app_struct *This_App)
{
	int i, j, now = 1, u, v, k;
	vector<edge> M;
	//edge edge_t;

	vector<node> tnodelist;
	node merge_t;
	//vector<int> nodes_to_delete;

	//a new nodelist
	for (i = 0; i < (*This_App).Node[now].size(); i++)
	{
		tnodelist.push_back((*This_App).Node[now][i]);
	}

	for (i = 0; i < (*This_App).prec_value.size(); i++)
	{
		for (j = i + 1; j < (*This_App).prec_value.size(); j++)
		{
			//there is a ring between i and j
			if (((*This_App).prec_value[i][j]>0) && ((*This_App).prec_value[j][i]>0))
			{
				u = i;
				v = j;
				merge_t = tnodelist[v];

				//add the consist of the child to parent
				for (k = 0; k < merge_t.consist.size(); k++)
					tnodelist[u].consist.push_back(merge_t.consist[k]);

				tnodelist[u].comp += merge_t.comp;
				tnodelist.erase(tnodelist.begin() + v);

				for (k = 0; k < (*This_App).prec_value.size(); k++)
				{
					if ((u != k) && (v != k))
					{
						//merge the edges of the nodes (cmn costs)
						(*This_App).prec_value[u][k] += (*This_App).prec_value[v][k];
						(*This_App).prec_value[k][u] += (*This_App).prec_value[k][v];
					}

				}
				(*This_App).prec_value.erase((*This_App).prec_value.begin() + v);
				for (k = 0; k < (*This_App).prec_value.size(); k++)
				{
					//edge_matrix[j].erase(edge_matrix[j].begin() + nodes_to_delete[i]);
					(*This_App).prec_value[k].erase((*This_App).prec_value[k].begin() + v);
				}
				j--;
			}
		}
	}
	//return M;
	//insert the nodelist to node matrix
	(*This_App).Node[1].clear();
	(*This_App).Node[1] = tnodelist;


	//update n
	//now = Node.size() - 1;
	(*This_App).n = (*This_App).Node[now].size();

	for (i = 0; i < (*This_App).Node[now].size(); i++)
		(*This_App).Node[now][i].Num = i;

	//update the Edge vector
	(*This_App).Edge.clear();
	edge tedge;

	for (i = 0; i < (*This_App).n; i++)
	{
		//clear the children and parents
		(*This_App).Node[now][i].children.clear();
		(*This_App).Node[now][i].parents.clear();
	}

	for (i = 0; i < (*This_App).n; i++)
	{
		//clear the children and parents
		//Node[now][i].children.clear();
		//Node[now][i].parents.clear();

		for (j = i; j < (*This_App).n - 1; j++)
		{
			if ((*This_App).prec_value[i][j]>0)
			{
				tedge.u_parent = i;
				tedge.v_child = j;
				tedge.weight = (*This_App).prec_value[i][j];
				(*This_App).Edge.push_back(tedge);

				//add the child and parent
				(*This_App).Node[now][i].children.push_back(j);
				(*This_App).Node[now][j].parents.push_back(i);
			}
		}
	}

	//start and the end
	for (i = 1; i < (*This_App).n - 1; i++)
	{
		if ((*This_App).Node[now][i].parents.empty())
		{
			(*This_App).Node[now][0].children.push_back(i);
			(*This_App).Node[now][i].parents.push_back(0);

		}
		if ((*This_App).Node[now][i].children.empty())
		{
			(*This_App).Node[now][(*This_App).n - 1].parents.push_back(i);
			(*This_App).Node[now][i].children.push_back((*This_App).n - 1);
		}
	}
}
bool if_need_repair(app_struct *This_App)
{
	int i, j;
	for (i = 0; i < (*This_App).n; i++)
	{
		for (j = i + 1; j < (*This_App).n; j++)
		{
			if (((*This_App).prec_value[i][j]>0) && ((*This_App).prec_value[j][i]>0))
			{
				return true;
			}
		}
	}
	return false;
}
*/
int machine_assign_EFT(vector<machine> tmach, mytask tempt)
{
	int key, i, tm = tmach.size();
	vector<float> cpl_time(tm);
	key = tm;

	for (i = 0; i<tm; i++)
	{
		cpl_time[i] = tempt.job_earliest[i]>tmach[i].mach_time ? tempt.job_earliest[i] : tmach[i].mach_time;
		cpl_time[i] += tempt.comp_time / tmach[i].speed;
	}

	if (key == tm)
	{
		key = get_min(cpl_time);
	}
	return key;
}
float makespan_calculate(app_struct *This_App, environment_struct This_Envi, vector<machine> tmach, vector<int> permutation, int *mach_assign)
{
	float tenergy;
	int i, j, k, task, mach1, vol1, task1, now = 1, mach_num = tmach.size();
	//vector<machine> tmach(mach_num);
	vector<mytask> ttask((*This_App).n);
	tenergy = 0;

	/*for (i = 0; i < tmach.size(); i++)
	{
	tmach[i].mach_time = 0;
	tmach[i].mach_num = 0;
	tmach[i].speed = mach_speed[i];
	}*/

	for (i = 0; i < (*This_App).n; i++)
	{
		ttask[i].job_complete = 0;
		for (j = 0; j < mach_num; j++)
			ttask[i].job_earliest.push_back(0);

		ttask[i].job_start = 0;
		ttask[i].job_period = 0;
		ttask[i].order = i;
		ttask[(*This_App).Node[now][i].Num].comp_time = (*This_App).Node[now][i].comp;
	}

	int flag1 = 0, t1, t2;
	while (flag1<(*This_App).n)
	{
		task = permutation[flag1];
		flag1++;
		if (task == 0)
			mach1 = 0;
		else
			mach1 = machine_assign_EFT(tmach, ttask[task]);

		ttask[task].job_start = ttask[task].job_earliest[mach1]>tmach[mach1].mach_time ? ttask[task].job_earliest[mach1] : tmach[mach1].mach_time;
		ttask[task].job_period = ttask[task].comp_time / tmach[mach1].speed;
		ttask[task].job_complete = ttask[task].job_start + ttask[task].job_period;
		ttask[task].machine = mach1;

		*(mach_assign + task) = mach1;
		tmach[mach1].mach_time = ttask[task].job_complete;
		tmach[mach1].mach_job.push_back(task);

		//considering the earliest time it could be processed
		for (j = 0; j < (*This_App).Node[now][task].children.size(); j++)
		{
			task1 = (*This_App).Node[now][task].children[j];
			for (k = 0; k<mach_num; k++)
			{
				t1 = k<mach1 ? k : mach1;
				t2 = k>mach1 ? k : mach1;
				if (k == mach1)
				{
					ttask[task1].job_earliest[k] = ttask[task].job_complete>ttask[task1].job_earliest[k] ? ttask[task].job_complete : ttask[task1].job_earliest[k];
				}
				else
				{
					if ((This_Envi.bandwidth[t1*(mach_num - 1) + t2] != 0) && (tmach[mach1].type + tmach[k].type == 3))
						ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k] > (ttask[task].job_complete + (*This_App).prec_value[task][task1] / This_Envi.bandwidth[t1*(mach_num - 1) + t2] + This_Envi.delay) ? ttask[task1].job_earliest[k] : (ttask[task].job_complete + (*This_App).prec_value[task][task1] / This_Envi.bandwidth[t1*(mach_num - 1) + t2] + This_Envi.delay);
					else if (This_Envi.bandwidth[t1*(mach_num - 1) + t2] == 0)
						ttask[task1].job_earliest[k] = RAND_MAX;
					else
						ttask[task1].job_earliest[k] = ttask[task1].job_earliest[k] > (ttask[task].job_complete + (*This_App).prec_value[task][task1] / This_Envi.bandwidth[t1*(mach_num - 1) + t2]) ? ttask[task1].job_earliest[k] : (ttask[task].job_complete + (*This_App).prec_value[task][task1] / This_Envi.bandwidth[t1*(mach_num - 1) + t2]);
				}
			}

		}

	}

	float makespan1 = 0;
	for (i = 0; i<tmach.size(); i++)
	{
		if (tmach[i].mach_time>makespan1)
			makespan1 = tmach[i].mach_time;
	}

	(*This_App).Partition.resize(3);
	for (i = 0; i < 3; i++)
	{
		(*This_App).Partition[i].resize(tmach[i].mach_job.size());
		(*This_App).Partition[i] = tmach[i].mach_job;
	}
	return makespan1;
}
void simple_partition(app_struct *This_App, environment_struct This_Envi)
{
	int i, j, k, flag1=0, task, taskp;
	vector<mytask> ttask((*This_App).n);
	for (i = 0; i < (*This_App).n; i++)
	{
		ttask[i].choose = false;
		ttask[i].comp_time = (*This_App).pro_time1[i];
		ttask[i].job_complete = 0;
		ttask[i].job_period = 0;
		ttask[i].job_start = 0;
		ttask[i].machine = 0;//0:=things, 1:=fog
		ttask[i].job_earliest.resize(2);
	}
	vector<node> Node = (*This_App).Node[(*This_App).Node.size() - 1];
	vector<int> avaliable_tasks, t_av_tasks;
	bool test = true;
	vector<float> mach_time;
	
	for (i = 0; i < Node[0].children.size(); i++)
	{
		//avaliable_tasks.push_back(Node[0].children[i]);
		task = Node[0].children[i];
		ttask[task].choose = true;
		ttask[task].machine = 0; //things
		//avaliable_tasks.push_back(task);
	}

	mach_time.push_back(0);
	mach_time.push_back(0);

	for (i = 0; i < Node[0].children.size(); i++)
	{
		task = Node[0].children[i];
		for (j = 0; j < Node[task].children.size(); j++)
		{
			test = true;
			for (k = 0; k < Node[Node[task].children[j]].parents.size(); k++)
			{
				test = test && ttask[Node[Node[task].children[j]].parents[k]].choose;
				if (!test)
					break;
			}
			if (test)
			{
				avaliable_tasks.push_back(Node[task].children[j]);
				ttask[Node[task].children[j]].job_earliest[0] = ttask[Node[task].children[j]].job_earliest[0]>ttask[task].job_complete ? ttask[Node[task].children[j]].job_earliest[0] : ttask[task].job_complete;
				mach_time[0] += (*This_App).pro_time1[task] / This_Envi.speed[0];
			}
		}
	}
	float avg_things_dis = 1, avg_fog_dis = 5;

	float energy[2], comp_time[2];
	while (!ttask[ttask.size()-1].choose)
	{
		for (i = 0; i < avaliable_tasks.size(); i++)
		{
			task = avaliable_tasks[i];
			if (ttask[task].choose)
			{
				avaliable_tasks.erase(avaliable_tasks.begin() + i);
				i--;
				continue;
			}
			else
			{
				energy[0] = 0;
				energy[1] = 0;
				comp_time[0] = mach_time[0] > ttask[task].job_earliest[0] ? mach_time[0] : ttask[task].job_earliest[0];
				comp_time[1] = mach_time[1] > ttask[task].job_earliest[1] ? mach_time[1] : ttask[task].job_earliest[1];

				for (j = 0; j < Node[task].parents.size(); j++)
				{
					taskp = Node[task].parents[j];
					if (ttask[taskp].machine == 0)
					{
						//0->0
						//comp energy
						energy[0] += E_comp_things*((*This_App).pro_time1[task]);
						energy[0] += (*This_App).edge_matrix[taskp][task] * E_elec * 2 + deta_amp*(*This_App).edge_matrix[taskp][task] * avg_things_dis* avg_things_dis;

						comp_time[0] += (*This_App).pro_time1[task] / This_Envi.speed[0];

						//0->1
						//send
						energy[1] += (*This_App).edge_matrix[taskp][task] * E_elec + deta_amp*(*This_App).edge_matrix[taskp][task] * avg_things_dis * avg_things_dis;
						//transfer
						comp_time[1] += (*This_App).edge_matrix[taskp][task] / This_Envi.bandwidth[1];
						//comp
						comp_time[1] += (*This_App).pro_time1[task] / This_Envi.speed[1];
					}
					else
					{
						//1->0
						energy[0] += E_comp_things*(*This_App).pro_time1[task];
						//receive
						energy[0] += (*This_App).edge_matrix[taskp][task] * E_elec;
						
						//transfer
						comp_time[0] += (*This_App).edge_matrix[taskp][task] / This_Envi.bandwidth[1];
						//comp
						comp_time[0] += (*This_App).pro_time1[task] / This_Envi.speed[0];
						
						//1->1
						comp_time[1] += (*This_App).pro_time1[task] / This_Envi.speed[1];
					}
				}

				//make a judge
				if ((comp_time[0] <= comp_time[1]) && (energy[0] <= energy[1]))
					ttask[task].machine = 0;
				else if ((comp_time[0] >= comp_time[1]) && (energy[0] >= energy[1]))
					ttask[task].machine = 1;
				else
				{
					if ((energy[0] - energy[1]) / (energy[0] + energy[1]) + (comp_time[0] - comp_time[1]) / (comp_time[0] + comp_time[1]) > 0)
						ttask[task].machine = 1;
					else
						ttask[task].machine = 0;
				}
				ttask[task].choose = true;
				ttask[task].job_complete = comp_time[ttask[task].machine];
				mach_time[ttask[task].machine] = ttask[task].job_complete;
			}
		}

		t_av_tasks.clear();
		for (i = 0; i < avaliable_tasks.size(); i++)
		{
			task = avaliable_tasks[i];
			for (j = 0; j < Node[task].children.size(); j++)
			{
				test = true;
				for (k = 0; k < Node[Node[task].children[j]].parents.size(); k++)
				{
					taskp = Node[Node[task].children[j]].parents[k];
					test = test && ttask[taskp].choose;
					
					if (!test)
						break;
				}
				if (test)
				{
					t_av_tasks.push_back(Node[task].children[j]);
					//ttask[Node[task].children[j]].job_earliest[ttask[taskp].machine] = ttask[Node[task].children[j]].job_earliest[ttask[task].machine]>ttask[Node[Node[task].children[j]].parents[k]].job_complete ? ttask[Node[task].children[j]].job_earliest[ttask[task].machine] : ttask[Node[Node[task].children[j]].parents[k]].job_complete;
					ttask[Node[task].children[j]].job_earliest[0] = ttask[Node[task].children[j]].job_earliest[0]>ttask[task].job_complete ? ttask[Node[task].children[j]].job_earliest[0] : ttask[task].job_complete;
				}
			}
		}
		avaliable_tasks = t_av_tasks;
	}

	(*This_App).Partition.resize(3);
	for (i = 0; i < (*This_App).n; i++)
	{
		(*This_App).Partition[ttask[i].machine].push_back(i);
		(*This_App).Node[1][i].mach = ttask[i].machine;
	}

	/*vector<node> things_tasks;
	things_tasks.resize((*This_App).n);
	node newtask;
	for (i = 0; i < (*This_App).Partition[0].size(); i++)
	{
		newtask.Num = (*This_App).Partition[0][i];
		//newtask.order = i;
		newtask.mach = -1;
		newtask.blvl = -1;
		newtask.parents.clear();
		newtask.children.clear();
		for (j = 0; j < Node[newtask.Num].parents.size(); j++)
		{
			if (ttask[Node[newtask.Num].parents[j]].machine == 0)
				newtask.parents.push_back(Node[newtask.Num].parents[j]);
		}

		for (j = 0; j < Node[newtask.Num].children.size(); j++)
		{
			if (ttask[Node[newtask.Num].children[j]].machine == 0)
				newtask.children.push_back(Node[newtask.Num].children[j]);
		}

		if (newtask.Num != 0)
			things_tasks[newtask.Num] = newtask;
	}
	(*This_App).partition_things.resize(This_Envi.fog_num);
	//if (This_Envi.fog_num>=Node[0].children.size())

	vector<vector<int>> partition_things_t;
	stack<node> temp;
	node popnode;

	i = 0, j=0;
	int machnum = 0;
	for (i = 0; i < things_tasks.size(); i++)
	{		
		if (things_tasks[i].blvl == -1)
		{
			temp.push(things_tasks[i]);
			things_tasks[i].blvl = 100;
			while (!temp.empty())
			{
				popnode = temp.top();
				temp.pop();
				things_tasks[popnode.Num].mach = machnum;
				for (i = 0; i < popnode.children.size(); i++)
				{
					if (things_tasks[popnode.children[i]].blvl == -1)
					{
						temp.push(things_tasks[popnode.children[i]]);
						things_tasks[popnode.children[i]].blvl = 100;

					}
				}
				
				for (i = 0; i < popnode.parents.size(); i++)
				{
					if (things_tasks[popnode.parents[i]].blvl == -1)
					{
						temp.push(things_tasks[popnode.parents[i]]);
						things_tasks[popnode.parents[i]].blvl = 100;
					}
				}
			}
			machnum++;
		}
		/*else if ((things_tasks[i].blvl>0) && (things_tasks[i].parents.size()==1))
		{
			for (j = 0; j < partition_things_t.size(); j++)
			{
				if (things_tasks[i].parents[0] == partition_things_t[j][0])
				{
					partition_things_t[j].push_back(things_tasks[i].Num);
					things_tasks[i].blvl = 1000;
				}
			}
		}*/
		//else
	//	{

	//	}
		
	/*}
	(*This_App).partition_things.resize(machnum);
	for (i = 0; i < (*This_App).Partition[0].size(); i++)
	{
		if (things_tasks[(*This_App).Partition[0][i]].blvl == 100)
		{
			machnum = things_tasks[(*This_App).Partition[0][i]].mach;
			(*This_App).partition_things[machnum].push_back((*This_App).Partition[0][i]);
		}
	}
	int maxnum=0, mx;
	for (i = 0; i < (*This_App).partition_things.size(); i++)
	{
		if (maxnum < (*This_App).partition_things[i].size())
		{
			maxnum = (*This_App).partition_things[i].size();
			mx = i;
		}
	}
	ttask.clear();*/

}
void partition_graph(app_struct *This_App, environment_struct This_Envi)
{
	//vector<float> speed = { 1, 5, 10 };
	vector<float> speed(3);
	vector<node> tnode;
	float delay = 0;
	int i, j, now = 1;
	int mach_num = 3;

	vector<machine> tmach(mach_num);

	for (i = 0; i < tmach.size(); i++)
	{
		tmach[i].mach_time = 0;
		//tmach[i].mach_num = 0;
		tmach[i].speed = speed[i];
		tmach[i].type = i;
	}


	//sort with b-level
	for (i = 0; i < (*This_App).n; i++)
		tnode.push_back((*This_App).Node[now][i]);

	sort(tnode.begin(), tnode.end(), cmparebl);

	//initialize the permutation
	vector<int> permutation((*This_App).n);
	for (i = 0; i < (*This_App).n; i++)
	{
		permutation[i] = (*This_App).Node[now][i].Num;
	}

	vector<float> bandwidth;
	//initialize the bandwidth
	bandwidth.resize(mach_num*(mach_num + 1) / 2);
	bandwidth[0] = 0; //[0][0]
	bandwidth[1] = 1; //[0][1] things-fog, 2
	bandwidth[2] = 0; //[0][2] things-cloud, no connection
	bandwidth[3] = 0; //[1][1]
	bandwidth[4] = 3; //[1][2] fog-cloud, 5
	bandwidth[5] = 0; //[2][2] 

	vector<int> mach_assign((*This_App).n);
	makespan_calculate(This_App, This_Envi, tmach, permutation, &(mach_assign[0]));

	for (i = 0; i < (*This_App).n; i++)
	{
		(*This_App).Node[now][i].mach = mach_assign[i];
	}
	

	//print out
	//graphviz_print(*(This_App));

}