module Q23 where

import System.Random
import Data.Array

rndSelect x y = case (rndSelectTest x y) of Left x  -> x
                                            Right y -> error y

rndSelectTest :: [a] -> Int -> Either [a] String
rndSelectTest x y =
  let
    check :: [a] -> Int -> Either [a] String
    check x y
      | y < 1          = Right "Selecting less than one element is not supported"
      | y > (length x) = Right "Selecting more than length elements is not supported"
      | otherwise      = Left (rndSelectTest' (mkStdGen 1) x y)

    rndSelectTest' :: StdGen -> [a] -> Int -> [a]
    rndSelectTest' _         _      0 = []
    rndSelectTest' generator items numItems =
      let
        (randomNumber, nextGenerator) = next generator
        (chosenItem, remainingItems)  = pick items (mod randomNumber (length items))
      in
        chosenItem:(rndSelectTest' nextGenerator remainingItems (numItems-1))


    pick :: [a] -> Int -> (a, [a])
    pick [] _ = error "This is impossible"
    pick (x:xs) 0 = (x, xs)
    pick (x:xs) index = let (y, ys) = pick xs (index - 1) in (y, x:ys)
  in
    check x y