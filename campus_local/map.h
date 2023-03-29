#ifndef MAP_H
#define MAP_H
#include<bits/stdc++.h>
class position
{
public:
    int id;
    std::string name;
    double x,y;
    position(int id, std::string name) : id(id), name(name){};
    ~position(){};
};
class path
{
private:
    std::vector<position> road;
public:
    path(std::initializer_list<position> h1) : road(h1.begin(), h1.end()){};
    void push_back(const position &a)
    {
        road.push_back(a);
    }
    void clear()
    {
        road.clear();
    }
    const std::string output();
};
class map
{
private:
    std::vector<std::vector<std::pair<int, int>>> mp;
public:
    /// @brief 使用文件数据载入地图
    /// @param fname 文件名
    map(const std::string fname);

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
    std::vector<std::pair<int,int>>& operator [](unsigned int now);

    /// @brief 返回一个vector,vector存储该点的邻点邻边
    /// @param now 表示该地点
    /// @return vector中元素的first存储的是邻点编号,second存储的是边的长度
    std::vector<std::pair<int,int>>& operator [](position now);
};


#endif // MAP_H
