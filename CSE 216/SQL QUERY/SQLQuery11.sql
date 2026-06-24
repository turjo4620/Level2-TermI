
-- selecting top 3 rows

SELECT TOP 3
* 
FROM customers

-- selecting top 3 customers with the highest score

SELECT TOP 3 *
FROM customers
ORDER BY score DESC

SELECT TOP 3 *
FROM customers
ORDER BY score ASC