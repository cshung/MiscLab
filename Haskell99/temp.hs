data IntervalTree = Leaf { src :: Int, dst :: Int } | Node { size :: Int, left :: IntervalTree, right :: IntervalTree } deriving (Show)

getSize :: IntervalTree -> Int
getSize Leaf { src = leafSrc, dst = leafDst } = leafDst - leafSrc + 1
getSize Node { size = nodeSize }              = nodeSize

deleteByIndex :: IntervalTree -> Int -> (Int, IntervalTree)
deleteByIndex Leaf { src = leafSrc, dst = leafDst } index
  | (index == 1)                        = (leafSrc, Leaf { src = leafSrc + 1, dst = leafDst })
  | (index == leafSize)                 = (leafDst, Leaf { src = leafSrc, dst = leafDst - 1 })
  | ((1 < index) && (index < leafSize)) = ((leafSrc + index - 1), Node { size = leafSize - 1, left = Leaf { src = leafSrc, dst = (leafSrc + index - 2)}, right = Leaf { src = (leafSrc + index), dst = leafDst }})
  | otherwise                           = error "Element not found for deletion"
  where
    leafSize = leafDst - leafSrc + 1

deleteByIndex Node { size = nodeSize, left = nodeLeft, right = nodeRight } index
  |              1 <= index && index <= leftSize = let (deletedValue, deletedSubTree) = (deleteByIndex nodeLeft index)              in (deletedValue, Node { size = nodeSize - 1, left = deletedSubTree, right = nodeRight })
  | (leftSize + 1) <= index && index <= nodeSize = let (deletedValue, deletedSubTree) = (deleteByIndex nodeRight (index - leftSize))in (deletedValue, Node { size = nodeSize - 1, left = nodeLeft, right = deletedSubTree  })
  | otherwise                                    = error "Element not found for deletion"
  where
    leftSize = getSize nodeLeft

indent :: Int -> String
indent 0 = ""
indent x = ' ':(indent (x - 1))

preOrder :: IntervalTree -> Int -> String
preOrder Leaf { src = src, dst = dst } indentValue = (indent indentValue) ++ "(" ++ (show src) ++ ", " ++ (show dst) ++ ")\n"
preOrder Node {size = nodeSize, left = nodeLeft, right = nodeRight} indentValue = (indent indentValue) ++ "[" ++ (show nodeSize) ++ "]\n" ++ (preOrder nodeLeft (indentValue + 2)) ++ (preOrder nodeRight (indentValue + 2))

main =
  let
    tree1 = Leaf { src = 1, dst = 10}
    (del2, tree2) = deleteByIndex tree1 4
    (del3, tree3) = deleteByIndex tree2 7
    (del4, tree4) = deleteByIndex tree3 1
    (del5, tree5) = deleteByIndex tree4 6
    (del6, tree6) = deleteByIndex tree5 3
  in
    do
      putStrLn (preOrder tree1 0)
      putStrLn (show del2)
      putStrLn ""
      putStrLn (preOrder tree2 0)
      putStrLn (show del3)
      putStrLn ""
      putStrLn (preOrder tree3 0)
      putStrLn (show del4)
      putStrLn ""
      putStrLn (preOrder tree4 0)
      putStrLn (show del5)
      putStrLn ""
      putStrLn (preOrder tree5 0)
      putStrLn (show del6)
      putStrLn ""
      putStrLn (preOrder tree6 0)