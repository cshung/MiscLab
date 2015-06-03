import Test.HUnit
import Q09

assertEqualIntListList :: String -> [[Int]] -> [[Int]] -> Assertion
assertEqualIntListList = assertEqual

test1 = TestCase (assertEqualIntListList "pack []      should be []."            ([]           ) (pack []     ))
test2 = TestCase (assertEqualIntListList "pack [1]     should be [[1]]."         ([[1]]        ) (pack [1]    ))
test3 = TestCase (assertEqualIntListList "pack [1,2]   should be [[1],[2]]."     ([[1],[2]]    ) (pack [1,2]  ))
test4 = TestCase (assertEqualIntListList "pack [1,2,1] should be [[1],[2],[1]]." ([[1],[2],[1]]) (pack [1,2,1]))
test5 = TestCase (assertEqualIntListList "pack [1,1,2] should be [[1,1],[2]]."   ([[1,1],[2]]  ) (pack [1,1,2]))
test6 = TestCase (assertEqualIntListList "pack [1,2,2] should be [[1],[2,2]]."   ([[1],[2,2]]  ) (pack [1,2,2]))

main = runTestTT $ TestList [test1,test2,test3,test4,test5,test6]