module Q27 where

combineWithRest :: [a] -> Int -> [([a],[a])]
combineWithRest [] _ = []
combineWithRest (p:q) 1 = 
  let
    rest = combineWithRest q 1
  in
    ([p],q):(map (\(x, y) -> (x, p:y)) rest)
    
combineWithRest (p:q) r = 
  let
    restTake = (combineWithRest q (r-1))
    restDrop = (combineWithRest q r)
  in
    (map (\(x, y) -> (p:x, y)) restTake) ++ (map (\(x, y) -> (x, p:y)) restDrop)

group :: [Int] -> [a] -> [[[a]]]
group []    x = [[]]
group (p:q) x = foldr (++) [] (map (\(x, y) -> map (\g -> x:g) (group q y)) (combineWithRest x p))