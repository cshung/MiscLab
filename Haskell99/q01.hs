module Q01 where

myLast :: [a] -> a
myLast y = case myLastTest(y) of Just x  -> x
                                 Nothing -> error "Cannot obtain myLast of an empty list"

myLastTest :: [a] -> Maybe a
myLastTest (x:[])  = Just x
myLastTest  []     = Nothing
myLastTest  (_:t)  = myLastTest(t)
