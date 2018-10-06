#include"stdafx.h"

int partition(int low, int high, int *arr, float *value)
{
	int t = *(arr + low);
	//int pivotkey = Population[*(arr+low)].makespan;
	float pivotkey = value[*(arr + low)];
	while (low < high)
	{
		//while (low < high && Population[*(arr+high)].makespan >= pivotkey) --high;
		while (low < high && value[*(arr + high)] >= pivotkey) --high;
		*(arr + low) = *(arr + high);
		while (low < high && value[*(arr + low)] <= pivotkey) ++low;
		*(arr + high) = *(arr + low);
	}
	*(arr + low) = t;
	return low;
}

void qSort(int low, int high, int *arr, float *value)
{
	int pivotloc;
	if (low < high)
	{
		pivotloc = partition(low, high, arr, value);
		qSort(low, pivotloc - 1, arr, value);
		qSort(pivotloc + 1, high, arr, value);
	}
}

int get_min(vector<float> arr)
{
	int i;
	float min;
	int min_num = 0, count = 1, min_num2;
	const int max_M = 50;

	int rank[max_M];
	int numN = arr.size();
	for (i = 0; i<numN; i++)
		rank[i] = i;

	qSort(0, numN - 1, rank, &(arr[0]));

	//	qSort(0, Indi_Num-1, arr, makespan);

	min = arr[rank[0]];
	//	min = *(arr+0);

	for (i = 1; i<numN; i++)
	{
		if (arr[rank[i]] != min)
		{
			count = i;
			break;
		}
		else
			count++;
	}

	if (count != 0)
		min_num2 = rand() % count;
	else
		i = 0;
	min_num = rank[min_num2];

	return min_num;
}

bool cmpare(const edge &a, const edge &b)
{
	return a.rate > b.rate;
}
bool cmparebl(const node &a, const node &b)
{
	return a.blvl > b.blvl;
}
bool complare(int a, int b)
{
	return a > b;
}