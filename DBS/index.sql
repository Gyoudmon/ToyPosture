USE dbs_scores;

CREATE INDEX idx_student_name ON students(name);
CREATE INDEX idx_course_name ON courses(name);
CREATE INDEX idx_score ON scores(point, timestamp);

