greedy :: (s -> [a] -> (Maybe a, [a])) -> (a -> s -> s) -> ([a], s) -> ([a], s)
greedy chooseBest apply (options, state) =
    case chooseBest state options of
        (Nothing, options')     -> (options', state) 
        (Just chosen, options') -> greedy chooseBest apply (options', state')
            where state' = apply chosen state