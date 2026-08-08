/*
1. List managers whose departments have average salaries higher than the overall company average,
for departments located in Toronto and Oxford.
*/

SELECT *
FROM EMPLOYEES E
JOIN DEPARTMENTS D 
ON E.DEPARTMENT_ID = D.DEPARTMENT_ID
JOIN LOCATIONS L 
ON D.LOCATION_ID = L.LOCATION_ID
WHERE E.MANAGER_ID IS NOT NULL
AND L.CITY IN ('Toronto', 'Oxford')
AND E.DEPARTMENT_ID IN 
(
  SELECT E2.DEPARTMENT_ID
  FROM EMPLOYEES E2
  GROUP BY E2.DEPARTMENT_ID
  HAVING AVG(SALARY) > 
  (
    SELECT AVG(SALARY)
    FROM EMPLOYEES
  )
)

/*
2. Find employees who both work in departments with more than 5 employees AND have salaries
greater than the overall average salary across all employees.
*/

SELECT *
FROM EMPLOYEES E 
WHERE E.DEPARTMENT_ID IN 
(
  SELECT E1.DEPARTMENT_ID
  FROM EMPLOYEES E1
  GROUP BY E1.DEPARTMENT_ID
  HAVING COUNT(EMPLOYEE_ID) > 5
)
AND E.SALARY >
(
  SELECT AVG(SALARY)
  FROM EMPLOYEES
)


/*
3. Write a SQL query for employees in departments that have managers, with no job history records,
and salary > dept average. Show full_name, salary, dept_name, and label 'Stable High Earner' if
salary > 1.7 times dept average, else 'Dept Above Avg'.
*/

SELECT 
  E.FIRST_NAME || ' ' || E.LAST_NAME "FULL_NAME",
  E.SALARY,
  D.DEPARTMENT_NAME,
  CASE
    WHEN E.SALARY > 
    (
      SELECT AVG(E2.SALARY) * 1.7
      FROM EMPLOYEES E2
      WHERE E2.DEPARTMENT_ID = E.DEPARTMENT_ID
    ) THEN 'STABLE HIGH EARNER'
    ELSE 'DEPT ABOVE AVG'
    END AS LABEL
FROM EMPLOYEES E 
JOIN DEPARTMENTS D
ON E.DEPARTMENT_ID = D.DEPARTMENT_ID
WHERE E.MANAGER_ID IS NOT NULL
AND E.SALARY >
(
  SELECT AVG(E2.SALARY)
  FROM EMPLOYEES E2
  WHERE E2.DEPARTMENT_ID = E.DEPARTMENT_ID
) 
AND NOT EXISTS
(
  SELECT 1
  FROM JOB_HISTORY J
  WHERE J.EMPLOYEE_ID = E.EMPLOYEE_ID
);


/*
4. Find employees who are either in departments with more than 5 employees or have a job with
minimum salary above 10000.
Display: employee_id, first_name, last_name, department_id, job_id, salary.
*/


SELECT 
  E.EMPLOYEE_ID,
  E.FIRST_NAME,
  E.LAST_NAME,
  E.DEPARTMENT_ID,
  E.JOB_ID,
  E.SALARY
FROM EMPLOYEES E
WHERE EXISTS
(
  SELECT 1
  FROM EMPLOYEES E1
  WHERE E1.DEPARTMENT_ID = E.DEPARTMENT_ID
  GROUP BY E1.DEPARTMENT_ID
  HAVING COUNT(E1.EMPLOYEE_ID) > 5
)
OR E.JOB_ID IN 
(
  SELECT J1.JOB_ID
  FROM JOBS J1
  WHERE MIN_SALARY > 10000
)

/*
5. Write an SQL query to find employees who satisfy exactly one of the following conditions:
(i) they work in a department with more than 5 employees, or
(ii) their job has a minimum salary greater than 10000.
Employees who satisfy both conditions or neither condition must be excluded. Display employee
ID, full name, department ID, job ID, and salary.
*/

-- XOR KORA LAGBE

SELECT
  E.EMPLOYEE_ID,
  E.FIRST_NAME || ' ' || E.LAST_NAME AS FULL_NAME,
  E.DEPARTMENT_ID,
  E.JOB_ID,
  E.SALARY
FROM EMPLOYEES E
WHERE
(
    EXISTS
    (
        SELECT 1
        FROM EMPLOYEES E1
        WHERE E1.DEPARTMENT_ID = E.DEPARTMENT_ID
        GROUP BY E1.DEPARTMENT_ID
        HAVING COUNT(*) > 5
    )
    AND NOT EXISTS
    (
        SELECT 1
        FROM JOBS J
        WHERE J.JOB_ID = E.JOB_ID
        AND J.MIN_SALARY > 10000
    )
)
OR
(
    NOT EXISTS
    (
        SELECT 1
        FROM EMPLOYEES E1
        WHERE E1.DEPARTMENT_ID = E.DEPARTMENT_ID
        GROUP BY E1.DEPARTMENT_ID
        HAVING COUNT(*) > 5
    )
    AND EXISTS
    (
        SELECT 1
        FROM JOBS J
        WHERE J.JOB_ID = E.JOB_ID
        AND J.MIN_SALARY > 10000
    )
);
