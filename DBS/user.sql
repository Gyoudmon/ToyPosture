CREATE LOGIN student
    WITH PASSWORD = 's_scores',
         CHECK_POLICY = OFF,
         DEFAULT_DATABASE = dbs_scores;

CREATE LOGIN instructor
    WITH PASSWORD = 'i_scores',
         CHECK_POLICY = OFF,
         DEFAULT_DATABASE = dbs_scores;
    

USE dbs_scores;
CREATE USER student FOR LOGIN student;
GRANT SELECT ON courses TO student;
GRANT SELECT ON scores TO student;

CREATE USER instructor FOR LOGIN instructor;
GRANT SELECT, INSERT, UPDATE, DELETE TO instructor;

SELECT name, type_desc as type FROM sys.database_principals
WHERE type NOT IN ('A', 'G', 'R', 'X')
    AND sid IS NOT null
    AND name != 'guest';

