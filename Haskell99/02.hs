myButLast :: [a] -> a
myButLast (x:y:[]) = x
myButLast (x:[])   = error "Wrong"
myButLast []       = error "Wrong"
myButLast (_:t)    = myButLast(t)

