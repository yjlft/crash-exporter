### 简介 ###

**`CrashExporter`** 是基于开源项目crashrpt基础上改造的、基于Windows平台的一个轻量级的异常信息导出组件。
原[crashrpt](http://code.google.com/p/crashrpt/) 可以通过邮件或者http发送错误报告。由于项目实际，也为了减少代码阅读量，去除了发送功能并添加了导出异常信息功能。


**`CrashExporter`** 中包含三个工程由复杂到简单分别为`01CrashExporter`、`02CrashExporterLite` 和 `03CrashExporterMini`。
  * `01CrashExporter` 功能比较齐全，安全性和完整性比较好；需要`CrashRpt.dll`、`CrashSender.exe` 以及 `CrashRpt.h`这三个文件。

  * `02CrashExporterLite` 是Lite版的CrashExporter。 能导出dump和堆栈信息文件；同01一样，可以导出常见的异常信息;需要CrashExporterLite.dll 和 CrashRpt.h这两个文件。

  * `03CrashExporterMini` 是Mini版的`CrashExporter`。 仅能能导出SEH异常的dump和堆栈信息文件，仅支持vc6。好在只有一个头文件CrashExporterMini.h






### 01CrashExporter ###

<> 参考自开源项目[crashrpt](http://code.google.com/p/crashrpt/) 版本号v.1.4.2\_r1609。


<> 该项目根据开源项目`CrashRpt`进行修改，具体:
  * 移除了报告分析模块（项目中的processing部分）；
  * 去除支持的录像、http、smtp、mapi等功能；
  * 精简`CrashRpt`。去除原来CrashSender的所有界面，增加自定义的导出界面；
  * 去除了RegKey等API,去除不感兴趣的dwflags属性；
  * 默认使用静默导出方式；静默时，会在程序的当前文件夹生成以崩溃时间命名的zip压缩包；
  * 增加堆栈记录（stackwalk）功能（参考自[stackwalker](http://stackwalker.codeplex.com)）；
  * 增加支持vc6的头文件（`CrashRpt` for vc6.0.h）。


<> 主要功能介绍：
  * 有两种工作模式：静默模式和弹出对话框模式。
    * 静默模式：默认以当前的时间为名称（精确到秒），打包成zip在程序工作目录；
    * 弹出对话框模式：当程序出现异常时弹出对话框，手动输入文件名称和工作目录。

  * 异常报告内容：`MiniDump`文件、系统信息xml文件、堆栈信息文件和屏幕截屏文件(可选)。
    * `MiniDump`文件：该文件为`MiniDumpNormal`类型dump文件，只包括了最必要的信息，用于恢复故障进程的所有线程的调用堆栈，以及查看故障时刻局部变量的值；
    * 系统信息xml文件：记录了操作系统相关的信息和打包文件的一些信息等，该文件可以通过`crAddProperty`接口进行定制。
    * 堆栈信息文件：记录了异常时程序的模块调用和堆栈信息打印，可以定位到代码文件的某一行。
    * 屏幕截屏文件：可以调用接口`crAddScreenshot`来抓取崩溃时屏幕的截屏。
  * 添加自定义的文件。 使用接口`CrAddFile`，将出现异常时其他信息文件一起保存到压缩包。


<> 支持vc6.0以上版本。






### 02CrashExporterLite ###

<> 参考自文章[Effective Exception Handling in Visual C++](http://www.codeproject.com/Articles/207464/Exception-Handling-in-Visual-Cplusplus)和[stackwalker](http://stackwalker.codeplex.com)


<> 具体工作：
  * 整合上面这两篇文章内容；
  * 增加了文件记录异常堆栈信息功能；
  * 封装导出接口，封装在crashrpt.h中。


<> 主要功能介绍：
  * 当程序Crash时，在程序的工作目录生成MiniDump文件和堆栈信息文件；
  * 可以指定Dump文件以及堆栈打印的文件名；倘若不输入或者文件名为空，则以当前的时间为名称。


<> 支持vc6.0以上版本。






### 03CrashExporterMini ###

<> 参考自文章[StackWalk - Win32 samples](http://win32.mvps.org/misc/stackwalk.html)、[stackwalkerC++异常中的堆栈跟踪](http://bbs.csdn.net/topics/10469709)以及[软件Release版本Crash堆栈信息收集](http://blog.csdn.net/boythl/article/details/2868344)


<> 具体工作：
  * 整合上面这三篇文章内容，封装在头文件CrashExporterMini.h中；
  * 去除不重要的堆栈跟踪；
  * 增加了文件记录异常堆栈信息功能；


<> 主要功能介绍：
  * 定义CrashExporterMini对象即可使用；默认以当前的时间为名称，另外可以在构造函数中指定自定义的名称；


<> 支持vc6.0以上版本。