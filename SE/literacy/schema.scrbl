#lang scribble/base

@require{literacy.rkt}

@handbook-story{数据库设计}

需求分析和架构设计已经覆盖了数据实体对象，详细设计阶段也已经将数据对象和图元对象分离。
接下来就该按照一定的范式规划数据模型实体了。

@handbook-scenario{概念模型设计}

@tamer-figure["erd" "实体-关系图"]{@image["stone/vsdx/ER.png" #:scale 0.32]}
本系统包含课程、方案、模块、材料、存储空间、材料清单六个实体，按照惯例，主属性带下划线：

@itemlist[
 @item{课程：@pk{课程编号}，课程名}
 @item{方案：@pk{方案编号}}
 @item{模块：@pk{模块编号}}
 @item{材料：@pk{材料编号}，材料名，类型，单价，单价单位，购买链接}
 @item{存储空间：@pk{仓储编号}，仓储名}
 @item{材料清单：@pk{清单编号}，申请日期}
]

实体之间具有如下关系：

@itemlist[
 @item{一节课包含至少一个方案}
 @item{一个方案包含至少一个模块}
 @item{一个模块包含多个材料}
 @item{一种材料可以存储在多个空间里}
 @item{一个存储空间可以存放多种材料}
 @item{一个材料清单包含多种材料}
 @item{一个材料可以多次购买}
 @item{一个材料清单包含多种材料}
 @item{一个材料可以多次购买}
 @item{一个方案对应一个购买清单}
 @item{材料清单和材料通过“购买”关系关联，该关系包含属性“数量”}
 @item{材料和存储空间通过“存储”关系关联，该关系包含属性“数量”、“槽位”和“入库时间”}
]

经冲突处理和优化后可得系统全局E-R图(@tamer-figure-ref{erd})，
碰巧本系统的实体关系本来就很清晰，无太大异议。

@handbook-scenario{逻辑模型设计}

按如下思路将前述概念模型转化为逻辑模型：

@itemlist[
 @item{一个实体型转换为一个关系模式，实体属性对应关系属性}
 @item{一对一联系与其中一端关系模式合并}
 @item{一对多联系与N端关系模式合并}
 @item{多对多联系转化为一个关系模式，且关系主键是各端实体主键的组合}
 @item{多个实体之间的多元联系转化为一个关系模式，且关系主键是各端实体主键的组合}
 @item{主键相同的关系模式可以合并}
]

于是得到如下基本关系模式，主键依旧带下划线：

@itemlist[
 @item{Course(@pk{cid}, cname)}
 @item{Solution(@pk{sid}, @pk{cid}, oid)}
 @item{Module(@pk{mid}, @pk{sid})}
 @item{Apparatus(@pk{aid}, @pk{mid}, aname, type, price, unit, url)}
 @item{Storage(@pk{sid}, sname)}
 @item{Store(@pk{aid}, @pk{sid}, samount, slot, sdate)}
 @item{Order(@pk{oid}, odate)}
 @item{Purchase(@pk{oid}, @pk{aid}, pamount)}
]

且很容易论证，在现有属性集中，该关系模型已经满足3NF（及BCNF），
即，非主属性都不函数传递依赖于候选关键字，且所用函数依赖都包含候选关键字。
因此，无需进一步微调关系模式。

也因为数据模型清晰明了，通常规模也不会特别大，因此本文不再赘述数据库实现考虑。

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-reference[]
