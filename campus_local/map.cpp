#include "map.h"
#include"basic.h"
#include<QJsonArray>
map::map(const QString fname)
{
    QJsonObject mapdata;
    if(!open_json(fname,mapdata))
    {
        qDebug()<<"read the map data error!\n";
        return;
    }
    this->tot=mapdata.value("PlaceNumber").toString().toInt();
    this->idtopos=std::vector<position>(tot);
    this->mp=std::vector<std::vector<std::pair<int,int>>>(tot,std::vector<std::pair<int,int>>());
    QJsonValue tmp=mapdata.value("Places");
    if(tmp.isNull()||tmp.type()!=QJsonValue::Array)
    {
        qDebug()<<"read the pos data error!\n";
        return;
    }
    QJsonArray marray=tmp.toArray();
    for(int i=0;i<marray.size();i++)
    {
        QJsonValue tmp=marray.at(i).toObject();
        if(tmp.type()!=QJsonValue::Object)
        {
            qDebug()<<QString("read the data at %1 %2").arg(i).arg("error!\n");
            return;
        }
        //tnnd 不类型检查了 开摆
        QJsonObject tmp2=tmp.toObject();
        QString name;
        int id,posx,posy,bposx,bposy;
        name=tmp2.value("PlaceName").toString();
        id=tmp2.value("PlaceCode").toString().toInt();
        posx=tmp2.value("Position").toArray().at(0).toString().toInt();
        posy=tmp2.value("Position").toArray().at(1).toString().toInt();
        bposx=tmp2.value("ButtonPos").toArray().at(0).toString().toInt();
        bposy=tmp2.value("ButtonPos").toArray().at(1).toString().toInt();
        idtopos[id]=position(id,name,posx,posy,bposx,bposy);
        strtoid[name]=id;
        QJsonArray edge=tmp2.value("NearestNeighbor").toArray();
        for(int i=0,nxt,len;i<edge.size();i++)
        {
            nxt=edge.at(i).toObject().value("PlaceCode").toString().toInt();
            len=edge.at(i).toObject().value("Distance").toString().toInt();
            this->mp[id].push_back({nxt,len});
            this->mp[nxt].push_back({id,len});
        }
    }

}
std::vector<int> map::dijkstra(position begin)
{
        std::vector<int>dis(tot,0x3f3f3f3f);
        std::vector<bool>vis(tot,0);
        std::priority_queue<std::pair<int,int>,std::vector<std::pair<int,int>>,std::greater<std::pair<int,int>>> que;
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
    std::priority_queue<std::tuple<int,int,int>,std::vector<std::tuple<int,int,int>>,std::greater<std::tuple<int,int,int>>>que;
    que.push(std::make_tuple(0,s,s));
    for(auto&e:mp[s])
    {
        if(e.second==0)
        {
            dis[e.first]=0;
            que.push(std::make_tuple(0,s,e.first));
        }

    }
    while(que.size())
    {
        auto [curdis,preid,curid]=que.top();
        que.pop();
        if(pre[curid]==-1)
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
            //if(e.second==0)continue;
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
path map::route(position begin,const std::vector<position>&need)
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
    //qDebug()<<now_status<<"????\n";
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
    path tmp=map::route(idtopos[pos[now_pos]],idtopos[pos[0]]);
    for(int i=tmp.size()-1;i>=1;i--)
    {
        ret.push_front(tmp[i]);
    }
    while(now_status!=1)
    {
        int pre_pos=pre[now_status][now_pos];
        now_status^=(1<<now_pos);
        path tmp=map::route(idtopos[pos[pre_pos]],idtopos[pos[now_pos]]);
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
const std::vector<QString> path::outputvec(map&mp)const
{
    std::vector<QString>ans;
    if(!this->size())
    {
        qDebug()<<"there is nothing in path!\n";
        return ans;
    }
    QString tems="从"+this->road[0].name+"出发";
    double nowx=this->road[0].x,nowy=this->road[0].y;
    int nowid=this->road[0].id;
    //lamda表达式
    auto judge=[](double x,double y,double x2,double y2)
    {
        if(abs(x2-x)/abs(y2-y)>1)
        {
            if(x2>x) return east;
            return west;
        }
        if(y2>y) return south;
        return north;

    };
    std::unordered_map<int,QString>tmp;
    tmp[north]=QString("北");
    tmp[south]=QString("南");
    tmp[west]=QString("西");
    tmp[east]=QString("东");
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
        if(len==0)
        {
            nowid=this->road[i].id;
            nowx=this->road[i].x;
            nowy=this->road[i].y;
            tems=tems+QString("到达%1").arg(this->road[i].name);
            continue;
        }
        //根据相对相对位置得出对应string
        int dir=judge(nowx,nowy,this->road[i].x,this->road[i].y);
        tems=tems+QString("向");
        for(int j=3;j>=0;j--)
        {
            if((dir>>j)&1)
            {
                tems=tems+tmp[(1<<j)];
            }
        }
        tems=tems+QString("方走大约%1米到达%2").arg(len).arg(this->road[i].name);
        ans.push_back(tems);
        tems.clear();
        nowid=this->road[i].id;
        nowx=this->road[i].x;
        nowy=this->road[i].y;
    }
    return ans;

}
const QString path::output(map&mp)const
{
    auto tmp=outputvec(mp);
    QString ans;
    for(auto&e:tmp)
    {
        ans=ans+e+QString("\n");
    }
    return ans;
}
const QString map::navigate(const int stid,const int edid)
{
    if(stid>=tot||edid>=tot)
    {
        qDebug()<<"In navigate,id not found!\n";
        return QString("");
    }
    return this->route(this->idtopos[stid],this->idtopos[edid]).output(*this);
}

const QString map::navigate(const int stid,const std::vector<int>&edids)
{
    if(stid>=tot)
    {
        qDebug()<<"In navigate,id not found!\n";
        return QString("");
    }
    std::vector<position>need;
    for(auto&e:edids)
    {
        if(e>=tot)
        {
            qDebug()<<"In navigate,id not found!\n";
            return QString("");
        }
        need.push_back(this->idtopos[e]);
    }
    return this->route(this->idtopos[stid],need).output(*this);
}
