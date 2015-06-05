module Q26 where

combine :: [a] -> Int -> [[a]]
combine [] _ = []
combine p 1 = map (\x -> [x]) p
combine (p:q) r = map ((:) p) (combine q (r-1)) ++ (combine q r)