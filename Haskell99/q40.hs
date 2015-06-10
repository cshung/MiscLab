module Q40 where

import Q39
import Avl

goldbach :: Int -> (Int, Int)
goldbach x = case (tryGoldbach x) of Left result   -> result
                                     Right message -> error message

tryGoldbach :: Int -> Either (Int, Int) String
tryGoldbach x =
  let
    check x
      | x <= 0         = Right "Nonpositive integer is not supported"
      | (mod x 2) == 1 = Right "Odd integer is not supported"
      | otherwise      = Left (goldbachInternal x primesList primesTree)
    primesList = primesR 2 x
    primesTree = foldr (flip insert) Empty primesList
  in
    check x

goldbachInternal :: Int -> [Int] -> Tree Int -> (Int, Int)
goldbachInternal _ [] _    = error "This is impossible - well - given Goldbach conjecture"
goldbachInternal x (p:q) r = if (contain r (x - p)) then (p, x - p) else (goldbachInternal x q r)