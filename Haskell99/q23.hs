module Q23 where

import System.Random
import AvlMap

rndSelect x y = case (rndSelectTest x y) of Left result   -> result
                                            Right message -> error message

unJust :: Maybe a -> a
unJust (Just x) = x

toTreeMap :: [a] -> TreeMap Int a
toTreeMap entries = foldr (\(index, value) accMap -> unJust (insert accMap index value)) empty (zip [1..length entries] entries)

rndSelectTest :: [a] -> Int -> Either [a] String
rndSelectTest x y =
  let
    check :: [a] -> Int -> Either [a] String
    check x y
      | y < 1          = Right "Selecting less than one element is not supported"
      | y > (length x) = Right "Selecting more than length elements is not supported"
      | otherwise      = Left (rndSelectTest' (mkStdGen 1) (toTreeMap x) y)

    rndSelectTest' :: StdGen -> TreeMap Int a -> Int -> [a]
    rndSelectTest' _         _      0 = []
    rndSelectTest' generator items numItems =
      let
        (randomNumber, nextGenerator) = next generator
        ((_,chosenItem), remainingItems)  = unJust (deleteByIndex items (1 + (mod randomNumber (getSize items))))
      in
        chosenItem:(rndSelectTest' nextGenerator remainingItems (numItems-1))
  in
    check x y