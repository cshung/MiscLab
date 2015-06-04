module Q22 where

range :: Int -> Int -> [Int]
range x y = case (rangeTest x y) of Left x  -> x
                                    Right y -> error y

rangeTest :: Int -> Int -> Either [Int] String
rangeTest x y =
  let
    check :: Int -> Int -> Either [Int] String
    check x y
      | y < x     = Right "y < x is not supported"
      | otherwise = Left (rangeTest' x y)

    rangeTest' :: Int -> Int -> [Int]
    rangeTest' x y = if (x == y) then [x] else x:(rangeTest' (x + 1) y)
  in
    check x y