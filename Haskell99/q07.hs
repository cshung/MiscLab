module Q07 where

data NestedList a = Elem a | List [NestedList a]

flatten :: NestedList a -> [a]
flatten x = 
  let
    flatten' :: NestedList a -> [a] -> [a]  
    flatten' (Elem x)  = \y -> x:y
    flatten' (List y) = foldr (.) (\x -> x) $ map flatten' y
  in
    (flatten' x) []