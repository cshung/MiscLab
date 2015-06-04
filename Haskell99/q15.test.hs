import Test.HUnit
import Q15

assertEqualMaybeIntList :: String -> Maybe [Int] -> Maybe [Int] -> Assertion
assertEqualMaybeIntList = assertEqual

test1  = TestCase (assertEqualMaybeIntList "repliTest []    0    should be []       ." (Just []       ) (repliTest []    0   ))
test2  = TestCase (assertEqualMaybeIntList "repliTest [1]   0    should be []       ." (Just []       ) (repliTest [1]   0   ))
test3  = TestCase (assertEqualMaybeIntList "repliTest [1,2] 0    should be []       ." (Just []       ) (repliTest [1,2] 0   ))
test4  = TestCase (assertEqualMaybeIntList "repliTest []    1    should be []       ." (Just []       ) (repliTest []    1   ))
test5  = TestCase (assertEqualMaybeIntList "repliTest [1]   1    should be [1]      ." (Just [1]      ) (repliTest [1]   1   ))
test6  = TestCase (assertEqualMaybeIntList "repliTest [1,2] 1    should be [1,2]    ." (Just [1,2]    ) (repliTest [1,2] 1   ))
test7  = TestCase (assertEqualMaybeIntList "repliTest []    2    should be []       ." (Just []       ) (repliTest []    2   ))
test8  = TestCase (assertEqualMaybeIntList "repliTest [1]   2    should be [1,1]    ." (Just [1,1]    ) (repliTest [1]   2   ))
test9  = TestCase (assertEqualMaybeIntList "repliTest [1,2] 2    should be [1,1,2,2]." (Just [1,1,2,2]) (repliTest [1,2] 2   ))
test10 = TestCase (assertEqualMaybeIntList "repliTest [1,2] (-1) should be Nothing  ." (Nothing       ) (repliTest [1,2] (-1)))

main = runTestTT $ TestList [test1,test2,test3,test4,test5,test6,test7,test8,test9,test10]