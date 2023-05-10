#lang scribble/manual

@(require "literacy.rkt")

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-root-story{开发环境}

本系统的实现语言是 ISO C++(stdc++17)，整体风格偏向“带类(和共享指针)的 C”。
在 macOS 下构建无需额外配置，
在 Windows 下不可用 cygwin 等模拟环境，
需将 @filepath{C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build}
加入环境变量。

此外，构建之前需要先安装@hyperlink["https://racket-lang.org/download/"]{Racket}，
配置好环境变量再安装我自己的构建工具：

@itemlist[#:style 'compact
          @commandline{raco pkg install -i --auto digimon}]

@handbook-scenario{源码结构}

本系统以游戏化的形式呈现，其中的游戏引擎源码依赖 SDL2（该套源码也是我本人所写，
并与其他用C++写的课程项目共享，但该引擎的实现细节不在本文档中详述）。
以下是本系统源码的目录组织及简要说明。

@tamer-figure!["srctree" "源码目录结构"]{
 @filesystem-tree[
 '(|.|
   (info.rkt . "Racket 软件包元信息文件")
   (GradeManagementSystem.cpp . "本系统入口文件（含GameUI）")
   ((digitama . "私有源码")
    (IMS . "系统直接相关源码")
    (big_bang . "系统依赖的游戏引擎源码"))
   ((stone . "资源目录")
    (mascot . "游戏引擎预设资源，在本系统中用于表示学生的虚拟形象")
    (IMS . "系统直接相关资源和素材"))
   (tamer . "含系统测试数据")
   (literacy . "本文档源码"))]}

文件构建完成，可执行文件位于源码根目录下的@filepath{compiled/native}中。

@handbook-scenario{构建系统}

在终端下进入到源码根目录中，执行命令：

@itemlist[#:style 'compact
          @commandline{raco wisemon -d cc GradeManagementSystem.cpp}]

Windows 下构建要多等几秒，让 MSVC 构建工具设置好临时的编译环境变量。

@handbook-scenario[#:tag "run"]{运行系统}

构建完，可执行程序路径是源码根目录下的 @filepath{compiled/native/[体系结构]/GradeManagementSystem.exe}。
可直接运行：

@itemlist[#:style 'compact
          @commandline{compiled/native/x86_64-macosx/GradeManagementSystem --in tamer/gms.db --out gms.out}]

并用 @envvar{--in} 和 @envvar{--out} 分别设定导入和导出文件。
如果不设导出文件，则直接覆盖导入文件。
如果不指定文件，系统仍可运行但无法导出数据。

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-reference[]
