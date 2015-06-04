module Q10 where
import Q09

encode :: (Eq a) => [a] -> [(a,Int)]
encode x = 
  let
    compact :: [a] -> (a, Int)
    compact [] = error "Internal error - pack should not include empty list"
    compact (x:xs) = (x, length(x:xs))
  in
    map compact (pack x)