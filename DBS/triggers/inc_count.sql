CREATE TRIGGER inc_count ON students AFTER INSERT
AS BEGIN
    DECLARE @new_id int;
    SELECT @new_id= INSERTED.class_id FROM INSERTED;
    UPDATE classes SET count=count+1 WHERE classes.id = @new_id
END

