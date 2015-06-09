module Q33 where

import Q32

coPrime :: Int -> Int -> Bool
coPrime x y = case (tryCoPrime x y) of  Left result   -> result
                                        Right message -> error message

tryCoPrime :: Int -> Int -> Either Bool String
tryCoPrime x y = case (tryMyGCD x y) of Left result   -> Left  (result == 1)
                                        Right message -> Right message