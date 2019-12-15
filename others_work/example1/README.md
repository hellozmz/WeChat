代码来源：https://blog.csdn.net/weixin_41423378/article/details/82228177

功能：
client以广播的方式发送消息，所有的客户端都能接受到消息
server端可以看见所有消息
server可以看见客户端的名字

缺点：
server端只能查看，不能说话
系统使用了死循环的方式去监听消息，导致cpu占用100%