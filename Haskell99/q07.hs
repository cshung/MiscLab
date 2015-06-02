module Q07 where

data NestedList a = Elem a | List [NestedList a]

flatten :: (NestedList a) -> [a]
flatten (Elem x)  = [x]
flatten (List y) =
  let
    flattenList :: [NestedList a] -> [a]
    flattenList []     = []
    flattenList (x:xs) = (flatten x) ++ (flattenList xs)
  in
    flattenList y