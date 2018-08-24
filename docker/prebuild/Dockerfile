# Pull base image.
FROM mysql:5.7

# install dependences
RUN apt-get update \
    && apt-get upgrade -y \
    \
    && echo "install tools and repos" \
    && apt-get install -y git gcc cmake \
    && apt-get install -y build-essential libboost-all-dev libmysqlclient-dev \
	  \
    && echo "install mysqludf-redis" \
    && cd ~ \
    && git clone https://github.com/chenyilin37/redis-udf.git  \
    && cd redis-udf \
    && g++ -shared -o mysqludf-redis.so -fPIC -I /usr/include/mysql -l boost_serialization -l boost_system -l boost_thread anet.c redis_client.cpp redis_udf.cpp \
    && cp mysqludf-redis.so /usr/lib/mysql/plugin/ \
    && chmod 777 /usr/lib/mysql/plugin/mysqludf-redis.so \
    \
    && echo "install mysqludf-json" \
    && cd ~ \
    && git clone --depth=1 https://github.com/chenyilin37/lib_mysqludf_json \
    && cd lib_mysqludf_json \
    && gcc $(mysql_config --cflags) -shared -fPIC -o mysqludf-json.so lib_mysqludf_json.c \
    && cp mysqludf-json.so /usr/lib/mysql/plugin \
    \
    && echo "clean" \
    && cd ~ \
    && rm -rf * \
    && apt-get autoremove -y && apt-get autoclean -y \
    && apt-get clean && rm -rf /var/lib/apt/lists/* \
    && tar -zcvf mysqludf-redis.tar.gz \
     /usr/lib/mysql/plugin/mysqludf-redis.so \
     /usr/lib/mysql/plugin/mysqludf-json.so \
     /usr/lib/x86_64-linux-gnu/libboost_serialization.so.1.62.0 \
     /usr/lib/x86_64-linux-gnu/libboost_system.so.1.62.0 \
     /usr/lib/x86_64-linux-gnu/libboost_thread.so.1.62.0 