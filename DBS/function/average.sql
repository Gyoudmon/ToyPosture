CREATE FUNCTION average_result_set(@time bigint)
RETURNS TABLE
AS
RETURN
(
    select students.dept_id, students.class_id, scores.course_id,
        avg(scores.point) as average,
        count(scores.point) as total
    from students inner join scores
    on scores.student_id = students.id
    where scores.timestamp = time
    group by students.dept_id, students.class_id, scores.course_id
);

