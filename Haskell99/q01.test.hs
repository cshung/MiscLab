import Test.HUnit
import Q01

assertEqualMaybeInt::String -> Maybe Int -> Maybe Int -> Assertion
assertEqualMaybeInt = assertEqual

test1 = TestCase (assertEqualMaybeInt "myLastTest []     should be Nothing." Nothing  (myLastTest []    )) 
test2 = TestCase (assertEqual         "myLastTest [0]    should be Just 0. " (Just 0) (myLastTest [0]   ))
test3 = TestCase (assertEqual         "myLastTest [0, 1] should be Just 1. " (Just 1) (myLastTest [0, 1]))

main = runTestTT $ TestList [test1, test2, test3]