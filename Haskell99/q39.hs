module Q39 where

import Q31

primesR :: Int -> Int -> [Int]
primesR x y = case (tryPrimesR x y) of Left result   -> result
                                       Right message -> error message

tryPrimesR :: Int -> Int -> Either [Int] String
tryPrimesR x y
  | x <= 0    = Right "x <= 0 is not supported"
  | x > y     = Right "x > y is not supported"
  | otherwise = Left (filter (isPrime) [x..y])