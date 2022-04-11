CREATE PROCEDURE del_student
    @id int
AS
    DELETE FROM scores WHERE student_id = @id;
    DELETE FROM students WHERE id = @id

