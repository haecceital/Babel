-- this is unfinished

import qualified Data.Map as Map
import Data.List (foldl')

  
unigram :: [String] -> Map.Map String Int
unigram wds = foldl' (\acc word -> Map.insertWith (+) word 1 acc) Map.empty wds


bigram :: [String] -> Map.Map String (Map.Map String Int)
bigram wds =
    let pairs = zip wds (drop 1 wds)
    in foldl' (\acc (cur, nxt) -> Map.insertWith (Map.unionWith (+)) cur (Map.singleton nxt 1) acc) Map.empty pairs


normalize :: Map.Map String Int -> Map.Map String (Map.Map String Int) -> Map.Map String (Map.Map String Double)
normalize unigramMap bigramMap =
    Map.mapWithKey (\cur innerMap ->
        let total = fromIntegral $ Map.findWithDefault 0 cur unigramMap
        in Map.map (\cnt -> fromIntegral cnt / total) innerMap)
    bigramMap