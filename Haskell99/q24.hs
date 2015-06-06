module Q24 where

import Q22
import Q23

diffSelect :: Int -> Int -> [Int]
diffSelect numRandomElements numElements = case (diffSelectTest numRandomElements numElements) of Left result   -> result
                                                                                                  Right message -> error message

diffSelectTest :: Int -> Int -> Either [Int] String
diffSelectTest numRandomElements numElements =
  let
    check :: Int -> Int -> Either [Int] String
    check numRandomElements numElements
      | numRandomElements < 1           = Right "Selecting less than one element is not supported"
      | numRandomElements > numElements = Right "Selecting more than one numElements is not supported"
      | otherwise                       = Left (diffSelectTest' numRandomElements numElements)

    diffSelectTest' :: Int -> Int -> [Int]
    diffSelectTest' numRandomElements numElements = rndSelect (range 1 numElements) numRandomElements
  in
    check  numRandomElements numElements