#pragma once
#include"stdafx.h"

int AS_num, AS_num2;
vector<Indi> AS;   //global AS
vector<Indi> AS_2; //temp AS

int check_dominance(Indi &a, Indi &b)  //返回1代表a支配b, 返回-1代表b支配a, 0表示互不支配
{
	int flag1 = 0;
	int flag2 = 0;
	float delta1 = float(0.5);

	if ((b.makespan - a.makespan>delta1) || (b.energy_t - a.energy_t>delta1))
		flag1 = 1;

	if ((a.makespan - b.makespan>delta1) || (a.energy_t - b.energy_t>delta1))
		flag2 = 1;


	if (flag1 == 1 && flag2 == 0)
		return (1);
	else if (flag1 == 0 && flag2 == 1)
		return (-1);
	else
		return (0);
}

int check_equal(Indi &a, Indi &b)//返回0说明不相同
{
	float delta1 = float(0.5);

	if (((a.makespan - b.makespan)*(a.makespan - b.makespan)>delta1*delta1) || ((a.energy_t - b.energy_t)*(a.energy_t - b.energy_t)>delta1*delta1))
		return(0);

	return(1);
}

vector<Indi> Update_AS(Indi &x)
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

vector<Indi> Update_AS2(Indi &x)
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