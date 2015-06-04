import Test.HUnit
import Q18

assertEqualEitherIntListString :: String -> Either [Int] String -> Either [Int] String -> Assertion
assertEqualEitherIntListString = assertEqual

test1 = TestCase (assertEqualEitherIntListString "sliceTest [1,2] 1 0 should be Right q > r is not supported      ." (Right "q > r is not supported"     ) (sliceTest [1,2] 1 0))
test2 = TestCase (assertEqualEitherIntListString "sliceTest [1,2] 0 0 should be Right q < 1 is not supported      ." (Right "q < 1 is not supported"     ) (sliceTest [1,2] 0 0))
test3 = TestCase (assertEqualEitherIntListString "sliceTest [1,2] 1 3 should be Right r > length is not supported ." (Right "r > length is not supported") (sliceTest [1,2] 2 3))
test4 = TestCase (assertEqualEitherIntListString "sliceTest [1,2] 1 1 should be Left [1]                          ." (Left [1]                           ) (sliceTest [1,2] 1 1))
test5 = TestCase (assertEqualEitherIntListString "sliceTest [1,2] 1 2 should be Left [1,2]                        ." (Left [1,2]                         ) (sliceTest [1,2] 1 2))
test6 = TestCase (assertEqualEitherIntListString "sliceTest [1,2] 2 2 should be Left [2]                          ." (Left [2]                           ) (sliceTest [1,2] 2 2))

main = runTestTT $ TestList [test1,test2,test3,test4,test5,test6]