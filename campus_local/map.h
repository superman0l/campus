#ifndef MAP_H
#define MAP_H
#include<bits/stdc++.h>
#include<QString>
#include<QDebug>
class position
{
public:
    int id;
    QString name;
    double x,y;
    position():id(-1),name(""),x(0.0),y(0.0){};
    position(int id, QString name) : id(id), name(name){};
    ~position(){};
};
class path
{
private:
    std::deque<position> road;
public:
    path(){};
    path(std::initializer_list<position> h1) : road(h1.begin(), h1.end()){};
    int size() const
    {
        return road.size();
    }
    position operator [](unsigned int idx) const
    {
        if(idx>=size())
        {
            qDebug()<<"class path access error!\n";
            return position();
        }else
        {
            return road[idx];
        }
    }
    position& operator [] (unsigned int idx)
    {
        return road[idx];
    }
    void push_back(const position &a)
    {
        road.push_back(a);
    }
    void push_front(const position &a)
    {
        road.push_front(a);
    }
    void clear()
    {
        road.clear();
    }
    const QString output();
};
class map
{
private:
    /// @brief mp[i][j]表示编号为i的点,其第j条边所连端点和权值
    std::vector<std::vector<std::pair<int, int>>> mp;
    /// @brief 0~tot-1为现有position编号
    int tot;
    /// @brief idtopos idtopos[i]返回编号为i的position
    std::vector<position>idtopos;
    /// @brief dijkstra
    /// @param begin 表示起点
    /// @return 一个vector表示起点到各地点的最短距离
    std::vector<int> dijkstra(position begin);
public:
    /// @brief 使用文件数据载入地图
    /// @param fname 文件名
    map(const QString fname);

    /// @brief 返回从begin到end的导航路径
    /// @param begin 出发点
    /// @param end 目标终点
    /// @return 导航路径
    path route(position begin, position end);

    /// @brief 返回从begin途径need中所有点后,再次回到begin的导航路径
    /// @param begin 出发点和结束点
    /// @param need 需要途径的点集
    /// @return 导航路径
    path route(position begin, std::initializer_list<position> need);

    /// @brief 返回一个vector,vector存储该点的邻点邻边
    /// @param  now 表示该点的编号
    /// @return vector中元素的first存储的是邻点编号,second存储的是边的长度
    std::vector<std::pair<int,int>>& operator [](unsigned int now)
    {
        return mp[now];
    }

    /// @brief 返回一个vector,vector存储该点的邻点邻边
    /// @param now 表示该地点
    /// @return vector中元素的first存储的是邻点编号,second存储的是边的长度
    std::vector<std::pair<int,int>>& operator [](position now)
    {
        return mp[now.id];
    }
};
std::vector<int> map::dijkstra(position begin)
{
        std::vector<int>dis(tot,0x3f3f3f3f);
        std::vector<bool>vis(tot,0);
        std::priority_queue<std::pair<int,int>> que;
        dis[begin.id]=0;
        que.push(std::make_pair(0,begin.id));
        while(que.size())
        {
            auto [curdis,curid]=que.top();
            que.pop();
            if(vis[curid])
            {
                continue;
            }else
            {
                vis[curid]=1;
            }
            for(auto&e:mp[curid])
            {
                if(dis[e.first]>dis[curid]+e.second)
                {
                    dis[e.first]=dis[curid]+e.second;
                    que.push(std::make_pair(dis[e.first],e.first));
                }
            }
        }
        return dis;
}
path map::route(position begin,position end)
{
    std::vector<int>dis(tot,0x3f3f3f3f);
    std::vector<int>pre(tot,-1);
    int s=begin.id;
    int t=end.id;
    dis[s]=0;
    std::priority_queue<std::tuple<int,int,int>>que;
    que.push(std::make_tuple(0,s,s));
    while(que.size())
    {
        auto [curdis,preid,curid]=que.top();
        que.pop();
        if(pre[curid]!=-1)
        {
            pre[curid]=preid;
            if(curid==t)
            {
                break;
            }
        }else
        {
            continue;
        }
        for(auto&e:mp[curid])
        {

            if(dis[e.first]>e.second+dis[curid])
            {
                dis[e.first]=e.second+dis[curid];
                que.push(std::make_tuple(dis[e.first],curid,e.first));
            }
        }
    }
    path ret;
    if(pre[t]==-1)
    {
        qDebug()<<"Unable to access "<<end.name<<"\n";
        return ret;
    }
    while(t!=s)
    {
        ret.push_front(idtopos[t]);
        t=pre[t];
    }
    ret.push_front(idtopos[s]);
    return ret;
}
path map::route(position begin,std::initializer_list<position>need)
{
    std::vector<int>pos;
    pos.push_back(begin.id);
    for(auto&e:need)
    {
        pos.push_back(e.id);
    }
    std::sort(pos.begin(),pos.end());
    pos.erase(std::unique(pos.begin(),pos.end()),pos.end());
    int cnt=pos.size();
    if(cnt>20)
    {
        qDebug()<<"too many positions to find the shortest path!";
        return path();
    }
    if(cnt==1)
    {
        return path();
    }
    for(int i=1;i<cnt;i++)
    {
        if(pos[i]==begin.id)
        {
            std::swap(pos[i],pos[0]);
            break;
        }
    }
    std::vector<std::vector<int>>dis(cnt);
    for(int i=0;i<cnt;i++)
    {
        dis[i]=map::dijkstra(idtopos[pos[i]]);
    }
    std::vector<std::vector<int>>dp(1<<cnt,std::vector<int>(cnt,0x3f3f3f3f));
    std::vector<std::vector<int>>pre(1<<cnt,std::vector<int>(cnt,-1));
    dp[1][0]=0;
    for(int i=1;i<(1<<cnt);i++)
    {
        for(int j=0;j<cnt;j++)
        {
            if((i>>j)&1)
            {
                for(int k=0;k<cnt;k++)
                {
                    if((i>>k)&1) continue;
                    if((i^(1<<k))==(1<<cnt)-1)
                    {
                        if(dp[i^(1<<k)][k]>dp[i][j]+dis[j][pos[k]]+dis[k][pos[0]])
                        {
                            dp[i^(1<<k)][k]=dp[i][j]+dis[j][pos[k]]+dis[k][pos[0]];
                            pre[i^(1<<k)][k]=j;
                        }
                    }else
                    {
                        if(dp[i^(1<<k)][k]>dp[i][j]+dis[j][pos[k]])
                        {
                            dp[i^(1<<k)][k]=dp[i][j]+dis[j][pos[k]];
                            pre[i^(1<<k)][k]=j;
                        }
                    }
                }
            }
        }
    }
    int now_status=(1<<cnt)-1;
    int now_mx=0x3f3f3f3f;
    int now_pos=-1;
    for(int j=1;j<cnt;j++)
    {
        if(dp[now_status][j]<now_mx)
        {
            now_pos=j;
            now_mx=dp[now_status][j];
        }
    }
    path ret;
    while(now_status!=1)
    {
        int pre_pos=pre[now_status][now_pos];
        now_status^=(1<<now_pos);
        path tmp=map::route(idtopos[pre_pos],idtopos[now_pos]);
        //tmp[0]即pre_pos,不插入
        for(int i=tmp.size()-1;i>=1;i--)
        {
            ret.push_front(tmp[i]);
        }
        now_pos=pre_pos;
    }
    ret.push_front(idtopos[pos[0]]);
    return ret;
}
#endif // MAP_H
