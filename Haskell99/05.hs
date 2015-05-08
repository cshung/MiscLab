myReverse :: [a] -> [a]
myReverse x = myReverse2 x [] where 
    myReverse2 :: [a] -> [a] -> [a]
    myReverse2 []     acc = acc
    myReverse2 (x:xs) acc = myReverse2 xs (x:acc)