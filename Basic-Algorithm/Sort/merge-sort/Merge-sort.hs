mergeSsort :: (Ord a) => [a] -> [a]
mergeSsort []  = []
mergeSsort [x] = [x]
mergeSsort xs  = sorted
    where
        (left, right) = splitAt (length xs `div` 2) xs
        left = mergeSsort left
        right = mergeSort right
        sorted = 