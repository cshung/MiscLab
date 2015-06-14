module AvlSet
(
  TreeSet(),
  empty,
  getSize,
  insert,
  deleteByIndex,
  contain,
  toList
)

where

import qualified AvlMap

data TreeSet a = Map (AvlMap.TreeMap a ())

empty :: TreeSet a
empty = Map AvlMap.empty

getSize :: TreeSet a -> Int
getSize (Map map) = AvlMap.getSize map

insert :: (Ord a) => TreeSet a -> a -> TreeSet a
insert (Map oldMap) keyToInsert = case (AvlMap.insert oldMap keyToInsert ()) of Just newMap -> Map newMap
                                                                                Nothing     -> error "Key duplicated"

deleteByIndex :: TreeSet a -> Int -> (a, TreeSet a)
deleteByIndex (Map oldMap) indexToDelete = case (AvlMap.deleteByIndex oldMap indexToDelete) of Just ((deletedValue, _), newMap) -> (deletedValue, Map newMap)
                                                                                               Nothing                          -> error "Index not found"

contain :: (Ord a) => TreeSet a -> a -> Bool
contain (Map oldMap) keyToFind = AvlMap.containsKey oldMap keyToFind

toList :: TreeSet a -> [a]
toList (Map oldMap) = map (\(x, _) -> x) (AvlMap.toList oldMap)