module Q34 where

import Q33

totient :: Int -> Int
totient x = case (tryTotient x) of Left result   -> result
                                   Right message -> error message

tryTotient :: Int -> Either Int String
tryTotient x
  | x <= 0    = Right "Nonpositive integer is not supported"
  | otherwise = Left (foldr (+) 0 $ map (\x -> if x then 1 else 0) $ map (coPrime x) [1..x])