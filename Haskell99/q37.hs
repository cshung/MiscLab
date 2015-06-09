module Q37 where

import Q36

phi :: Int -> Int
phi x = case (tryPhi x) of Left result   -> result
                           Right message -> error message

tryPhi :: Int -> Either Int String
tryPhi x =
  let
    power :: Int -> Int -> Int
    power p 0 = 1
    power p 1 = p
    power p m = let s = power p (quot m 2) in if (mod m 2) == 0 then s * s else s * s * p
    
    build :: [(Int, Int)] -> Int
    build result = foldr (*) 1 $ map (\(p,m) -> (p - 1) * (power p (m - 1))) result
  in
    case (tryPrimeFactorsMult x) of Left result   -> Left (build result)
                                    Right message -> Right message