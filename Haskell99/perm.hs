pick :: [a] -> [(a,[a])]

pick [x] = [(x,[])]
pick (x:xs) = [(x, xs)] ++ (map (\(p, q) -> (p, x:q)) (pick xs))

perm :: [a] -> [[a]]
perm [x] = [[x]]
perm x = foldr (++) [] (map (\(p, q) -> map ((:) p) (perm(q))) (pick x))
