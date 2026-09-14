-- DBMS OUTPUT Hello world!
-- DECLARE, BEGIN, EXCEPTION, END;
BEGIN
  DBMS_OUTPUT.PUT_LINE ('Hello world');
END;
DECLARE
ENAME VARCHAR2 (100);

--===================================================
BEGIN
  SELECT
    (FIRST_NAME || ' ' || LAST_NAME) INTO ENAME
  FROM
    EMPLOYEES
  WHERE
    EMPLOYEE_ID = 100;
  DBMS_OUTPUT.PUT_LINE ('The name is : ' || ENAME);
END;
--===================================================
-- USE of SQL functions
DECLARE
JDATE DATE;
MONTHS NUMBER;
RMONTHS NUMBER;
BEGIN
  SELECT
    HIRE_DATE INTO JDATE
  FROM
    EMPLOYEES
  WHERE
    EMPLOYEE_ID = 100;
  MONTHS := MONTHS_BETWEEN(SYSDATE, JDATE);
  RMONTHS := ROUND(MONTHS, 0);
  DBMS_OUTPUT.PUT_LINE ('The employee worked ' || RMONTHS || ' months.');
END;
--===================================================
-- IF ELSE COMMAND
DECLARE
JDATE DATE;
YEARS NUMBER;
BEGIN
  SELECT
    HIRE_DATE INTO JDATE
  FROM
    EMPLOYEES
  WHERE
    EMPLOYEE_ID = 100;
  YEARS := (MONTHS_BETWEEN(JDATE, SYSDATE)) / 12;
  IF YEARS >= 10 THEN
    DBMS_OUTPUT.PUT_LINE ('10 years or more!');
  ELSE
    DBMS_OUTPUT.PUT_LINE ('Less than 10 years!');
  END IF;
END;
--===================================================
-- IF ELSE FOR grade level of employee
DECLARE
ESALARY NUMBER;
BEGIN
  SELECT
    SALARY INTO ESALARY
  FROM
    EMPLOYEES
  WHERE
    EMPLOYEE_ID = 100;
  IF ESALARY < 1000 THEN
    DBMS_OUTPUT.PUT_LINE ('Job grade is D');
    ELSIF ESALARY >= 1000
    AND ESALARY < 2000 THEN
    DBMS_OUTPUT.PUT_LINE ('Job grade is C');
    ELSIF ESALARY >= 2000
    AND ESALARY < 3000 THEN
    DBMS_OUTPUT.PUT_LINE ('Job grade is B');
    ELSIF ESALARY >= 3000
    AND ESALARY < 5000 THEN
    DBMS_OUTPUT.PUT_LINE ('Job grade is A');
  ELSE
    DBMS_OUTPUT.PUT_LINE ('Job grade is A+');
  END IF;
END;
--===================================================
-- EXCEPTION HANDLING
DECLARE
JDATE DATE;
YEARS NUMBER;
BEGIN
  SELECT
    HIRE_DATE INTO JDATE
  FROM
    EMPLOYEES
  WHERE
    EMPLOYEE_ID = 10000;
  YEARS := (MONTHS_BETWEEN(SYSDATE, JDATE)) / 12;
  IF YEARS > 10 THEN
    DBMS_OUTPUT.PUT_LINE ('10 MORE');
  ELSE
    DBMS_OUTPUT.PUT_LINE ('LESS 10');
  END IF;
EXCEPTION
  WHEN NO_DATA_FOUND THEN
    DBMS_OUTPUT.PUT_LINE ('Employee is not present in database.');
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE ('I dont know what happened!');
  END;
  --===================================================
  /*
  b. Write an example PL/SQL block that inserts a new arbitrary row to the COUNTRIES table. The block should handle the exception DUP_VAL_ON_INDEX and OTHERS. Run the block
  for different COUNTRY_ID and observe the cases when above exception occurs
  */
  DECLARE
  V_COUNTRY_ID COUNTRIES.COUNTRY_ID % TYPE := '10';
  V_COUNTRY_NAME COUNTRIES.COUNTRY_NAME % TYPE := 'USA';
  V_REGION_ID COUNTRIES.REGION_ID % TYPE := 1;
  BEGIN
    INSERT INTO COUNTRIES (COUNTRY_ID, COUNTRY_NAME, REGION_ID)
    VALUES
    (V_COUNTRY_ID, V_COUNTRY_NAME, V_REGION_ID);
    DBMS_OUTPUT.PUT_LINE ('SUCCESS');
  EXCEPTION
    WHEN DUP_VAL_ON_INDEX THEN
      DBMS_OUTPUT.PUT_LINE ('Error: COUNTRY_ID already exists.');
    WHEN OTHERS THEN
      DBMS_OUTPUT.PUT_LINE ('Other error occurred: ' || SQLERRM);
    END;
    
    -- LOOPS IN PL SQL
    DECLARE
    BEGIN
      FOR i IN 1..100
        LOOP
        DBMS_OUTPUT.PUT_LINE (i);
      END LOOP;
    END;
    
    --===================================================
    DECLARE
    I NUMBER;
    BEGIN
      I := 1;
      WHILE I <= 100
      LOOP
      DBMS_OUTPUT.PUT_LINE (I);
      I := I + 1;
    END LOOP;
  END;
  
  --===================================================
  DECLARE
  i NUMBER;
  BEGIN
    --this is an unconditional loop, must have EXIT WHEN inside loop
    i := 1;
    LOOP
    DBMS_OUTPUT.PUT_LINE (i);
    i := i + 1;
    EXIT
    WHEN (i > 100);
    END LOOP;
  END;
  
  --===================================================
  -- counts the number of employees who worked 10 years or more
  DECLARE
  YEARS NUMBER;
  COUNTER NUMBER;
  BEGIN
    COUNTER := 0;
    FOR R IN (SELECT HIRE_DATE FROM EMPLOYEES)
      LOOP
      YEARS := (MONTHS_BETWEEN(SYSDATE, R.HIRE_DATE) / 12);
      IF YEARS >= 10 THEN
        COUNTER := COUNTER + 1;
      END IF;
    END LOOP;
    DBMS_OUTPUT.PUT_LINE ('Number of employees worked 10 years or more: ' || COUNTER);
  END;
  
  --===================================================
  --increases salary of each employee X by 15% who have worked in the company for 10 years or more.
  DECLARE
  YEARS NUMBER;
  COUNTER NUMBER;
  OLD_SAL NUMBER;
  NEW_SAL NUMBER;
  BEGIN
    COUNTER := 0;
    FOR R IN (SELECT EMPLOYEE_ID, SALARY, HIRE_DATE FROM EMPLOYEES)
      LOOP
      OLD_SAL := R.SALARY;
      YEARS := (MONTHS_BETWEEN(SYSDATE, R.HIRE_DATE) / 12);
      IF YEARS >= 10 THEN
        UPDATE EMPLOYEES
        SET SALARY = SALARY * 1.15
        WHERE
          EMPLOYEE_ID = R.EMPLOYEE_ID;
      END IF;
      SELECT
        SALARY INTO NEW_SAL
      FROM
        EMPLOYEES
      WHERE
        EMPLOYEE_ID = R.EMPLOYEE_ID;
      DBMS_OUTPUT.PUT_LINE ('Employee id:' || R.EMPLOYEE_ID || ' Salary: ' || OLD_SAL || ' -> ' || NEW_SAL);
    END LOOP;
    COMMIT; -- THIS IS IMPORTANT
  END;
  
  /*
  Write a PL/SQL block that will print ‘Happy Anniversary X’ for each employee X whose
  hiring date is today. Use cursor FOR loop for the task
  */
  DECLARE
  HIRE_DAY NUMBER;
  HIRE_MONTH NUMBER;
  BEGIN
    FOR R IN (SELECT EMPLOYEE_ID, HIRE_DATE FROM EMPLOYEES)
      LOOP
      HIRE_DAY := EXTRACT(DAY FROM R.HIRE_DATE);
      HIRE_MONTH := EXTRACT(MONTH FROM R.HIRE_DATE);
      IF HIRE_DAY = EXTRACT(DAY FROM SYSDATE)
        AND HIRE_MONTH = EXTRACT(MONTH FROM SYSDATE) THEN
        DBMS_OUTPUT.PUT_LINE ('HAPPY ANNIVERSARY ' || R.EMPLOYEE_ID);
      END IF;
    END LOOP;
  END;
  
  -- PROCEDURE
  CREATE
  OR REPLACE PROCEDURE IS_SENIOR_EMPLOYEE IS JDATE DATE;
  YEARS NUMBER;
  BEGIN
    SELECT
      HIRE_DATE INTO JDATE
    FROM
      EMPLOYEES
    WHERE
      EMPLOYEE_ID = 100;
    YEARS := (MONTHS_BETWEEN(SYSDATE, JDATE) / 12);
    IF YEARS >= 10 THEN
      DBMS_OUTPUT.PUT_LINE ('The employee worked 10 years or more');
    ELSE
      DBMS_OUTPUT.PUT_LINE ('The employee worked less than 10 years');
    END IF;
  END;
  DECLARE
  BEGIN
    IS_SENIOR_EMPLOYEE;
  END;
  
  --=====================================--
  CREATE
  OR REPLACE PROCEDURE IS_SENIOR_EMPLOYEE (EID IN VARCHAR2) IS JDATE DATE;
  YEARS NUMBER;
  BEGIN
    SELECT
      HIRE_DATE INTO JDATE
    FROM
      EMPLOYEES
    WHERE
      EMPLOYEE_ID = EID;
    YEARS := (MONTHS_BETWEEN(SYSDATE, JDATE) / 12);
    IF YEARS >= 10 THEN
      DBMS_OUTPUT.PUT_LINE ('The employee worked 10 years or more');
    ELSE
      DBMS_OUTPUT.PUT_LINE ('The employee worked less than 10 years');
    END IF;
  END;
  DECLARE
  BEGIN
    IS_SENIOR_EMPLOYEE (100);
    IS_SENIOR_EMPLOYEE (105);
  END;
  --===============================--
  CREATE
  OR REPLACE PROCEDURE IS_SENIOR_EMPLOYEE (EID IN VARCHAR2, MSG OUT VARCHAR2) IS JDATE DATE;
  YEARS NUMBER;
  BEGIN
    SELECT
      HIRE_DATE INTO JDATE
    FROM
      EMPLOYEES
    WHERE
      EMPLOYEE_ID = EID;
    YEARS := (MONTHS_BETWEEN(SYSDATE, JDATE) / 12);
    IF YEARS >= 10 THEN
      MSG := 'The employee worked 10 years or more';
    ELSE
      MSG := 'The employee worked less than 10 years';
    END IF;
  EXCEPTION
    WHEN NO_DATA_FOUND THEN
      MSG := 'No employee found.';
    WHEN TOO_MANY_ROWS THEN
      MSG := 'More than one employee found.';
    WHEN OTHERS THEN
      MSG := 'Some unknown error occurred.';
    END;
    DECLARE
    MESSAGE VARCHAR2 (100);
    BEGIN
      IS_SENIOR_EMPLOYEE (100000, MESSAGE);
      DBMS_OUTPUT.PUT_LINE (MESSAGE);
      IS_SENIOR_EMPLOYEE (105, MESSAGE);
      DBMS_OUTPUT.PUT_LINE (MESSAGE);
    END;
    
    --===================================
    -- SQL FUNCTIONS
    CREATE
    OR REPLACE FUNCTION GET_SENIOR_EMPLOYEE (EID IN VARCHAR2) RETURN VARCHAR2 IS JDATE DATE;
    YEARS NUMBER;
    MSG VARCHAR2 (100);
    BEGIN
      SELECT
        HIRE_DATE INTO JDATE
      FROM
        EMPLOYEES
      WHERE
        EMPLOYEE_ID = EID;
      YEARS := (MONTHS_BETWEEN(SYSDATE, JDATE) / 12);
      IF YEARS >= 10 THEN
        MSG := 'The employee worked 10 years or more';
      ELSE
        MSG := 'The employee worked less than 10 years';
      END IF;
      RETURN MSG; --return the message
    EXCEPTION
      --you must return value from this section also
      WHEN NO_DATA_FOUND THEN
        RETURN 'No employee found.';
      WHEN TOO_MANY_ROWS THEN
        RETURN 'More than one employee found.';
      WHEN OTHERS THEN
        RETURN 'Some unknown error occurred.';
      END;
      DECLARE
      MESSAGE VARCHAR2 (100);
      BEGIN
        MESSAGE := GET_SENIOR_EMPLOYEE (10000);
        DBMS_OUTPUT.PUT_LINE (MESSAGE);
        MESSAGE := GET_SENIOR_EMPLOYEE (105);
        DBMS_OUTPUT.PUT_LINE (MESSAGE);
      END;