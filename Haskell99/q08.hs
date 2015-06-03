module Q08 where

compress :: (Eq a) => [a] -> [a]
compress x =
  let
    push :: (Eq a) => a -> [a] -> [a]
    push x [] = [x]
    push y (x:xs) = if y == x then x:xs else y:x:xs
  in
    foldr push [] x 