FROM redis
COPY redis.conf /etc/redis/redis.conf
RUN apt-get update && apt-get upgrade -y \
    && ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime \
    && echo 'Asia/Shanghai' >/etc/timezone

CMD [ "redis-server", "/etc/redis/redis.conf" ]