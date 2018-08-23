# redis-udf

redis_version:4.0.8

mysql:5.6

dependence : boost mysql

## 安装boost
### Ubuntu/Debian/Linux Mint
  sudo apt-get install libboost-all-dev

### Mac
  brew install boost


sudo apt-get update #更新软件库
sudo apt-get install mysql-server #安装mysql,期间如果要求输入密码， 那就输入吧. 
sudo mysql -u root -p #登录mysql查看是否安装成功

## 添加mysql.h头文件
   安装mysql后，不一定有mysql.h头文件，需要另行安装；
### ubuntu下   
   audo apt-get install libmysqlclient-dev

### centos下 :
   yum install mysql-devel

## 编译
   g++ -shared -o myredis-udf.so -fPIC -I /usr/include/mysql -l boost_serialization -l boost_system -l boost_thread  anet.c redis_client.cpp redis_udf.cpp


## 将编译出的myredis.so文件拷贝到mysql的插件目录下并授权
   sudo cp myredis-udf.so /usr/lib/mysql/plugin/ & sudo chmod 777 /usr/lib/mysql/plugin/myredis-udf.so

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
   DROP FUNCTION IF EXISTS `redis_getset`; create function redis_getset returns string soname 'myredis.so';
   DROP FUNCTION IF EXISTS `redis_hset`; create function redis_hset returns string soname 'myredis.so';   
   DROP FUNCTION IF EXISTS `redis_hget`; create function redis_hget returns string soname 'myredis.so';   
   DROP FUNCTION IF EXISTS `redis_hmget`; create function redis_hmget returns string soname 'myredis.so';   
   DROP FUNCTION IF EXISTS `redis_hmset`; create function redis_hmset returns string soname 'myredis.so';   
   DROP FUNCTION IF EXISTS `redis_hdel`; create function redis_hdel returns string soname 'myredis.so';

  DROP FUNCTION IF EXISTS `redis_sadd`; create function redis_sadd returns string soname 'myredis.so';
  DROP FUNCTION IF EXISTS `redis_srem`; create function redis_srem returns string soname 'myredis.so';
  DROP FUNCTION IF EXISTS `redis_zadd`; create function redis_zadd returns string soname 'myredis.so';
  DROP FUNCTION IF EXISTS `redis_zrem`; create function redis_zrem returns string soname 'myredis.so';


## 测试UDF
   启动mysql控制台
   mysql>SELECT redis_set('foo','bared');    
			+--------------------------+       
			| redis_set('foo','bared') |    
			+--------------------------+    
			| connection was closed    |    
			+--------------------------+    
			1 row in set (0.00 sec)    
			    
   mysql>SELECT redis_get('foo');
   
 
 ## 通过MySQL触发器刷新Redis
     借助触发器监听INSERT、UPDATE、DELETE等基本操作,实现自动调用UDF
 
 ### 常见一个表 
     mysql> SELECT * FROM Student;   
     +----------+---------+------+------+---------+   
     | Sid      | Sname   | Sage | Sgen | Sdept   |   
     +----------+---------+------+------+---------+   
     | 09388123 | Lucy    |   18 | F    | AS2-123 |   
     | 09388165 | Rose    |   19 | F    | SS3-205 |   
     | 09388308 | zhsuiy  |   19 | F    | MD8-208 |   
     +----------+---------+------+------+---------+   
     3 rows in set (0.00 sec)   
     
### 定义一个触发器     
    mysql  > DELIMITER $   
        > CREATE TRIGGER tg_student    
        > AFTER INSERT on Student    
        > FOR EACH ROW    
        > BEGIN   
        > SET @id = (SELECT redis_hset(CONCAT('stu_', new.Sid), 'id', CAST(new.Sid AS CHAR(8))));   
        > SET @name = (SELECT redis_hset(CONCAT('stu_', new.Sid), 'name', CAST(new.Sname AS CHAR(20))));   
        > Set @age = (SELECT redis_hset(CONCAT('stu_', new.Sid), 'age', CAST(new.Sage AS CHAR)));    
        > Set @gender = (SELECT redis_hset(CONCAT('stu_', new.Sid), 'gender', CAST(new.Sgen AS CHAR)));    
        > Set @dept = (SELECT redis_hset(CONCAT('stu_', new.Sid), 'department', CAST(new.Sdept AS CHAR(10))));       
        > END $   

### 插入一条记录
    mysql> INSERT INTO Student VALUES('09388165', 'Rose', 19, 'F', 'SS3-205');    
    Query OK, 1 row affected (0.27 sec)    
 
     mysql> SELECT * FROM Student;    
     +----------+---------+------+------+---------+    
     | Sid      | Sname   | Sage | Sgen | Sdept   |    
     +----------+---------+------+------+---------+    
     | 09388123 | Lucy    |   18 | F    | AS2-123 |    
     | 09388165 | Rose    |   19 | F    | SS3-205 |    
     | 09388308 | zhsuiy  |   19 | F    | MD8-208 |    
     | 09388318 | daemon  |   18 | M    | ZS4-630 |    
     | 09388321 | David   |   20 | M    | ZS4-731 |    
     | 09388334 | zhxilin |   20 | M    | ZS4-722 |    
     +----------+---------+------+------+---------+    
     6 rows in set (0.00 sec)    
 
### 查看Redis的结果
    127.0.0.1:6379> HGETALL stu_09388165    
    1) "id"    
    2) "09388165"    
    3) "name"    
    4) "Rose"    
    5) "age"    
    6) "19"    
    7) "gender"    
    8) "F"    
    9) "department"    
    10) "SS3-205"    
 
## 常见问题
### Broken pipe 
   查看redis.conf中timeout参数，如果没有设置或设置了数值，都改为0；
   
### Connection Refused

### connection was closed

## TODO
兼容单引号


