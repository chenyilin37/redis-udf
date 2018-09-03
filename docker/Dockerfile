# Pull base image.
FROM mysql:5.7

ADD mysqludf-redis.tar.gz /


#
COPY mysqld.cnf /etc/mysql/mysql.conf.d/
# COPY setup.sh /mysql/setup.sh
# COPY schema.sql /mysql/schema.sql

# ENV MYSQL_ALLOW_EMPTY_PASSWORD yes
# CMD ["sh", "/mysql/setup.sh"]