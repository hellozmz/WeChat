# 基本操作

## 编译命令

编译所有的二进制文件
```
bazel build //...
```

也可以根据自己的需要编译特定的target，比如：
```
bazel build //src:server
```
```
bazel build //src:client
```

## debug

socket断开连接后有一个TIME_WAIT阶段，对于调试很不方便，需要不断的等待。可以在编译的时候添加参数`--copt -Ddebug_wechat=1`来解决。注意，使用参数编译后，启动二进制要使用`./bazel-bin/src/{server, client}`，不可以使用`bazel run src:{server, client}`，因为这样会重新编译，没有加进去编译参数。

已经支持glog，可以通过log进行调试。

## 启动命令

* server端
```
bazel run src:server
```
或者使用
```
./bazel-bin/src/server
```
* client端
```
./bazel-bin/src/client
```

## 待优化

* 调整server中select成epoll
* 给线程起名字
* 使用mysql去存储数据
* 做成docker image，便于别人部署
* 客户端都退出后，服务端自己也会退出
* 当使用中文聊天，删除两次才会去掉一个中文字符，但是这就删除掉了两个中文字符。

## 现有功能

* server->all client
* client->server

## TODO

* client需要有各自的名字
* client发送数据给server后，需要server端再广播出来（保存数据需要使用锁）
* server端断开后，client端不能刷大量log
* client中可以划分等级，比如添加管理员的用户，普通用户，可以考虑加上派生类去支持该功能
* server端定位是服务器：监听数据变化，记录用户，存储数据，消息转发等
* server端有存储数据的需要，现在可以先写文件，后续可以做成读写数据库的方式
