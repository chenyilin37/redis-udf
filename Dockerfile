# Pull base image.
FROM mysql:5.7

# install dependences
RUN apt-get update \
    && apt-get upgrade
    
RUN apt-get install -y git gcc cmake

RUN apt update \
    && apt install -y build-essential libboost-all-dev libmysqlclient-dev

RUN cd ~ \
    && git clone https://github.com/chenyilin37/redis-udf.git  \
    && cd redis-udf \
    && g++ -shared -o myredis-udf.so -fPIC -I /usr/include/mysql -l boost_serialization -l boost_system -l boost_thread anet.c redis_client.cpp redis_udf.cpp \
    && cp myredis-udf.so /usr/lib/mysql/plugin/ \
    && chmod 777 /usr/lib/mysql/plugin/myredis-udf.so

# if you are modifying this file and want to use mysql below version 5.7.8, you should uncomment this section to get lib_mysqludf_json.so and uncomment line 106 to install json udf at startup
RUN cd ~ \
    && git clone --depth=1 https://github.com/chenyilin37/lib_mysqludf_json \
    && cd lib_mysqludf_json \
    && gcc $(mysql_config --cflags) -shared -fPIC -o mysqludf_json.so lib_mysqludf_json.c \
    && cp mysqludf_json.so /usr/lib/mysql/plugin


# clean
RUN cd ~ \
    && rm -rf * \
    && apt-get remove -y git wget gcc cmake wget\
    && apt-get autoremove -y && apt-get autoclean -y \
    && apt-get clean && rm -rf /var/lib/apt/lists/*