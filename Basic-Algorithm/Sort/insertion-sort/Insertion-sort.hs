import Data.STArray (ST, STArray, runSTArray, newListArray, readArray, writeArray)

insertionSortST :: Ord a => [a] -> [a]
insertionSortST xs = elems $ runSTArray $ do
    let n = length xs
    arr <- newListArray (0, n - 1) xs :: ST s (STArray s Int a)
    forM_ [1 .. n - 1] $ \i -> do
        key <- readArray arr i
        let shift j
            | j < 0 = writeArray arr (j + 1) key
            | otherwise = do
                v <- readArray arr j
                if v > key
                    then writeArray arr (j + 1) v >> shift (j - 1)
                    else writeArray arr (j + 1) key
        shift (i - 1)
    return arr