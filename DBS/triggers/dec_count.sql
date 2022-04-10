CREATE TRIGGER dec_count ON students AFTER DELETE
AS BEGIN
    DECLARE @old_id int;
    SELECT @old_id= DELETED.class_id FROM DELETED;
    UPDATE classes SET count=count-1 WHERE classes.id = @old_id
END

