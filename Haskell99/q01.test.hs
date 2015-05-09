import Test.HUnit
import Q01

test1 = TestCase (assertEqual "myLast [0] should be 0." 0 (myLast [0]))
test2 = TestCase (assertEqual "myLast [0, 1] should be 1"  1 (myLast [0, 1]))

main = runTestTT $ TestList [test1, test2]