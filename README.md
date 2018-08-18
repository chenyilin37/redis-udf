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

export REDIS_HOST=<缺省：localhost>
export REDID_PASS=<缺省：foobared>

执行下列命令，使生效：
source /etc/profile


## 在mysql中，执行下列脚本建立自定义函数

DROP FUNCTION IF EXISTS `redis_set`; create function redis_set returns string soname 'myredis.so';
DROP FUNCTION IF EXISTS `redis_get`; create function redis_get returns string soname 'myredis.so';
DROP FUNCTION IF EXISTS `redis_del`; create function redis_del returns string soname 'myredis.so';
DROP FUNCTION IF EXISTS `redis_hset`; create function redis_hset returns string soname 'myredis.so';
DROP FUNCTION IF EXISTS `redis_hget`; create function redis_hget returns string soname 'myredis.so';
DROP FUNCTION IF EXISTS `redis_hmget`; create function redis_hmget returns string soname 'myredis.so';
DROP FUNCTION IF EXISTS `redis_hmset`; create function redis_hmset returns string soname 'myredis.so';
DROP FUNCTION IF EXISTS `redis_getset`; create function redis_getset returns string soname 'myredis.so';


## 测试
启动mysql控制台
mysql>SELECT redis_set('foo','bared');
mysql>SELECT redis_get('foo');

## 常见问题
### Broken Pipe

### Connection Refused





