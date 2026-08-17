/*
1. Find all employees who earn more than the salary of at least five distinct employees who work in
a different department but are located in the same city as the first employee's department. Display
the employee_id, full_name, department_name, and salary of these employees, in descending
order of their department_name. If the department name is the same, break the tie by choosing the
employee with more salary and lower employee_id.
*/

SELECT 
  E.EMPLOYEE_ID, 
  E.FIRST_NAME || ' ' || E.LAST_NAME,
  D.DEPARTMENT_NAME,
  E.SALARY
FROM EMPLOYEES E
JOIN DEPARTMENTS D 
ON E.DEPARTMENT_ID = D.DEPARTMENT_ID
JOIN LOCATIONS L 
ON D.LOCATION_ID = L.LOCATION_ID
WHERE 
(
  SELECT COUNT(DISTINCT E1.EMPLOYEE_ID)
  FROM EMPLOYEES E1
  JOIN DEPARTMENTS D1
  ON E1.DEPARTMENT_ID = D1.DEPARTMENT_ID
  JOIN LOCATIONS L1
  ON D1.LOCATION_ID = L1.LOCATION_ID
  WHERE D1.DEPARTMENT_NAME <> D.DEPARTMENT_NAME
  AND L1.CITY = L.CITY
  AND E1.SALARY < E.SALARY
) >= 5
ORDER BY 
  D.DEPARTMENT_NAME DESC,
  E.SALARY DESC,
  E.EMPLOYEE_ID ASC;

/*
2. Find the departments located in ‘Americas’ that have a department manager whose salary is
between 10000 and 15000 (inclusive), and who manages at least two employees. Do not include
the departments which has no manager. Show only the department’s name, the manager’s full
name, salary of the manager, in descending order of the manager’s salary and ascending order of
the department name.
*/

SELECT 
    D.DEPARTMENT_NAME,
    E.FIRST_NAME || ' ' || E.LAST_NAME AS MANAGER_NAME,
    E.SALARY AS MANAGER_SALARY
FROM EMPLOYEES E 
JOIN DEPARTMENTS D 
ON E.EMPLOYEE_ID = D.MANAGER_ID
JOIN LOCATIONS L 
ON D.LOCATION_ID = L.LOCATION_ID
JOIN COUNTRIES C 
ON C.COUNTRY_ID = L.COUNTRY_ID
JOIN REGIONS R
ON R.REGION_ID = C.REGION_ID 
WHERE R.REGION_NAME = 'AMERICAS'
AND D.MANAGER_ID IS NOT NULL
AND E.SALARY BETWEEN 10000 AND 15000
AND (
  SELECT COUNT(*)
  FROM EMPLOYEES E1
  WHERE E1.MANAGER_ID = E.EMPLOYEE_ID 
) >= 2
ORDER BY
  E.SALARY DESC,
  D.DEPARTMENT_NAME ASC;
  
  
/*
3. List departments where at least one employee has held every job title currently assigned to
employees in that department (based on job history). For each such department, display the
department name and a CASE statement indicating whether the department’s average salary is
above or below the company average.
*/

SELECT
    D.DEPARTMENT_NAME,
    CASE
        WHEN AVG(E.SALARY) >
             (SELECT AVG(SALARY)
              FROM EMPLOYEES)
        THEN 'ABOVE'
        ELSE 'BELOW OR EQUAL'
    END AS SALARY_STATUS
FROM EMPLOYEES E
JOIN DEPARTMENTS D
    ON E.DEPARTMENT_ID = D.DEPARTMENT_ID
WHERE EXISTS
(
    SELECT 1
    FROM EMPLOYEES E1
    WHERE E1.DEPARTMENT_ID = D.DEPARTMENT_ID

      AND NOT EXISTS
      (
          SELECT 1
          FROM EMPLOYEES E2
          WHERE E2.DEPARTMENT_ID = D.DEPARTMENT_ID

            AND NOT EXISTS
            (
                SELECT 1
                FROM JOB_HISTORY J
                WHERE J.EMPLOYEE_ID = E1.EMPLOYEE_ID
                  AND J.JOB_ID = E2.JOB_ID
            )
      )
)
GROUP BY
    D.DEPARTMENT_ID,
    D.DEPARTMENT_NAME;
    
    
    
    
    
/*
4. Find employees who are either managers of more than one department (by being the manager of
employees from multiple departments) or who have a salary greater than the average salary of all
employees, or both. Exclude those who work in departments located in 'Seattle'. Additionally,
print a type for each employee as either “Multi-Dept Manager” or “Above Avg Salary” (for employees satisfying both the criteria, printing either of the types will suffice).
*/

SELECT E.EMPLOYEE_ID
FROM EMPLOYEES E 
JOIN DEPARTMENTS D 
ON E.DEPARTMENT_ID = D.DEPARTMENT_ID
JOIN LOCATIONS L 
ON D.LOCATION_ID = L.LOCATION_ID
WHERE 
(
(
  SELECT COUNT(DISTINCT E1.DEPARTMENT_ID)
  FROM EMPLOYEES E1
  WHERE E.EMPLOYEE_ID = E1.MANAGER_ID
) >= 2
OR E.SALARY > (SELECT AVG(SALARY) FROM EMPLOYEES)
)
AND L.CITY <> 'SEATTLE'


/*
5. For each department, show the department_name, employee_id, and salary of the highest-paid
employee(s). If a department has no employees, do not include it in the results. Display the output
in descending order of the salary. If two employees have the same salary, prioritize the one whose
department name is lexicographically larger. If a tie still exists, prioritize the employee with the
lower employee_id.
*/


SELECT D.DEPARTMENT_NAME, E.EMPLOYEE_ID, E.SALARY
FROM EMPLOYEES E 
JOIN DEPARTMENTS D 
ON E.DEPARTMENT_ID = D.DEPARTMENT_ID
WHERE E.SALARY =
(
SELECT MAX(E1.SALARY) 
FROM EMPLOYEES E1
WHERE E.DEPARTMENT_ID = E1.DEPARTMENT_ID
)
ORDER BY 
  E.SALARY DESC,
  D.DEPARTMENT_NAME DESC,
  E.EMPLOYEE_ID ASC;

