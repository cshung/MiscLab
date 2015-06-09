module Q35 where

primeFactors :: Int -> [Int]
primeFactors x = case (tryPrimeFactors x) of Left result   -> result
                                             Right message -> error message

tryPrimeFactors :: Int -> Either [Int] String
tryPrimeFactors x =
  let
    check :: Int -> Either [Int] String
    check x
      | x <= 0    = Right "Nonpositive integer is not supported"
      | otherwise = Left (tryPrimeFactors' x [2..(1 + floor (sqrt (fromIntegral x)))])

    tryPrimeFactors' :: Int -> [Int] -> [Int]
    tryPrimeFactors' 1 _     = []
    tryPrimeFactors' p []    = [p]
    tryPrimeFactors' p (q:r) = if (mod p q) == 0 then q:(tryPrimeFactors' (quot p q) (q:r)) else (tryPrimeFactors' p r)
  in
    check x