module Q11 where

import Q10

data Entry a = Single a | Multiple Int a deriving (Show, Eq)

encodeModified  :: (Eq a) => [a] -> [Entry a]
encodeModified x =
  let
    simplify :: (Eq a) => (a, Int) -> Entry a
    simplify (x, 1) = Single x
    simplify (x, c) = (Multiple c x)
  in
    map simplify $ encode x