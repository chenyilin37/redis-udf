# Pull base image.
FROM mysql:5.7

ADD mysqludf-redis.tar.gz /

#
ENV MYSQL_ALLOW_EMPTY_PASSWORD yes

#
COPY mysqld.cnf /etc/mysql/mysql.conf.d/
COPY setup.sh /mysql/setup.sh
COPY schema.sql /mysql/schema.sql

#
CMD ["sh", "/mysql/setup.sh"]