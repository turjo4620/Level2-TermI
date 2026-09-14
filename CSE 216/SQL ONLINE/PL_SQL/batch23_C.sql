/*
1. Write a PL/SQL trigger that will enforce the following business rule:
The total salary paid to all employees in any single department must never exceed
$50,000 after any salary modification.
*/

CREATE OR REPLACE TRIGGER SALARY_LIMIT
FOR INSERT OR UPDATE ON EMPLOYEES
COMPOUND TRIGGER

  V_DEPARTMENT_ID EMPLOYEES.DEPARTMENT_ID%TYPE;
  
  AFTER EACH ROW IS 
  BEGIN
    V_DEPARMENT_ID := :NEW.DEPARTMENT_ID;
    
  END AFTER EACH ROW;
  
  AFTER STATEMENT IS 
  
  DECLARE
    V_TOTAL_SALARY NUMBER;
    
  BEGIN 
    SELECT NVL(SUM(SALARY), 0)
    INTO V_TOTAL_SALARY
    FROM EMPLOYEES
    WHERE DEPARTMENT_ID = V_DEPARTMENT_ID;
    
    IF V_TOTAL_SALARY > 50000 THEN
      RAISE_APPLICATION_ERROR(
        -20002;
        'LIMIT EXCEEDED'
      );
    
    END AFTER STATEMENT;
END;
/
  
  
  /*
  2. Write a PL/SQL function that checks whether an employee is currently paid more than
they should be according to their job and years of service.
The function should:
● Take one input parameter: employee ID
● Check if the employee exists in the EMPLOYEES table. If not → return NULL
● Calculate years of service = number of full years since hire date until today (use
FLOOR)
● Apply this simple overpay rule:
○ If years of service ≥ 10 → employee should not earn more than 95% of
maximum salary
○ If years of service ≥ 5 and < 10 → employee should not earn more than
90% of the maximum salary
○ If years of service < 5 → employee should not earn more than 85% of the
maximum salary

● Return:
○ 1 → if the employee is overpaid (salary > the allowed percentage of
maximum salary)
○ 0 → if the employee is within the allowed range
○ -1 → if the employee has no job or the job data is missing
  
  */
  
  CREATE OR REPLACE FUNCTION EMPLOYEE_SALARY_PAY(
    P_EMPLOYEE_ID EMPLOYEES.EMPLOYEE_ID%TYPE
  )
  RETURN NUMBER
    IS 
    YEARS NUMBER;
    JDATE DATE;
    MAX_SALARY EMPLOYEES.SALARY%TYPE;
    SALARY_P EMPLOYEES.SALARY%TYPE;
    JOB_ID_USE EMPLOYEES.JOB_ID%TYPE;
    
  BEGIN
    IF P_EMPLOYEE_ID IS NULL THEN
    RETURN NULL;
    END IF;
    
    BEGIN
    SELECT HIRE_DATE, SALARY, JOB_ID
    INTO JDATE, SALARY_P, JOB_ID_USE
    FROM EMPLOYEES 
    WHERE EMPLOYEE_ID = P_EMPLOYEE_ID;
    
    EXCEPTION
      WHEN NO_DATA_FOUND THEN
        RETURN NULL;
      END;
    IF JOB_ID_USE IS NULL THEN
    RETURN -1;
    END IF;
    
    
    YEARS := FLOOR(MONTHS_BETWEEN(SYSDATE, JDATE) / 12);
    
    BEGIN
    
    SELECT MAX_SALARY
    INTO MAX_SALARY
    FROM JOBS
    WHERE JOB_ID = JOB_ID_USE;
    
    EXCEPTION 
      WHEN NO_DATA_FOUND THEN
      RETURN -1;
    END;
    
    IF MAX_SALARY IS NULL THEN
    RETURN -1;
    END IF;
    
    IF YEARS >= 10 THEN 
      IF SALARY_P > MAX_SALARY * 0.95 THEN
        RETURN 1;
      ELSE
        RETURN 0;
      END IF;
      
    ELSIF YEARS >= 5 THEN
      IF SALARY_P > MAX_SALARY * 0.90 THEN  
        RETURN 1;
      ELSE
        RETURN 0;
      END IF;
      
    ELSE
      IF SALARY_P > MAX_SALARY * 0.85 THEN  
        RETURN 1;
        ELSE RETURN 0;
      END IF;
    END IF;
    END;
    /
    
    

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    