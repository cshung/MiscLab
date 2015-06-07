data Tree a = Empty | Node { value :: a, size :: Int, height :: Int, left :: Tree a, right :: Tree a} deriving (Show)

getHeight :: Tree a -> Int
getHeight Empty = 0
getHeight Node { height = height } = height

getSize :: Tree a -> Int
getSize Empty = 0
getSize Node { size = size } = size

buildTree :: Tree a -> a -> Tree a -> Tree a
buildTree left x right = Node {value = x, size = (getSize left) + (getSize right) + 1, height = (max (getHeight left) (getHeight right)) + 1, left = left, right = right}

insert :: (Show a, Ord a) => Tree a -> a -> Tree a
insert Empty x = Node { value = x, size = 1, height = 1, left = Empty, right = Empty }
insert Node { value = value, left = left, right = right } x =
  if x > value then
    balance (buildTree left value (insert right x))
  else
    balance (buildTree (insert left x) value right)

balance :: (Show a) => Tree a -> Tree a
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
      otherwise -> error ("Balance failed - the subtree is \n" ++ (preOrder x 0))

rotateLeft :: (Show a) => Tree a -> Tree a
rotateLeft oldTop =
  let
    Node { value = oldTopValue  , left = oldTopLeft  , right = oldTopRight   } = oldTop       -- to rotate left, top   must be non empty
    Node { value = oldRightValue, left = oldRightLeft, right = oldRightRight } = right oldTop -- to rotate left, right must be non empty
    newLeft = buildTree oldTopLeft oldTopValue oldRightLeft
  in
   buildTree newLeft oldRightValue oldRightRight

rotateRight :: (Show a) => Tree a -> Tree a
rotateRight oldTop =
  let
    Node { value = oldTopValue , left = oldTopLeft , right = oldTopRight  } = oldTop      -- to rotate right, top  must be non empty
    Node { value = oldLeftValue, left = oldLeftLeft, right = oldLeftRight } = left oldTop -- to rotate right, left must be non empty
    newRight = buildTree oldLeftRight oldTopValue oldTopRight
  in
   buildTree oldLeftLeft oldLeftValue newRight

select :: (Show a) => Tree a -> Int -> a
select Empty _ = error "Unexpected"
select Node { value = value, size = size, left = left, right = right } index
  | index < split  = select left index
  | index == split = value
  | otherwise      = select right (index - split)
  where split = (getSize left) + 1

deleteByIndex :: (Show a) => Tree a -> Int -> (a, Tree a)
deleteByIndex Empty _ = error "Unexpected"
deleteByIndex node index
  | index < split  = let (deletedValue, deletedLeft ) = (deleteByIndex nodeLeft index)            in (deletedValue, balance (buildTree deletedLeft nodeValue nodeRight))
  | index == split = (nodeValue, deleteRootNode node)
  | otherwise      = let (deletedValue, deletedRight) = (deleteByIndex nodeRight (index - split)) in (deletedValue, balance (buildTree nodeLeft nodeValue deletedRight))
  where nodeValue = value node
        nodeLeft  = left node
        nodeRight = right node
        split     = (getSize nodeLeft) + 1

deleteRootNode :: (Show a) => Tree a -> Tree a
deleteRootNode Empty = error "Unexpected"
deleteRootNode Node {left = Empty, right = right} = right
deleteRootNode Node {left = left,  right = Empty} = left
deleteRootNode node = balance (buildTree (left node) firstValue tree) where (firstValue, tree) = deleteFirst (right node)

deleteFirst :: (Show a) => Tree a -> (a, Tree a)
deleteFirst Node {left = Empty, value = value, right = right} = (value, right)
deleteFirst Node {left = left, value = value, right = right} = (resultValue, balance (buildTree resultTree value right)) where (resultValue, resultTree) = (deleteFirst left)

indent :: Int -> String
indent 0 = ""
indent x = ' ':(indent (x - 1))

preOrder :: (Show a) => Tree a -> Int -> String
preOrder Empty x = indent x ++ "(null)\n"
preOrder Node { value = value, size = size, height = height, left = left, right = right } x = indent x ++ (show value) ++ "(size = " ++ (show size) ++ ", height = " ++ (show height) ++ ")\n" ++ (preOrder left (x + 2)) ++ (preOrder right (x + 2))