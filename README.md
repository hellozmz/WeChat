# C++多线程聊天室

聊天室类似于微信里的群聊功能，每个用户都可以发言，其他的用户可以观察到群中聊天。用户是有角色划分的，分为管理员和普通用户。管理员可以屏蔽特定的用户发言，用户可以与指定用户私聊。

## 基本思路

服务端一直运行，使用socket套接字，监听特定的端口。用户可以启动多个客户端，利用socket与服务端建立连接，实现客户端与服务端之间的消息通信。服务端负责将接收到的消息以广播的方式发出去，或者按照用户指定发送给特定的用户。

## 基本功能

* server端：
  * 监听并接收所有client端的消息
  * 屏蔽指定的client发言（后续可调整成让指定的用户下线）(todo)
  * 以广播的方式发送消息给所有的client，除去当前用户
  * 转发消息给指定的用户(todo)

* client端：
  * 管理员
    * 屏蔽指定用户发言(todo)
  * 普通用户
    * 支持client起名字
    * 可以查看当前的所有用户和群组(todo)，格式：`AllUser`
    * 创建群组(todo)，格式：`CreateGroup ${group name} ${userid list}`，创建成功会返回一串数字`${group ID}`，标识该群组
    * 支持与特定的client聊天，或者多个用户(todo)
      * 客户端指定目标名称，格式：`SayTo ${group name} ${msg}`，和多个用户之间聊天时，用户列表使用英文逗号分割，用户列表两端使用空格间隔开SayTo关键字和后面的发送信息

## 可扩展功能

* 系统做成可配置的，比如监听的端口号
* 支持测试
* 完善的启动脚本
* 聊天记录存储在数据库中(todo)
* 系统封装成docker image
* 为了便于编译，将bazel cache打包到docker image里

## 技术栈

* c++11
* bazel
* IO多路复用
* 多线程
* docker

## Docker内集成环境

* Centos 7
* bazel 0.17.2
* g++ 4.8.5
* git 2.2.1
* boost 1.68.0

## docker常用命令

* 制作镜像

`docker build -t wechat:test .`

* 运行镜像

`docker start wechat:test`

`docker run -it wechat:test /bin/bash`

`docker exec -it {ID} /bin/bash`

## 开发进度

Time | 工作内容 | 备注
:-: | :-: | :-:
2019/12/08 | 做计划 | 添加成员 @95xueqian
2019/12/15 | 开发出聊天的demo | 原型demo开发完毕<br>客户端发送名字给服务器，服务器记录名字的功能未实现
2019/12/22 | 完善功能 | 支持glog,gtest,docker 大量todo待完成
2019/12/29 | 测试验收 | 支持用户添加名字，查询所有用户，退出客户端，修复部分bug，支持boost库
2019/12/31 | 完善代码，添加测试用例 | 没有拆分出来过多的函数，不好使用ut进行测试，后续完善<br>socket请求的测试方法还不熟悉，需要调研
