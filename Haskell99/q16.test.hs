import Test.HUnit
import Q16

assertEqualMaybeIntList :: String -> Maybe [Int] -> Maybe [Int] -> Assertion
assertEqualMaybeIntList = assertEqual

test1 = TestCase (assertEqualMaybeIntList "dropEveryTest [1,2,3,4] 0 should be Nothing     ." (Nothing     ) (dropEveryTest [1,2,3,4] 0))
test2 = TestCase (assertEqualMaybeIntList "dropEveryTest [1,2,3,4] 1 should be Just []     ." (Just []     ) (dropEveryTest [1,2,3,4] 1))
test3 = TestCase (assertEqualMaybeIntList "dropEveryTest [1,2,3,4] 2 should be Just [1,3]  ." (Just [1,3]  ) (dropEveryTest [1,2,3,4] 2))
test4 = TestCase (assertEqualMaybeIntList "dropEveryTest [1,2,3,4] 3 should be Just [1,2,4]." (Just [1,2,4]) (dropEveryTest [1,2,3,4] 3))
test5 = TestCase (assertEqualMaybeIntList "dropEveryTest [1,2,3,4] 4 should be Just [1,2,3]." (Just [1,2,3]) (dropEveryTest [1,2,3,4] 4))
test6 = TestCase (assertEqualMaybeIntList "dropEveryTest [1,2,3,4] 5 should be Nothing     ." (Nothing     ) (dropEveryTest [1,2,3,4] 5))

main = runTestTT $ TestList [test1,test2,test3,test4,test5,test6]