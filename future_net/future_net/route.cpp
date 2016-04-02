#include "route.h"
#include "lib_record.h"
#include <stdio.h>

#include <map>
#include <cstring>

using namespace std;

struct AdjList {//邻接表
    int num;
    int des[9],cost[9],lnkId[9];
}edge[605];

struct AdjMatNode {//邻接矩阵
    int lnkId,cost;
}adjMat[605][605];

int sta,des,allVertexNum;
int vset[50],inclVertexNum;//必经点集合及个数
int dfsPath[605];
int ansCost,ansPath[605],ansLen;

bool vis[605];

const int INF=0x3f3f3f3f;


void readTopo_Demand(char *topo[5000],int edge_num,char *demand) {//从字符数组中读入边和必经点
    int ista,ides;
    int lnkId,srcId,desId,cost;
    map<int,int> vmp;
    allVertexNum=0;
    memset(adjMat,0x3f,sizeof(adjMat));
    for(int i=0;i<edge_num;++i) {//读入边至邻接矩阵
        sscanf(topo[i],"%d,%d,%d,%d",&lnkId,&srcId,&desId,&cost);
        if(vmp[srcId]==0)//起点未出现过
            vmp[srcId]=++allVertexNum;
        if(vmp[desId]==0)//终点未出现过
            vmp[desId]=++allVertexNum;
        ista=vmp[srcId];
        ides=vmp[desId];
        if(adjMat[ista][ides].cost>cost) {//只保留权值最小的边
            adjMat[ista][ides].cost=cost;
            adjMat[ista][ides].lnkId=lnkId;
        }
    }

    for(int i=1;i<=allVertexNum;++i) {//邻接矩阵转换成邻接表
        for(int j=1;j<=allVertexNum;++j) {
            if(adjMat[i][j].cost!=INF) {
                int index=edge[i].num;
                edge[i].des[index]=j;
                edge[i].cost[index]=adjMat[i][j].cost;
                edge[i].lnkId[index]=adjMat[i][j].lnkId;
                ++edge[i].num;
            }
        }
    }

    sscanf(demand,"%d,%d,%d",&srcId,&desId,vset);//读入起止点和第一个必经点
    sta=vmp[srcId];
    des=vmp[desId];
    vset[0]=vmp[vset[0]];
    inclVertexNum=1;
    int tmp=0;
    while(demand[tmp]!='\0'&&demand[tmp]!='|')
        ++tmp;
    while(demand[tmp]!='\0') {//读入后续的必经点
        vset[inclVertexNum]=0;
        ++tmp;
        while('0'<=demand[tmp]&&demand[tmp]<='9') {
            vset[inclVertexNum]=vset[inclVertexNum]*10+demand[tmp]-'0';
            ++tmp;
        }
        vset[inclVertexNum]=vmp[vset[inclVertexNum]];
        ++inclVertexNum;
	while(demand[tmp]!='\0'&&demand[tmp]!='|')
		++tmp;
    }
}

inline bool isInclude() {
    for(int i=0;i<inclVertexNum;++i)
        if(!vis[vset[i]])
            return false;
    return true;
}

void dfs(int cur,int dep,int cost) {
    if(cost>=ansCost)
        return ;
    ///剪枝待完成

    if(cur==des) {
        if(cost<ansCost&&isInclude()) {
            ansCost=cost;
            ansLen=dep;
            for(int i=0;i<dep;++i)
                ansPath[i]=dfsPath[i];
        }
        return ;
    }
    AdjList u=edge[cur];
    for(int i=0;i<u.num;++i) {
        if(!vis[u.des[i]]) {
            vis[u.des[i]]=true;
            dfsPath[dep]=u.lnkId[i];
            dfs(u.des[i],dep+1,cost+u.cost[i]);
            vis[u.des[i]]=false;
        }
    }
}

//你要完成的功能总入口
void search_route(char *topo[5000], int edge_num, char *demand)
{
    readTopo_Demand(topo,edge_num,demand);

    ansCost=INF;
    ansLen=0;
    memset(vis,false,sizeof(vis));
    vis[sta]=true;
    dfs(sta,0,0);

    for (int i = 0; i < ansLen; i++)
        record_result(ansPath[i]);
}
