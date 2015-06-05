import System.Random

randomNumberSequence :: Int -> Int -> [Int]
randomNumberSequence bound length =
  let
    randomNumberSequence' :: StdGen -> Int -> Int -> [Int]
    randomNumberSequence' _         _     0      = []
    randomNumberSequence' generator bound length = let (value, nextGenerator) = (next generator) in (mod value bound) : (randomNumberSequence' nextGenerator bound (length - 1))
  in 
   randomNumberSequence' (mkStdGen 1) bound length