module Q10 where

encode :: (Eq a) => [a] -> [(a,Int)]
encode x =
  let
    encode' :: (Eq a) => a -> [(a,Int)] -> [(a,Int)]
    encode' x [] = [(x,1)]
    encode' y ((x,c):xs) = if y == x then ((x,c+1):xs) else ((y,1):(x,c):xs)
  in
    foldr encode' [] x 