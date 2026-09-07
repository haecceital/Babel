glub :: Ord a => [a] -> [a]
glub []  = []
glub [x] = [x]
glub (x:y:xs)
    | x > y     = y : glub (x:xs)
    | otherwise = x : glub (y:xs)

bubbleSort :: (Ord a, Eq a) => [a] -> [a]
bubbleSort xs
    | xs == xs' = xs
    | otherwise = bubbleSort xs'
    where xs' = glub xs