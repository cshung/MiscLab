module Q02 where

myButLastTest :: [a] -> Either a String
myButLastTest (x:y:[]) = Left x
myButLastTest (x:[])   = Right "Cannot obtain myButLast of a list with one element"
myButLastTest []       = Right "Cannot obtain myButLast of an empty list"
myButLastTest (_:t)    = myButLastTest(t)

myButLast :: [a] -> a
myButLast y = case myButLastTest(y) of (Left  x) -> x
                                       (Right y) -> error y

