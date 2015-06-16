import qualified AvlMap
import qualified BinomialHeap

let numSignals = 5
let nand  = 1
let gates = [(nand, 1, 1, 3), (nand, 2, 2, 4), (nand, 3, 4, 5)]
let inputs = foldr (\(x,y) z -> AvlMap.insert z x y) empty [("a", 1),("b", 2)]
