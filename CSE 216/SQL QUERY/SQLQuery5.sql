/* Retrieve all customers
and sort the result by the 
highest score first */

SELECT * 
FROM customers

--ORDER BY score DESC

ORDER BY score ASC -- ASC na bolleo chole (By default ASC)

-- Nested sorting

SELECT * 
FROM customers
ORDER BY
	country ASC,
	score DESC