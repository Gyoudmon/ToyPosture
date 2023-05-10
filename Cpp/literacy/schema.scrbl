#lang scribble/base

@require{literacy.rkt}

@handbook-story{数据模型}

成绩管理系统是一类定义良好的系统，除在具体应用场景某些成绩参数难以量化外，其数据模型本身比较清晰明了。

@handbook-scenario{概念模型设计}

本系统包含班级、学生、课程三个实体，按照惯例，主属性带下划线：

@itemlist[
 @item{班级：@pk{班级编号}}
 @item{学生：@pk{学号}，形象，昵称，性别，年龄}
 @item{课程：@pk{课程代号}，课程名称，学分}
]

实体之间具有如下关系：

@itemlist[
 @item{一个班级管理多个学生}
 @item{一个学生选修多门课程，一门课程可以被多个同学选修}
 @item{班级通过分配座位“管理”学生，该关系包含属性“课桌号”和课桌的“座位号”}
 @item{学生通过参与考试“选修”课程，该关系包含属性“考试时间”和“考试成绩”}
]

可得如下实体关系图：
@tamer-figure!["erd" "实体-关系图"]{@image["stone/SE/ER.png" #:scale 0.75]}

@handbook-scenario{逻辑模型设计}

按如下思路将前述概念模型转化为逻辑模型：

@itemlist[
 @item{一个实体型转换为一个关系模式，实体属性对应关系属性}
 @item{一对多联系转化为一个关系模式，其主键为多端主属性，联系属性转化为关系属性}
 @item{多对多联系转化为一个关系模式，联系属性转化为关系属性，且关系主键是各端实体及联系主键的组合}
]

于是得到如下基本关系模式，主键依旧带下划线：

@itemlist[
 @item{Class(@pk{clsId})}
 @item{Discipline(@pk{disCode}, name, credit)}
 @item{Student(@pk{sNo}, avatar, nickname, gender, age)}
 @item{Seat(@pk{sNo}, clsId, desk, seat)}
 @item{Grade(@pk{sNo}, @pk{disCode}, @pk{timestamp}, scores)}
]

且很容易论证，在现有属性集中，该关系模型已经满足3NF（及BCNF），因此，无需进一步微调关系模式。

@handbook-scenario{物理模型设计}

本系统的用户主要是STEM教师，就目前重视STEM教育的情况来看，
数据规模不会特别大，因此无需动用数据库管理系统，而将所有数据集保存在单个文件中。
相对于 Excel 这样的电子表格，该存储方案的优势是可直接应用数据库设计范式、无冗余数据。

数据以文本形式按行存储，其格式为：
@centered{@tt{marker:field1,field2, ...}}

简而言之，每一行数据以关系实体类型标记开始，后跟分号(@tt{:})，
字段以逗号(@tt{，})分隔，逗号两边若存在空格则忽略。例如：@linebreak[]

@tabular[#:style 'boxed
         (list (list @tt{c:700})
               (list @tt{d:1,Mathematics,6})
               (list @tt{s:0,6103,txm,Girl,12})
               (list @tt{sc:4,501,2,6})
               (list @tt{sdt:2,1,20220630,9.0 14.0 18.0 23.0 13.0 3.0}))]

其中，类型标记以字符为单位，多字符类型可用于提示组合主键及其存储循序。
特别需要说明的是，学生@bold{单科单次}考试的成绩仍被设计成了列表类型，以空格分隔。
这个设计的用意主要有两个：对于义务教育中的科学课，考试成绩按题型登记，减少教师算分出错的可能；
对于STEM教育，成绩可能跟考试无关，而是按多元能力（比如批判性思维、逻辑思维、动手能力等）
指标分别打分。

@handbook-scenario{类型系统与类型安全}

C++ 是静态强类型语言。即，每一个数据都必须显式声明其类型，
并由编译器保证编译过程中的类型正确。
但 C++ 不直接提供运行期的类型检查，
而编译期类型安全并不能确保运行期也安全，
最典型的例子是 @racketerror{nullptr} 引发的段错误。
即使运行期的类型安全得到保证，也仍然不能确保数据满足业务契约。
因此，开发者应当自行负责对数据的有效性检查。

在本系统中，关系模式各自实现为一个业务实体类(源码位于@filepath{digitama/IMS/entity}目录)，
这些类负责各自数据的内存布局、导入导出时的类型检查。

@tamer-c++-class['GMSEntity "业务实体类" "entity/entity.hpp"]

@tamer-code-ref{hpp:GMSEntity}是业务实体类的公共父类@cpp:type{GMSEntity}。
它规定所有子类都必须实现 @cpp:function{primary_key()}
和 @cpp:function{to_string()} 方法。
前者仅对单字段主键实体类有意义(多字段主键实体对非模型层代码透明)；
后者用于将该实体对象转化为正确的记录格式以导出到文件。
导入记录时的类型检查由各子类的构造函数负责，
并在碰到无效输入时直接抛出异常。

@tamer-c++-class['ClassEntity "实体类 ClassEntity" "entity/class.hpp"]

以@tamer-code-ref{hpp:ClassEntity}的@cpp:type{ClassEntity}为例，
它有一种构造方法，用于从记录中导入实体对象(@tamer-code-ref{cpp:ClassEntity::ClassEntity})。
记录来源有两种可能，一是存储文件，二是用户输入。
后者不包括类型标记，记录的其他数据格式相同，
因此调用该构造函数之前应当指示它跳过类型标识。

@tamer-c++-function['ClassEntity::ClassEntity "ClassEntity::ClassEntity" "entity/class.cpp"]

字符串扫描算法由我的游戏引擎提供(@filepath{digitama/big_bang/datum/string.cpp})，
不在此详述。简而言之，记录格式中可以插入不引发歧义的空格；
记录字段的主要数据类型是@cpp:type{byte}、@cpp:type{uint64_t}、@cpp:type{std::string}和@cpp:type{double}。
对于整数类型的字段，@racketerror{0}表示无效输入；
对于@cpp:type{std::string}类型的字段，当其值域是有限集时，
程序会在导入时将其值转化为枚举类型、导出时还原成字符串。
例如@cpp:type{DisciplineEntity}对课程名称的处理(@tamer-code-ref{cpp:DisciplineEntity::name_to_type})：

@tamer-c++-function['DisciplineEntity::name_to_type "课程名称到枚举类型的转化" "entity/discipline.cpp"]

最后，所有的实体类同时也提供了一个静态方法@cpp:function{prompt()}(和
一个可选的@cpp:function{update_prompt()})，用于帮助用户创建(更新)记录，其格式为“字段名:字段类型”。
如@cpp:function{StudentEntity::prompt()}(@tamer-code-ref{cpp:StudentEntity::prompt})。

@tamer-c++-function['StudentEntity::prompt "StudentEntity 输入提示信息" "entity/student.cpp"]

实际运行效果见@tamer-figure-ref{s:create}，窗口底部有一行高的用户输入区域。

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-reference[]
