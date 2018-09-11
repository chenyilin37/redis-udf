# redis-udf

redis_version:4.0.8+

mysql:5.6+

dependence : boost mysql

# Docker安装Redis
    docker pull redis

    docker run -d -p 6379:6379 \
        -v /Users/Shared/redis/data:/data \
        -v /Users/Shared/redis/redis.conf:/usr/local/etc/redis/redis.conf \
        --restart=always \
        --name myredis redis \
        redis-server /usr/local/etc/redis/redis.conf
        
         
   启动redis-cli
    docker run -it --link myredis:redis --rm redis redis-cli -h redis -p 6379

   进入容器
   docker run -it --link myredis:redis --rm redis sh -c 'bash'

# Docker安装MySQL
## 创建镜像
### 预构建
 下载：https://github.com/chenyilin37/redis-udf/blob/master/Dockerfile
 cd /docker/prebuild
 docker build -t goas/mysql-with-redis-udf:5.7 .

在容器中，执行：
 ldd mysqludf_redis.so
 
tar -zcvf  mysqludf-deps.tar.gz  \
     /usr/lib/mysql/plugin/mysqludf_redis.so \
     /usr/lib/mysql/plugin/mysqludf-json.so \
     /usr/lib/x86_64-linux-gnu/libboost_serialization.so.1.62.0 \
     /usr/lib/x86_64-linux-gnu/libboost_system.so.1.62.0 \
     /usr/lib/x86_64-linux-gnu/libboost_thread.so.1.62.0 
     
### 正式构建
 cd /docker
 编辑mysqld.cnf
 docker build -t goas/mysql-with-redis-udf:5.7 .
 docker push goas/mysql-with-redis-udf:5.7

## 安装MySQL
 docker pull goas/mysql-with-redis-udf:5.7

 docker run -d -p 3306:3306 --privileged=true -v /Users/Shared/mysql/data:/var/lib/mysql \
	 -e MYSQL_ROOT_PASSWORD=chenyl \
	 -e MYSQL_USER=chenyl \
	 -e MYSQL_PASSWORD=123456 \
	 -e REDIS_HOST=192.168.1.8 \
         -e REDIS_AUTH=foobared \
         --restart=always \
	 --name macmysql goas/mysql-with-redis-udf:5.7
 
 docker run -it --link macmysql:mysql --rm goas/mysql-with-redis-udf:5.7 sh -c 'bash'


## 手动安装
### 安装boost
#### Ubuntu/Debian/Linux Mint
  sudo apt-get install libboost-all-dev

#### Mac
  brew install boost

### 安装mysql
    sudo apt-get update #更新软件库
    sudo apt-get install mysql-server #安装mysql,期间如果要求输入密码， 那就输入吧. 

#### 开启mysql远程访问
    修改mysql配置，允许远程登录
    $ sudo vim /etc/mysql/mariadb.conf.d/50-server.cnf
    #将bind-address这行注释掉,然后重启

#### 设置root账号可以远程登录

    sudo mysql -u root -p #登录mysql查看是否安装成功

    use mysql;
    GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' IDENTIFIED BY 'root账号密码' WITH GRANT OPTION;
    flush privileges;

    $ sudo /etc/init.d/mysql restart

    然后就可以使用其他客户端直接连接了
    
    
### 添加mysql.h头文件
   安装mysql后，不一定有mysql.h头文件，需要另行安装；
### ubuntu下   
   sudo apt-get install default-libmysqlclient-dev

### centos下 :
   yum install mysql-devel

## 编译
   g++ -shared -o mysqludf_redis.so -fPIC -I /usr/include/mysql -l boost_serialization -l boost_system -l boost_thread  anet.c redis_client.cpp redis_udf.cpp

## 打包备份
    检查依赖关系：
    ldd mysqludf_redis.so

    将UDF及依赖一起打包，将来备用，就不必变异
    tar -zcvf  mysqludf-5.7-rpi3.tar.gz  \
    /usr/lib/mysql/plugin/mysqludf_redis.so \
    /usr/lib/mysql/plugin/mysqludf-json.so \
    /usr/lib/x86_64-linux-gnu/libboost_serialization.so.1.62.0 \
    /usr/lib/x86_64-linux-gnu/libboost_system.so.1.62.0 \
    /usr/lib/x86_64-linux-gnu/libboost_thread.so.1.62.0 


## 将编译出的mysqludf_redis.so文件拷贝到mysql的插件目录下并授权
   sudo cp mysqludf_redis.so /usr/lib/mysql/plugin/ & sudo chmod 777 /usr/lib/mysql/plugin/mysqludf_redis.so
  
  启动 mysqld ：/etc/init.d/mysql restart
  
  
  对mariadb：
  sudo cp mysqludf_redis.so /usr/lib/arm-linux-gnueabihf/mariadb18/plugin/ & sudo chmod 777 /usr/lib/arm-linux-gnueabihf/mariadb18/plugin/mysqludf_redis.so

    mariadb的插件目录，登录MySQL后，SHOW VARIABLES LIKE 'plugin_dir';
    
    MariaDB [(none)]> SHOW VARIABLES LIKE 'plugin_dir';
    +---------------+------------------------------------------------+
    | Variable_name | Value                                          |
    +---------------+------------------------------------------------+
    | plugin_dir    | /usr/lib/arm-linux-gnueabihf/mariadb18/plugin/ |
    +---------------+------------------------------------------------+
    1 row in set (0.01 sec)


## 设置环境变量
   在/etc/profile中，添加如下语句：

   export REDIS_HOST=<缺省：localhost>
   export REDIS_AUTH=<缺省：foobared>

执行下列命令，使生效：
   source /etc/profile



## 在mysql中，执行下列脚本建立自定义函数


  DROP FUNCTION IF EXISTS `test_add`; create function test_add returns INTEGER soname 'mysqludf_redis.so';   
  DROP FUNCTION IF EXISTS `show_envs`; create function show_envs returns string soname 'mysqludf_redis.so';   

  DROP FUNCTION IF EXISTS `redis_set`; create function redis_set returns string soname 'mysqludf_redis.so';   
  DROP FUNCTION IF EXISTS `redis_get`; create function redis_get returns string soname 'mysqludf_redis.so';   
  DROP FUNCTION IF EXISTS `redis_del`; create function redis_del returns string soname 'mysqludf_redis.so';   
  DROP FUNCTION IF EXISTS `redis_getset`; create function redis_getset returns string soname 'mysqludf_redis.so';   
  DROP FUNCTION IF EXISTS `redis_hset`; create function redis_hset returns string soname 'mysqludf_redis.so';   
  DROP FUNCTION IF EXISTS `redis_hget`; create function redis_hget returns string soname 'mysqludf_redis.so';   
  DROP FUNCTION IF EXISTS `redis_hmget`; create function redis_hmget returns string soname 'mysqludf_redis.so';   
  DROP FUNCTION IF EXISTS `redis_hmset`; create function redis_hmset returns string soname 'mysqludf_redis.so';    
  DROP FUNCTION IF EXISTS `redis_hdel`; create function redis_hdel returns string soname 'mysqludf_redis.so';   
  DROP FUNCTION IF EXISTS `redis_sadd`; create function redis_sadd returns string soname 'mysqludf_redis.so';   
  DROP FUNCTION IF EXISTS `redis_srem`; create function redis_srem returns string soname 'mysqludf_redis.so';    
  DROP FUNCTION IF EXISTS `redis_zadd`; create function redis_zadd returns string soname 'mysqludf_redis.so';    
  DROP FUNCTION IF EXISTS `redis_zrem`; create function redis_zrem returns string soname 'mysqludf_redis.so';     




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
 
## SD卡镜像
### 备份SD卡
     使用 dd 命令可以直接备份SD卡。这里树莓派的SD卡的路径是 /dev/sdc1 和 /dev/sdc2 ，所以备份整个SD卡的路径就是 /dev/sdc。
     输入备份命令：
     $ sudo dd if=/dev/sdc | gzip>/home/lixinxing/raspberry.gz
 
### 写入备份文件
     $ sudo gzip -dc /home/lixinxing/raspberry.gz | sudo dd of=/dev/sdc
     其中备份文件的位置、文件名和 SD卡的路径要根据实际选择。
     这样就将备份还原到树莓派了，可以将SD卡插入树莓派启动！
 
 
## 常见问题
### 时区时钟同步
    MySQL服务器、Redis服务器、应用服务器时区时钟要同步，安装时注意！
    
    Dockerfile 时区设置
    RUN ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime
    RUN echo 'Asia/Shanghai' >/etc/timezone

    Java 时区设置
    注意,如果能确保所在主机/etc/timezone内容正确,则不需要再对Java时区进行设置
    
    TimeZone tz = TimeZone.getTimeZone("Asia/Shanghai");
    TimeZone.setDefault(tz);        
        
### Broken pipe 
   查看redis.conf中timeout参数，如果没有设置或设置了数值，都改为0；
   
### Connection Refused

### connection was closed

## TODO
引号兼容


