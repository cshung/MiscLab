module Q31 where

isPrime :: Int -> Bool 
isPrime x = case (tryIsPrime x) of Left result   -> result
                                   Right message -> error message

tryIsPrime :: Int -> Either Bool String
tryIsPrime x
  | x <= 0    = Right "Nonpositive integer is not supported"
  | x == 1    = Left (True)
  | x == 2    = Left (True) -- need special case as (1 + floor sqrt 2) == 2
  | otherwise = Left (not ((any (\y -> y == 0) (map (\y -> mod x y) [2..(1 + floor (sqrt (fromIntegral x)))]))))
