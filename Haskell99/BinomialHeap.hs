module BinomialHeap where

data BinomialNode a = Node { size :: Int, value :: a, children :: [BinomialNode a] } deriving (Show)
type BinomialHeap a = [BinomialNode a]

mergeOneNode :: (Ord a) => BinomialHeap a -> BinomialNode a -> BinomialHeap a
mergeOneNode []    insertNode = [insertNode]
mergeOneNode queue insertNode =
  let
    (headNode:tail) = queue
    Node {size = headNodeSize,   value = headNodeValue,   children = headNodeChildren   } = headNode
    Node {size = insertNodeSize, value = insertNodeValue, children = insertNodeChildren } = insertNode
  in
    if (headNodeSize > insertNodeSize) then
      insertNode:queue
    else
      if (headNodeSize < insertNodeSize) then
        headNode:(mergeOneNode tail insertNode)
      else
        if (headNodeValue < insertNodeValue) then
          mergeOneNode tail (Node { size = headNodeSize * 2, value = headNodeValue, children = insertNode:headNodeChildren })
        else
          mergeOneNode tail (Node { size = headNodeSize * 2, value = insertNodeValue, children = headNode:insertNodeChildren })

insert :: (Ord a) => BinomialHeap a -> a -> BinomialHeap a
insert queue insertValue = mergeOneNode queue Node { size = 1, value = insertValue, children = []}

findMinTree :: (Ord a) => BinomialHeap a -> (BinomialNode a, BinomialHeap a)
findMinTree []          = error "This is impossible"
findMinTree (head:tail) =
  let
    findMinTree' :: (Ord a) => BinomialHeap a -> BinomialNode a -> (BinomialNode a, BinomialHeap a)
    findMinTree' []          currentMinTree = (currentMinTree, [])
    findMinTree' (head:tail) currentMinTree =
      let
        headValue           = value head
        currentMinTreeValue = value currentMinTree
      in
        if (headValue > currentMinTreeValue) then
          let 
            (minTree, minHeap) = findMinTree' tail currentMinTree
          in 
            (minTree, head:minHeap)
        else
          let 
            (minTree, minHeap) = findMinTree' tail head
          in 
            (minTree, currentMinTree:minHeap)
  in
    findMinTree' tail head

findMin :: (Ord a) => BinomialHeap a -> Maybe a
findMin []    = Nothing
findMin queue = Just (value (fst(findMinTree queue)))

deleteMin :: (Ord a) => BinomialHeap a -> Maybe (a, BinomialHeap a)
deleteMin [] = Nothing
deleteMin queue =
  let
    (minTree, minHeap) = findMinTree queue
  in
    Just ((value minTree), foldr (flip mergeOneNode) minHeap (children minTree))