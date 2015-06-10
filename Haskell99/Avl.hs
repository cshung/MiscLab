module Avl
(
  Tree(Empty),
  insert,
  deleteByIndex,
  toList,
  getSize,
  contain
)

where

data Tree a = Empty | Node { value :: a, size :: Int, height :: Int, left :: Tree a, right :: Tree a}

getHeight :: Tree a -> Int
getHeight Empty = 0
getHeight Node { height = height } = height

getSize :: Tree a -> Int
getSize Empty = 0
getSize Node { size = size } = size

buildTree :: Tree a -> a -> Tree a -> Tree a
buildTree left x right = Node {value = x, size = (getSize left) + (getSize right) + 1, height = (max (getHeight left) (getHeight right)) + 1, left = left, right = right}

balance :: Tree a -> Tree a
balance Empty = Empty
balance x     =
  let
    getBalance :: Tree a -> Int
    getBalance x = (getHeight (right x)) - (getHeight (left x))
  in
    case (getBalance x) of
      1  -> x
      0  -> x
      -1 -> x
      2  -> if (getBalance (right x)) == (-1) then
              rotateLeft (buildTree (left x) (value x) (rotateRight (right x)))
           else
             rotateLeft x
      -2 -> if (getBalance (left x)) == 1 then
              rotateRight (buildTree (rotateLeft (left x)) (value x) (right x))
            else
              rotateRight x
      otherwise -> error "This is impossible"

insert :: (Ord a) => Tree a -> a -> Tree a
insert Empty x = Node { value = x, size = 1, height = 1, left = Empty, right = Empty }
insert Node { value = value, left = left, right = right } x =
  if x > value then
    balance (buildTree left value (insert right x))
  else
    balance (buildTree (insert left x) value right)


rotateLeft :: Tree a -> Tree a
rotateLeft oldTop =
  let
    Node { value = oldTopValue  , left = oldTopLeft  , right = oldTopRight   } = oldTop       -- to rotate left, top   must be non empty
    Node { value = oldRightValue, left = oldRightLeft, right = oldRightRight } = right oldTop -- to rotate left, right must be non empty
    newLeft = buildTree oldTopLeft oldTopValue oldRightLeft
  in
   buildTree newLeft oldRightValue oldRightRight

rotateRight :: Tree a -> Tree a
rotateRight oldTop =
  let
    Node { value = oldTopValue , left = oldTopLeft , right = oldTopRight  } = oldTop      -- to rotate right, top  must be non empty
    Node { value = oldLeftValue, left = oldLeftLeft, right = oldLeftRight } = left oldTop -- to rotate right, left must be non empty
    newRight = buildTree oldLeftRight oldTopValue oldTopRight
  in
   buildTree oldLeftLeft oldLeftValue newRight

deleteByIndex :: Tree a -> Int -> (a, Tree a)
deleteByIndex Empty _ = error "Unexpected 1"
deleteByIndex node index
  | index < split  = let (deletedValue, deletedLeft ) = (deleteByIndex nodeLeft index)            in (deletedValue, balance (buildTree deletedLeft nodeValue nodeRight))
  | index == split = (nodeValue, deleteRootNode node)
  | otherwise      = let (deletedValue, deletedRight) = (deleteByIndex nodeRight (index - split)) in (deletedValue, balance (buildTree nodeLeft nodeValue deletedRight))
  where nodeValue = value node
        nodeLeft  = left node
        nodeRight = right node
        split     = (getSize nodeLeft) + 1

deleteRootNode :: Tree a -> Tree a
deleteRootNode Empty = error "Unexpected 2"
deleteRootNode Node {left = Empty, right = right} = right
deleteRootNode Node {left = left,  right = Empty} = left
deleteRootNode node = balance (buildTree (left node) firstValue tree) where (firstValue, tree) = deleteFirst (right node)

deleteFirst :: Tree a -> (a, Tree a)
deleteFirst Node {left = Empty, value = value, right = right} = (value, right)
deleteFirst Node {left = left, value = value, right = right} = (resultValue, balance (buildTree resultTree value right)) where (resultValue, resultTree) = (deleteFirst left)

contain :: (Ord a) => Tree a -> a -> Bool
contain Empty _           = False
contain node  searchValue =
  let
    Node { value = nodeValue, left = nodeLeft, right = nodeRight } = node
  in 
    if (searchValue == nodeValue) then 
      True 
    else
      if (searchValue < nodeValue) then
        contain nodeLeft searchValue
      else
        contain nodeRight searchValue

toList :: Tree a -> [a]
toList Empty = []
toList Node {left = left, value = value, right = right} = (toList left) ++ (value: (toList right))