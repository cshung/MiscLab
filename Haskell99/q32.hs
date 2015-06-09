module Q32 where

myGCD :: Int -> Int -> Int
myGCD x y = case (tryMyGCD x y) of Left result   -> result
                                   Right message -> error message

tryMyGCD :: Int -> Int -> Either Int String
tryMyGCD x y
  | x == 0    = Right "Zero is not supported"
  | y == 0    = Right "Zero is not supported"
  | x < 0     = tryMyGCD (-x) y
  | y < 0     = tryMyGCD x (-y)
  | y > x     = tryMyGCD y x
  | otherwise = let r = mod x y in if (r == 0) then Left y else tryMyGCD y r