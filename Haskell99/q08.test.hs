import Test.HUnit
import Q08

assertEqualIntList :: String -> [Int] -> [Int] -> Assertion
assertEqualIntList = assertEqual

test1 = TestCase (assertEqualIntList "compress []      should be []."       ([]     ) (compress []     ))
test2 = TestCase (assertEqualIntList "compress [1]     should be [1]."      ([1]    ) (compress [1]    ))
test3 = TestCase (assertEqualIntList "compress [1,2]   should be [1,2]."    ([1,2]  ) (compress [1,2]  ))
test4 = TestCase (assertEqualIntList "compress [1,2,1] should be [1,2,1]. " ([1,2,1]) (compress [1,2,1]))
test5 = TestCase (assertEqualIntList "compress [1,1,2] should be [1,2]. "   ([1,2]  ) (compress [1,1,2]))
test6 = TestCase (assertEqualIntList "compress [1,2,2] should be [1,2]. "   ([1,2]  ) (compress [1,2,2]))

main = runTestTT $ TestList [test1,test2,test3,test4,test5,test6]