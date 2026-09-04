import Data.Set (Set, member, insert)
import Data.List (foldl')


dfs :: Ord a => (a -> [a]) -> (a -> s -> s) -> (a -> s -> s) -> a -> (Set a, s) -> (Set a, s)
dfs neighbors preVisit postVisit node (visited, state)
    | member node visited = (visited, state)
    | otherwise =
        let visited'             = insert node visited
            state'               = preVisit node state
            (visited'', state'') = foldl' (\acc n -> dfs neighbors preVisit postVisit n acc) (visited', state') (neighbors node)
        in (visited'', postVisit node state'')
