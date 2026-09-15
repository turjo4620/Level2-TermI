-- LONGEST SERVING EMPLOYEE

/*
Write a PL/SQL procedure named LONGEST_SERVING_EMPLOYEE that takes a
REGION_NAME as input and identifies the employee who has been working for the longest time within
that region.
*/

CREATE OR REPLACE PROCEDURE LONGEST_SERVING_EMPLOYEE (
    P_REGION_NAME IN VARCHAR2
)
IS
    V_FULL_NAME       VARCHAR2(100);
    V_JOB_TITLE       JOBS.JOB_TITLE%TYPE;
    V_HIRE_DATE       EMPLOYEES.HIRE_DATE%TYPE;
    V_COUNTRY_NAME    COUNTRIES.COUNTRY_NAME%TYPE;
    V_DEPARTMENT_NAME DEPARTMENTS.DEPARTMENT_NAME%TYPE;
    V_CITY            LOCATIONS.CITY%TYPE;
BEGIN

    SELECT E.FIRST_NAME || ' ' || E.LAST_NAME,
           J.JOB_TITLE,
           E.HIRE_DATE,
           C.COUNTRY_NAME,
           D.DEPARTMENT_NAME,
           L.CITY
    INTO   V_FULL_NAME,
           V_JOB_TITLE,
           V_HIRE_DATE,
           V_COUNTRY_NAME,
           V_DEPARTMENT_NAME,
           V_CITY
    FROM EMPLOYEES E
    JOIN JOBS J
        ON E.JOB_ID = J.JOB_ID
    JOIN DEPARTMENTS D
        ON E.DEPARTMENT_ID = D.DEPARTMENT_ID
    JOIN LOCATIONS L
        ON D.LOCATION_ID = L.LOCATION_ID
    JOIN COUNTRIES C
        ON L.COUNTRY_ID = C.COUNTRY_ID
    JOIN REGIONS R
        ON C.REGION_ID = R.REGION_ID
    WHERE R.REGION_NAME = P_REGION_NAME
    ORDER BY E.HIRE_DATE ASC
    FETCH FIRST 1 ROW ONLY;

    DBMS_OUTPUT.PUT_LINE('Full Name: ' || V_FULL_NAME);
    DBMS_OUTPUT.PUT_LINE('Job Title: ' || V_JOB_TITLE);
    DBMS_OUTPUT.PUT_LINE('Hire Date: ' || V_HIRE_DATE);
    DBMS_OUTPUT.PUT_LINE('Country: ' || V_COUNTRY_NAME);
    DBMS_OUTPUT.PUT_LINE('Department: ' || V_DEPARTMENT_NAME);
    DBMS_OUTPUT.PUT_LINE('City: ' || V_CITY);

EXCEPTION
    WHEN NO_DATA_FOUND THEN
        DBMS_OUTPUT.PUT_LINE(
            'No employee found in region ' || P_REGION_NAME
        );

    WHEN TOO_MANY_ROWS THEN
        DBMS_OUTPUT.PUT_LINE('Multiple employees found.');

    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Error: ' || SQLERRM);
END;
/

/*
Write a procedure named RANK_JOBS in Oracle HR schema. The procedure should rank jobs based on
the following criteria:
1. Number of Employees: Jobs should be ranked in descending order of their number of
employees. The job with the highest number of employees should be ranked as 1.
2. Average Salary: If multiple jobs have the same number of employees, the ranking should be
determined based on the average salary in that job, in descending order.
*/

CREATE OR REPLACE PROCEDURE RANK_JOBS
IS
    V_RANK NUMBER := 0;

BEGIN

    FOR R IN (
        SELECT J.JOB_TITLE,
               COUNT(E.EMPLOYEE_ID) AS TOTAL_EMPLOYEES,
               AVG(E.SALARY) AS AVG_SALARY,
               MAX(E.SALARY) AS MAX_SALARY,
               MIN(E.SALARY) AS MIN_SALARY
        FROM JOBS J
        LEFT JOIN EMPLOYEES E
            ON J.JOB_ID = E.JOB_ID
        GROUP BY J.JOB_ID, J.JOB_TITLE
        ORDER BY COUNT(E.EMPLOYEE_ID) DESC,
                 AVG(E.SALARY) DESC
    )
    LOOP

        V_RANK := V_RANK + 1;

        DBMS_OUTPUT.PUT_LINE(
            'Rank: ' || V_RANK ||
            ' | Job: ' || R.JOB_TITLE ||
            ' | Employees: ' || R.TOTAL_EMPLOYEES ||
            ' | Average Salary: ' || ROUND(R.AVG_SALARY, 2) ||
            ' | Maximum Salary: ' || R.MAX_SALARY ||
            ' | Minimum Salary: ' || R.MIN_SALARY
        );

    END LOOP;

END;
/



/*
Question 3.
Create a trigger that activates when an employee leaves the job (i.e., when a DELETE operation is
performed on the Employee table).
1 new table: Leaves
(fields:
employee_id,
employee_working_instead_of_him/her,
current date
)
Conditions:
1. If that employee has a manager, his/her work should be done by the employee who has the same
manager and has the closest salary to him\her.
2. In case the employee is a manager, his/her work is done by a manager who has the closest
subordinate count as him/her.
3. If an employee meets both conditions 1 and 2 (is a manager and has a manager), go for 2.
4. If no substitute employee is found, keep that field null.
No changes in the Job table and the Job_history table are necessary for your ease.

*/

CREATE TABLE LEAVES (
    EMPLOYEE_ID NUMBER,
    EMPLOYEE_WORKING_INSTEAD_OF_HIM_HER NUMBER,
    CURRENT_DATE DATE
);


CREATE OR REPLACE TRIGGER EMPLOYEE_LEAVES
AFTER DELETE ON EMPLOYEES
FOR EACH ROW

DECLARE
    V_SUBSTITUTE NUMBER;
    V_SUBORDINATE_COUNT NUMBER;
BEGIN

    /*
       First check whether the deleted employee was a manager.
    */

    SELECT COUNT(*)
    INTO V_SUBORDINATE_COUNT
    FROM EMPLOYEES
    WHERE MANAGER_ID = :OLD.EMPLOYEE_ID;


    IF V_SUBORDINATE_COUNT > 0 THEN

        /*
           Deleted employee was a manager.
           Find another manager with closest subordinate count.
        */

        BEGIN

            SELECT EMPLOYEE_ID
            INTO V_SUBSTITUTE
            FROM (
                SELECT E.EMPLOYEE_ID,
                       ABS(
                           (SELECT COUNT(*)
                            FROM EMPLOYEES S
                            WHERE S.MANAGER_ID = E.EMPLOYEE_ID)
                           - V_SUBORDINATE_COUNT
                       ) DIFF
                FROM EMPLOYEES E
                WHERE EXISTS (
                    SELECT 1
                    FROM EMPLOYEES S
                    WHERE S.MANAGER_ID = E.EMPLOYEE_ID
                )
                AND E.EMPLOYEE_ID <> :OLD.EMPLOYEE_ID
                ORDER BY DIFF, E.EMPLOYEE_ID
            )
            WHERE ROWNUM = 1;

        EXCEPTION
            WHEN NO_DATA_FOUND THEN
                V_SUBSTITUTE := NULL;
        END;


    ELSIF :OLD.MANAGER_ID IS NOT NULL THEN

        /*
           Employee has a manager.
           Find same-manager employee with closest salary.
        */

        BEGIN

            SELECT EMPLOYEE_ID
            INTO V_SUBSTITUTE
            FROM (
                SELECT EMPLOYEE_ID,
                       ABS(SALARY - :OLD.SALARY) DIFF
                FROM EMPLOYEES
                WHERE MANAGER_ID = :OLD.MANAGER_ID
                AND EMPLOYEE_ID <> :OLD.EMPLOYEE_ID
                ORDER BY DIFF, EMPLOYEE_ID
            )
            WHERE ROWNUM = 1;

        EXCEPTION
            WHEN NO_DATA_FOUND THEN
                V_SUBSTITUTE := NULL;
        END;

    ELSE

        V_SUBSTITUTE := NULL;

    END IF;


    INSERT INTO LEAVES (
        EMPLOYEE_ID,
        EMPLOYEE_WORKING_INSTEAD_OF_HIM_HER,
        CURRENT_DATE
    )
    VALUES (
        :OLD.EMPLOYEE_ID,
        V_SUBSTITUTE,
        SYSDATE
    );

END;
/
