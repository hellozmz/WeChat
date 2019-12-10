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
* client做成继承的方式
* 给线程起名字

## 现有功能

* server->all client
* client->server

## TODO

* client需要有各自的名字
* client发送数据给server后，需要server端再广播出来（保存数据需要使用锁）
* server端断开后，client端不能刷大量log
