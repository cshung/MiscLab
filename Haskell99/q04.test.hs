import Test.HUnit
import Q04

test1 = TestCase (assertEqual "mylength []    should be 0. " (0) (mylength []   ))
test2 = TestCase (assertEqual "mylength [1]   should be 1. " (1) (mylength [1]  ))
test3 = TestCase (assertEqual "mylength [1,2] should be 2. " (2) (mylength [1,2]))

main = runTestTT $ TestList [test1,test2,test3]