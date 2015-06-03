module Q07 where

data NestedList a = Elem a | List [NestedList a]

flatten (Elem x)  = [x]
flatten (List y) = foldr (++) [] $ map flatten y
