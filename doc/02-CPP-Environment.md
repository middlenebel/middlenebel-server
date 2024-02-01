
# Start Front

*ng serve*
.../middlenebel-front$ ng serve --port 8081

*lite-server*
.../middlenebel-front$ npm run dev
Note: the port is configured in package.json in definition of 'dev'

*Docker*
docker run -p 8081:80 my-angular-app


# Configure IDE for debug

https://github.com/microsoft/vscode-makefile-tools/issues/212

https://github.com/bloominstituteoftechnology/CS-Wiki/wiki/C-and-Cpp-Debugging-in-VS-Code

# Installing MySQL C++ Connector
https://www.codingfriends.com/index.php/2010/02/17/mysql-connection-example/comment-page-1/
https://askubuntu.com/questions/165868/installing-mysql-connector-c
https://dev.mysql.com/doc/connector-cpp/8.3/en/connector-cpp-installation-binary.html#connector-cpp-installation-binary-linux
https://aaronxu17.github.io/blog/install-mysql-connector/

apt-get install libmysqlcppconn-dev

git clone https://github.com/mysql/mysql-connector-cpp.git

#  Util Commands

**Find library installed (as mysql)** apt list --installed |grep mysql

libmysqlcppconn-dev/stable,now 1.1.12-4 amd64 [installed]
libmysqlcppconn7v5/stable,now 1.1.12-4 amd64 [installed,automatic]
mysql-common/stable,now 5.8+1.1.0 all [installed,automatic]

**Find installation location of package** dpkg -L libmysqlcppconn-dev

/usr/include/mysql_connection.h
/usr/include/mysql_driver.h
/usr/include/mysql_error.h
[...]
/usr/lib/x86_64-linux-gnu/libmysqlcppconn.a
/usr/lib/x86_64-linux-gnu/libmysqlcppconn.so
