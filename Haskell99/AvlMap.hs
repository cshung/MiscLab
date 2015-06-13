module AvlMap where

data Tree tKey tValue = Empty | Node { key :: tKey, value :: tValue, size :: Int, height :: Int, left :: Tree tKey tValue, right :: Tree tKey tValue} deriving (Show)

getHeight :: Tree tKey tValue -> Int
getHeight Empty = 0
getHeight Node { height = height } = height

getSize :: Tree tKey tValue -> Int
getSize Empty = 0
getSize Node { size = size } = size

buildTree :: Tree tKey tValue -> tKey -> tValue -> Tree tKey tValue -> Tree tKey tValue
buildTree left key value right = Node {key = key, value = value, size = (getSize left) + (getSize right) + 1, height = (max (getHeight left) (getHeight right)) + 1, left = left, right = right}

balance :: Tree tKey tValue -> Tree tKey tValue
balance Empty = Empty
balance x =
  let
    getBalance :: Tree tKey tValue -> Int
    getBalance x = (getHeight (right x)) - (getHeight (left x))
  in
    case (getBalance x) of
      1  -> x
      0  -> x
      -1 -> x
      2  -> if (getBalance (right x)) == (-1) then
              rotateLeft (buildTree (left x) (key x) (value x) (rotateRight (right x)))
           else
             rotateLeft x
      -2 -> if (getBalance (left x)) == 1 then
              rotateRight (buildTree (rotateLeft (left x)) (key x) (value x) (right x))
            else
              rotateRight x
      otherwise -> error "This is impossible"

insert :: (Ord tKey) => Tree tKey tValue -> tKey -> tValue -> Tree tKey tValue
insert Empty key value = Node { key = key, value = value, size = 1, height = 1, left = Empty, right = Empty }
insert Node { key = nodeKey, value = nodeValue, left = left, right = right } insertKey insertValue =
  if insertKey > nodeKey then
    balance (buildTree left nodeKey nodeValue (insert right insertKey insertValue))
  else
    balance (buildTree (insert left insertKey insertValue) nodeKey nodeValue right)

rotateLeft :: Tree tKey tValue -> Tree tKey tValue
rotateLeft oldTop =
  let
    Node { key = oldTopKey  , value = oldTopValue  , left = oldTopLeft  , right = oldTopRight   } = oldTop       -- to rotate left, top   must be non empty
    Node { key = oldRightKey, value = oldRightValue, left = oldRightLeft, right = oldRightRight } = right oldTop -- to rotate left, right must be non empty
    newLeft = buildTree oldTopLeft oldTopKey oldTopValue oldRightLeft
  in
   buildTree newLeft oldRightKey oldRightValue oldRightRight

rotateRight :: Tree tKey tValue -> Tree tKey tValue
rotateRight oldTop =
  let
    Node { key = oldTopKey , value = oldTopValue , left = oldTopLeft , right = oldTopRight  } = oldTop      -- to rotate right, top  must be non empty
    Node { key = oldLeftKey, value = oldLeftValue, left = oldLeftLeft, right = oldLeftRight } = left oldTop -- to rotate right, left must be non empty
    newRight = buildTree oldLeftRight oldTopKey oldTopValue oldTopRight
  in
   buildTree oldLeftLeft oldLeftKey oldLeftValue newRight

deleteByKey :: (Ord tKey) => Tree tKey tValue -> tKey -> (tKey, tValue, Tree tKey tValue)
deleteByKey Empty _ = error "Unexpected 3"
deleteByKey node deleteKey
  | deleteKey <  nodeKey  = let (deletedKey, deletedValue, deletedLeft ) = (deleteByKey nodeLeft deleteKey) in (deletedKey, deletedValue, balance (buildTree deletedLeft nodeKey nodeValue nodeRight))
  | deleteKey == nodeKey  = (nodeKey, nodeValue, deleteRootNode node)
  | otherwise             = let (deletedKey, deletedValue, deletedRight) = (deleteByKey nodeRight deleteKey) in (deletedKey, deletedValue, balance (buildTree nodeLeft nodeKey nodeValue deletedRight))
  where nodeKey   = key node
        nodeValue = value node
        nodeLeft  = left node
        nodeRight = right node

deleteByIndex :: Tree tKey tValue -> Int -> (tKey, tValue, Tree tKey tValue)
deleteByIndex Empty _ = error "Unexpected 1"
deleteByIndex node index
  | index < split  = let (deletedKey, deletedValue, deletedLeft ) = (deleteByIndex nodeLeft index)            in (deletedKey, deletedValue, balance (buildTree deletedLeft nodeKey nodeValue nodeRight))
  | index == split = (nodeKey, nodeValue, deleteRootNode node)
  | otherwise      = let (deletedKey, deletedValue, deletedRight) = (deleteByIndex nodeRight (index - split)) in (deletedKey, deletedValue, balance (buildTree nodeLeft nodeKey nodeValue deletedRight))
  where nodeKey   = key node
        nodeValue = value node
        nodeLeft  = left node
        nodeRight = right node
        split     = (getSize nodeLeft) + 1

deleteRootNode :: Tree tKey tValue -> Tree tKey tValue
deleteRootNode Empty = error "Unexpected 2"
deleteRootNode Node {left = Empty, right = right} = right
deleteRootNode Node {left = left,  right = Empty} = left
deleteRootNode node = balance (buildTree (left node) firstKey firstValue tree) where (firstKey, firstValue, tree) = deleteFirst (right node)

deleteFirst :: Tree tKey tValue -> (tKey, tValue, Tree tKey tValue)
deleteFirst Node {left = Empty, key = key, value = value, right = right} = (key, value, right)
deleteFirst Node {left = left, key = key, value = value, right = right} = (resultKey, resultValue, balance (buildTree resultTree key value right)) where (resultKey, resultValue, resultTree) = (deleteFirst left)

valueByKey :: (Ord tKey) => Tree tKey tValue -> tKey -> Maybe tValue
valueByKey Empty _           = Nothing
valueByKey node  searchKey    =
  let
    Node { key = nodeKey, value = nodeValue, left = nodeLeft, right = nodeRight } = node
  in
    if (searchKey == nodeKey) then
      Just nodeValue
    else
      if (searchKey < nodeKey) then
        valueByKey nodeLeft searchKey
      else
        valueByKey nodeRight searchKey

containsKey:: (Ord tKey) => Tree tKey tValue -> tKey -> Bool
containsKey node searchKey = case (valueByKey node searchKey) of Just _  -> True
                                                                 Nothing -> False

setValueByKey :: (Ord tKey) => Tree tKey tValue -> tKey -> tValue -> Either (Tree tKey tValue) String
setValueByKey Empty _         _             = Right "Value not found"
setValueByKey node  searchKey replaceValue  =
  let
    Node { key = nodeKey, value = nodeValue, left = nodeLeft, right = nodeRight } = node
  in
    if (searchKey == nodeKey) then
      Left (buildTree nodeLeft nodeKey replaceValue nodeRight)
    else
      if (searchKey < nodeKey) then
        case (setValueByKey nodeLeft searchKey replaceValue)  of Left  replacedLeft  -> Left (buildTree replacedLeft nodeKey replaceValue nodeRight)
                                                                 Right message       -> Right message
      else
        case (setValueByKey nodeRight searchKey replaceValue) of Left  replacedRight -> Left (buildTree nodeLeft nodeKey replaceValue replacedRight)
                                                                 Right message       -> Right message

toList :: Tree tKey tValue -> [(tKey, tValue)]
toList Empty = []
toList Node {left = left, key = key, value = value, right = right} = (toList left) ++ ((key, value): (toList right))