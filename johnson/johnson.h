/************************************************************
Johnson.h: Johnson�㷨���洢Ϊ�ڽӱ�
Date: 2014/1/5
Author: searchop
************************************************************/

#ifndef ALGRAPH_H
#define ALGRAPH_H

#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include <algorithm>
#include <functional>
using namespace std;

//�ڽӱ�Ľṹ
struct ArcNode          //����
{
	int source;        //ͼ�иû���Դ�ڵ�
	int adjvex;        //�û���ָ��Ķ����λ��
	ArcNode *nextarc;  //ָ����һ������ָ��
	int weight;         //ÿ���ߵ�Ȩ��
};

template <typename VertexType>
struct VertexNode           //ͷ���
{
	VertexType data;    //������Ϣ
	ArcNode *firstarc;  //ָ���һ�������ڸö���Ļ���ָ��
	int key;            //Prim:�������Ӹö�������н������б�����С�ߵ�Ȩ��; 
	                    //BellmanFord:��¼��Դ��㵽�ý������·��Ȩ�ص��Ͻ�
	VertexNode *p;      //ָ�������еĸ��ڵ�
	int indegree;       //��¼ÿ����������
};

const int SIZE = 6;

//ͼ�Ĳ���
template <typename VertexType>
class ALGraph
{
public:
	typedef VertexNode<VertexType> VNode;
	ALGraph(int verNum) : vexnum(verNum), arcnum(0)
	{
		for (int i = 0; i < MAX_VERTEX_NUM; i++)
		{
			vertices[i].firstarc = NULL;
			vertices[i].key = INT_MAX/2;
			vertices[i].p = NULL;
			vertices[i].indegree = 0;
		}
	}

	//�����㷨����410ҳͼ����Ȩ����ͼ��
	void createWDG()
	{
		cout << "�����㷨����410ҳͼ����Ȩ����ͼ��..." << endl;
		int i;
		for (i = 1; i < vexnum; i++)
			vertices[i].data = 'a' + i - 1;		

		insertArc(1, 2, 3);
		insertArc(1, 3, 8);
		insertArc(1, 5, -4);
		insertArc(2, 4, 1);
		insertArc(2, 5, 7);
		insertArc(3, 2, 4);
		insertArc(4, 3, -5);
		insertArc(4, 1, 2);
		insertArc(5, 4, 6);
	}

	void createG()
	{
		cout << "����ͼG'...." << endl;
        vertices[0].data = 's';
		insertArc(0, 1, 0);
		insertArc(0, 2, 0);
		insertArc(0, 3, 0);
		insertArc(0, 4, 0);
		insertArc(0, 5, 0);
	}

	//Johnson�㷨����ʹ��BellmanFord�㷨��ʹ���еıߵ�Ȩ�ر�Ϊ�Ǹ�ֵ��
	//Ȼ������dijkstra�㷨������Ե����·��
	int **Johnson()
	{
		createG();          //����G��
		displayGraph();

		if (!BellmanFord(1))
		   cout << "the input graph contains a negative-weight cycle" << endl;
		else
		{
			int h[SIZE];
			int i, j, k;

			//������h[]��ֵ��Ϊ����BellmanFord��ȡ�õ�ֵ��h[i]Ϊ���s������������·��
			for (i = 0; i < vexnum; i++)
			    h[i] = vertices[i].key;
			//�������еıߣ����ߵ�Ȩֵ���¸�ֵ,�������еıߵ�Ȩֵ��Ϊ��ֵ
			for (i = 0; i < vexnum; i++)
			{
				ArcNode *arc = vertices[i].firstarc;
				for (; arc != NULL; arc = arc->nextarc)
					arc->weight = arc->weight + h[arc->source] - h[arc->adjvex];
			}	
			cout << "�ı�Ȩ�غ��ͼΪ��" << endl;
			displayGraph();
			int **d = new int *[SIZE];
			for (j = 0; j < SIZE; j++)
				d[j] = new int[SIZE];
			//��ÿ���������dijkstra�㷨�����ÿ���㵽����������·����������key��
			for (k = 1; k < SIZE; k++)
			{
				Dijkstra(k+1);
				for (i = 1; i < SIZE; i++)
					d[k][i] = vertices[i].key + h[i] - h[k];
			}
			cout << "��������Ľ��Ե���̾��룺" << endl;
			displayTwoDimArray(d);
			return d;
		}
	}

	//���һ����ά����
	void displayTwoDimArray(int **p)
	{
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
				cout << p[i][j] << " ";
			cout << endl;
		}
		cout << "~~~~~~~~~~~~~~~" << endl;
	}

	//��ӡ�ڽ�����
	virtual void displayGraph()
	{
		for (int i = 0; i < vexnum; i++)
		{
			cout << "��" << i+1 << "�������ǣ�" << vertices[i].data
				<< " ��������Ϊ��" << vertices[i].indegree << " �ڽӱ�Ϊ: ";
			ArcNode *arcNode = vertices[i].firstarc;
			while (arcNode != NULL)
			{
				cout << " -> " << vertices[arcNode->adjvex].data
					<< "(" << arcNode->weight << ")";
				arcNode = arcNode->nextarc;
			}
			cout << endl;
		}
		cout << "*******************************************************" << endl;
	}

	//PVnode����׼��
	class PVNodeCompare
	{
	public:
		bool operator() (VNode *pvnode1, VNode *pvnode2)
		{
			return pvnode1->key > pvnode2->key;
		}
	};

	//��ÿ���������·�����ƺ�ǰ�������г�ʼ�������·����ʼ��ΪINT_MAX, p��ʼ��ΪNULL
	//����Դ�ڵ��key��ʼ��Ϊ0
	void InitalizeSingleSource(int index)
	{
		for (int i = 0; i < MAX_VERTEX_NUM; i++)
		{
			vertices[i].key = INT_MAX>>2;
			vertices[i].p = NULL;
		}
		vertices[index].key = 0;
	}

	//�Աߣ�u, v�������ɳڣ���Ŀǰs��v�����·��v.key��s��u�����·������w(u, v)��ֵ���бȽ�
	//����Ⱥ����ֵ��������и��£���v.key���̣����ҽ�p��Ϊu
	void relax(ArcNode *arc)
	{
		//��Ȼ����ˣ���
		if (vertices[arc->adjvex].key > vertices[arc->source].key + arc->weight)
		{
			vertices[arc->adjvex].key = vertices[arc->source].key + arc->weight;
			vertices[arc->adjvex].p = &vertices[arc->source];
		}
	}

	//BellmanFord, indexΪʵ�ʵڼ�����
	bool BellmanFord(int index)
	{
		InitalizeSingleSource(index-1);
		for (int i = 1; i < vexnum; i++)     //ѭ��������vexnum-1��
		{
			//�������еıߣ�����ÿ���߽���һ���ɳ�
			for (int j = 0; j < vexnum; j++)
			{
				for (ArcNode *arc = vertices[j].firstarc; arc != NULL; arc = arc->nextarc)
					relax(arc);
			}
		}
		//�ٴα������еıߣ����ͼ���Ƿ����Ȩ��Ϊ��ֵ�Ļ�·��������ڣ��򷵻�false
		for (int j = 0; j < vexnum; j++)
		{
			for (ArcNode *arc = vertices[0].firstarc; arc != NULL; arc = arc->nextarc)
			{
				if (vertices[arc->adjvex].key > vertices[arc->source].key + arc->weight)
					return false;
			}
		}
		cout << "BellmanFord����ĵ�Դ���·����" << endl;
		for (int i = 1; i < vexnum; i++)
		{
			printPath(index-1, i);
		}
		cout << "**************************************************" << endl;
		return true;
	}

	void Dijkstra(int index)
	{
		InitalizeSingleSource(index-1);
		vector<VNode> snode;       //�����Ѿ��ҵ����·���Ľ��
		vector<VNode *> que;       //�������ָ������飬���������ִ�жѵ��㷨

		//�����ָ������У��γ���keyΪ�ؼ�ֵ����С��
		for (int i = 0; i < vexnum; i++)
			que.push_back(&(vertices[i]));
		//ʹque����pvnodecompare׼�򹹳�һ����С��
		make_heap(que.begin(), que.end(), PVNodeCompare());   

		while (que.empty() == false)
		{
			//��������ӵ����Сkey�Ľ�����
			VNode *node = que.front();
			pop_heap(que.begin(), que.end(), PVNodeCompare());   //�Ӷ���ɾ����С�Ľ�㣬ֻ�Ƿŵ���vector�����
			que.pop_back();      //��vector�е������㳹��ɾ������Ϊ���滹Ҫ������һ�Σ�����Ӱ�����Ķ�����pop�㷨��
			snode.push_back(*node);
			for (ArcNode *arc = node->firstarc; arc != NULL; arc = arc->nextarc)
				relax(arc);
			make_heap(que.begin(), que.end(), PVNodeCompare());
		}
		cout << "Dijkstra����ĵ�Դ���·����" << endl;
		for (int i = 1; i < vexnum; i++)
		{
			if (i != index-1)
			   printPath(index-1, i);
		}
		cout << "**************************************************" << endl;
	}

protected:
	//����һ������
	void insertArc(int vHead, int vTail, int weight)
	{
		//����һ������
		ArcNode *newArcNode = new ArcNode;
		newArcNode->source = vHead;
		newArcNode->adjvex = vTail;
		newArcNode->nextarc = NULL;
		newArcNode->weight = weight;

		//arcNode ��vertics[vHead]���ڽӱ�
		ArcNode *arcNode = vertices[vHead].firstarc;
		if (arcNode == NULL)
			vertices[vHead].firstarc = newArcNode;
		else
		{
			while (arcNode->nextarc != NULL)
			{
				arcNode = arcNode->nextarc;		
			}
			arcNode->nextarc = newArcNode;
		}
		arcnum++;
		vertices[vTail].indegree++;         //�Ի���β������ȼ�1
	}

	//��ӡԴ�ڵ㵽i�����·��
	void printPath(int i, int j)
	{
		cout << "��Դ�ڵ� " << vertices[i].data << " ��Ŀ�Ľ�� "
			<< vertices[j].data << " �����·���ǣ�" /*<< endl*/;
		__printPath(&vertices[i], &vertices[j]);
		cout << " Ȩ��Ϊ��" << vertices[j].key << endl;
	}

	void __printPath(VNode* source, VNode* dest)
	{
		if (source == dest)
			cout << source->data << "->";
		else if (dest->p == NULL)
			cout << " no path��" << endl;
		else
		{
			__printPath(source, dest->p);
			cout << dest->data << "->";
		}
	}

private:
	//const���ݳ�Ա�����ڹ��캯�����ʼ��
	static const int MAX_VERTEX_NUM = 20;  //��󶥵����

	VNode vertices[MAX_VERTEX_NUM];      //��Ž�������
	int vexnum;             //ͼ�ĵ�ǰ������
	int arcnum;             //ͼ�Ļ���
};

#endif