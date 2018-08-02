### 编译

##### centos7的编译依赖项

```shell
sudo yum install patch
```

##### 编译步骤
- git clone https://github.com/okblockchainlab/zen.git
- cd zen
- ./build.sh (only run this script if you first time build the project)
- ./runbuild.sh


### mac编译时遇到的一些错误及解决方法
**ERROR:**  
- configure: error: C compiler cannot create executables
- configure: error: C++ compiler cannot create executables
- clang: fatal error: unsupported option '-fopenmp'

这是因为zen所使用的编译器为g++-5和gcc-5，且支持openmp。而mac系统自带的编译工具中没有这俩命令，且自带的clang也不支持-fopenmp选项。

解决方法是安装支持openmp的gcc编译工具：
1. 安装MacPorts工具，官网：[www.macports.org](www.macports.org)
- sudo port -v selfupdate
- sudo port install gcc5
- ln -s /opt/local/bin/g++-mp-5 /opt/local/bin/g++-5  
ln -s /opt/local/bin/gcc-mp-5 /opt/local/bin/gcc-5

**ERROR:** ginstall: command not found
解决方法：brew install coreutils


**ERROR:** cargo: cannot execute binary file
原因：depends/packages/rusk.mk中直接下载bin文件，但下载的版本是固定的。如果出现cargo不能执行的错误，很可能是下载的版本与当前系统不对应。

解决方法：你可以根据自己系统的版本修改depends/packages/rusk.mk中的下载包和哈希值。


### 其它注意项
- 在mac系统中，测试程序test_okwallet遇到一个问题：main函数return以后，程序会崩溃报错：Segmentation fault: 11。当把dlclose（在TearDownTestCase中）注释掉以后，程序正常退出不会崩溃。这个现象只在mac系统出现，在centos7中没出现。  
后来查到这个[issues](https://github.com/rust-lang/rust/issues/28794)，说是可能_tlv_atexit注册回调函数后，只有整个进程退出回调函数才会被调用，而此时这个dylib已不存在，所以崩溃。解决方法是将dlopen的mode改为RTLD_NODELETE。验证后发现有效。
