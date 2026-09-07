binarySearchOnAnswer :: ((a, a) -> a) -> (a -> a -> Bool) -> (a, a) -> (a -> Bool) -> a
binarySearchOnAnswer findMid isAdjacent (left, right) check
    | isAdjacent left right = left
    | otherwise = case check mid of
        True  -> binarySearchOnAnswer findMid isAdjacent (mid, right) check
        False -> binarySearchOnAnswer findMid isAdjacent (left, mid) check
    where
        mid = findMid (left, right)


binarySearchOnData :: Ord a => [a] -> a -> Maybe a
binarySearchOnData [] _ = Nothing
binarySearchOnData [x] target
    | x == target = Just x
    | otherwise   = Nothing
binarySearchOnData xs target
    | mid < target = binarySearchOnData (drop (midIdx + 1) xs) target
    | mid > target = binarySearchOnData (take midIdx xs) target
    | otherwise    = Just target
    where
        midIdx = length xs `div` 2
        mid    = xs !! midIdx