module Q19 where
import Q17

rotate :: [a] -> Int -> [a]
rotate p q =
  let
    (r, s) = split p (mod q (length p))
  in
    s ++ r