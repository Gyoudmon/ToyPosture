#lang scribble/manual

@(require "literacy.rkt")

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-root-story{开发环境}

本系统的实现语言是 Oracle Java(最低 jdk11)， IDE 是 Eclipse，没有额外配置。

@handbook-scenario{源码结构}

以下是本系统源码的目录组织及简要说明。

@tamer-figure!["srctree" "源码目录结构"]{
 @filesystem-tree[
 '(|.|
   (info.rkt . "Racket 软件包元信息文件")
   ((src/gms/wargrey . "源码目录及默认包")
    (GradeManagementSystem.java . "系统入口")
    (view . "视图层包源码")
    (model . "模型层包源码"))
   (tamer . "含系统测试数据")
   (literacy . "本文档源码"))]}

@handbook-scenario[#:tag "run"]{运行系统}

构建完，Java 字节码目录位于源码根目录下的 @filepath{compiled}。
可直接运行：

@itemlist[#:style 'compact
          @commandline{java -cp compiled gms/wargrey/GradeManagementSystem tamer/gms.db gms.out}]

程序接受的前两个参数分别表示导入和导出文件。
如果不设导出文件，则直接覆盖导入文件。
如果不指定文件，系统仍可运行但无法导出数据。

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-reference[]
