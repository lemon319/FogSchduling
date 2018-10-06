#include"stdafx.h"


/*
void graphviz_print(app_struct This_App)
{
	int i, j, u, v;
	//for (i = 0; i < 3; i++)
	//{
	cout << "node[color = \"grey\"]" << endl;
	for (j = 0; j < This_App.Partition[0].size(); j++)
	{
		//father[label = "father", shape = "box"]
		//mother[label = "mother", shape = "box"]
		cout << This_App.Partition[0][j] << "[label = \"" << This_App.Partition[0][j] << "\"]" << endl;
	}
	cout << "node[color = \"red\"]" << endl;
	for (j = 0; j < This_App.Partition[1].size(); j++)
	{
		cout << This_App.Partition[1][j] << "[label = \"" << This_App.Partition[1][j] << "\"]" << endl;
	}

	cout << "node[color = \"blue\"]" << endl;
	for (j = 0; j < This_App.Partition[2].size(); j++)
	{
		cout << This_App.Partition[2][j] << "[label = \"" << This_App.Partition[2][j] << "\"]" << endl;
	}
	//}

	for (i = 0; i < This_App.Edge.size(); i++)
	{
		u = This_App.Edge[i].u_parent;
		v = This_App.Edge[i].v_child;
		//mach1 = nodelist[u].mach;
		//mach2 = nodelist[v]
		cout << u << "->" << v << "[label = \"" << This_App.Edge[i].weight << "\"]" << endl;
	}
}*/