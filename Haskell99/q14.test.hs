import Test.HUnit
import Q14

assertEqualIntList :: String -> [Int] -> [Int] -> Assertion
assertEqualIntList = assertEqual

test1 = TestCase (assertEqualIntList "dupli []      should be []       ." ([]       ) (dupli []   ))
test2 = TestCase (assertEqualIntList "dupli [1]     should be [1,1]    ." ([1,1]    ) (dupli [1]  ))
test3 = TestCase (assertEqualIntList "dupli [1,2]   should be [1,1,2,2]." ([1,1,2,2]) (dupli [1,2]))

main = runTestTT $ TestList [test1,test2,test3]