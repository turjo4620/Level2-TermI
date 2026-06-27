/*
Consider your given HR schema. Display the Job ID and the "Salary Gap" (difference
between Maximum and Minimum salary) for jobs where the gap is greater than 3000.
Exclude any jobs where the average salary is less than 6000. Then finally, sort the
result by the Salary Gap in descending order.
*/

SELECT
JOB_ID,
(MAX_SALARY - MIN_SALARY) AS "Salary Gap"
FROM JOBS
WHERE (MAX_SALARY - MIN_SALARY) > 3000 AND ((MIN_SALARY + MAX_SALARY) / 2) > 6000
-- ORDER BY (MAX_SALARY - MIN_SALARY) DESC
ORDER BY "Salary Gap" DESC

