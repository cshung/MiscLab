import Test.HUnit
import Q39

test1 = TestCase (assertEqual "tryPrimesR (-1) 1 should be Right x <= 0 is not supported." (Right "x <= 0 is not supported") (tryPrimesR (-1) 1))
test2 = TestCase (assertEqual "tryPrimesR 3   2  should be Right x > y is not supported ." (Right "x > y is not supported" ) (tryPrimesR 3   2 ))
test3 = TestCase (assertEqual "tryPrimesR 11 20  should be Left [11, 13, 17, 19]        ." (Left [11, 13, 17, 19]          ) (tryPrimesR 11 20 ))

main = runTestTT $ TestList [test1,test2,test3]