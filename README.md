# 多reactor模型

### 部分修改

为了实现边缘触发，而外的添加了读写缓冲区。

### 编译

```bash
git clone https://github.com/huangbaba123456/reactor.git
```

克隆下来后

```
cd reactor
mkdir build
cd build
cmake ..
make
```

然后找到可执行文件，执行它

```
cd ../bin
./reactor
```

主reactor默认监听在10020端口

### 测试连接

```
telnet 127.0.0.1 10020
```

### 切换select

默认使用epoll加边缘触发，如果想使用select，打开主CMakeLists.txt，取消注释
![image-20250515003226595](C:\Users\32301\AppData\Roaming\Typora\typora-user-images\image-20250515003226595.png)