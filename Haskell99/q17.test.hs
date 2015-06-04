import Test.HUnit
import Q17

assertEqualMaybeIntListPair :: String -> Maybe ([Int], [Int]) -> Maybe ([Int], [Int]) -> Assertion
assertEqualMaybeIntListPair = assertEqual

test1 = TestCase (assertEqualMaybeIntListPair "splitTest [1,2] (-1) should be Nothing        ." (Nothing        ) (splitTest [1,2] (-1)))
test2 = TestCase (assertEqualMaybeIntListPair "splitTest [1,2] 0    should be Just ([],[1,2])." (Just ([],[1,2])) (splitTest [1,2] 0   ))
test3 = TestCase (assertEqualMaybeIntListPair "splitTest [1,2] 1    should be Just ([1],[2]) ." (Just ([1],[2]) ) (splitTest [1,2] 1   ))
test4 = TestCase (assertEqualMaybeIntListPair "splitTest [1,2] 2    should be Just ([1,2],[])." (Just ([1,2],[])) (splitTest [1,2] 2   ))
test5 = TestCase (assertEqualMaybeIntListPair "splitTest [1,2] 3    should be Nothing        ." (Nothing        ) (splitTest [1,2] 3   ))

main = runTestTT $ TestList [test1,test2,test3,test4,test5]