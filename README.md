# gd32e23x_template
本项目为`GD32E230Fx`系列的基于Clion的CMake开发的工程模板。本人暂未入门，强行上强度，放弃keil，拥抱开源。遂尝试使用arm-none-eabi-gcc进行开发。
有幸寻得[@mo10 ](https://github.com/mo10)大佬的帮助，本项目的基础目录架构与CMakeLists.txt与toolchain.cmake均为大佬提供。

## 关于本项目
本项目默认的芯片型号为`GD32E230F4`，但是可以根据需要修改为其他型号，具体修改方法请参考下方`关于链接脚本`的说明。
### 版本号
默认版本号为`0.0.1`，在`CMakeLists.txt`中修改`PROJECT_VERSION`即可。
### 项目名称
默认项目名称为`gd32e23x_template`，在`CMakeLists.txt`中修改`PROJECT_NAME`即可。请先修改项目名称再配置编译环境。
### 软件IIC与硬件IIC
本项目中提供了软件IIC与硬件IIC的驱动，但是默认使用硬件IIC，如果需要使用软件IIC，请在`board_config.h`中取消注释`// #define SOFTWARE_IIC`(line 8)。
### 编译选项
本项目预留了两个编译选项，`OPT1`与`OPT2`，默认均为空，请根据需要自行修改，例如`OPT1`为`_[HW_IIC]`，`OPT2`为`_[NO_LED]`。
`OPT1`与`OPT2`均在`CMakeLists.txt`中可修改。
### 关于编译日期
本项目在`CMakeLists.txt`中添加了编译日期。

### 关于led
本项目默认开启了LED闪烁，并使用TIMER16进行定时。


## 关于C标准库的printf的重写
在Keil开发中，ARMClang有自己的microLIB，所以直接调用，然后重写fputc函数即可，但在gcc中需要重写`_write`函数，本项目模板中已经在`main.c`中完成重写。
同时需要添加`--spaces=nano.spaces`编译参数。
但是printf本身占用flash比较大，建议谨慎使用，尤其是本项目搭建时候采用的型号为`GD32E230F4V6`内存非常有限，重写后加上spaces设置，目前能用。

## 添加源文件与头文件
在`ProjectDir/CMakeLists.txt`中21行左右，添加对应源文件即可。

```cmake
set(TARGET_C_SRC
        ${CMAKE_SOURCE_DIR}/src/main.c
        ${CMAKE_SOURCE_DIR}/src/gd32e23x_it.c
        ${CMAKE_SOURCE_DIR}/src/systick.c
)
```
## 关于链接脚本
注意芯片选型, 不同型号的芯片 FLASH 和 RAM 大小不同。需要修改链接脚本`ld/gd32e23x_gcc.ld`

| 芯片型号       | FLASH | RAM |
|------------|-------|-----|
| GD32E230F4 | 16K   | 4K  |
| GD32E230F8 | 64K   | 8K  |

```linkerscript
/* memory map */
MEMORY
{
FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 16K
RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 4K
}
```

## 关于Startup文件

[@mo10 ](https://github.com/mo10)大佬提供的一些想法，具体如下。但是我目前还没测试到实际的影响和作用范围，所以暂未同步进来，仅作为备忘内容
```asm
Reset_Handler:
    ldr   r0, =_sp
    mov   sp, r0
    ldr   r0, =_end
    msr   msplim, r0
/* copy the data segment into ram */
    movs  r1, #0
    b  LoopCopyDataInit
```


## Ref

1. 参考LD/Startup

[https://github.com/Noveren/gd32e23x-template/blob/main/gd32e23x/template/linker.ld](https://github.com/Noveren/gd32e23x-template/blob/main/gd32e23x/template/linker.ld)

[https://github.com/Noveren/gd32e23x-template/blob/main/gd32e23x/template/startup.s](https://github.com/Noveren/gd32e23x-template/blob/main/gd32e23x/template/startup.s)

2. 官方LD/Startup

使用Embedded Builder工具生成的C标准库生成的模板
