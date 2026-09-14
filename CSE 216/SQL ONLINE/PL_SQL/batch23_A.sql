/* 1. Write a PL/SQL trigger that will enforce the following business rule: No employee’s salary should ever exceed the maximum salary and fall below the
minimum salary defined for their job in the JOBS table. */
CREATE
OR REPLACE TRIGGER CHECK_EMPLOYEE_SALARY BEFORE INSERT
OR UPDATE OF SALARY,
JOB_ID ON EMPLOYEES FOR EACH ROW
  DECLARE
  V_MIN_SALARY JOBS.MIN_SALARY % TYPE;
  V_MAX_SALARY JOBS.MAX_SALARY % TYPE;
  BEGIN
    SELECT
      MIN_SALARY,
      MAX_SALARY INTO V_MIN_SALARY,
      V_MAX_SALARY
    FROM
      JOBS
    WHERE
      JOB_ID = : NEW.JOB_ID;
    IF : NEW.SALARY < V_MIN_SALARY
      OR : NEW.SALARY > V_MAX_SALARY THEN
      RAISE_APPLICATION_ERROR (- 20001, 'SALARY IS OUTSIDE THE RANGE.');
    END IF;
  END;
  UPDATE EMPLOYEES
  SET SALARY = 10
  WHERE
    EMPLOYEE_ID = 100;
    /*
    2. Write a PL/SQL procedure that does the following:
    ● Takes two input values: employee ID and new department ID
    ● If the employee exists and the department is valid:
    ○ Save the current job details into the JOB_HISTORY table before the
    change
    ○ Move the employee to the new department
    ○ Increase their salary by 15%
    ● Set an OUT parameter with a valid message (a short explanation like “Employee
    transferred successfully with 15% salary increase,” or “Employee does not exist.”,
    etc.)
    */
    CREATE
    OR REPLACE PROCEDURE TRANSFER_EMPLOYEE (P_EMPLOYEEID IN NUMBER, P_NEW_DEPARTMENT_ID IN NUMBER, MESSAGE OUT VARCHAR2) IS V_JOB_ID EMPLOYEES.JOB_ID % TYPE;
  V_OLD_DEPARTMENT_ID EMPLOYEES.DEPARTMENT_ID % TYPE;
  V_SALARY EMPLOYEES.SALARY % TYPE;
  V_START_DATE EMPLOYEES.HIRE_DATE % TYPE;
  V_DEPARTMENT_ID DEPARTMENTS.DEPARTMENT_ID % TYPE;
  BEGIN
    -- CHECKING WHETHER EMPLOYEE EXISTS
    BEGIN
      SELECT
        JOB_ID,
        DEPARTMENT_ID,
        SALARY,
        HIRE_DATE INTO V_JOB_ID,
        V_OLD_DEPARTMENT_ID,
        V_SALARY,
        V_START_DATE
      FROM
        EMPLOYEES
      WHERE
        EMPLOYEE_ID = P_EMPLYEE_ID;
    EXCEPTION
      WHEN NO_DATA_FOUND THEN
        P_MESSAGE := 'DOES NOT EXIST';
        RETURN;
      END;
      
      -- DEPARTMENT EXISTS
      BEGIN
        SELECT
          DEPARTMENT_ID INTO V_DEPARTMENT_ID
        FROM
          DEPARTMENTS
        WHERE
          DEPARTMENT_ID = P_NEW_DEPARTMENT_ID;
      EXCEPTION
        WHEN NO_DATA_FOUND THEN
          P_MESSAGE := 'DOES NOT EXIST';
          RETURN;
        END;
        INSERT INTO JOB_HISTORY (EMPLOYEE_ID, START_DATE, END_DATE, JOB_ID, DEPARTMENT_ID)
        VALUES
        (p_employee_id, v_start_date, SYSDATE, v_job_id, v_old_department_id);
        UPDATE EMPLOYEES
        SET SALARY = SALARY * 1.15
        WHERE
          EMPLOYEE_ID = P_EMPLOYEE_ID;
        P_MESSAGE := 'SUCCESS'
      END;