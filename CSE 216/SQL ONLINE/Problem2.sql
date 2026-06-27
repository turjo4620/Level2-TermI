/*
Group employees by their hiring month, using the three-letter format (JAN, FEB, ...,
DEC). For each month, display the number of employees whose last name contains
the letter “a” (case-insensitive). Sort the results chronologically from JAN to DEC.
*/


-- SELECT 
-- EXTRACT(MONTH FROM HIRE_DATE) AS month
-- FROM EMPLOYEES

-- SELECT
-- COUNT(*) AS employee_count,
-- TO_CHAR(HIRE_DATE, 'MON') AS month
-- FROM EMPLOYEES
-- WHERE LAST_NAME LIKE '%a%'
-- GROUP BY 
--   EXTRACT(MONTH FROM HIRE_DATE),
--   TO_CHAR(HIRE_DATE, 'MON')
-- ORDER BY EXTRACT(MONTH FROM HIRE_DATE) ASC


SELECT
TO_CHAR(HIRE_DATE, 'MON'),
COUNT(*) AS employee_count
FROM EMPLOYEES
WHERE LOWER(LAST_NAME) LIKE '%a%'
GROUP BY 
  TO_CHAR(HIRE_DATE, 'MON'),
  EXTRACT(MONTH FROM HIRE_DATE)
ORDER BY EXTRACT(MONTH FROM HIRE_DATE) ASC


