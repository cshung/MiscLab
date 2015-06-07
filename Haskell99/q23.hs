module Q23 where

import System.Random
import Avl

data IndexedValue a = IndexedValue (Int, a) deriving (Show)

instance Eq (IndexedValue a)
  where
    IndexedValue(x,_) == IndexedValue(y,_) = (x == y)

instance Ord (IndexedValue a)
  where
   IndexedValue(x,_) `compare` IndexedValue(y,_) = compare x y

toIndexedValue :: (Int, a) -> IndexedValue a
toIndexedValue (p,q) = IndexedValue (p, q)

toTree :: [a] -> Tree (IndexedValue a)
toTree x = foldr (flip insert) Empty $ map toIndexedValue (zip [1..length x] x)

rndSelect x y = case (rndSelectTest x y) of Left x  -> x
                                            Right y -> error y

rndSelectTest :: [a] -> Int -> Either [a] String
rndSelectTest x y =
  let
    check :: [a] -> Int -> Either [a] String
    check x y
      | y < 1          = Right "Selecting less than one element is not supported"
      | y > (length x) = Right "Selecting more than length elements is not supported"
      | otherwise      = Left (rndSelectTest' (mkStdGen 1) (toTree x) y)

    rndSelectTest' :: StdGen -> Tree (IndexedValue a) -> Int -> [a]
    rndSelectTest' _         _      0 = []
    rndSelectTest' generator items numItems =
      let
        (randomNumber, nextGenerator) = next generator
        (IndexedValue(_,chosenItem), remainingItems)  = deleteByIndex items (1 + (mod randomNumber (getSize items)))
      in
        chosenItem:(rndSelectTest' nextGenerator remainingItems (numItems-1))
  in
    check x y