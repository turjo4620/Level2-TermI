/* 1. Write a PL/SQL trigger that will enforce the following business rule:
If an employee has a manager, that manager must be in the same department as the
employee. */
CREATE
OR REPLACE TRIGGER CHECK_MANAGER_DEPARTMENT BEFORE INSERT
OR UPDATE OF MANAGER_ID,
DEPARTMENT_ID ON EMPLOYEES FOR EACH ROW
  DECLARE
  V_MANAGER_DEPARTMENT EMPLOYEES.DEPARTMENT_ID % TYPE;
  BEGIN
    IF : NEW.MANAGER_ID IS NOT NULL THEN
      SELECT
        DEPARTMENT_ID INTO V_MANAGER_DEPARTMENT
      FROM
        EMPLOYEES
      WHERE
        EMPLOYEE_ID = : NEW.MANAGER_ID
        IF : NEW.DEPARTMENT_ID <> V_MANAGER_DEPARTMENT THEN
          RAISE_APPLICATION_ERROR (- 20001, 'EMPLOYEE AND MANAGER MUST BE IN THE SAME DEPARTMENT');
        END IF;
    END IF;
  END;
  / CREATE
  OR REPLACE TRIGGER CHECK_MANAGER_DEPARTMENT BEFORE INSERT
  OR UPDATE ON EMPLOYEES FOR EACH ROW
    DECLARE
    V_MANAGER_D
    BEGIN
      IF : NEW.MANAGER_ID IS NOT NULL THEN
        SELECT
          DEPARTMENT_ID INTO V_MANAGER_D
        FROM
          EMPLOYEES
        WHERE
          EMPLOYEE_ID = : NEW.MANAGER_ID;
          IF V_MANAGER_D <> : NEW.DEPARTMENT_ID THEN
            RAISE_APPLICATION_ERROR (- 20001, 'PROBLEM');
          END IF;
      END IF;
    END;
    /
    /*
    2. Create the following table in the HR schema:
    CREATE TABLE SALARY_AUDIT_LOG (
    audit_id NUMBER GENERATED ALWAYS AS IDENTITY
    CONSTRAINT salary_audit_pk PRIMARY KEY,
    employee_id NUMBER
    CONSTRAINT salary_audit_emp_nn NOT NULL,
    old_salary NUMBER(8,2)
    CONSTRAINT salary_audit_old_nn NOT NULL,
    new_salary NUMBER(8,2)
    CONSTRAINT salary_audit_new_nn NOT NULL,
    changed_by VARCHAR2(30)
    CONSTRAINT salary_audit_user_nn NOT NULL,
    change_date DATE
    CONSTRAINT salary_audit_date_nn NOT NULL,
    reason VARCHAR2(200),
    CONSTRAINT salary_audit_emp_fk
    FOREIGN KEY (employee_id)
    REFERENCES employees(employee_id)
    );
    Now, write a PL/SQL function that updates an employee’s salary according to the
    following requirements:
    ● Take four input parameters: employee ID, percentage change, user, and reason. ● The company policy for salary change percentage is:
    ○ Maximum salary increase: 30%
    ○ Maximum salary decrease: 20%
    ● Make proper validation of all inputs
    ● Calculate the new salary and update it in the appropriate table. ● Insert a record into SALARY_AUDIT_LOG
    ● Returns appropriate status message, like ‘Invalid percentage range,’ or ‘Invalid
    employee ID,’ or ‘Salary updated successfully and audit recorded,’ etc. */
    
    CREATE TABLE SALARY_AUDIT_LOG (
    audit_id NUMBER GENERATED ALWAYS AS IDENTITY
        CONSTRAINT salary_audit_pk PRIMARY KEY,

    employee_id NUMBER
        CONSTRAINT salary_audit_emp_nn NOT NULL,

    old_salary NUMBER(8,2)
        CONSTRAINT salary_audit_old_nn NOT NULL,

    new_salary NUMBER(8,2)
        CONSTRAINT salary_audit_new_nn NOT NULL,

    changed_by VARCHAR2(30)
        CONSTRAINT salary_audit_user_nn NOT NULL,

    change_date DATE
        CONSTRAINT salary_audit_date_nn NOT NULL,

    reason VARCHAR2(200),

    CONSTRAINT salary_audit_emp_fk
        FOREIGN KEY (employee_id)
        REFERENCES employees(employee_id)
);


CREATE OR REPLACE FUNCTION SALARY_UPDATE
(
  P_EMPLOYEE_ID IN NUMBER,
  P_PERCENTAGE IN NUMBER,
  P_USER IN VARCHAR2,
  P_REASON IN VARCHAR2
)
RETURN VARCHAR2
IS
  V_OLD_SALARY EMPLOYEES.SALARY%TYPE;
  V_NEW_SALARY EMPLOYEES.SALARY%TYPE;
  

BEGIN
  IF P_EMPLOYEE_ID IS NULL THEN
    RETURN 'INVALID';
  END IF;
  
  IF P_PERCENTAGE IS NULL OR P_PERCENTAGE < -20 OR P_PERCENTAGE > 30
  THEN RETURN 'INVALID';
  END IF;
  
  IF P_USER IS NULL OR LENGTH(TRIM(P_USER)) = 0
  OR LENGTH(P_USER) > 30
  THEN RETURN 'INVALID';
  END IF;
  
  IF P_REASON IS NULL OR
  LENGTH(TRIM(P_REASON)) = 0
  OR LENGTH(P_REASON) > 200
  THEN RETURN 'INVALID';
  END IF;
  
  BEGIN
    SELECT SALARY
    INTO V_OLD_SALARY
    FROM EMPLOYEES 
    WHERE EMPLOYEE_ID = P_EMPLOYEE_ID;
    
    EXCEPTION
      WHEN NO_DATA_FOUND THEN
        RETURN 'INVALID';
      END;
      
      IF V_OLD_SALARY IS NULL
      THEN RETURN 'SALARY IS NULL';
      END IF;
      
      
      V_NEW_SALARY := V_OLD_SALARY * (1 + P_PERCENTAGE / 100);
      
      UPDATE EMPLOYEES
      SET SALARY = V_NEW_SALARY
      WHERE EMPLOYEE_ID = P_EMPLOYEE_ID;
      
      
      INSERT INTO SALARY_AUDIT_LOG
      (
        employee_id,
        old_salary,
        new_salary,
        changed_by,
        change_date,
        reason
      )
      VALUES
      (
        p_employee_id,
        v_old_salary,
        v_new_salary,
        p_user,
        SYSDATE,
        p_reason
      );
      
      RETURN 'SUCCESS';
      
      EXCEPTION
        WHEN OTHERS
        THEN RETURN 'FAILED' || SQLERRM;
END;
/

DECLARE
    v_result VARCHAR2(4000);
BEGIN
    v_result := SALARY_UPDATE(101, 15, 'TURJO', 'Annual performance adjustment');
    DBMS_OUTPUT.PUT_LINE(v_result);
END;
/

SELECT line, position, text 
FROM all_errors 
WHERE name = 'SALARY_UPDATE' 
ORDER BY sequence;