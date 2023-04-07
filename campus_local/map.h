#ifndef MAP_H
#define MAP_H
#include<bits/stdc++.h>
#include<QString>
#include<QDebug>
#include<QJsonObject>
#include<QJsonArray>
#include "basic.h"
class position
{
public:
    int id;
    QString name;
    double x,y;
    position():id(-1),name(""),x(0.0),y(0.0){};
    position(int id): id(id){

    };
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

    /// @brief idtopos idtoname[i]返回编号为i的位置名称
    std::vector<QString>idtoname;

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

#endif // MAP_H
