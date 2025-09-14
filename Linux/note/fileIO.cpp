//****************************************文件io******************************
//站在内存的角度去理解io操作
    //标准c库的文件擦送做可以跨平台（带哦用把不同平台的系统api）
//虚拟地址空间
    //虚拟地址空间：是不存在的，时程序眼虚拟出来的，（进程是运行中的程序），

//文件描述符
    //程序（不占用内存空间，只占用磁盘空间）
    //进程（正在运行的程序，占用内存空间）

//文件描述符
    //文件描述符在内核区（pcb 进程控制块）（内核就是一个程序）
    //文件描述符表（数组）{一个进程可以打开多个文件}
    //前三个是被输入输出错误描述符占用（一个文件可以有多个文件描述符）


//open打开文件
    // int open(const char* pathname,int flags);
    //int open(const char* pathname,int flags,mode_t mode);
    //int close(int fd);
    //ssize_t read(int fd,void *buf,size_t count);
    //ssize_t write(int fd,void *buf,size_t count);
    //off_t lseek(int fd,void *buf,size_t count);

    // umsask
//