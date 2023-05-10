#lang scribble/base

@require{literacy.rkt}

@handbook-story{系统设计与实现}

对于简单信息管理系统，一旦数据模型确定，软件架构也就基本确定了。
鉴于本系统不需要解决实际问题，采用纯命令行接口用于示范就够了。

@handbook-scenario{架构设计}

本系统的架构模式是经典 @emph{Model-View-Controller}，
在该架构下的系统类图如@tamer-figure-ref{s:mvc}。

@tamer-figure!["s:mvc" "系统类图"]{@image["stone/SE/MVC.png" #:scale 0.618]}

此系统类图省略了部分辅助类和枚举类。
在讨论详细设计之前，先对该类图作些简要说明。

@itemlist[
 #:style 'compact
 
 @item{本系统的开发遵循“接口导向设计”原则。
  @java:type{GradeManagementSystem}是命令行用户接口类，充当控制器角色。
  除读取键盘输入外，还通过实现@java:type{IModelListener}
  和@java:type{IMenuListener}两个接口响应业务事件。【源码文件：
  @filepath{src/gms/wargrey/GradeManagementSystem.java}】}
 
 @item{主界面组合了一系列菜单对象，用户输入中对菜单的操作会转发给
  相应的菜单对象，该菜单对象负责解读用户操作，并以业务事件形式反馈给主界面。
【源码文件：@filepath{src/gms/wargrey/view/AbstractMenu.java}】}
 
 @item{业务模型对象(@java:type{GMSModel})组合了一系列实体对象(@java:type{GMSEntity})。
  前者负责组织和操作后者(数据结构和业务算法)，后者负责对自己数据的保存和加载。【源码文件：
  @filepath{src/gms/wargrey/model/GMSModel.java}】}
 
 ]

图中省略号省掉的是该位置处的对等类。
比如，与@java:type{GradeEntity}对等的类有@java:type{ClassEntity}、@java:type{DisciplineEntity}等；
与@java:type{ToplevelMenu}对等的类有@java:type{DisciplineMenu}、@java:type{GradeMenu}等。

@handbook-scenario[#:tag "dml"]{实体操作语义}

由于Java没有头文件，在本文中插入Java方法声明会连带着实现细节，
因此从调用方入手来考察基础语义(@tamer-code-ref{java:ems})。

@tamer-java['java:ems "实体操作语义" "GradeManagementSystem.java" #px"Entity Manipulation"]

都是常规操作，需要特别说明的是“删除”语义，
所有对实体的删除均只删除这个实体本身，不级联或递归，也无法撤销。

@handbook-scenario{用户工作流}

请先回顾@secref{run}了解程序启动选项。

@tamer-figure!['s:manipulation "数据操作示例"]{@image["stone/screenshots/manipulation.png" #:scale 0.72]}

启动程序时如果指定了输入文件，系统会自动加载文件里的记录。
在程序运行期间，用户仍可通过手动执行主菜单的“载入数据”重新加载输入文件里的内容。

班级、课程、学生三个主实体的创建并没有明确的顺序，
创建学生实体可以分配给一个不存在的班。但是登记成绩时必须要现有学生和课程。

实体的更新与创建相似：
进入相应实体的管理菜单、选择“更新”、根据该实体的@java:type{update_prompt()}信息
输入新的数据。从数据记录的角度看，“更新”与“创建”的差别在于前者不识别主键，
且留白字段视为“不做更改”字段。

删除实体较为简单，进入对应菜单执行“删除”操作，输入所需的主键信息即可。
请回顾@secref{dml}了解本系统中的“删除”语义。

最后，软件使用过程中对实体所做的修改必须手动执行主菜单的“保存数据”才会导出到指定文件，
无自动保存。

@tamer-figure-ref{s:manipulation}演示了对班级和课程实体的操作过程，这两个实体比较简单。
课程实体施加更多的约束，课程代号和课程名称实际上是冗余设计，其中一个能完全决定另一个。
这里不做深究。

@handbook-scenario{数据结构与算法}

本系统的实体对象较少，关系简单，且所有实体的主属性可排序。
因此使用@java:type{java.util.SortedMap}作为数据结构即可满足需求。
实体对象的主属性作为@java:type{SortedMap}实例的键，
实体对象作为@java:type{SortedMap}的值(@tamer-code-ref{java:dt})。

@java:variable{GMSModel.scores}的类型定义最为复杂，
实际上只不过是将成绩按照时间戳和课程分别组织在嵌套的@java:type{SortedMap}实例中了。

@tamer-java['java:dt "实体对象管理" "model/GMSModel.java" #px"Entity Runtime Organization"]

对实体对象的操作就转化为了对相应@java:type{SortedMap}实例的操作，
因此实现本系统的过程中无需考虑特别细致的算法，而只需关注业务需求：

@itemlist[
 #:style 'compact
 
 @item{查询实体数据。}
 @item{插入操作不会产生冗余数据，如果主键已存在，则抛出异常，并以合适的消息告知用户。}
 @item{更新和删除操作不递归不级联，如果主键不存在，则抛出异常，并以合适的消息告知用户。}
 @item{所有数据操作成功均会以事件形式通知主界面，主界面决定是否以及如何告知用户。}
 @item{“载入数据”等价于“批量删除/清理所有已有数据”再“批量创建文件里的所有记录”。}
 ]

@handbook-action{数据查询}

本系统的数据不存储在数据库系统中，因此无法搭上 SQL 的便利，
因而数据查询服务只能以@java:type{GMSModel}方法的形式按需提供(@tamer-code-ref{java:dqa})。

@tamer-java['java:dqa "实体数据查询方法" "model/GMSModel.java" #px"Query and Aggregation"]

在终端展示数据也是个技术活，本系统有两种策略。
对于班级、课程这类样本量不大的实体，采用按列平铺的方式呈现。

@tamer-java-function['displayBriefStudentSet "平铺显示姓名和学号" "GradeManagementSystem.java"]

对于学生这类样本可能很大的实体，如果只需要知道姓名和学号的对应关系，展示方式同上(@tamer-code-ref{java:displayBriefStudentSet})；
如果需要详细信息，那就按行显示，每显示几条就等用户按回车再继续(@tamer-code-ref{java:displayStudentSet})。

@tamer-java-function['displayStudentSet "分屏显示学生信息" "GradeManagementSystem.java"]

@handbook-action{数据操作(插入、更新、删除)}

这几类数据操作本身没啥特别的。
只是在实现上效仿了观察者模式，当数据操作正常完成时，
会向主界面发送相应的业务事件(@tamer-code-ref{java:IModelListener})。
特别的，对于@java:type{GradeEntity}这个非模型层透明的弱实体，
它们的信息除了通过事件方式通知主界面，
更常用的还是主界面根据自己的需要通过数据查询服务来拉取最新信息。

@tamer-java-class['IModelListener "模型事件监听器接口" "model/IModelListener.java"]

而当操作失败时，使用异常处理机制通知主界面(@tamer-code-ref{java:updateStudentFromUser})。

@tamer-java-function['updateStudentFromUser "根据用户输入更新学生信息" "model/GMSModel.java"]

菜单系统在设计上也采用类似的事件机制。不再详述。

@handbook-action{去重和其他}

对于重视类型安全的设计，本系统已经从多个角度确保了模型层里不存在重复数据。
对于多个用户借用其他软件合并存储文件而导致数据重复的情况，
则在被系统加载时就会顺便剔除后出现的记录。
因为初次“加载数据”等价于“批量插入文件里的所有记录”，
每一次“插入”都会接受与用户输入新数据一样的安全性检查。
而且，这个剔除操作不会直接修改数据源文件，
仅当用户手动执行“保存数据”操作时才会将干净的数据写回去。

@handbook-scenario{应用举例：成绩单}

最后以成绩单功能来综合展示以上算法的运行效果，给2号学生的数学成绩加8分。

@tamer-figure!['s:grade_report "成绩单功能"]{@image["stone/screenshots/grade_report.png" #:scale 1.0]}

@tamer-figure-ref{s:grade_report}展示了一些上面可能未提及的细节：
@itemlist[
 #:style 'compact

 @item{显示菜单之后，在等待用户输入操作指令之前，还会显示一些有用的数据。
  比如“成绩管理菜单”处显示的时间戳信息。其他菜单显示的就是它们各自操作的实体信息。
  因为无什么操作，用户都会自己做先查询，那就自动化好了。}

 @item{对成绩的操作必须先指明构成它的组合主键的三个参数：学号、课程代号及时间戳。
  在操作过程中，学生信息和时间戳都以平铺方式呈现，成绩单加了一些字符修饰。
  然后，该显示的显示，该更新的更新，更新完了还要再显示一遍。}

 @item{组合主键不可修改，可变的只有成绩一栏，这意味着新建和更新的步骤也一致。}
 ]

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-reference[]
