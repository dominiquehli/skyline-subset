#include "SkyTreeUtil.h"

void InsertSkyline(vector<Point>& SkylineList, SNode& SkyNode)
{
	int nNumChild = (int)SkyNode.nNumChild;
	if (nNumChild > 0)
	{
		int nNumPnt = (int)SkyNode.NodePointList.size();
		for (int nPnt = 0; nPnt < nNumPnt; nPnt++)
			SkylineList.push_back(SkyNode.NodePointList[nPnt]);

		for (int nChild = 0; nChild < nNumChild; nChild++)
			InsertSkyline(SkylineList, SkyNode.ChildNode[nChild]);
	}
	else
	{
		int nNumPnt = (int)SkyNode.NodePointList.size();
		for (int nPnt = 0; nPnt < nNumPnt; nPnt++)
			SkylineList.push_back(SkyNode.NodePointList[nPnt]);
	}
}


void ClearSkyTree(SNode& SkyTree)
{
	stack<SNode> Stack;
	PushStack(Stack, SkyTree);

	while (!Stack.empty())
	{
		delete[] Stack.top().ChildNode;
		Stack.pop();
	}
}


void PushStack(stack<SNode>& Stack, SNode& SkyNode)
{
	int nNumChild = (int)SkyNode.nNumChild;
	if (nNumChild > 0)
	{
		Stack.push(SkyNode);
		for (int nChild=0; nChild<nNumChild; nChild++)
			PushStack(Stack, SkyNode.ChildNode[nChild]);
	}
}


void PrintSkyTree(int nHeight, SNode& SkyNode)
{
	int nNumChild = (int)SkyNode.nNumChild;
	if (nNumChild > 0)
	{
		for (int nBlank = 0; nBlank < nHeight; nBlank++) cout << "\t";
		{
			int nNumPnt = SkyNode.NodePointList.size();
			
			cout << "ID: ";
			for (int nPnt = 0; nPnt < nNumPnt; nPnt++)
				cout << SkyNode.NodePointList[nPnt][0] << " ";
			cout << "\tLattice: " << SkyNode.nLatticeID << endl;
		}

		for (int nChild=0; nChild<nNumChild; nChild++)
			PrintSkyTree(nHeight + 1, SkyNode.ChildNode[nChild]);
	}
	else
	{
		for (int nBlank = 0; nBlank < nHeight; nBlank++) cout << "\t";
		{
			int nNumPnt = SkyNode.NodePointList.size();
			
			cout << "ID: ";
			for (int nPnt = 0; nPnt < nNumPnt; nPnt++)
				cout << SkyNode.NodePointList[nPnt][0] << " ";
			cout << "\tLattice: " << SkyNode.nLatticeID << endl;
		}
	}
}