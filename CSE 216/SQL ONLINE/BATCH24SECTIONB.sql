SELECT EMPLOYEE_ID, JOB_ID,
(FIRST_NAME || ' ' || LAST_NAME) FULL_NAME,
SALARY
FROM EMPLOYEES
WHERE FIRST_NAME LIKE 'M%__%k'

----------------------------------------

SELECT  (FIRST_NAME || ' ' || LAST_NAME) FULL_NAME,
EMAIL
FROM EMPLOYEES
WHERE LOWER(FIRST_NAME) LIKE '%e%' 
AND INSTR(LOWER(FIRST_NAME), 'e') =  LENGTH(LAST_NAME) - INSTR(LOWER(LAST_NAME), 'e')  + 1
AND EXTRACT(MONTH FROM HIRE_DATE) BETWEEN 7 AND 12


-------------------------------------------

SELECT DEPARTMENT_ID,
SUM(SALARY) total_qualified_payroll
FROM EMPLOYEES
WHERE EXTRACT(MONTH FROM (ADD_MONTHS(HIRE_DATE, 120))) BETWEEN 1 AND 6
GROUP BY DEPARTMENT_ID



-------------------------------------------


CREATE TABLE projects
(
project_id NUMBER(4),
project_name VARCHAR2(50),
budget NUMBER(10, 2),
start_date DATE,
end_date DATE,
status CHAR(1)
)


ALTER TABLE projects
ADD CONSTRAINT pk
PRIMARY KEY(project_id)

ALTER TABLE projects
ADD CONSTRAINT uk
UNIQUE(project_name)

ALTER TABLE projects
ADD CONSTRAINT check_budget
CHECK(budget > 50000)

ALTER TABLE projects
ADD CONSTRAINT check_status
CHECK(status IN ('N', 'A', 'F'))


ALTER TABLE projects
ADD  manager_name VARCHAR2(40)

SELECT * FROM projects




-------------------------------------------


ALTER TABLE EMPLOYEES
ADD is_hired_in_first_half CHAR(1) CHECK (is_hired_in_first_half IN ('Y', 'N'))

UPDATE EMPLOYEES
SET is_hired_in_first_half = CASE
      WHEN EXTRACT(MONTH FROM HIRE_DATE) BETWEEN 1 AND 6 THEN 'Y'
      ELSE 'N'
      END;
      
ALTER TABLE EMPLOYEES
ADD work_email VARCHAR2(50)

UPDATE EMPLOYEES 
SET work_email = LOWER(FIRST_NAME) || '.' || LOWER(SUBSTR(JOB_ID, 1, 2)) || TO_CHAR(HIRE_DATE, 'YYYY') || '@company.com'



