// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

// stdafx.h
#pragma once
#include "targetver.h"
#include <tchar.h>

// TODO:
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <time.h>
#include <malloc.h>

#include <list>
#include <vector>
#include <stack>
//#include<stdafx.h>
//#include <random>
#include <iomanip>
#include <string>
//#include "math_operation.cpp"
using namespace std;
#include <algorithm>
//#include "server.h"
//#include"processing.h"

//#ifdef _MSC_VER
//#if _MSC_VER<=1200
//#pragma optimize("g",off)
//#endif
//#endif

//edge vector
typedef struct edge
{
	int u_parent;
	int v_child;
	float weight;
	float rate;
};

typedef struct node
{
	int Num;
	vector<int> parents;
	vector<int> children;
	float comp;
	vector<int> consist;
	float blvl;
	float tlvl;
	int mach;
	int type;
};


//machine???
//for each machine
typedef struct machine
{
	vector <int> mach_job;//job sequence
	float mach_time;//
	float speed;//xxx cycles/s
	int type;
	int subtype;
	//bool avaliable;
};


typedef struct mytask
{
	int num; //the task number
	int order;//the order in the sequence
	int machine;//
	int type;

	//to calculate the makespan
	vector<float> job_earliest;
	float job_complete;
	float job_start;
	float job_period;

	//computation time
	float comp_time;

	bool choose;
	double p;
};


typedef struct app_struct
{
	int n; //number of the tasks
	vector<vector <bool> > prec_constraint;//if j is precedent of i, then prec_constraint[j][i] = 1
	//vector<vector <bool> > after_constraint(n); ??
	vector<vector <float> > edge_matrix;//store the graph weight, not changed
	//precedence constraint matrix
	vector<vector <float> > prec_value; //store the graph and changed when coarsening
	//the processing time of jobs
	vector<float> pro_time1;
	vector<float> ddl;
	vector<bool> if_hard_ddl;
	//vector<edge> Edge;
	//node vector
	vector<vector <node> > Node;
	vector<vector <int> > Partition;
	vector<vector<int>> partition_things;
	
	float arrive_time;
	float start_time;
	int order;
	float percentage_hard;
	float low_bound_ddl;
	float high_bound_ddl;
	//int cluster;
};

typedef struct environment_struct
{
	vector<float> bandwidth_in;
	vector<float> speed;
	vector<float> bandwidth;
	float distance_square_things;
	vector<vector<float>> battery_capacity;
	float delay;

	int things_num;
	vector<int> things_one;
	int fog_num;
	int cloud_num;
	int now_cluster;

	vector<float> mach_time;
	float lifetime;
	double tenergy;
};

//for each individual
typedef struct Individual
{
	//_task task[max_n];
	float makespan;
	float energy_t;
	vector<vector<int>> permutation;//processing order
	//vector<int> mach_assign;//the machine assigned
	vector<float> tmach_time; //the machine finish time
	vector<float> battery_consumption; //the battery consumption
	//vector<int> job_earliest;
	//int idle_count;
	float time_f;
	float total_tardiness;


}Indi;

#define E_elec 0.0524
#define deta_amp 0.000001 //10^(-12)*1024*1024
#define V_dd 1

#define E_max_cloud 400
#define E_max_fog 400
#define E_static_cloud 100
#define E_static_fog 40
#define E_comp_things 0.00073
#define cop 3

//vector<vector<float>> bandwidth_in(3);

int partition(int low, int high, int *arr, float *value);
void qSort(int low, int high, int *arr, float *value);
int get_min(vector<float> arr);

//to compare different elements
bool cmpare(const edge &a, const edge &b);
bool cmparebl(const node &a, const node &b);
bool complare(int a, int b);

//partition the tasks
void rating(app_struct *This_App);
void coarsening(vector<edge> M, app_struct *This_App);
vector<edge> GRDY(app_struct *This_App);
void Repair(app_struct *This_App);
bool if_need_repair(app_struct *This_App);
void simple_partition(app_struct *This_App, environment_struct This_Envi);

void Calculate_B(app_struct *This_App); 
void Calculate_B1(app_struct *This_App);

int machine_assign_EFT(vector<machine> tmach, mytask tempt);
float makespan_calculate(app_struct *This_App, environment_struct This_Envi, vector<machine> tmach, vector<int> permutation, int *mach_assign);
void partition_graph(app_struct *This_App, environment_struct This_Envi);

//read the data
void ReadData_rnc1(app_struct *This_App);
//void graphviz_print(app_struct This_App);

// TODO: evolution needed
/*void Initialization(app_struct This_App);
void Evolution(app_struct This_App, environment_struct This_Envi);
void decode(int l, app_struct This_App);//for the l-th individual
mytask happen_permutation2(int j, vector<vector <float> > Q, mytask *x);
void evalution(int l, app_struct This_App, environment_struct This_Envi);
int machine_assign_EFT_in(vector<machine> tmach, mytask tempt);
int Calculate_p(app_struct This_App, vector<Indi> AS_2);

//pareto check
int check_dominance(Indi &a, Indi &b); 
int check_equal(Indi &a, Indi &b);
vector<Indi> Update_AS(Indi &x);
vector<Indi> Update_AS2(Indi &x);*/