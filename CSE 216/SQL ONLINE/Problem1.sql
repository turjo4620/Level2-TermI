/*
Show the employee ID and last name of employees whose last name contains at
least one pair of identical consecutive characters (e.g., AA, bB, Cc, dd, etc.). Sort by
last name.
*/


SELECT
EMPLOYEE_ID,
LAST_NAME
FROM EMPLOYEES
WHERE LOWER(LAST_NAME) LIKE '%aa%'
      OR LOWER(LAST_NAME) LIKE '%bb%'
      OR LOWER(LAST_NAME) LIKE '%cc%'
ORDER BY LAST_NAME