slidingWindow :: (s -> a -> s) -> (s -> a -> s) -> (s -> Bool) -> [a] -> ((i, i), s) -> ((i, i), s)
slidingWindow enter leave isValid list ((start, end), state)
    | start == length list && end == start = ((start, end), state)
    | isValid state && start < length list = slidingWindow enter leave isValid ((start + 1, end), state')
        where state' = enter state $ list !! start
    | otherwise = slidingWindow enter leave isValid ((start, end + 1), state')
        where state' = leave state $ list !! end