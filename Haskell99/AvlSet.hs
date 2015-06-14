module AvlSet
(
  TreeSet(),
  empty,
  getSize,
  insert,
  tryInsert,
  deleteByIndex,
  tryDeleteByIndex,
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

tryInsert :: (Ord a) => TreeSet a -> a -> Maybe (TreeSet a)
tryInsert (Map oldMap) keyToInsert = case (AvlMap.tryInsert oldMap keyToInsert ()) of Just newMap -> Just (Map newMap)
                                                                                      Nothing     -> Nothing

insert :: (Ord a) => TreeSet a -> a -> TreeSet a
insert oldSet keyToInsert = case (tryInsert oldSet keyToInsert) of Just result -> result
                                                                   Nothing     -> error "Key duplicated"

tryDeleteByIndex :: TreeSet a -> Int -> Maybe (a, TreeSet a)
tryDeleteByIndex (Map oldMap) indexToDelete = case (AvlMap.tryDeleteByIndex oldMap indexToDelete) of Just ((deletedValue, _), newMap) -> Just (deletedValue, Map newMap)
                                                                                                     Nothing                          -> Nothing

deleteByIndex :: TreeSet a -> Int -> (a, TreeSet a)
deleteByIndex oldSet indexToDelete = case (tryDeleteByIndex oldSet indexToDelete) of Just result -> result
                                                                                     Nothing     -> error "Index not found"

contain :: (Ord a) => TreeSet a -> a -> Bool
contain (Map oldMap) keyToFind = AvlMap.containsKey oldMap keyToFind

toList :: TreeSet a -> [a]
toList (Map oldMap) = map (\(x, _) -> x) (AvlMap.toList oldMap)