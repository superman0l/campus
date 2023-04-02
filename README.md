# campus

## 编译运行

编译需要的环境配置与软件：

- Visual Studio 2017+ or Qt Creator
- CMake(Qt Creator自带)

## For User


## For Developer

### campus_local

存储本项目头文件,源文件,ui文件以及CmakeList.txt文件

### example

存储本项目Json文件,日志文件格式样例

### algorithm

本项目相关算法说明

### Json(已添加到.gitignore中)

存储程序运行中所需Json文件

### 使用Qt Creator的开发流程

1.打开Qt Creator->打开项目,选中campus_local中的CmakeLists.txt文件

![image-20230329225632055](https://cdn.jsdelivr.net/gh/Kanata369/blogimage@main/img/image-20230329225632055.png)

2.进入配置项目页面,选择默认套件Desktop Qt 6.4.2 MinGW 64-bit即可,勾选Debug即可,注意工程文件夹统一命名**build**防止误传(build已添加到.gitignore中),然后点击Configure Project构建工程文件

![image-20230329230116378](https://cdn.jsdelivr.net/gh/Kanata369/blogimage@main/img/image-20230329230116378.png)

3.如何调试

调试->开始调试

### 参考资料

Qt学习:

- https://wiki.qt.io/Qt_for_Beginners (基本概念)
- https://wiki.qt.io/Category:HowTo (官方教程)
- https://www.cnblogs.com/GeophysicsWorker/p/16163965.html (Qt多线程)
- https://doc.qt.io/qt-6/reference-overview.html (Qt reference)
- https://blog.csdn.net/cpp_learner/article/details/118421096 (Qt Json库)

