mergeHelper :: Ord a => [a] -> [a] -> [a]
mergeHelper xs [] = xs
mergeHelper [] ys = ys
mergeHelper (x:xs) (y:ys)
    | x < y     = x : mergeHelper xs (y:ys)
    | otherwise = y : mergeHelper (x:xs) ys


mergeSort :: Ord a => [a] -> [a]
mergeSort []  = []
mergeSort [x] = [x]
mergeSort xs  = mergeHelper (mergeSort left) (mergeSort right)
    where (left, right) = splitAt (length xs `div` 2) xs