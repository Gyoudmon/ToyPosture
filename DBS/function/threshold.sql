CREATE FUNCTION excellence_result_set(@timestamp bigint, @threshold float)
RETURNS TABLE
AS
RETURN
(
    select students.dept_id, students.class_id, scores.course_id, count(scores.point) as total
    from students inner join scores
    on scores.student_id = students.id
    where scores.timestamp = @timestamp and scores.point >= @threshold
    group by students.dept_id, students.class_id, scores.course_id
);
