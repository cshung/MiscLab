module Q09 where

pack :: (Eq a) => [a] -> [[a]]
pack x =
  let
    pack' :: (Eq a) => a -> [[a]] -> [[a]]
    pack' x [] = [[x]]
    pack' y ((xx:xxs):xs) = if y == xx then ((y:xx:xxs):xs) else ([y]:(xx:xxs):xs)
  in
    foldr pack' [] x 