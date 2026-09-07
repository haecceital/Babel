quickSort :: (Ord a) => [a] -> [a]
quickSort []     = []
quickSort (x:xs) = quickSort smaller ++ [x] ++ quickSort larger
    where
        smaller = [y | y <- xs, y <= x]
        larger  = [y | y <- xs, y >  x]
