#lang scribble/base

@require{literacy.rkt}

@handbook-story{系统设计与实现}

对于简单信息管理系统，一旦数据模型确定，软件架构也就基本确定了。
鉴于本系统需要解决实际问题，纯命令行接口和传统的图形用户接口都不符合用户期望，
因此本系统沿用了游戏化概念而设计成了半游戏半应用结构(@tamer-figure-ref{s:mvc})。

@tamer-figure-here["s:main" "主界面"]{@image["stone/screenshots/main.png" #:scale 0.24]}

@tamer-figure-ref{s:main}主界面展示了系统的主要素：

@itemlist[
 #:style 'compact
 
 @item{本系统虽然是游戏界面，但仍然保留了命令行风格的菜单栏。
  该菜单栏只能通过数字键盘操作，不响应鼠标事件。}
  
 @item{班级实现为“门”和六边形实验桌，学生按照班级以虚拟形象出现在各自的座位上。
  它们响应鼠标事件，除刷新信息外，也可操作学生调座位(或调班)。}
 
 @item{课程实现为“书”放置在讲台上(点击讲台会取消选中课程，并平铺其他课程)，
  它们响应鼠标事件用以刷新学生的“血条”。“血条”反映该科目成绩，并用不同颜色表示不同等级。}

 @item{成绩单实现为侧边栏元件，不响应用户事件，只负责更新成绩信息。
  学生成绩单还额外以下标形式显示最近两次考试的“进步分”。}

 ]

@handbook-scenario{架构设计}

无论是作为图形界面应用，还是作为游戏应用，对象导向的分析和设计都比较合理。
因此，本系统的架构模式是经典 @emph{Model-View-Controller}，
在该架构下的系统类图如@tamer-figure-ref{s:main}。

@tamer-figure-here["s:mvc" "系统类图"]{@image["stone/SE/MVC.png" #:scale 0.64]}

此系统类图省略了游戏引擎方面的细节，因而比系统的实际实现要简单很多。
在讨论详细设计之前，先对该类图作些简要说明。

@itemlist[
 #:style 'compact
 
 @item{本系统的开发遵循“接口导向设计”原则。
  @cpp:type{GradeManagementPlane}是主界面类，充当控制器角色。
  除了分派系统事件和用户事件外，还通过实现@cpp:type{IModelListener}
  和@cpp:type{IMenuListener}两个接口响应业务事件；}
 
 @item{主界面上组合了一系列视图对象，在系统依赖的底层游戏引擎语义下，
  视图对象可以分为图元(@cpp:type{IGraphlet})
  和精灵(@cpp:type{ISprite})两大类。
  具体的视图对象实现各自的接口。
  在实现细节上，菜单对象(和未在图中画出的角色虚拟形象对象)是一类特殊的视图，
  通过实现访问者模式将子界面嵌入主界面中，以复用基础视图对象，
  在此不做详述。}
 
 @item{模型对象(@cpp:type{GradeManagementSystemModel})
  组合了一系列实体对象(@cpp:type{GMSEntity})。
  前者负责组织和操作后者(数据结构和业务算法)，
  后者负责对自己数据的保存和加载。}
                   
 @item{图中省略号省掉的是该位置处的对等类。
  比如，与@cpp:type{StudentSprite}对等的类有@cpp:type{DisciplineSprite}、@cpp:type{DoorSprite}等；
  与@cpp:type{SeatEntity}对等的类有@cpp:type{ClassEntity}、@cpp:type{GradeEntity}等；
  与@cpp:type{ToplevelMenu}对等的类有@cpp:type{DisciplineMenu}、@cpp:type{GradeMenu}等。}

 ]

@handbook-scenario{接口导向设计}

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-reference[]
