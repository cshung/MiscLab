myLength :: [a] -> Int
myLength []    = 0
myLength (_:t) = 1 + myLength t
