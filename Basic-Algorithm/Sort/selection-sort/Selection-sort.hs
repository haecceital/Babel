select :: Ord a => [a] -> (a, [a])
select [x] = (x, [])
select (x:xs)
    | x < minV  = (x, minV : rest)
    | otherwise = (minV, x : rest)
    where (minV, rest) = select xs


selectionSort :: Ord a => [a] -> [a]
selectionSort [] = []
selectionSort xs = minV : selectionSort rest
    where (minV, rest) = select xs