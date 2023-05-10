#lang scribble/base

@require{literacy.rkt}

@handbook-story{数据模型}

成绩管理系统是一类定义良好的系统，除在具体应用场景某些成绩参数难以量化外，其数据模型本身比较清晰明了。

@handbook-scenario{概念模型设计}

本系统包含班级、学生、课程三个实体，按照惯例，主属性带下划线：

@itemlist[
 @item{班级：@pk{班级编号}}
 @item{学生：@pk{学号}，昵称，性别，年龄}
 @item{课程：@pk{课程代号}，课程名称，学分}
]

实体之间具有如下关系：

@itemlist[
 @item{一个班级管理多个学生}
 @item{一个学生选修多门课程，一门课程可以被多个同学选修}
 @item{学生通过参与考试“选修”课程，该关系包含属性“考试时间”和“考试成绩”}
]

可得如下实体关系图：
@tamer-figure!["erd" "实体-关系图"]{@image["stone/SE/ER.png" #:scale 0.75]}

@handbook-scenario{逻辑模型设计}

按如下思路将前述概念模型转化为逻辑模型：

@itemlist[
 @item{一个实体型转换为一个关系模式，实体属性对应关系属性}
 @item{一对多联系与多多端合并转化为一个关系模式，其主键为多端主属性}
 @item{多对多联系转化为一个关系模式，联系属性转化为关系属性，且关系主键是各端实体及联系主键的组合}
]

于是得到如下基本关系模式，主键依旧带下划线：

@itemlist[
 @item{Class(@pk{clsId})}
 @item{Discipline(@pk{disCode}, name, credit)}
 @item{Student(@pk{sNo}, clsId, nickname, gender, age)}
 @item{Grade(@pk{sNo}, @pk{disCode}, @pk{timestamp}, scores)}
]

且很容易论证，在现有属性集中，该关系模型已经满足3NF（及BCNF），因此，无需进一步微调关系模式。

@handbook-scenario{物理模型设计}

本系统的所有数据均以文本形式按行存储在一个文件里，其格式为：
@centered{@tt{tag:field1,field2, ...}}

简而言之，每一行数据以关系实体类型标记开始，后跟分号(@tt{:})，
字段以逗号(@tt{，})分隔，逗号两边若存在空格则忽略。例如：@linebreak[]

@tabular[#:style 'boxed
         (list (list @tt{c:700})
               (list @tt{d:1,Mathematics,6})
               (list @tt{s:700,6103,txm,Girl,12})
               (list @tt{sdt:2,1,20220630,9.0 14.0 18.0 23.0 13.0 3.0}))]

其中，类型标记以字符为单位，多字符类型可用于提示组合主键及其存储循序。
特别需要说明的是，学生@bold{单科单次}考试的成绩仍被设计成了列表类型，以空格分隔。
这个设计的用意是让教师按题型登记学生成绩，减少算分出错的可能。

@handbook-scenario{类型系统与类型安全}

Java 是静态强类型语言。即，每一个数据都必须显式声明其类型，并由编译器保证编译过程中的类型正确。
同时 Java 也直接提供运行期类型检查，并通过异常处理机制反馈给软件工程师。
因此，相对于C和C++这类系统级语言，企业级的Java程序天然安全很多。
但是语言层面的类型安全机制仍然不能确保数据满足业务契约。
因此，开发者仍然需要自行负责对数据的有效性检查。

在本系统中，关系模式各自实现为一个业务实体类(源码位于@java:package{gms.wargrey.model.entity}包中)，
这些类负责各自数据的内存布局、导入导出时的类型检查。

@tamer-java-class['GMSEntity "业务实体类" "model/GMSEntity.java"]

@tamer-code-ref{java:GMSEntity}是业务实体类的公共父类@java:type{GMSEntity}。
它规定所有子类都必须覆盖 @java:function{hashCode()} 和 @java:function{toString()} 方法。
前者返回值用作实体主键(该返回值只对单字段主键实体类有意义，多字段主键实体对非模型层代码透明)；
后者用于将该实体对象转化为正确的记录格式以导出到文件。
导入记录时的类型检查由各子类的构造函数负责。
(在真实应用中，这个设计不算靠谱。对于本系统而言，按此思路覆盖
@java:function{hashCode()} 和 @java:function{equals()}
也并未违反这两个函数的设计规范，因此不再增设函数。)

@tamer-java-function['DisciplineEntity "构造函数 DisciplineEntity()" "model/entity/DisciplineEntity.java"]

以@tamer-code-ref{java:DisciplineEntity}的构造函数@java:type{DisciplineEntity()}为例说明实体类对记录的处理惯例。
记录来源有两种可能，一是存储文件，二是用户输入。
后者不包括类型标记，记录的其他数据格式相同，
因此调用该构造函数之前应当指示它跳过类型标识。
记录格式中可以插入不引发歧义的空格；
记录字段的主要数据类型是@java:type{byte}、@java:type{int}、@java:type{String}和@java:type{double}。
对于整数类型的字段，@racketerror{0}表示无效输入；
对于@java:type{String}类型的字段，当其值域是有限集时，
系统会配备相应的枚举类型。
由于Java重度依赖异常处理机制，因此如果系统发现输入数据不合契约，
也直接抛出相应的@java:type{RuntimeException}统一处理。
按理说，对于格式上的错误(从而导致tokens数组下标越界)，
应该给用户一个更人性化的错误信息，
此处就把脏活交给运行时类型检查机制来处理好了，
反正它总是要做这件事的。

@tamer-java-function[#:class 'StudentEntity 'prompt "StudentEntity 输入提示信息" "model/entity/StudentEntity.java"]

最后，所有的实体类同时也提供了一个静态方法@java:function{prompt()}(和
一个可选的@java:function{update_prompt()})，用于帮助用户创建(更新)记录，其格式为“字段名:字段类型”。
如@java:function{StudentEntity.prompt()}(@tamer-code-ref{java:StudentEntity.prompt})。

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-reference[]
