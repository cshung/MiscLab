module Q36 where

import Q35

primeFactorsMult :: Int -> [(Int, Int)]
primeFactorsMult x = case (tryPrimeFactorsMult x) of Left result   -> result
                                                     Right message -> error message

tryPrimeFactorsMult :: Int -> Either [(Int, Int)] String
tryPrimeFactorsMult x =
  let
    build :: [Int] -> [(Int, Int)]
    build []    = []
    build [p]   = [(p, 1)]
    build (p:q) = let ((r,s):t) = build q in if (p == r) then (r,(s+1)):t else (p,1):(r,s):t
  in
    case (tryPrimeFactors x) of Left result   -> Left (build result)
                                Right message -> Right message