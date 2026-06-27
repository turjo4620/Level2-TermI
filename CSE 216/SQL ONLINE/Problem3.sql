/*
HR wants to maintain employee performance reviews.
Create a table named PERFORMANCE_REVIEW with columns: REVIEW_ID,
EMPLOYEE_ID, REVIEW_YEAR, RATING, REMARKS.
Ensure that:
● Each rating is linked to an employee
● Rating must be between 1 and 5
● Remarks are optional
Insert one record into the table.

*/

CREATE TABLE PERFORMANCE_REVIEW (
    REVIEW_ID INT PRIMARY KEY,
    EMPLOYEE_ID INT NOT NULL,
    REVIEW_YEAR INT,
    RATING INT NOT NULL CHECK (RATING BETWEEN 1 AND 5),
    REMARKS VARCHAR2(200),
    FOREIGN KEY (EMPLOYEE_ID) REFERENCES EMPLOYEES(EMPLOYEE_ID)
);
  
  INSERT INTO PERFORMANCE_REVIEW(REVIEW_ID, EMPLOYEE_ID, REVIEW_YEAR, RATING, REMARKS)
  VALUES(1, 100, 2023, 4, 'Good');
  
  SELECT
  *
  FROM PERFORMANCE_REVIEW