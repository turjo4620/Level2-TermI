/*
1. Find employees who are either in departments with more than 5 employees or have a job title
with a minimum salary above 10000, or both. Exclude those in departments where the manager
earns less than their department's average.
*/

SELECT *
FROM EMPLOYEES E
JOIN JOBS J
ON E.JOB_ID = J.JOB_ID
WHERE 
(
EXISTS
(
  SELECT 1
  FROM EMPLOYEES E1
  WHERE E.DEPARTMENT_ID = E1.DEPARTMENT_ID -- CORELATION THAKTE HOBE, E ER DEPARTMENT?
  GROUP BY E1.DEPARTMENT_ID
  HAVING COUNT(EMPLOYEE_ID) > 5
)
OR
-- ETO KICHU LAGBEI NAH! JUST
J.MIN_SALARY > 10000
-- J.JOB_TITLE IN 
-- (
--   SELECT JOB_TITLE
--   FROM JOBS 
--   WHERE MIN_SALARY > 10000
-- )
)

AND 
-- E.DEPARTMENT_ID NOT IN
-- NOT EXISTS USE KORA BHALO
NOT EXISTS
(
  SELECT 1
  FROM EMPLOYEES E2 
  JOIN EMPLOYEES M 
  ON M.EMPLOYEE_ID = E2.MANAGER_ID
  WHERE E.DEPARTMENT_ID = E2.DEPARTMENT_ID 
  AND M.SALARY < (
                        SELECT AVG(SALARY)
                        FROM EMPLOYEES E3
                        WHERE E3.DEPARTMENT_ID = M.DEPARTMENT_ID
                   ) 
);

/*
2. For each country, count the number of departments. Display only the country_name and
department_count, in ascending order of the country_name. Include the countries having no
departments, too.
*/

-- BOLSE JE COUNTRY ER KONO DEPARTMENT NAI OR COUNT O ZERO LIKHTE HOBE, COUNT(*) DILE NULL ER COUNT O 1 HOBE!
-- SO LEFT JOIN USE KORE KORTE HOBE

SELECT
C.COUNTRY_NAME,
COUNT(DEPARTMENT_ID) "DEPARTMENT_COUNT"
FROM COUNTRIES C 
LEFT JOIN LOCATIONS L 
ON L.COUNTRY_ID = C.COUNTRY_ID
LEFT JOIN DEPARTMENTS D
ON L.LOCATION_ID = D.LOCATION_ID
GROUP BY C.COUNTRY_NAME
ORDER BY C.COUNTRY_NAME ASC;


/*
3. For each department, find the employee_id, full_name, salary, department_name, and job title of
the second-highest-paid employee(s) i.e. employee(s) having the second-highest salary. If a
department has fewer than two employees, do not include it in the results. Display the output in
descending order of the salary. If two employees have the same salary, prioritize the one whose
department name is lexicographically smaller. If a tie still exists, prioritize the employee with the
lower employee_id.
*/

SELECT E.EMPLOYEE_ID, E.FIRST_NAME || ' ' || E.LAST_NAME "FULL_NAME",
E.SALARY, D.DEPARTMENT_NAME, J.JOB_TITLE
FROM EMPLOYEES E 
JOIN DEPARTMENTS D 
ON E.DEPARTMENT_ID = D.DEPARTMENT_ID
JOIN JOBS J
ON E.JOB_ID = J.JOB_ID
-- BHALO KORE DEKHA LAGBE
WHERE E.SALARY = 
(
  SELECT MAX(E1.SALARY)
  FROM EMPLOYEES E1
  WHERE E.DEPARTMENT_ID = E1.DEPARTMENT_ID
  AND E1.SALARY < 
  (
    SELECT MAX(E2.SALARY)
    FROM EMPLOYEES E2
    WHERE E1.DEPARTMENT_ID = E2.DEPARTMENT_ID
  )
)
AND EXISTS
(
  SELECT 1
  FROM EMPLOYEES E3
  WHERE E3.DEPARTMENT_ID = E.DEPARTMENT_ID
  GROUP BY E3.DEPARTMENT_ID
  HAVING (COUNT(E3.EMPLOYEE_ID)) >= 2
)
ORDER BY E.SALARY DESC,
D.DEPARTMENT_NAME ASC,
E.EMPLOYEE_ID ASC;



/*
4. Find the employee_id, first_name, and salary of employees in descending order of the salary and
ascending order of the employee ID, who meet exactly one of the following two conditions:
a. They report to a manager whose salary is greater than 15000.
b. They work in a department located in 'Seattle'.
*/

SELECT
    E.EMPLOYEE_ID,
    E.FIRST_NAME,
    E.SALARY

FROM EMPLOYEES E

JOIN DEPARTMENTS D
    ON E.DEPARTMENT_ID = D.DEPARTMENT_ID

LEFT JOIN EMPLOYEES M
    ON M.EMPLOYEE_ID = E.MANAGER_ID

JOIN LOCATIONS L
    ON L.LOCATION_ID = D.LOCATION_ID

WHERE
(
    (M.SALARY > 15000 AND L.CITY <> 'Seattle')

    OR

    (M.SALARY <= 15000 AND L.CITY = 'Seattle')
)

ORDER BY
    E.SALARY DESC,
    E.EMPLOYEE_ID ASC;


/*
5. Find employees (first and last name), their departments, and salary, for those who earn more than
the average salary in their own department. Only consider departments where there is at least one
employee earning less than the company average salary and at least one earning more than the
company average salary. Use a CASE statement to categorize salary as 'High' (if above 10,000),
'Medium' (if between 5,000 and 10,000), or 'Low' (if below 5,000).
*/

SELECT E.FIRST_NAME, E.LAST_NAME, E.DEPARTMENT_ID, E.SALARY,
CASE
  WHEN E.SALARY > 10000 THEN 'HIGH'
  WHEN E.SALARY BETWEEN 5000 AND 10000 THEN 'MEDIUM'
  WHEN E.SALARY < 5000 THEN 'LOW'
  END AS 
   LABEL
FROM EMPLOYEES E
WHERE E.SALARY > (
                 SELECT AVG(SALARY)
                 FROM EMPLOYEES E1
                 WHERE E1.DEPARTMENT_ID = E.DEPARTMENT_ID
                 )
AND ((SELECT AVG(SALARY) FROM EMPLOYEES) > ANY (SELECT E2.SALARY FROM EMPLOYEES E2 WHERE E.DEPARTMENT_ID = E2.DEPARTMENT_ID))
AND ((SELECT AVG(SALARY) FROM EMPLOYEES) < ANY (SELECT E2.SALARY FROM EMPLOYEES E2 WHERE E.DEPARTMENT_ID = E2.DEPARTMENT_ID));
