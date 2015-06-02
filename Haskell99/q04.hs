module Q04 where

mylength :: [a] -> Int
mylength x =
  let
    mylength' :: [a] -> Int -> Int
    mylength' [] acc = acc
    mylength' (x:xs) acc = mylength' xs (acc + 1)
  in
    mylength' x 0