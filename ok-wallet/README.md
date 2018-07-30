sudo yum install patch


### 编译时遇到的一些错误及解决方法
错误: 执行./zcutil/build-mac.sh时错误：
configure: error: C compiler cannot create executables
configure: error: C++ compiler cannot create executables
将zcutil/build-mac.sh和depends/builders/darwin.mk中的
export CC=gcc-5
export CXX=g++-5
改为：
export CC=gcc
export CXX=g++

编译libsnark时报错：
clang: fatal error: unsupported option '-fopenmp'
