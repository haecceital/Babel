dac :: (a -> [a]) -> ([b] -> b) -> (a -> Bool) -> (a -> b) -> a -> b
dac divide merge isBase solve problem
    | isBase problem = solve problem
    | otherwise = merge solved
        where solved = [dac divide merge isBase solve p | p <- divide problem]


-- example ↓

mergeSort :: Ord a => [a] -> [a]
mergeSort list = dac divide merge isBase solve list
    where
        divide :: [a] -> [[a]]
        divide xs = [fst t, snd t]
            where t = splitAt (length xs `div` 2) xs

        merge :: Ord a => [[a]] -> [a]
        merge [left, right] = mergeHelper left right
            where
                mergeHelper :: Ord a => [a] -> [a] -> [a]
                mergeHelper [] []         = []
                mergeHelper [] (y:ys)     = y : mergeHelper [] ys
                mergeHelper (x:xs) []     = x : mergeHelper xs []
                mergeHelper (x:xs) (y:ys) = if x < y then x : mergeHelper xs (y:ys) else y : mergeHelper (x:xs) ys

        isBase :: [a] -> Bool
        isBase [_] = True
        isBase x   = False

        solve = id :: [a] -> [a]
