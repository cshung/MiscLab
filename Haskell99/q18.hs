module Q18 where

slice :: [a] -> Int -> Int -> [a]
slice p q r = case (sliceTest p q r) of Left x  -> x
                                        Right y -> error y
                                        
sliceTest :: [a] -> Int -> Int -> Either [a] String
sliceTest p q r =
  let
    check :: [a] -> Int -> Int -> Either [a] String
    check p q r
      | q > r          = Right "q > r is not supported"      -- this ensure q <= r
      | q < 1          = Right "q < 1 is not supported"      -- this ensure 1 <= q, which also imply 1 <= q <= r
      | r > (length p) = Right "r > length is not supported" -- this ensure r <= length, which also imply 1 <= q <= r <= length
      | otherwise      = Left (sliceTest' p q r)

    sliceTest' :: [a] -> Int -> Int -> [a]
    sliceTest' p     1 r = prefix p r
    sliceTest' []    r s = error "This is impossible"
    sliceTest' (p:q) r s = sliceTest' q (r-1) (s-1)
    
    prefix _     0 = []
    prefix []    r = error "This is impossible"
    prefix (p:q) r = p:(prefix q (r-1))
  in
    check p q r