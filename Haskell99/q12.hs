module Q12 where

import Q11

decodeModified  :: (Eq a) => [Entry a] -> [a] 
decodeModified x =
  let
    expand :: (Eq a) => (Entry a) -> [a]
    expand (Single x) = [x]
    expand (Multiple 0 x) = []
    expand (Multiple c x) = x:(expand (Multiple (c-1) x))
  in
    foldr (++) [] $ map expand x