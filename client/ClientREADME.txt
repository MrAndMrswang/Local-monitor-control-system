client.h resource.h stdafx.h  targetver.h均为创建MFC程序原始文件。
clientDlg.h 其中注释较为详细，写出适配VS2017版本特性地方，需要留意。
里面用到了异步I/O模型 WSAAsyncSelect函数，使用该函数的时候需要知道你个Windows句柄以获取发生的事件。
主要的两个成员函数就是
1、和服务器链接，处理接收和发送的函数。
2、处理屏幕信息，获取信息转化为位图信息。