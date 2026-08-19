/*
1. List departments where every employee has at least one job change, and display the department
name and the number of employees. Use a CASE statement to show if the department’s average
salary is ’Above’ or ‘Below or equal to’ the company average.
*/

SELECT 
COUNT(E.EMPLOYEE_ID) NUM_EMPLOYEES,
D.DEPARTMENT_NAME,
CASE
  WHEN AVG(E.SALARY) > 
    (
      SELECT AVG(SALARY)
      FROM EMPLOYEES
    ) THEN 'Above company average'
    ELSE 'Below or equal to the company average'
    END AS "SALARY_STATUS"
FROM EMPLOYEES E
JOIN DEPARTMENTS D
ON D.DEPARTMENT_ID = E.DEPARTMENT_ID
WHERE NOT EXISTS
(
SELECT 1 
FROM EMPLOYEES E2
WHERE E.DEPARTMENT_ID = E2.DEPARTMENT_ID
AND NOT EXISTS
(
  SELECT 1
  FROM JOB_HISTORY J 
  WHERE J.EMPLOYEE_ID = E2.EMPLOYEE_ID
)
)
GROUP BY D.DEPARTMENT_ID, D.DEPARTMENT_NAME;


-- EITATE EKTU JHAMELA ASE
/*
2. Find employees who are either among the top 3 highest-earning employees in their department or
have a job title not held by anyone else in their department, or both. Exclude those who work in a
department with a manager whose first name is 'Steven'. Additionally, print a type for each
employee as either “Top 3 Salary” or “Unique Job” (for employees satisfying both the criteria,
printing either of the types will suffice).
*/

SELECT
    E.FIRST_NAME || ' ' || E.LAST_NAME AS FULL_NAME,
    E.SALARY,
    J.JOB_TITLE,

    CASE
        WHEN
            (
                SELECT COUNT(*)
                FROM HR.EMPLOYEES E2
                WHERE E2.DEPARTMENT_ID = E.DEPARTMENT_ID
                  AND E2.SALARY > E.SALARY
            ) < 3
        THEN 'Top 3 Salary'

        WHEN
            (
                SELECT COUNT(*)
                FROM HR.EMPLOYEES E3
                WHERE E3.DEPARTMENT_ID = E.DEPARTMENT_ID
                  AND E3.JOB_ID = E.JOB_ID
            ) = 1
        THEN 'Unique Job'
    END AS EMPLOYEE_TYPE

FROM HR.EMPLOYEES E

JOIN HR.DEPARTMENTS D
    ON E.DEPARTMENT_ID = D.DEPARTMENT_ID

JOIN HR.JOBS J
    ON E.JOB_ID = J.JOB_ID

WHERE
(
    (
        SELECT COUNT(*)
        FROM HR.EMPLOYEES E2
        WHERE E2.DEPARTMENT_ID = E.DEPARTMENT_ID
          AND E2.SALARY > E.SALARY
    ) < 3

    OR

    (
        SELECT COUNT(*)
        FROM HR.EMPLOYEES E3
        WHERE E3.DEPARTMENT_ID = E.DEPARTMENT_ID
          AND E3.JOB_ID = E.JOB_ID
    ) = 1
)

AND NOT EXISTS
(
    SELECT 1
    FROM HR.EMPLOYEES M
    WHERE M.EMPLOYEE_ID = D.MANAGER_ID
      AND M.FIRST_NAME = 'Steven'
);




/*
3. List the department_name, city, and country_name for departments that have no employees hired
before July 1, 2000, have a manager, and the manager’s job is not titled 'Stock Manager'. Display
only departments that have at least one employee in ascending order of country and descending
order of city. If there still exists a tie, display the lexicographically smaller department_name first.
*/

SELECT D.DEPARTMENT_NAME, L.CITY, C.COUNTRY_NAME
FROM DEPARTMENTS D
JOIN LOCATIONS L 
ON D.LOCATION_ID = L.LOCATION_ID
JOIN COUNTRIES C
ON C.COUNTRY_ID = L.COUNTRY_ID
WHERE 

  EXISTS
    (
      SELECT 1
      FROM EMPLOYEES E1
      WHERE E1.DEPARTMENT_ID = D.DEPARTMENT_ID
    )

  AND NOT EXISTS
    (
      SELECT 1
      FROM EMPLOYEES E2
      WHERE E2.DEPARTMENT_ID = D.DEPARTMENT_ID
      AND E2.HIRE_DATE <  DATE '2000-07-01'
    )
    
  AND D.MANAGER_ID IS NOT NULL
    
  AND EXISTS
  (
    SELECT 1
    FROM EMPLOYEES E3
    JOIN JOBS J 
    ON E3.JOB_ID = J.JOB_ID
    WHERE E3.EMPLOYEE_ID = D.MANAGER_ID 
    AND J.JOB_TITLE <> 'STOCK MANAGER'
  )
  ORDER BY C.COUNTRY_NAME ASC,
           L.CITY DESC,
           D.DEPARTMENT_NAME ASC





/*
4. Find all employees who have a job title with a minimum salary greater than the average minimum
salary of all jobs, and work in departments that have more than 5 employees.
*/

SELECT 
    E.EMPLOYEE_ID,
    E.FIRST_NAME,
    E.LAST_NAME,
    E.JOB_ID,
    J.MIN_SALARY,
    D.DEPARTMENT_NAME
FROM EMPLOYEES E
JOIN JOBS J 
    ON E.JOB_ID = J.JOB_ID
JOIN DEPARTMENTS D 
    ON E.DEPARTMENT_ID = D.DEPARTMENT_ID
WHERE J.MIN_SALARY > (
    SELECT AVG(MIN_SALARY)
    FROM JOBS
)
AND E.DEPARTMENT_ID IN (
    SELECT DEPARTMENT_ID
    FROM EMPLOYEES
    GROUP BY DEPARTMENT_ID
    HAVING COUNT(EMPLOYEE_ID) > 5
);


/*

5. For each department, calculate its total salary expense. Categorize this expense as 'Low' (total
salary ≤ 15000), 'Medium' (total salary > 15000 and ≤ 50000), 'High' (total salary > 50000 and ≤
100000), or 'Very High' (total salary > 100000). Display department_name, total_salary_expense,

and expense_category in descending order of the total salary expense and ascending order of
department_name. Include departments with no employees (their total expense should be 0 and
categorized as 'Low').
*/

SELECT
D.DEPARTMENT_NAME,
NVL(SUM(E.SALARY), 0) AS "TOTAL_SALARY_EXPENSE",
CASE 
  WHEN NVL(SUM(E.SALARY), 0) <= 15000 THEN 'Low'
  WHEN NVL(SUM(E.SALARY), 0) <= 50000 THEN 'Medium'
  WHEN NVL(SUM(E.SALARY), 0) <= 100000 THEN 'High'
  ELSE 'Very High'
  END AS EXPENSE_CATEGORY
FROM EMPLOYEES E
RIGHT JOIN DEPARTMENTS D
ON E.DEPARTMENT_ID = D.DEPARTMENT_ID
GROUP BY D.DEPARTMENT_ID, D.DEPARTMENT_NAME
ORDER BY TOTAL_SALARY_EXPENSE DESC, D.DEPARTMENT_NAME ASC;
