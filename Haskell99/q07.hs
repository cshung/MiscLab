module Q07 where

data NestedList a = Elem a | List [NestedList a]

flatten :: NestedList a -> [a]
flatten x =
  let
    flatten' :: NestedList a -> [a] -> [a]    
    flatten' (Elem x)     acc = x:acc
    flatten' (List [] )   acc = acc
    flatten' (List (p:q)) acc = flatten' p (flatten' (List q) acc)
  in
    flatten' x []