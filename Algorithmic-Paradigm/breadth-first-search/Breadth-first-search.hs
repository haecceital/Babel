import Data.Set (Set, notMember, insert, fromList, toList)
import Data.List (foldl')


nub' :: Eq a => [a] -> [a]
nub' = toList . fromList


bfs :: Ord a => (a -> [a]) -> (a -> s -> s) -> [a] -> (Set a, s) -> (Set a, s)
bfs neighbors whenVisit frontiers (visited, state)
    | null frontiers = (visited, state)
    | otherwise =
        let (visited', state') = foldl' (\acc n -> (insert n (fst acc), whenVisit n (snd acc))) (visited, state) frontiers
            newFrontiers       =  nub' $ concatMap (filter (\n -> notMember n visited') . neighbors) frontiers
        in bfs neighbors whenVisit newFrontiers (visited', state')
