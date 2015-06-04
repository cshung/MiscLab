module Q15 where

repli :: [a] -> Int -> [a]
repli p q = case (repliTest p q) of Just x  -> x
                                    Nothing -> error "Repeat < 0 times is not supported"

repliTest :: [a] -> Int -> Maybe [a]
repliTest p q =
  let
    check' :: [a] -> Int -> Maybe [a]
    check' p q
      | q < 0     = Nothing
      | otherwise = Just (repli' p q q)

    repli' :: [a] -> Int -> Int -> [a]
    repli' [] _ _ = []
    repli' (p:q) r s = if (s == 0) then (repli' q r r) else p:(repli' (p:q) r (s - 1))
  in
    check' p q