//GDB 是一种调试工具
    //1.启动程序，按照自定义的方式运行程序
    //2.在指定的位置断点调试
    //3.程序停止后，可以检查程序员中发生的事情
    //4.可以改变程序，将一个BUG产生的影响修正从而测试其他BUG

//在编译的时候关闭优化选项（-o）打开调试选项（-g）  打开所以warning可以发现许多的问题，比卖你一些不必的BUG
    //启动和退出             gdb   (可执行文件)      quit
    //给程序设置参数/获取设置参数
    //    set args 10 20                    show args
    //GDB使用帮助               help
//查看当前文件代码   list/l(从默认位置显示)
    //                 list/l  行号   (从指定行位置显示)
    //                 list/l  函数名 (从指定函数显示)
//查看非当前文件代码
    //                  list/l  文件名：行号
    //                  list/l  文件名：函数名
//设置显示的行数
    //                  show list/listsize
    //                  set  list/listsize 行数
//设置断点
    //                  b/break     行号
    //                  b/break     函数名
    //                  b/break     文件名：行号
    //                  b/break     文件名：函数
//查看断点
    //                  i/info b/break
//删除断点
    //                  d/del/delete 断点编号
//设置断点无效
    //                  dis/disable 断点编号
//设置断点生效
    //                  ena/enable 断点编号
//设置条件断点(一般用在循环的位置)
    //                  b/break 10 if i==5




//运行GDB程序
    //start(程序停在第一行)
    //run(遇到断点才停)
//继续运行，到下一个断点停
    //c/continue
//向下执行一行代码（不会进入函数体）
    //n/next
//变量操作
    //p/print变量名(打印变量值)
    //ptype  变量名（打印变量类型）
//向下单步调试（遇到函数进入函数体）
    //s/step
    //finish（跳出函数体）
//自动变量操作
    //display num(自动打印指定变量的值)
    //i/info display
    //undisplay 编号
//其他操作
    //set var 变量名=变量值
    //until(跳出循环)