#ifndef MAP_H
#define MAP_H
#include<bits/stdc++.h>
#include<QString>
#include<QDebug>

class map;//提前声明map

class position
{
public:
    int id;
    QString name;
    int x,y;
    int bx,by;
    int type;
    position():id(-1),name(""),x(-1),y(-1),bx(-1),by(-1),type(0){};
    position(int id, QString name) : id(id), name(name){};
    position(int id,QString name,int x,int y,int bx,int by,int type=1):id(id),name(name),x(x),y(y),bx(bx),by(by),type(type){};
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
    const QString output(map&)const;
    const std::vector<QString> outputvec(map&)const;
};
class map
{
public:
    /// @brief mp[i][j]表示编号为i的点,其第j条边所连端点和权值
    std::vector<std::vector<std::pair<int, int>>> mp;

    std::unordered_map<QString,int>strtoid;
    /// @brief 0~tot-1为现有position编号
    int tot;

    /// @brief dijkstra
    /// @param begin 表示起点
    /// @return 一个vector表示起点到各地点的最短距离
    std::vector<int> dijkstra(position begin);
    /// @brief idtopos idtopos[i]返回编号为i的position
    std::vector<position>idtopos;

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
    path route(position begin,const std::vector<position>&need);

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


    /// @brief navigate 获取导航路线文字说明
    /// @param stid 起始地点
    /// @param edid 目的地点
    /// @return QString即为文字说明
    const QString navigate(const int stid,const int edid);


    /// @brief navigate 获取导航路线文字说明
    /// @param stid 起始地点
    /// @param edids 目的地点
    /// @return QString即为文字说明
    const QString navigate(const int stid,const std::vector<int>&edids);
    /// @brief get_id 地名转化为特定id
    /// @param s 地名
    /// @return id
    int get_id(const QString&s);
};
#endif // MAP_H
