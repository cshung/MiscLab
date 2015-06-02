module Q05 where

myreverse :: [a] -> [a]
myreverse x = 
  let
    myreverse' :: [a] -> [a] -> [a]
    myreverse' []     acc = acc
    myreverse' (x:xs) acc = myreverse' xs (x:acc)
  in
    myreverse' x []