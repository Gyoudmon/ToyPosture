USE dbs_scores;

ALTER TABLE students
  ADD CONSTRAINT chk_gender CHECK (gender in ('F', 'M'));

ALTER TABLE scores
  ADD CONSTRAINT chk_type CHECK (type in ('M', 'E', 'T'));

ALTER TABLE scores
  ADD CONSTRAINT chk_point CHECK (point BETWEEN 0 AND 100);

