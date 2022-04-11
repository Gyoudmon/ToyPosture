CREATE PROCEDURE analyse_scores
    @department nvarchar(256),
    @class int,
    @course nvarchar(256),
    @timestamp int,
    @threshold float
AS
    DECLARE @dept_id int, @course_id int;
    DECLARE @total float, @average float, @excellence float, @pass float;
    DECLARE @erate float, @prate float;
    
    SELECT @dept_id = id FROM departments WHERE name = @department;
    SELECT @course_id = id FROM courses WHERE name = @course;

    if (@dept_id is null) set @dept_id = 0;
    if (@course_id is null) set @course_id = 0;

    SELECT @total = total, @average = average FROM average_result_set(@timestamp)
     WHERE dept_id = @dept_id AND class_id = @class AND course_id = @course_id;

    SELECT @excellence = total FROM excellence_result_set(@timestamp, @threshold)
     WHERE dept_id = @dept_id AND class_id = @class AND course_id = @course_id;

    SELECT @pass = total FROM excellence_result_set(@timestamp, 60)
     WHERE dept_id = @dept_id AND class_id = @class AND course_id = @course_id;

    if ((@excellence is null) or (@total is null))
     set @erate = 0.0
    else
     set @erate = @excellence / @total

    if ((@pass is null) or (@total is null))
     set @prate = 0.0
    else
     set @prate = @pass / @total

    select @department as department, @class as class,
     @average as average, (@erate * 100.0) as execllence_rate, (@prate * 100.0) as pass_rate;
