module AvlMap
(
  Tree(),
  empty,
  insert,
  containsKey,
  valueByKey,
  setValueByKey,
  deleteByKey,
  valueByIndex,
  setValueByIndex,
  deleteByIndex
)
where

data Tree tKey tValue = Empty | Node { key :: tKey, value :: tValue, size :: Int, height :: Int, left :: Tree tKey tValue, right :: Tree tKey tValue}

empty :: Tree tKey tValue
empty = Empty

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

insert :: (Ord tKey) => Tree tKey tValue -> tKey -> tValue -> Maybe (Tree tKey tValue)
insert Empty key value = Just Node { key = key, value = value, size = 1, height = 1, left = Empty, right = Empty }
insert Node { key = nodeKey, value = nodeValue, left = left, right = right } insertKey insertValue =
  if insertKey > nodeKey then
    case insert right insertKey insertValue of Just insertedRight -> Just (balance (buildTree left nodeKey nodeValue insertedRight))
                                               Nothing            -> Nothing
  else
    if insertKey == nodeKey then
      Nothing
    else
      case insert left insertKey insertValue of Just insertedLeft -> Just (balance (buildTree insertedLeft nodeKey nodeValue right))
                                                Nothing           -> Nothing

byKey :: (Ord tKey) => Tree tKey tValue -> tKey -> (Tree tKey tValue -> (b, Tree tKey tValue)) -> Maybe (b, Tree tKey tValue)
byKey Empty _        _               = Nothing
byKey node searchKey processFunction
  | searchKey <  nodeKey  = case (byKey nodeLeft  searchKey processFunction) of (Just (processResult, processedLeft )) -> Just (processResult, balance (buildTree processedLeft nodeKey nodeValue nodeRight))
                                                                                Nothing                                -> Nothing
  | searchKey >  nodeKey  = case (byKey nodeRight searchKey processFunction) of (Just (processResult, processedRight)) -> Just (processResult, balance (buildTree nodeLeft nodeKey nodeValue processedRight))
                                                                                Nothing                                -> Nothing
  | searchKey == nodeKey  = Just (processFunction node)
  | otherwise             = error "This is impossible"
  where nodeKey   = key node
        nodeValue = value node
        nodeLeft  = left node
        nodeRight = right node

valueByKey :: (Ord tKey) => Tree tKey tValue -> tKey -> Maybe tValue
valueByKey node searchKey = case (byKey node searchKey (\foundNode -> (value foundNode, foundNode))) of Just (foundValue, _) -> Just foundValue
                                                                                                        Nothing              -> Nothing

containsKey:: (Ord tKey) => Tree tKey tValue -> tKey -> Bool
containsKey node searchKey = case (valueByKey node searchKey) of Just _  -> True
                                                                 Nothing -> False

setValueByKey :: (Ord tKey) => Tree tKey tValue -> tKey -> tValue -> Maybe (Tree tKey tValue)
setValueByKey node searchKey replaceValue = case (byKey node searchKey (\foundNode -> ((), buildTree (left foundNode) (key foundNode) replaceValue (right foundNode)))) of Just (_,result) -> Just result
                                                                                                                                                                           Nothing         -> Nothing

deleteByKey :: (Ord tKey) => Tree tKey tValue -> tKey -> Maybe ((tKey, tValue), Tree tKey tValue)
deleteByKey node deleteKey = byKey node deleteKey (\foundNode -> ((key foundNode, value foundNode), deleteRootNode foundNode))

byIndex :: Tree tKey tValue -> Int -> (Tree tKey tValue -> (b, Tree tKey tValue)) -> Maybe (b, Tree tKey tValue)
byIndex Empty _    _               = Nothing
byIndex node index processFunction
  | index < split  = case (byIndex nodeLeft index            processFunction) of Just (processedResult, processedLeft ) -> Just (processedResult, balance (buildTree processedLeft nodeKey nodeValue nodeRight))
                                                                                 Nothing                                -> Nothing
  | index > split  = case (byIndex nodeRight (index - split) processFunction) of Just (processedResult, processedRight) -> Just (processedResult, balance (buildTree nodeLeft nodeKey nodeValue processedRight))
                                                                                 Nothing                                -> Nothing
  | index == split = Just (processFunction node)
  where nodeKey   = key node
        nodeValue = value node
        nodeLeft  = left node
        nodeRight = right node
        split     = (getSize nodeLeft) + 1

deleteByIndex :: Tree tKey tValue -> Int -> Maybe ((tKey, tValue), Tree tKey tValue)
deleteByIndex node deleteIndex = byIndex node deleteIndex (\foundNode -> ((key node, value node), deleteRootNode foundNode))

valueByIndex ::  Tree tKey tValue -> Int -> Maybe tValue
valueByIndex node searchIndex = case (byIndex node searchIndex (\foundNode -> (value foundNode, foundNode))) of Just (result, _) -> Just result
                                                                                                                Nothing          -> Nothing

setValueByIndex :: Tree Int tValue -> Int -> tValue -> Maybe (Tree Int tValue)
setValueByIndex node searchIndex replaceValue = case (byIndex node searchIndex (\foundNode -> ((), buildTree (left foundNode) (key foundNode) replaceValue (right foundNode)))) of Just (_,result) -> Just result
                                                                                                                                                                                   Nothing         -> Nothing

deleteRootNode :: Tree tKey tValue -> Tree tKey tValue
deleteRootNode Empty = error "Unexpected 2"
deleteRootNode Node {left = Empty, right = right} = right
deleteRootNode Node {left = left,  right = Empty} = left
deleteRootNode node = balance (buildTree (left node) firstKey firstValue tree) where (firstKey, firstValue, tree) = deleteFirst (right node)

deleteFirst :: Tree tKey tValue -> (tKey, tValue, Tree tKey tValue)
deleteFirst Node {left = Empty, key = key, value = value, right = right} = (key, value, right)
deleteFirst Node {left = left, key = key, value = value, right = right} = (resultKey, resultValue, balance (buildTree resultTree key value right)) where (resultKey, resultValue, resultTree) = (deleteFirst left)

toList :: Tree tKey tValue -> [(tKey, tValue)]
toList Empty = []
toList Node {left = left, key = key, value = value, right = right} = (toList left) ++ ((key, value): (toList right))