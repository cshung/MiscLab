elementAt :: [a] -> Int -> a
elementAt (x:t)  0 = x
elementAt (_:t)  y = elementAt t (y-1)
elementAt (_)    _ = error "Element not found"