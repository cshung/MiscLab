module Q13 where

data Entry a = Single a | Multiple Int a deriving (Show, Eq)

encodeDirect :: (Eq a) => [a] -> [Entry a]
encodeDirect x =
  let
    encodeDirect' :: (Eq a) => a -> [(a,Int)] -> [(a,Int)]
    encodeDirect' x [] = [(x,1)]
    encodeDirect' y ((x,c):xs) = if y == x then ((x,c+1):xs) else ((y,1):(x,c):xs)
    
    simplify :: (Eq a) => (a, Int) -> Entry a
    simplify (x, 1) = Single x
    simplify (x, c) = (Multiple c x)
  in
    map simplify $ foldr encodeDirect' [] x 