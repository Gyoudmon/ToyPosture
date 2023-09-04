#lang scribble/base

@require{literacy.rkt}

@handbook-story{系统设计与实现}

对于简单信息管理系统，一旦数据模型确定，软件架构也就基本确定了。
鉴于本系统需要解决实际问题，纯命令行接口和传统的图形用户接口都不符合用户期望，
因此本系统沿用了游戏化概念而设计成了半游戏半应用结构(@tamer-figure-ref{s:mvc})。

@tamer-figure!['s:main "主界面"]{@image["stone/screenshots/main.png" #:scale 0.24]}

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

@tamer-figure!["s:mvc" "系统类图"]{@image["stone/SE/MVC.png" #:scale 0.618]}

此系统类图省略了游戏引擎方面的细节，因而比系统的实际实现要简单很多。
在讨论详细设计之前，先对该类图作些简要说明。

@itemlist[
 #:style 'compact
 
 @item{本系统的开发遵循“接口导向设计”原则。
  @cpp:type{GradeManagementPlane}是主界面类，充当控制器角色。
  除了分派系统事件和用户事件外，还通过实现@cpp:type{IModelListener}
  和@cpp:type{IMenuListener}两个接口响应业务事件。【源码文件：
  @filepath{GradeManagementSystem.cpp}(也即程序入口文件)】}
 
 @item{主界面上组合了一系列视图对象，在我的游戏引擎语义下，
  视图对象可以分为图元(@cpp:type{IGraphlet})和精灵(@cpp:type{ISprite})两大类。
  具体的视图对象实现各自的接口，代表实体对象的视图对象不依赖实体对象，仅共享主键信息。
  【源码目录：@filepath{digitama/IMS/view}】
  @linebreak[]@linebreak[]
  在实现细节上，菜单对象(和未在图中画出的虚拟形象集对象)是一类特殊的视图，
  通过实现访问者模式将子界面嵌入主界面中，以复用基础视图对象，
  在此不做详述。【源码文件：
  @filepath{digitama/IMS/menu.cpp}、
  @filepath{digitama/IMS/avatar.cpp}】}
 
 @item{业务模型对象(@cpp:type{GMSModel})组合了一系列实体对象(@cpp:type{GMSEntity})。
  前者负责组织和操作后者(数据结构和业务算法)，后者负责对自己数据的保存和加载。【源码文件：
  @filepath{digitama/IMS/model.cpp}】}
 
 ]

图中省略号省掉的是该位置处的对等类。
比如，与@cpp:type{StudentSprite}对等的类有@cpp:type{DisciplineSprite}、@cpp:type{DoorSprite}等；
与@cpp:type{SeatEntity}对等的类有@cpp:type{ClassEntity}、@cpp:type{GradeEntity}等；
与@cpp:type{ToplevelMenu}对等的类有@cpp:type{DisciplineMenu}、@cpp:type{GradeMenu}等。

@handbook-scenario[#:tag "dml"]{实体操作语义}

本系统中的所有实体对象，除座位(@cpp:type{SeatEntity})是纯鼠标操作外，
其他实体对象以命令行风格的菜单操作为主。其详细接口见@tamer-code-ref{cpp:ems}。

@tamer-c++['cpp:ems "实体操作语义" "model.hpp" #px"Entity Manipulation"]

@handbook-action{新建(create/register)}

在内存中创建一个实体对象，并且在创建时确保主键不存在，否则创建失败。

@handbook-action{更新(update)}

在内存中更新指定主键的实体对象，主键不存在则更新失败。

@handbook-action{删除(delete)}

在内存中删除指定主键的实体对象，主键不存在则删除失败。
所有删除操作均只删除指定实体本身，不级联或递归，也无法撤销。

@handbook-action{清除(clear)}

在内存中删除游离的实体对象。

对于“主实体已删除、其管理的实体依旧存在”的情况，本系统新增了“清理”语义，
也即 @cpp:function{clear_detached_students} 和 @cpp:function{clear_detached_grades}。
用户自己决定是否通过菜单执行此操作，也算是一种比较弱的出错恢复途径。
比如，用户执行“删除班级”操作时手误错删了别的班，可以直接通过创建班号相同的班来恢复。
但当用户执行“清理游离学生”操作之后，被误删的班级的学生就真的找不回来了。
“清理游离成绩”同理。

@handbook-scenario{用户工作流}

请先回顾@secref{run}了解程序启动选项。

启动程序时如果指定了输入文件，系统会自动加载文件里的记录。
在程序运行期间，用户仍可通过手动执行主菜单的“载入数据”重新加载输入文件里的内容，
但不会弹出文件对话框选择文件。

以@tamer-figure-ref{s:create}为例，班级、课程、学生三个主实体的创建并没有明确的顺序，
即使最先创建学生实体，新生也会排在侧边栏等待分班。
学生创建之后，就可以对其排座位、登记成绩了。

注意窗口底部有一行用户输入区域(vim 风格的输入行)，
输入提示信息包含了@tamer-code-ref{cpp:StudentEntity::prompt}的返回值。

@tamer-figure!['s:create "登记新生信息"]{@image["stone/screenshots/create.png" #:scale 0.20]}

在学生管理界面，侧边栏多了一个“学生形象集”，左侧的编号对应着学生信息中的@tt{avatar:byte}。
每一个编号都对应着两个形象，按目前的设定，这两个形象由系统自动选择。
幼龄形象分配给不超过10岁的学生，少年形象分配给其他学生(@tamer-code-ref{cpp:StudentSprite})。

@tamer-c++['cpp:StudentSprite "学生精灵自动分配形象" "view/studentlet.cpp"
           #px"StudentSprite::StudentSprite" #px"[{][}]" #false]

除成绩实体外，其他实体的更新与创建相似：
进入相应实体的管理菜单、选择“更新”、根据该实体的@cpp:type{update_prompt()}信息
输入新的数据。从数据记录的角度看，“更新”与“创建”的差别在于前者不识别主键。

对成绩实体的操作比较复杂，需要先用鼠标选取目标学生和课程，再输入成绩对应的考试时间，
最后输入成绩。“更新”操作与之相同(@tamer-figure-ref{s:update})。

@tamer-figure!['s:update "修改成绩信息(设置时间戳)"]{@image["stone/screenshots/update.png" #:scale 0.20]}

删除实体较为简单，进入对应菜单执行“删除”操作，输入所需的主键信息即可。
请回顾@secref{dml}了解“删除”和“清理”相关语义。

最后，软件使用过程中对实体所做的修改必须手动执行主菜单的“保存数据”才会导出到指定文件，
无自动保存，也不弹出文件对话框。

@handbook-scenario{数据结构与算法}

本系统的实体对象较少，关系简单，且所有实体的主属性可排序。
因此使用 C++ STL @cpp:type{std::map} 作为数据结构即可满足需求。
实体对象的主属性作为@cpp:type{std::map}实例的键，
实体对象被包装在@cpp:type{std::shared_ptr}里作为@cpp:type{std::map}的值(@tamer-code-ref{cpp:dt})。
不过，对于本系统而言，使用共享指针的必要性并不高，
所有实体对象的归属已经清晰且唯一(注，
与实体关联的视图对象并不依赖实体对象，
它们仅通过主键关联)。

@cpp:variable{GMSModel::scores}的类型定义最为复杂，
实际上只不过是将成绩按照时间戳和课程分别组织在嵌套的@cpp:type{std::map}实例中了。

@tamer-c++['cpp:dt "实体对象管理" "model.hpp" #px"Entity Runtime Organization"]

对实体对象的操作就转化为了对相应@cpp:type{std::map}实例的操作，
因此实现本系统的过程中无需考虑特别细致的算法，而只需关注业务需求：

@itemlist[
 #:style 'compact
 
 @item{查询实体数据，并作简单聚合运算。}
 @item{插入操作不会产生冗余数据，如果主键已存在，则抛出异常，并以合适的消息告知用户。}
 @item{更新和删除操作不递归不级联，如果主键不存在，则抛出异常，并以合适的消息告知用户。}
 @item{插入和删除操作成功，则以事件形式通知主界面添加或删除相应的视图对象。}
 @item{“载入数据”等价于“批量删除/清理所有已有数据”再“批量创建文件里的所有记录”。}
 @item{“清除”等价于“批量删除所有主实体不存在的弱实体”。}
 ]

@handbook-action{数据查询与聚合运算}

本系统的数据不存储在数据库系统中，因此无法搭上 SQL 的便利，
因而数据查询服务只能以@cpp:type{GMSModel}方法的形式按需提供(@tamer-code-ref{cpp:dqa})。
这里我们以比较实用的成绩单功能为例来说明查询服务中的注意事项。

@tamer-c++['cpp:dqa "实体数据查询方法" "model.hpp" #px"Query and Aggregation"]

无论是班级成绩单，还是学生成绩单，获取最近一次成绩的时间戳是前提。
需要注意的是，学生以班级为单位组织，但课程与班级无关，
一个班级里的学生选修课程可能不同，或者因为某种原因没有参加某场考试。
因此，在确定班级成绩单的最近考试时间戳时，需要遍历全部学生，
选取共同的最近日期(@tamer-code-ref{cpp:GMSModel::get_class_latest_timestamp})。
这时就体现出选取“有序容器”@cpp:type{std::map}的便利了，
使用逆向迭代器即可迅速定位待查询的目标。

@tamer-c++-function['GMSModel::get_class_latest_timestamp "获取班级最近成绩时间戳" "model.cpp"]

同样也是因为数据规模不大，在设计实体类的时候，
没有把@cpp:type{StudentEntity}组合进@cpp:type{ClassEntity}，
学生和班级之间的桥梁是@cpp:type{SeatEntity}。

@tamer-c++['cpp:report "计算最近“进步分”" "../../GradeManagementSystem.cpp" #px"Gradelet::set_score_diffs"]

对于学生成绩单，还需要可视化最近两次考试的“进步分”。
为保持语义一致，@cpp:function{get_class_latest_timestamp}
和@cpp:function{get_student_latest_timestamp}都带有
第二个参数，用以指示“倒数第n个”时间戳。
同样，最近成绩时间戳或次近成绩时间戳，
缺失任何一个则该科成绩都没有“进步分”，
并设为@racketvalfont{NaN}(@tamer-code-ref{cpp:report})。
限于篇幅和排版，@tamer-code-ref{cpp:report}完整代码见
@filepath{GradeManagementSystem.cpp}。

@handbook-action{数据操作(插入、更新、删除)}

这几类数据操作本身没啥特别的。
只是在实现上效仿了观察者模式，当数据操作正常完成时，
会向主界面发送相应的业务事件(@tamer-code-ref{hpp:IModelListener})。
特别地，对于@cpp:type{SeatEntity}和@cpp:type{GradeEntity}这两个非模型层透明的弱实体，
它们的更新与否不以事件方式通知主界面，
而是主界面根据自己的需要通过数据查询服务来拉取最新信息。

@tamer-c++-class['IModelListener "模型事件监听器接口" "model.hpp"]

而当操作失败时，使用异常处理机制通知主界面(@tamer-code-ref{cpp:GMSModel::update_student_from_user_input})。

@tamer-c++-function['GMSModel::update_student_from_user_input "根据用户输入更新学生信息" "model.cpp"]

菜单系统在设计上也采用类似的事件机制。不再详述。

@handbook-action{清除游离数据}

请回顾@secref{dml}了解“删除”和“清理”相关语义。
在实现上，它特别在正确处理嵌套的@cpp:type{std::map}实例，
这是C++特有的陷阱。
这里以“清除游离成绩”为例(@tamer-code-ref{cpp:GMSModel::clear_detached_grades})。

@tamer-c++-function['GMSModel::clear_detached_grades "清除游离成绩" "model.cpp"]

C++ STL 迭代器机制设计的是真的很不优雅。

通过该操作删除的数据不会反映在主界面上，
因此只能借助外部工具来查看运行效果。
@tamer-figure-ref{s:delete}是在删除了1号学生和4号课程之后执行“清除游离成绩”的运行效果。

@tamer-figure!['s:delete "vimdiff 对比执行“清除游离成绩”后的文件"]{@image["stone/screenshots/clear.png" #:scale 0.24]}

@handbook-action{去重和其他}

对于重视类型安全的设计，本系统已经从多个角度确保了模型层里不存在重复数据。
对于多个用户借用其他软件合并存储文件而导致数据重复的情况，
则在被系统加载时就会顺便剔除后出现的记录。
因为初次“加载数据”等价于“批量插入文件里的所有记录”，
每一次“插入”都会接受与用户输入新数据一样的安全性检查。
而且，这个剔除操作不会直接修改数据源文件，
仅当用户手动执行“保存数据”操作时才会将干净的数据写回去。

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-reference[]
