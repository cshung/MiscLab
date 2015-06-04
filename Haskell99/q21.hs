module Q21 where

insertAt :: a -> [a] -> Int -> [a]
insertAt p q r = case (insertAtTest p q r) of Left x  -> x
                                              Right y -> error y

insertAtTest :: a -> [a] -> Int -> Either [a] String
insertAtTest p q r =
  let
    check p q r
      | r < 1              = Right "r < 1 is not supported"
      | r > (length q) + 1 = Right "r > length + 1 is not supported"
      | otherwise          = Left (insertAtTest' p q r)

    insertAtTest' :: a -> [a] -> Int -> [a]
    insertAtTest' p q 1     = p:q
    insertAtTest' _ [] _    = error "This is impossible"
    insertAtTest' p (q:r) s = q:(insertAtTest' p r (s-1))
  in
    check p q r
