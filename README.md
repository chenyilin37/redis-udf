# redis-udf

redis_version:4.0.8

mysql:5.6

dependence : boost mysql

## 安装boos
### Ubuntu/Debian/Linux Mint
sudo apt-get install libboost-all-dev

### Mac
brew install boost

## 添加mysql.h头文件
安装mysql后，不一定有mysql.h头文件，需要另行安装；
### ubuntu下   
audo apt-get install libmysqlclient-dev

### centos下 :
yum install mysql-devel

## 编译
g++ -shared -o libmyredis.so -fPIC -I /usr/include/mysql -l boost_serialization -l boost_system -l boost_thread  anet.c redis_client.cpp redis_udf.cpp


## 将编译出的libmyredis.so文件拷贝到mysql的插件目录下并授权
sudo cp libmyredis.so /usr/lib/mysql/plugin/ & sudo chmod 777 /usr/lib/mysql/plugin/libmyredis.so

## 设置环境变量
在/etc/profile中，添加如下语句：
export REDIS_HOST=localhost
export REDID_PASS=foobared




