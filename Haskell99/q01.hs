module Q01 where

myLast :: [a] -> a
myLast (x:[]) = x
myLast []     = error "Wrong"
myLast (_:t)  = myLast(t)
