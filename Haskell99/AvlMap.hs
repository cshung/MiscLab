module AvlMap
(
  TreeMap(),
  empty,
  getSize,
  insert,
  containsKey,
  getValueByKey,
  setValueByKey,
  deleteByKey,
  getValueByIndex,
  setValueByIndex,
  deleteByIndex,
  tryInsert,
  tryGetValueByKey,
  trySetValueByKey,
  tryDeleteByKey,
  tryGetValueByIndex,
  trySetValueByIndex,
  tryDeleteByIndex,
  toList
)
where

data TreeMap tKey tValue = Empty | Node { key :: tKey, value :: tValue, size :: Int, height :: Int, left :: TreeMap tKey tValue, right :: TreeMap tKey tValue}

empty :: TreeMap tKey tValue
empty = Empty

getHeight :: TreeMap tKey tValue -> Int
getHeight Empty = 0
getHeight Node { height = height } = height

getSize :: TreeMap tKey tValue -> Int
getSize Empty = 0
getSize Node { size = size } = size

buildTree :: TreeMap tKey tValue -> tKey -> tValue -> TreeMap tKey tValue -> TreeMap tKey tValue
buildTree left key value right = Node {key = key, value = value, size = (getSize left) + (getSize right) + 1, height = (max (getHeight left) (getHeight right)) + 1, left = left, right = right}

balance :: TreeMap tKey tValue -> TreeMap tKey tValue
balance Empty = Empty
balance x =
  let
    getBalance :: TreeMap tKey tValue -> Int
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

rotateLeft :: TreeMap tKey tValue -> TreeMap tKey tValue
rotateLeft oldTop =
  let
    Node { key = oldTopKey  , value = oldTopValue  , left = oldTopLeft  , right = oldTopRight   } = oldTop       -- to rotate left, top   must be non empty
    Node { key = oldRightKey, value = oldRightValue, left = oldRightLeft, right = oldRightRight } = right oldTop -- to rotate left, right must be non empty
    newLeft = buildTree oldTopLeft oldTopKey oldTopValue oldRightLeft
  in
   buildTree newLeft oldRightKey oldRightValue oldRightRight

rotateRight :: TreeMap tKey tValue -> TreeMap tKey tValue
rotateRight oldTop =
  let
    Node { key = oldTopKey , value = oldTopValue , left = oldTopLeft , right = oldTopRight  } = oldTop      -- to rotate right, top  must be non empty
    Node { key = oldLeftKey, value = oldLeftValue, left = oldLeftLeft, right = oldLeftRight } = left oldTop -- to rotate right, left must be non empty
    newRight = buildTree oldLeftRight oldTopKey oldTopValue oldTopRight
  in
   buildTree oldLeftLeft oldLeftKey oldLeftValue newRight

insert :: (Ord tKey) => TreeMap tKey tValue -> tKey -> tValue -> TreeMap tKey tValue
insert node key value = case (tryInsert node key value) of Just result -> result
                                                           Nothing     -> error "Cannot insert - key duplicated"

tryInsert :: (Ord tKey) => TreeMap tKey tValue -> tKey -> tValue -> Maybe (TreeMap tKey tValue)
tryInsert Empty key value = Just Node { key = key, value = value, size = 1, height = 1, left = Empty, right = Empty }
tryInsert Node { key = nodeKey, value = nodeValue, left = left, right = right } insertKey insertValue =
  if insertKey > nodeKey then
    case tryInsert right insertKey insertValue of Just insertedRight -> Just (balance (buildTree left nodeKey nodeValue insertedRight))
                                                  Nothing            -> Nothing
  else
    if insertKey == nodeKey then
      Nothing
    else
      case tryInsert left insertKey insertValue of Just insertedLeft -> Just (balance (buildTree insertedLeft nodeKey nodeValue right))
                                                   Nothing           -> Nothing

byKey :: (Ord tKey) => TreeMap tKey tValue -> tKey -> (TreeMap tKey tValue -> (b, TreeMap tKey tValue)) -> Maybe (b, TreeMap tKey tValue)
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

tryGetValueByKey :: (Ord tKey) => TreeMap tKey tValue -> tKey -> Maybe tValue
tryGetValueByKey node searchKey = case (byKey node searchKey (\foundNode -> (value foundNode, foundNode))) of Just (foundValue, _) -> Just foundValue
                                                                                                              Nothing              -> Nothing

getValueByKey :: (Ord tKey) => TreeMap tKey tValue -> tKey -> tValue
getValueByKey node searchKey = case (tryGetValueByKey node searchKey) of Just result -> result
                                                                         Nothing     -> error "Key not found"

containsKey:: (Ord tKey) => TreeMap tKey tValue -> tKey -> Bool
containsKey node searchKey = case (tryGetValueByKey node searchKey) of Just _  -> True
                                                                       Nothing -> False

trySetValueByKey :: (Ord tKey) => TreeMap tKey tValue -> tKey -> tValue -> Maybe (TreeMap tKey tValue)
trySetValueByKey node searchKey replaceValue = case (byKey node searchKey (\foundNode -> ((), buildTree (left foundNode) (key foundNode) replaceValue (right foundNode)))) of Just (_,result) -> Just result
                                                                                                                                                                              Nothing         -> Nothing

setValueByKey :: (Ord tKey) => TreeMap tKey tValue -> tKey -> tValue -> TreeMap tKey tValue
setValueByKey node searchKey replaceValue = case (trySetValueByKey node searchKey replaceValue) of Just result -> result
                                                                                                   Nothing     -> error "Key not found"

deleteByKey :: (Ord tKey) => TreeMap tKey tValue -> tKey -> ((tKey, tValue), TreeMap tKey tValue)
deleteByKey node deleteKey = case (tryDeleteByKey node deleteKey) of Just result -> result
                                                                     Nothing     -> error "Key not found"

tryDeleteByKey :: (Ord tKey) => TreeMap tKey tValue -> tKey -> Maybe ((tKey, tValue), TreeMap tKey tValue)
tryDeleteByKey node deleteKey = byKey node deleteKey (\foundNode -> ((key foundNode, value foundNode), deleteRootNode foundNode))

byIndex :: TreeMap tKey tValue -> Int -> (TreeMap tKey tValue -> (b, TreeMap tKey tValue)) -> Maybe (b, TreeMap tKey tValue)
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

deleteByIndex :: TreeMap tKey tValue -> Int -> ((tKey, tValue), TreeMap tKey tValue)
deleteByIndex node deleteIndex = case (tryDeleteByIndex node deleteIndex) of Just result -> result
                                                                             Nothing     -> error "Index not found"

tryDeleteByIndex :: TreeMap tKey tValue -> Int -> Maybe ((tKey, tValue), TreeMap tKey tValue)
tryDeleteByIndex node deleteIndex = byIndex node deleteIndex (\foundNode -> ((key foundNode, value foundNode), deleteRootNode foundNode))

tryGetValueByIndex ::  TreeMap tKey tValue -> Int -> Maybe tValue
tryGetValueByIndex node searchIndex = case (byIndex node searchIndex (\foundNode -> (value foundNode, foundNode))) of Just (result, _) -> Just result
                                                                                                                      Nothing          -> Nothing

getValueByIndex ::  TreeMap tKey tValue -> Int -> tValue
getValueByIndex node searchIndex = case (tryGetValueByIndex node searchIndex) of Just result -> result
                                                                                 Nothing     -> error "Index not found"

trySetValueByIndex :: TreeMap Int tValue -> Int -> tValue -> Maybe (TreeMap Int tValue)
trySetValueByIndex node searchIndex replaceValue = case (byIndex node searchIndex (\foundNode -> ((), buildTree (left foundNode) (key foundNode) replaceValue (right foundNode)))) of Just (_,result) -> Just result
                                                                                                                                                                                      Nothing         -> Nothing

setValueByIndex :: TreeMap Int tValue -> Int -> tValue -> TreeMap Int tValue
setValueByIndex node searchIndex replaceValue = case (trySetValueByIndex node searchIndex replaceValue) of Just result -> result
                                                                                                           Nothing     -> error "Index not found"

deleteRootNode :: TreeMap tKey tValue -> TreeMap tKey tValue
deleteRootNode Empty = error "Unexpected 2"
deleteRootNode Node {left = Empty, right = right} = right
deleteRootNode Node {left = left,  right = Empty} = left
deleteRootNode node = balance (buildTree (left node) firstKey firstValue tree) where (firstKey, firstValue, tree) = deleteFirst (right node)

deleteFirst :: TreeMap tKey tValue -> (tKey, tValue, TreeMap tKey tValue)
deleteFirst Node {left = Empty, key = key, value = value, right = right} = (key, value, right)
deleteFirst Node {left = left, key = key, value = value, right = right} = (resultKey, resultValue, balance (buildTree resultTreeMap key value right)) where (resultKey, resultValue, resultTreeMap) = (deleteFirst left)

toList :: TreeMap tKey tValue -> [(tKey, tValue)]
toList Empty = []
toList Node {left = left, key = key, value = value, right = right} = (toList left) ++ ((key, value): (toList right))