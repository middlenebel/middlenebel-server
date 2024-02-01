
# Hot-to Build Middlenebel

sudo chown -R owner:group directory

## Build Server

## Install Git

## Install g++
sudo apt install g++

## Install Make & CMake
sudo apt install make
sudo apt install cmake

## Install Boost
sudo apt install libboost-all-dev

## Install LibJson
sudo apt-get install libjsoncpp-dev

## Install LibRdKafka
sudo apt-get install -y librdkafka-dev

# Install MySQL Connector
sudo apt-get install libmysqlcppconn-dev

# Install kubect
sudo apt update
sudo apt-get install -y apt-transport-https ca-certificates curl
curl -fsSL https://pkgs.k8s.io/core:/stable:/v1.29/deb/Release.key | sudo gpg --dearmor -o /etc/apt/keyrings/kubernetes-apt-keyring.gpg
**This overwrites any existing configuration in /etc/apt/sources.list.d/kubernetes.list**
echo 'deb [signed-by=/etc/apt/keyrings/kubernetes-apt-keyring.gpg] https://pkgs.k8s.io/core:/stable:/v1.29/deb/ /' | sudo tee /etc/apt/sources.list.d/kubernetes.list
sudo apt-get install -y kubectl

## Download & Build LibCppKafka
sudo apt install libssl-dev
git clone https://github.com/mfontanini/cppkafka.git
cd cppkafka
mkdir build
cd build
cmake ..
make
sudo cp src/lib/libcppkafka.so.* /usr/lib
sudo ln -s /usr/lib/libcppkafka.so.* /usr/lib/libcppkafka.so
cd ../..

## Install Dockerd
sudo apt install docker.io -y

## Download & Build Middlenebel Server
git clone https://github.com/middlenebel/middlenebel-server.git
cd middlenebel-server
make all

echo "Installation finished!"
echo "You can start the server with ./main"

## Build Front
docker build -t my-angular-app .

See: https://blog.back4app.com/how-to-deploy-an-angular-application/#Step_3_Dockerize_the_Application
Run `npm audit` for details.
npm notice 
npm notice New minor version of npm available! 10.2.4 -> 10.4.0
npm notice Changelog: <https://github.com/npm/cli/releases/tag/v10.4.0>
npm notice Run `npm install -g npm@10.4.0` to update!
npm notice 

docker run -p 8081:80 my-angular-app

### Updating ng and npm
sudo apt install npm
sudo npm install -g npm@latest
npm install lite-server --save-dev


sudo apt install ng-common


**npm**
npm install npm@latest

### Installing HTTP Server
npm install lite-server --save-dev

### Build Front
ng build --configuration production

### Start build
cd dist
http-server -p 9000

Or

lite-server --baseDir="dist/project-name"

# openSUSE
- openSUSE: sudo zypper install git-core

## Install g++ (gcc-c++ in openSUSE)
- openSUSE: sudo zypper in gcc-c++

## Install Make
- openSUSE: sudo zypper install make

## openSUSE no compatible con BOOST

Dependencies:
npm install ng-circle-progress --save
OR????
https://www.infragistics.com/products/ignite-ui-angular/angular/components/circular-progress
ng add igniteui-angular


Build front:
ng build --configuration production
ng build --watch
lite-server --baseDir="dist/project-name"

npm run dev

npm install lite-server --save-dev

https://www.npmjs.com/package/lite-server

http-server -p 9000

------------ PROJECTS
projects/alpha-01	Librerias dinamicas C++. Makefile.
projects/boost/test_boost / 
projects/boost/test_http
projects/sharelib	Librerias dinamicas C. Ejecucion directa? error?


projects/boost/test_http
------------------------g	
punksystec@LAPTOP-CPH12905:~/projects/boost/test-http$ ./http_client_sync google.com 80
Usage: http-client-sync <host> <port> <target> [<HTTP version: 1.0 or 1.1(default)>]
Example:
    http-client-sync www.example.com 80 /
    http-client-sync www.example.com 80 / 1.0
punksystec@LAPTOP-CPH12905:~/projects/boost/test-http$ ./http_client_sync google.com 80 /
HTTP/1.1 301 Moved Permanently


-------------- Boost
https://www.boost.org/
https://boostorg.jfrog.io/artifactory/main/release/1.83.0/source/boost_1_83_0.tar.gz

https://curl.se/

punksystec@LAPTOP-CPH12905:~/projects/boost$ curl https://boostorg.jfrog.io/artifactory/main/release/1.83.0/source/boost_1_83_0.tar.gz --output boost_1_83_0.tar.gz
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
  0     0    0     0    0     0      0      0 --:--:--  0:00:01 --:--:--     0
punksystec@LAPTOP-CPH12905:~/projects/boost$
???
tar --bzip2 -xf /path/to/boost_1_82_0.tar.bz2

tar -xzvf boost_1_83_0.tar.gz

https://www.boost.org/doc/libs/1_83_0/more/getting_started/unix-variants.html
https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2005/n1810.html

Guidelines
A proposal is more than a wish. "C++ should support JPEG manipulation" is not a proposal. A class synopsis is not a proposal. Even the documentation for a widely used library isn't necessarily a proposal. A proposal should include the following things:

Motivation and scope. What user community is this library intended for, what problems does it address (and what problems does it deliberately not address), and how is it intended to be used?
How does it fit with the rest of the standard library as a coherent whole?
Design decisions. A problem usually has many solutions. Why this particular solution? How does this relate to previous work in this area, if any?
"Standardese." A standard is a contract between implementers and users, to make it possible for users to write portable code. It's supposed to say what implementers are permitted to do, what they're required to do, and what users can and can't count on. The "standardese" should match the general style of exposition of the standard, but it doesn't have to match the exact margins or fonts or section numbering; those things will all be changed anyway.
These things are important so that the standardization committee can evaluate whether the proposal solves the problem it's intended to solve. Remember that C++ committee members aren't necessarily experts in whatever domain you're addressing. A clear discussion of motivation and design decisions is more important than finalizing the exact standardese wording. You should expect a proposal to go through at least one revision before it's accepted, so, if the committee likes a proposal, there will be a later opportunity to adjust the precise wording.

SEE:
https://graphviz.org/





--------------
https://github.com/yhirose/cpp-httplib
https://asrob-uc3m.gitbooks.io/tutoriales/content/software/programming/cmake.html
cmake genera Makefile con opciones por defecto
cmake .


https://www.reddit.com/r/cpp_questions/comments/11r6ozi/c_websocket_and_http_client/

https://jasonco.org/2017/07/14/linux-project-rest-server/


$ sudo apt-get install libcpprest-dev


sudo apt-get install libjsoncpp-dev
sudo ln -s /usr/include/jsoncpp/json/ /usr/include/json

sudo apt-get install libgtest-dev

C++
https://es.stackoverflow.com/questions/108503/c17-qu%C3%A9-es-no-es-c
https://en.cppreference.com/w/cpp/17
https://www.w3schools.com/cpp/

C++Files
https://cplusplus.com/doc/tutorial/files/

C++Memory
https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process

C++HttpClient
https://stackoverflow.com/questions/822581/what-c-library-should-i-use-to-implement-a-http-client
https://www.curlpp.org/
https://cpp-netlib.org/#
https://www.w3.org/Library/
https://microsoft.github.io/cpprestsdk/namespaceweb_1_1http_1_1client.html
https://devblogs.microsoft.com/cppblog/the-c-rest-sdk-casablanca/
--
https://www.boost.org/
https://www.boost.org/doc/libs/master/libs/beast/doc/html/beast/quick_start/http_client.html

Code::Blocks
https://www.codeblocks.org/

Qt
https://forum.qt.io/topic/70839/how-to-load-a-dynamic-library-on-demand-from-a-qt-method
https://stackoverflow.com/questions/59006802/how-do-i-create-and-load-lib-dynamically-in-qt-c
https://www.qtcentre.org/threads/53122-Dynamically-loading-libraries


POCO
https://pocoproject.org/
https://github.com/microsoft/vcpkg
https://www.includecpp.org/


APIs Docker
https://docs.docker.com/engine/api/
https://docs.docker.com/engine/api/sdk/

C++ Client
https://github.com/lasote/docker_client

DL Libraries

https://stackoverflow.com/questions/496664/c-dynamic-shared-library-on-linux
https://stackoverflow.com/questions/483797/dynamic-shared-library-compilation-with-g
https://tldp.org/HOWTO/Program-Library-HOWTO/dl-libraries.html

https://www.earthinversion.com/utilities/load-shared-libraries-at-in-c/?utm_content=cmp-true

RTLD_LAZY, meaning ``resolve undefined symbols as code from the dynamic library is executed''
RTLD_NOW, meaning ``resolve all undefined symbols before dlopen() returns and fail if this cannot be done''
RTLD_GLOBAL may be optionally or'ed with either value in flag, meaning that the external symbols defined in the library will be made available to subsequently loaded libraries

https://www.devxperiences.com/pzwp1/2023/05/17/c-dynamic-loading-of-a-shared-library-also-create-and-load-one-on-your-own/

https://en.wikipedia.org/wiki/Executable_and_Linkable_Format

MUD
https://www.youtube.com/watch?v=q-yhk0chxyk
https://en.wikipedia.org/wiki/AberMUD


BOOKs
Deep C Secrets by van Linden
Kernighan and Ritchie Book on C

