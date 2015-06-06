import Test.HUnit
import Q25
                  
assertEqualIntList :: String -> [Int] -> [Int] -> Assertion
assertEqualIntList = assertEqual

test1 = TestCase (assertEqualIntList "rndPermu []      should be ." ([]      ) (rndPermu []     ))
test2 = TestCase (assertEqualIntList "rndPermu [1]     should be ." ([1]     ) (rndPermu [1]    ))
test3 = TestCase (assertEqualIntList "rndPermu [1,2]   should be ." ([1,2]   ) (rndPermu [1,2]  ))
test4 = TestCase (assertEqualIntList "rndPermu [1,2,3] should be ." ([1,3,2] ) (rndPermu [1,2,3]))

main = runTestTT $ TestList [test1,test2,test3,test4]