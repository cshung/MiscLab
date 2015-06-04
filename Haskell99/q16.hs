module Q16 where

dropEvery :: [a] -> Int -> [a]
dropEvery p q = case (dropEveryTest p q) of Just x  -> x
                                            Nothing -> error "Dropping index out of range not supported"

dropEveryTest :: [a] -> Int -> Maybe [a]
dropEveryTest p q = 
  let
    check' :: [a] -> Int -> Maybe [a]
    check' p q
      | q <= 0         = Nothing
      | q > (length p) = Nothing
      | otherwise      = Just (dropEveryTest' p q q)
    
    dropEveryTest' :: [a] -> Int -> Int -> [a]
    dropEveryTest' [] _ _ = []
    dropEveryTest' (p:q) r s = if s == 1 then (dropEveryTest' q r r) else p:(dropEveryTest' q r (s-1))
  in
    check' p q