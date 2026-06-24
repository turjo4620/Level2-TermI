-- retrieve customers with a score not equal to zero

SELECT *

FROM customers

--WHERE score != 0

--WHERE country != 'Germany'

--WHERE country != 'Germany' or score != 0

WHERE country != 'Germany' and score != 0				