greedy :: (s -> [a] -> (Maybe a, [a])) -> (a -> s -> s) -> ([a], s) -> ([a], s)
greedy chooseBest apply (options, state) =
    case chooseBest state options of
        (Nothing, remaining)     -> (remaining, state) 
        (Just chosen, remaining) -> greedy chooseBest apply (remaining, state')
            where state' = apply chosen state