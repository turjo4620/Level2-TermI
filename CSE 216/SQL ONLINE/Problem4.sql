/*
Find the employee ID, full name (first name <space> last name), and a generated
"User ID" based on the following conditions:
● The User ID should consist of the first 3 letters of the first name combined with
the last 3 digits of their phone number.
● Filter for employees whose phone number contains at least 10 characters and
works in department 50 or 80.
● Sort the results by the length of their first name in descending order.
You should write a single SQL query to achieve the desired result. No need to
write separate SQL queries.
*/

SELECT
EMPLOYEE_ID,
(FIRST_NAME || ' ' || LAST_NAME) AS "Full Name",
(SUBSTR(FIRST_NAME, 1, 3) || SUBSTR(PHONE_NUMBER, -3)) AS "USER ID"

-- SUBSTR(PHONE_NUMBER, LENGTH(PHONE_NUMBER) - 3 + 1, 3)
FROM EMPLOYEES
WHERE LENGTH(PHONE_NUMBER) >= 10 AND DEPARTMENT_ID IN (50, 80)
ORDER BY LENGTH(FIRST_NAME) DESC
