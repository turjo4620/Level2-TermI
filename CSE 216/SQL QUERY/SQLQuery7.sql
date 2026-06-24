-- Find the total score for each country

SELECT 
	country,
	--first_name,
	SUM(score) AS total_score
FROM customers

GROUP BY country --, first_name

-- all columns in the SELECT must be either aggregated or included in GROUP BY
