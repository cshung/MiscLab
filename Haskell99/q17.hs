module Q17 where

split :: [a] -> Int -> ([a], [a])
split p q = case (splitTest p q) of Just  x -> x
                                    Nothing -> error "Index out of range is not supported"

splitTest :: [a] -> Int -> Maybe ([a], [a])

splitTest p q =
  let
    check :: [a] -> Int -> Maybe ([a], [a])
    check p q
      | q < 0          = Nothing
      | q > (length p) = Nothing
      | otherwise      = Just (splitTest' p q)

    splitTest' :: [a] -> Int -> ([a], [a])
    splitTest' p 0 = ([],p)
    splitTest' [] r = error "This is impossible"
    splitTest' (p:q) r =
      let
        (s, t) = splitTest' q (r - 1)
      in
        (p:s, t)
  in
    check p q