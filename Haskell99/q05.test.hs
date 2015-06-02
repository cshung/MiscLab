import Test.HUnit
import Q05

assertEqualIntList :: String -> [Int] -> [Int] -> Assertion
assertEqualIntList = assertEqual

test1 = TestCase (assertEqualIntList "myreverse []    should be []. "    ([])    (myreverse []   ))
test2 = TestCase (assertEqualIntList "myreverse [1]   should be [1]. "   ([1])   (myreverse [1]  ))
test3 = TestCase (assertEqualIntList "myreverse [1,2] should be [2,1]. " ([2,1]) (myreverse [1,2]))

main = runTestTT $ TestList [test1,test2,test3]