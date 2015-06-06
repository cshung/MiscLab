module Q25 where

import Q23

rndPermu :: [a] -> [a]
rndPermu [] = []
rndPermu x  = rndSelect x (length(x))