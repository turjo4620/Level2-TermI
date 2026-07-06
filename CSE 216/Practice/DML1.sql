-- a. Update COMMISSION_PCT value to 0 for those employees who have NULL in that column.
-- b. Update salary of all employees to the maximum salary of the department in which he/she
-- works.
-- c. Update COMMISSION_PCT to 𝑁 times for each employee where 𝑁 is the number of
-- employees he/she manages. When 𝑁 = 0, keep the old value of COMMISSION_PCT column.
-- d. Update the hiring dates of all employees to the first day of the same year. Do not change this
-- for those employees who joined on or after year 2000.


-- UPDATE EMPLOYEES
-- SET COMMISSION_PCT = 0
-- WHERE COMMISSION_PCT IS NULL
-- 
-- SELECT *
-- FROM EMPLOYEES
-- 
-- SELECT
-- MAX(SALARY)
-- FROM EMPLOYEES
-- WHERE DEPARTMENT_ID = 90


-- UPDATE EMPLOYEES main
-- SET SALARY = (
--       SELECT MAX(SALARY)
--       FROM EMPLOYEES ref
--       WHERE ref.DEPARTMENT_ID = main.DEPARTMENT_ID
--       )


-- SELECT 
-- main.EMPLOYEE_ID,
--   (SELECT 
--   COUNT(*)
--   FROM EMPLOYEES ref 
--   WHERE ref.MANAGER_ID = main.EMPLOYEE_ID
--   ) AS count
-- FROM EMPLOYEES main


-- UPDATE EMPLOYEES main
-- SET COMMISSION_PCT = (
--                      SELECT 
--                      COUNT(*)
--                      FROM EMPLOYEES ref
--                      WHERE main.EMPLOYEE_ID = ref.MANAGER_ID
--                      ) * COMMISSION_PCT
-- WHERE (
--        SELECT 
--        COUNT(*)
--        FROM EMPLOYEES ref
--        WHERE main.EMPLOYEE_ID = ref.MANAGER_ID
--        ) > 0

-- DML -> CREATE, INSERT
--        UPDATE -> SET
--        DELETE


-- UPDATE EMPLOYEES 
-- SET HIRE_DATE = TRUNC(HIRE_DATE, 'YYYY')
-- WHERE EXTRACT(YEAR FROM HIRE_DATE) < 2000
