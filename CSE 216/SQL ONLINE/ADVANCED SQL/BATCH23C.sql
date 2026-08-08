/*
1. Find employees earning above their department's average salary in departments with more than 4
employees.
*/


SELECT *
FROM EMPLOYEES E
WHERE EXISTS
(
  SELECT 1
  FROM EMPLOYEES
  GROUP BY DEPARTMENT_ID
  HAVING COUNT(EMPLOYEE_ID) > 4
)
AND E.SALARY > 
(
  SELECT AVG(SALARY)
  FROM EMPLOYEES E1
  WHERE E.DEPARTMENT_ID = E1.DEPARTMENT_ID
)

/*
2. Find employees who either earn more than their manager's salary or have a salary greater than
their department's average salary. Print employee details with the type as either "Higher Than
Manager" or "Above Dept Avg".
*/

SELECT E.EMPLOYEE_ID,
  CASE 
    WHEN E.SALARY >
    (
      SELECT AVG(E2.SALARY)
      FROM EMPLOYEES E2
      WHERE E.DEPARTMENT_ID = E2.DEPARTMENT_ID
    ) THEN 'ABOVE DEPT AVG'
    WHEN E.SALARY > M.SALARY
    THEN 'HIGHER THAN MANAGER'
    END
FROM EMPLOYEES E 
JOIN EMPLOYEES M 
ON E.MANAGER_ID = M.EMPLOYEE_ID
WHERE E.SALARY >
(
  SELECT AVG(E2.SALARY)
  FROM EMPLOYEES E2
  WHERE E.EMPLOYEE_ID = E2.EMPLOYEE_ID
)
OR E.SALARY > M.SALARY


/*
3. Write a SQL query for employees whose salary beats their department average and whose
manager's salary beats the company average. Show full_name, salary, department_name, and
label it 'Dept Top Earner' if salary > 1.5 times dept average, else 'Dept Above Avg'.
*/

SELECT 
E.FIRST_NAME || ' ' || E.LAST_NAME,
E.SALARY,
D.DEPARTMENT_NAME,
  CASE 
    WHEN E.SALARY > 
    (
      SELECT AVG(SALARY) * 1.5
      FROM EMPLOYEES E1
      WHERE E.DEPARTMENT_ID = E1.DEPARTMENT_ID 
    ) THEN 'DEPT TOP EARNER'
    ELSE 'DEPT ABOVE AVG'
    END
FROM EMPLOYEES E 
JOIN EMPLOYEES M 
ON E.MANAGER_ID = M.EMPLOYEE_ID
JOIN DEPARTMENTS D
ON E.DEPARTMENT_ID = D.DEPARTMENT_ID
WHERE E.SALARY > 
(
  SELECT AVG(E1.SALARY)
  FROM EMPLOYEES E1
  WHERE E.DEPARTMENT_ID = E1.DEPARTMENT_ID
) 
AND M.SALARY > 
(
  SELECT AVG(SALARY)
  FROM EMPLOYEES
)


/*
4. Find employee_id, full name, and department name of employees whose department is located in
the same city as their manager’s department.
*/

SELECT E.EMPLOYEE_ID,
E.FIRST_NAME || ' ' || E.LAST_NAME,
D1.DEPARTMENT_NAME
FROM EMPLOYEES E
JOIN DEPARTMENTS D1
ON E.DEPARTMENT_ID = D1.DEPARTMENT_ID
JOIN LOCATIONS L 
ON L.LOCATION_ID = D1.LOCATION_ID
JOIN EMPLOYEES M 
ON E.MANAGER_ID = M.EMPLOYEE_ID
JOIN DEPARTMENTS D2
ON D2.DEPARTMENT_ID = M.DEPARTMENT_ID
WHERE D1.LOCATION_ID = D2.LOCATION_ID


/*
5. Write an SQL query to list all departments that satisfy the following conditions:
(i) every employee in the department earns more than 5000,
(ii) the department has at least one employee with job history, and
(iii) the maximum salary in the department is greater than the overall company average salary.
For each such department, display the department name, number of employees, average salary, and a
column called Salary_Level that shows
● 'Elite' if the department’s average salary is greater than 1.5 times the company average salary,
● 'Above Average' otherwise.
*/


SELECT 
  D.DEPARTMENT_NAME,
  COUNT(EMPLOYEE_ID),
  AVG(SALARY),
  CASE
    WHEN AVG(SALARY) > 
    (
      SELECT AVG(SALARY) * 1.5
      FROM EMPLOYEES
    ) THEN 'ELITE'
    ELSE 'ABOVE AVG'
    END
FROM EMPLOYEES E
JOIN DEPARTMENTS D
ON E.DEPARTMENT_ID = D.DEPARTMENT_ID
GROUP BY D.DEPARTMENT_ID, D.DEPARTMENT_NAME
HAVING MIN(SALARY) > 5000
AND EXISTS
(
    SELECT 1
    FROM JOB_HISTORY J
    JOIN EMPLOYEES E2
    ON J.EMPLOYEE_ID = E2.EMPLOYEE_ID
    WHERE E2.DEPARTMENT_ID = E.DEPARTMENT_ID
)
AND MAX(E.SALARY) > 
(
  SELECT AVG(SALARY)
  FROM EMPLOYEES
);
