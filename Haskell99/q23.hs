import System.Random
import Data.Array

rndSelect :: [a] -> Int -> [a]
rndSelect x y =
  let
    rndSelect' :: StdGen -> [a] -> Int -> [a]
    rndSelect' _         _      0 = []
    rndSelect' generator items numItems =
      let
        (randomNumber, nextGenerator) = next generator
        (chosenItem, remainingItems)  = pick items (mod randomNumber (length items))
      in 
        chosenItem:(rndSelect' nextGenerator remainingItems (numItems-1))
        
    
    pick :: [a] -> Int -> (a, [a])
    pick [] _ = error "This is impossible"
    pick (x:xs) 0 = (x, xs)
    pick (x:xs) index = let (y, ys) = pick xs (index - 1) in (y, x:ys)
  in
    rndSelect' (mkStdGen 1) x y