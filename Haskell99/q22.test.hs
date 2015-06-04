import Test.HUnit
import Q22

test1 = TestCase (assertEqual "rangeTest 1 0 should be Right y < x is not supported." (Right "y < x is not supported") (rangeTest 1 0))
test2 = TestCase (assertEqual "rangeTest 0 0 should be Left [0]                    ." (Left [0]                      ) (rangeTest 0 0))
test3 = TestCase (assertEqual "rangeTest 0 1 should be Left [0,1]                  ." (Left [0,1]                    ) (rangeTest 0 1))
test4 = TestCase (assertEqual "rangeTest 0 2 should be Left [0,1,2]                ." (Left [0,1,2]                  ) (rangeTest 0 2))
test5 = TestCase (assertEqual "rangeTest 1 2 should be Left [1,2]                  ." (Left [1,2]                    ) (rangeTest 1 2))

main = runTestTT $ TestList [test1,test2,test3,test4,test5]