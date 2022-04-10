USE dbs_scores;

insert into departments(id, name, tel) values (1, '生命科学', '0511');
insert into departments(id, name, tel) values (2, '物质科学', '0512');
insert into departments(id, name, tel) values (3, '空间科学', '0513');
insert into departments(id, name, tel) values (4, '环境科学', '0514');

insert into classes(id, count) values (401, 0);
insert into classes(id, count) values (501, 0);
insert into classes(id, count) values (601, 0);
insert into classes(id, count) values (602, 0);

insert into courses(id, name, period, credit) values (1, '语文', 50, 6);
insert into courses(id, name, period, credit) values (2, '数学', 50, 6);
insert into courses(id, name, period, credit) values (3, '英语', 50, 6);
insert into courses(id, name, period, credit) values (4, '科学', 20, 4);

insert into students(id, name, gender, dept_id, class_id) values (2, 'qht', 'M', 1, 602);
insert into students(id, name, gender, dept_id, class_id) values (7, 'hxl', 'F', 2, 602);
insert into students(id, name, gender, dept_id, class_id) values (11, 'nzt', 'F', 3, 602);
insert into students(id, name, gender, dept_id, class_id) values (16, 'lcy', 'M', 4, 602);
insert into students(id, name, gender, dept_id, class_id) values (18, 'zzy', 'F', 1, 602);
insert into students(id, name, gender, dept_id, class_id) values (21, 'zyf', 'F', 2, 602);
insert into students(id, name, gender, dept_id, class_id) values (24, 'dwh', 'M', 3, 602);
insert into students(id, name, gender, dept_id, class_id) values (25, 'xsl', 'M', 4, 602);

insert into scores(student_id, course_id, timestamp, point, type) values (2, 4, 202201, 90, 'E');
insert into scores(student_id, course_id, timestamp, point, type) values (7, 4, 202201, 91, 'E');
insert into scores(student_id, course_id, timestamp, point, type) values (11, 4, 202201, 95, 'E');
insert into scores(student_id, course_id, timestamp, point, type) values (16, 4, 202201, 89, 'E');
insert into scores(student_id, course_id, timestamp, point, type) values (18, 4, 202201, 96, 'E');
insert into scores(student_id, course_id, timestamp, point, type) values (21, 4, 202201, 96, 'E');
insert into scores(student_id, course_id, timestamp, point, type) values (24, 4, 202201, 70, 'E');
insert into scores(student_id, course_id, timestamp, point, type) values (25, 4, 202201, 64, 'E');

