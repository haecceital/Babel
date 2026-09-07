import Data.Array (Array, Ix, listArray, range, (!))


dp :: Ix i => (i, i) -> (Array i v -> i -> v) -> Array i v
dp bound equation = table
    where table = listArray bound [equation table i | i <- range bound]


-- examples ↓

fib :: Int -> Int
fib idx = table ! idx
    where
        eq :: Array Int Int -> Int -> Int
        eq _ 0 = 0
        eq _ 1 = 1
        eq table i = (table ! (i - 1)) + (table ! (i - 2))

        table = dp (0, idx) eq

-- 0/1 knapsack
knapsack :: Int -> [Int] -> [Int] -> Int
knapsack cap weights values = table ! (n, cap)
    where
        n = length weights
        wArr = listArray (1, n) weights :: Array Int Int
        vArr = listArray (1, n) values  :: Array Int Int

        recur :: Array (Int, Int) Int -> (Int, Int) -> Int
        recur _ (0, _) = 0
        recur table (i, j)
            | wArr ! i > j = table ! (i - 1, j)
            | otherwise    = max (table ! (i - 1, j)) (table ! (i - 1, j - wArr ! i) + vArr ! i)

        table = dp ((0, 0), (n, cap)) recur