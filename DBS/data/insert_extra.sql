USE dbs_scores;

insert into students(id, name, gender, dept_id, class_id) values (1, 'ck', 'M', 1, 401);
insert into students(id, name, gender, dept_id, class_id) values (3, 'cr', 'F', 2, 401);
insert into students(id, name, gender, dept_id, class_id) values (8, 'lcz', 'M', 3, 401);
insert into students(id, name, gender, dept_id, class_id) values (9, 'frl', 'F', 4, 401);
insert into students(id, name, gender, dept_id, class_id) values (10, 'lzx', 'M', 1, 401);
insert into students(id, name, gender, dept_id, class_id) values (19, 'hhf', 'M', 2, 401);
insert into students(id, name, gender, dept_id, class_id) values (26, 'stc', 'M', 3, 401);
insert into students(id, name, gender, dept_id, class_id) values (27, 'zym', 'F', 4, 401);

insert into students(id, name, gender, dept_id, class_id) values (4, 'jzf', 'M', 1, 501);
insert into students(id, name, gender, dept_id, class_id) values (6, 'zyx', 'F', 2, 501);
insert into students(id, name, gender, dept_id, class_id) values (12, 'cys', 'M', 3, 501);
insert into students(id, name, gender, dept_id, class_id) values (17, 'wby', 'M', 4, 501);
insert into students(id, name, gender, dept_id, class_id) values (23, 'cny', 'F', 1, 501);
insert into students(id, name, gender, dept_id, class_id) values (28, 'hyq', 'M', 2, 501);
insert into students(id, name, gender, dept_id, class_id) values (29, 'yw', 'F', 3, 501);
insert into students(id, name, gender, dept_id, class_id) values (30, 'jj', 'F', 4, 501);

insert into scores(student_id, course_id, timestamp, point, type) values (1, 4, 202201, 69, 'E');
insert into scores(student_id, course_id, timestamp, point, type) values (3, 4, 202201, 84, 'E');
insert into scores(student_id, course_id, timestamp, point, type) values (8, 4, 202201, 80, 'E');
insert into scores(student_id, course_id, timestamp, point, type) values (9, 4, 202201, 42, 'E');
insert into scores(student_id, course_id, timestamp, point, type) values (10, 4, 202201, 73, 'E');
insert into scores(student_id, course_id, timestamp, point, type) values (19, 4, 202201, 56, 'E');
insert into scores(student_id, course_id, timestamp, point, type) values (26, 4, 202201, 74, 'E');
insert into scores(student_id, course_id, timestamp, point, type) values (27, 4, 202201, 83, 'E');

insert into scores(student_id, course_id, timestamp, point, type) values (4, 4, 202201, 67, 'E');
insert into scores(student_id, course_id, timestamp, point, type) values (6, 4, 202201, 68, 'E');
insert into scores(student_id, course_id, timestamp, point, type) values (12, 4, 202201, 63, 'E');
insert into scores(student_id, course_id, timestamp, point, type) values (17, 4, 202201, 72, 'E');
insert into scores(student_id, course_id, timestamp, point, type) values (23, 4, 202201, 53, 'E');
insert into scores(student_id, course_id, timestamp, point, type) values (28, 4, 202201, 91, 'E');
insert into scores(student_id, course_id, timestamp, point, type) values (29, 4, 202201, 76, 'E');
insert into scores(student_id, course_id, timestamp, point, type) values (30, 4, 202201, 94, 'E');

SELECT * FROM classes;

