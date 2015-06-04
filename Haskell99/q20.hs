module Q20 where

removeAt :: [a] -> Int -> (a,[a])
removeAt p q = case (removeAtTest p q) of Left x  -> x
                                          Right y -> error y

removeAtTest :: [a] -> Int -> Either (a,[a]) String
removeAtTest p q = 
  let
    check :: [a] -> Int -> Either (a,[a]) String
    check p q
      | q < 1          = Right "q < 1 is not supported"
      | q > (length p) = Right "q > length is not supported"
      | otherwise      = Left (removeAtTest' p q)
      
    removeAtTest' :: [a] -> Int -> (a,[a])
    removeAtTest' []    1 = error "This is impossible"
    removeAtTest' (p:q) 1 = (p, q)
    removeAtTest' []    r = error "This is impossible"
    removeAtTest' (p:q) r = 
      let
        (s, t) = removeAt q (r-1)
      in
        (s, p:t)
  in
    check p q