module Q03 where

elementAt :: [a] -> Int -> a
elementAt x k = case elementAtTest x k of Left x -> x
                                          Right y -> error y

elementAtTest :: [a] -> Int -> Either a String
elementAtTest x k 
  | k > 0     = elementAtTestWithPositiveIndex x k
  | otherwise = Right "k <= 0 is not supported"

elementAtTestWithPositiveIndex :: [a] -> Int -> Either a String
elementAtTestWithPositiveIndex [] _     = Right "Element not found"
elementAtTestWithPositiveIndex (x:xs) 1 = Left x
elementAtTestWithPositiveIndex (x:xs) k = elementAtTestWithPositiveIndex xs (k-1)
