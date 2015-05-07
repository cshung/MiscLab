myLast :: [a] -> a
myLast (x:[]) = x
myLast (_:t) = myLast(t)
myLast [] = error "Wrong"
