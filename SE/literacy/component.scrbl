#lang scribble/base

@require{literacy.rkt}

@handbook-story{详细设计}

到这里，我们的视角已经逐渐从用户过渡到软件工程师了。
系统功能方面在需求分析阶段就已经说明了大部分，数据模型类推迟到下一节再细聊；其他代码都有很多现成的轮子可用，不再赘述。
因此本小节只讨论最有趣的 Game UI 框架，它可以完全自由发挥。

@handbook-scenario{Game UI 框架}

@tamer-figure["cd:ui_class" "Game UI 类图"]{@image["stone/vsdx/ui_class.png" #:scale 0.32]}
顾名思义，Game UI框架是为游戏UI设计的框架，但究其本质，不外乎计算机图形学的应用。它跟传统GUI控件不同的地方在于，
传统GUI控件已经被别人画好了，开箱就用；Game UI则需要软件工程师自己动手。本系统需要可视化房间平面图，
传统GUI控件显然是不够用的。

本系统的 Game UI 框架面向接口设计，其类图见@tamer-figure-ref{cd:ui_class}，
这样无论在什么平台，软件工程师都只需关注如何封装好对应平台的图形系统。

@itemlist[

 @item{分析类的中的 Frame 是开发环境的原生窗体，在 Game UI 框架中，它被封装在 IUniverse 接口的实现类中。
  换而言之，IUniverse 接口指代操作系统（或语言运行时库）视角的窗体，由操作系统管理，同时负责接受、
  过滤、分发各种事件，包括用户输入、窗口缩放、定时器等。}

 @item{IPlanet接口指代窗口中的一个“页”。实际上，它代表用户视角的窗体，兼代理响应或再分发IUniverse分发的事件。}

 @item{IGraphlet接口是IPlanet对象管理的可视化元素的顶级抽象类，称为图元，习惯上以后缀“-let”命名。
  其实例即为用户可以看到或操作的一个个界面元素。比如文本标签(Labellet)、按钮(Buttonlet)等。
  它的父类ISprite也是可视化元素，表示非图元的其他可视化元素，比如屏幕虚拟键盘等。}

 @item{枚举类型GraphAnchor指代IGraphlet实例的九个常用锚点，主要用于在重排图元时方便对齐，或以其他图元作为参照物。}

 @item{Game UI 框架也是访问者模式的应用，如此一个 IPlanet 实例也可以作为图元嵌套在另一个 IPlanet 实例里。这就是
  IScreen 接口和 IDisplay 接口存在的原因。}
]

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-reference[]
