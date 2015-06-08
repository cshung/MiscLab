module Q28 where

quicksort :: [a] -> (a -> a -> Ordering) -> [a]

quicksort [] _ = []
quicksort (p:q) comp = let (x, y) = partition p q comp in (quicksort x comp) ++ p : (quicksort y comp)

partition :: a -> [a] -> (a -> a -> Ordering) -> ([a], [a])
partition _ [] _ = ([], [])
partition p (q:r) comp = let (s, t) = partition p r comp in if (comp q p) == GT then (s, q:t) else (q:s, t)

mergesort :: [a] -> (a -> a -> Ordering) -> [a]
mergesort []  _    = []
mergesort [x] _    = [x]
mergesort p   comp = let (x, y) = split p in merge (mergesort x comp) (mergesort y comp) comp

split :: [a] -> ([a], [a])
split x = 
  let 
    split' :: [a] -> Int -> ([a], [a])
    split' []    _ = ([], [])
    split' p     0 = ([], p)
    split' (p:q) r  = let (x, y) = split' q (r-1) in (p:x, y)
  in
    split' x (quot (length x) 2)

merge :: [a] -> [a] -> (a -> a -> Ordering) -> [a]
merge []    p     _    = p
merge p     []    _    = p
merge (p:q) (r:s) comp = if comp p r == GT then r:(merge (p:q) s comp) else p:(merge q (r:s) comp)

lsort :: [[a]] -> [[a]]
lsort x = mergesort x (\p q -> compare (length p) (length q))

histogramAdd :: Int -> [(Int, Int)] -> [(Int, Int)]
histogramAdd p []        = [(p, 1)]
histogramAdd p ((q,r):s) = if p == q then ((q,(r+1)):s) else (q,r):(histogramAdd p s)

histogramValue :: Int -> [(Int, Int)] -> Int
histogramValue _ []        = error "This is impossible"
histogramValue p ((q,r):s) = if p == q then r else (histogramValue p s)

lfsort :: [[a]] -> [[a]]
lfsort x =
  let
    hist = foldr histogramAdd [] $ map length x
  in
    mergesort x (\p q -> compare (histogramValue (length p) hist) (histogramValue (length q) hist))