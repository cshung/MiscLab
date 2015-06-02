import Test.HUnit
import Q00

test1 = TestCase (assertEqual "one should be 1. " (1) (one))

main = runTestTT $ TestList [test1]