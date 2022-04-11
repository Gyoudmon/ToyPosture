USE dbs_scores;

CREATE Table departments (
    id int PRIMARY KEY NOT NULL,
    name nvarchar(10) NOT NULL,
    tel varchar(16)
);

CREATE Table classes (
    id int PRIMARY KEY NOT NULL,
    count int,
    monitor_id int,
);

CREATE Table students (
    id int PRIMARY KEY NOT NULL,
    name nvarchar(8) NOT NULL,
    gender char(1),
    address nvarchar(128),
    dept_id int,
    class_id int,

    foreign key (dept_id) references departments(id),
    foreign key (class_id) references classes(id)
);

CREATE Table courses (
    id int PRIMARY KEY NOT NULL,
    name nvarchar(10) NOT NULL,
    period int,
    credit int
);

CREATE Table scores (
    student_id int NOT NULL,
    course_id int NOT NULL,
    timestamp int NOT NULL,
    type char(1),
    point float NOT NULL,

    primary key(student_id, course_id, timestamp),
    foreign key(student_id) references students(id),
    foreign key(course_id) references courses(id)
);

ALTER Table classes
    Add foreign key (monitor_id) references students(id);

SELECT name, database_id, create_date FROM sys.databases;
SELECT name, id, crdate FROM sysobjects WHERE xtype='U';

