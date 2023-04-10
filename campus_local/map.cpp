#include "map.h"
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
constexpr int north=1;
constexpr int south=2;
constexpr int west=4;
constexpr int east=8;
const QString path::output(map&mp)const
{

    if(!this->size())
    {
        qDebug()<<"there is nothing in path!\n";
    }
    QString ret="从"+this->road[0].name+"出发";
    double nowx=this->road[0].x,nowy=this->road[0].y;
    int nowid=this->road[0].id;
    //lamda表达式
    auto judge=[](double x,double y,double x2,double y2)
    {
        return (((x2>x)*west)|((x>x2)*east)|((y2>y)*south)|((y>y2)*north));

    };
    std::unordered_map<int,QString>tmp;
    tmp[north]=QString("南方");
    tmp[south]=QString("北方");
    tmp[west]=QString("西方");
    tmp[east]=QString("东方");
    for(int i=1;i<this->size();i++)
    {
        int len=0x3f3f3f3f;
        for(auto&e:mp[nowid])
        {
            if(e.first==this->road[i].id)
            {
                len=std::min(len,e.second);
            }
        }
        //根据相对相对位置得出对应string
        int dir=judge(nowx,nowy,this->road[i].x,this->road[i].y);
        ret=ret+QString("然后向");
        for(int j=3;j>=0;j--)
        {
            if((dir>>j)&1)
            {
                ret=ret+tmp[(1<<j)];
            }
        }
        ret=ret+QString("方走大约%1米到达%2\n").arg(len).arg(this->road[i].name);
    }
    return ret;
}
