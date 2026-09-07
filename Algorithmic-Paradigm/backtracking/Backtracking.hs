import Data.List (foldl')


backtracking :: Ord a => (s -> a -> [a]) -> (s -> a -> Bool) -> (a -> s -> s) -> (a -> s -> s) -> a -> s -> s
backtracking neighbors isValid preVisit postVisit node state
    | not $ isValid state node = state
    | otherwise =
        let state'  = preVisit node state
            state'' = foldl' (\acc n -> backtracking neighbors isValid preVisit postVisit n acc) state' (neighbors state' node)
        in postVisit node state''
