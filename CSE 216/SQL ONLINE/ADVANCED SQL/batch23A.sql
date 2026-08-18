/*
1. Find job titles in departments where employees have worked more than 5 years on average, but
only include those jobs which have a maximum salary higher than the average max salary across
all jobs.
*/

SELECT DISTINCT J.JOB_TITLE
FROM EMPLOYEES E 
JOIN JOBS J
ON E.JOB_ID = J.JOB_ID
WHERE E.DEPARTMENT_ID IN
(
  SELECT DEPARTMENT_ID 
  FROM EMPLOYEES
  GROUP BY DEPARTMENT_ID
  HAVING AVG(MONTHS_BETWEEN(SYSDATE, HIRE_DATE) / 12) > 5
)
AND J.MAX_SALARY >
(
  SELECT AVG(MAX_SALARY)
  FROM JOBS

);


/*
2. Find employees who earn more than their department's average salary but do NOT work in
departments with more than 5 employees.
*/

SELECT EMPLOYEE_ID
FROM EMPLOYEES E1
WHERE SALARY > 
(
  SELECT AVG(SALARY)
  FROM EMPLOYEES E2
  WHERE E1.DEPARTMENT_ID = E2.DEPARTMENT_ID
) AND E1.DEPARTMENT_ID NOT IN
(
  SELECT P.DEPARTMENT_ID
  FROM EMPLOYEES P
  GROUP BY P.DEPARTMENT_ID
  HAVING COUNT(EMPLOYEE_ID) > 5
);

/*
3. Find employees from the USA who:
   - have a manager (using EXISTS)
   - have no job_history records (using NOT EXISTS)
   - have salary greater than their department average

   Display:
   - full_name
   - salary
   - CASE label:
       'USA Star'  -> salary > 1.4 * department average
       'USA Above' -> otherwise
*/

SELECT 
    E.FIRST_NAME || ' ' || E.LAST_NAME AS FULL_NAME,
    E.SALARY,

    CASE
        WHEN E.SALARY >
        (
            SELECT AVG(E2.SALARY) * 1.4
            FROM EMPLOYEES E2
            WHERE E2.DEPARTMENT_ID = E.DEPARTMENT_ID
        )
        THEN 'USA Star'
        ELSE 'USA Above'
    END AS LABEL

FROM EMPLOYEES E

JOIN DEPARTMENTS D
    ON E.DEPARTMENT_ID = D.DEPARTMENT_ID

JOIN LOCATIONS L
    ON D.LOCATION_ID = L.LOCATION_ID

JOIN COUNTRIES C
    ON L.COUNTRY_ID = C.COUNTRY_ID

WHERE C.COUNTRY_NAME = 'United States of America'

AND EXISTS
(
    SELECT 1
    FROM EMPLOYEES M
    WHERE M.EMPLOYEE_ID = E.MANAGER_ID
)

AND NOT EXISTS
(
    SELECT 1
    FROM JOB_HISTORY JH
    WHERE JH.EMPLOYEE_ID = E.EMPLOYEE_ID
)

AND E.SALARY >
(
    SELECT AVG(E2.SALARY)
    FROM EMPLOYEES E2
    WHERE E2.DEPARTMENT_ID = E.DEPARTMENT_ID
);


/*
4. Write an SQL query to list all departments where every employee earns more than 5000. For each
department, display the department name, the number of employees in that department, and a
column called Salary_Level that uses a CASE statement to show 'Above' if the department’s
average salary is higher than the overall company average salary, or 'Below or Equal' if it is not.
*/


SELECT 
  D.DEPARTMENT_NAME,
  COUNT(E.EMPLOYEE_ID),
  CASE 
    WHEN AVG(E.SALARY) > 
    (
      SELECT AVG(SALARY)
      FROM EMPLOYEES
    )
    THEN 'ABOVE'
    ELSE 'BELOW OR EQUAL'
  END AS SALARY_LEVEL
FROM DEPARTMENTS D 
JOIN EMPLOYEES E
ON D.DEPARTMENT_ID = E.DEPARTMENT_ID
GROUP BY D.DEPARTMENT_NAME
HAVING MIN(E.SALARY) > 5000


/*
5. Write an SQL query to find employees who earn more than the highest salary of at least one other
department. Only include employees whose department has at least 3 employees and who do not
have any records in the JOB_HISTORY table. For each qualifying employee, display the
employee ID, full name, department name, and salary.
*/


SELECT
    E.EMPLOYEE_ID,
    E.FIRST_NAME,
    E.LAST_NAME,
    D.DEPARTMENT_NAME,
    E.SALARY
FROM EMPLOYEES E
JOIN DEPARTMENTS D
ON E.DEPARTMENT_ID = D.DEPARTMENT_ID
WHERE E.SALARY > ANY
(
  SELECT MAX(SALARY)
  FROM EMPLOYEES E2
  WHERE E.DEPARTMENT_ID <> E2.DEPARTMENT_ID
  GROUP BY E2.DEPARTMENT_ID
)
AND E.DEPARTMENT_ID IN 
(
  SELECT E3.DEPARTMENT_ID
  FROM EMPLOYEES E3
  GROUP BY E3.DEPARTMENT_ID
  HAVING (COUNT(E3.EMPLOYEE_ID)) >= 3
)
-- AND E.EMPLOYEE_ID NOT IN
-- (
--   SELECT J.EMPLOYEE_ID
--   FROM JOB_HISTORY J
-- );
AND NOT EXISTS
(
  SELECT 1
  FROM JOB_HISTORY J 
  WHERE J.EMPLOYEE_ID = E.EMPLOYEE_ID
);


