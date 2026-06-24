/*
Retreive all customers and sort the results
by the country and then by the highest score
*/

SELECT *
FROM customers
ORDER BY
	country ASC,
	score DESC

	-- country ke age likhchi so country er priority beshi
	-- same value (repetition) thaklei nested sorting er meaning ase
