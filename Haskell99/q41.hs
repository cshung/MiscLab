module Q41 where

import Q39
import Q40
import Avl

goldbachList :: Int -> Int -> [(Int, Int)]
goldbachList x y = case (tryGoldbachList x y) of Left result   -> result
                                                 Right message -> error message
                                               
tryGoldbachList :: Int -> Int -> Either [(Int, Int)] String
tryGoldbachList x y =
  let
    check :: Int -> Int -> Either [(Int, Int)] String
    check x y
      | x <= 0    = Right "x <= 0 is not supported"
      | x > y     = Right "x > y is not supported"
      | otherwise = Left (tryGoldbachList' x y)
      
    tryGoldbachList' :: Int -> Int -> [(Int, Int)]
    tryGoldbachList' x y =
      let
        lower input = 2 * (quot input     2)
        upper input = 2 * (quot (input+1) 2)
        p = upper x
        q = lower y
        r = [p,p+2..q]
        primesList = primesR 2 q
        primesTree = foldr (flip insert) Empty primesList
      in
        map (\s -> goldbachInternal s primesList primesTree) r        
  in
    check x y